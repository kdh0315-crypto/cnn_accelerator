#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "parameter.h"

// module
#include "module/conv.h"

void print_window(uint8_t *win_out)
{
    printf("Line Buffer output - Window\n");
    for (int i = 0; i < CNN_KERNEL_SIZE; i++)
    {
        for (int j = 0; j < CNN_KERNEL_SIZE; j++)
        {
            printf("%3d ", win_out[i*CNN_KERNEL_SIZE+j]);
        }
        printf("\n");
    }
}

// debug: print a row-major uint8_t matrix of fixed width and actual (possibly unexpected) height
void print_matrix(uint8_t *mat, int width, int height, int expected_height, const char *label)
{
    printf("\n--- %s : %d x %d (expected height %d) ---\n", label, width, height, expected_height);
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            printf("%3d ", mat[r*width + c]);
        }
        printf("\n");
    }
}


// Test
static uint8_t img[INPUT_IMG_SIZE];

static int8_t test_weight[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE] = {16,16,16, 16,16,16, 16,16,16};
static int8_t test_bias = 0;

// safety upper bound on total main-loop cycles, so a bug that keeps re-asserting
// valid forever cannot hang this test harness (784 pixels + generous drain margin)
#define TOTAL_CYCLES (INPUT_IMG_SIZE + 4*INPUT_IMG_WIDTH)

int main(void)
{
    // 확인용: 각 픽셀에 (행*100 + 열) 넣기
    for (int y = 0; y < INPUT_IMG_HEIGHT; y++)
        for (int x = 0; x < INPUT_IMG_WIDTH; x++)
            img[y * INPUT_IMG_WIDTH + x] = (uint8_t)(y*10 + x);


    uint8_t conv_en = 1;

    conv2D_t conv2D0;
    conv2D_t conv2D1;

    conv2D_Init(&conv2D0, INPUT_IMG_WIDTH, INPUT_IMG_HEIGHT, FILTERED_IMG_WIDTH0, FILTERED_IMG_HEIGHT0);
    conv2D_Init(&conv2D1, CONV_IMG_WIDTH0, CONV_IMG_HEIGHT0, FILTERED_IMG_WIDTH1, FILTERED_IMG_HEIGHT1);
    conv2D_Reset(&conv2D0);
    conv2D_Reset(&conv2D1);

    uint8_t layer_out0;
    uint8_t layer_out1;
    uint8_t valid0;
    uint8_t valid1;

    // layer1 pre-pool (conv/filter only) stream -- width fixed (FILTERED_IMG_WIDTH1), height grows dynamically
    uint8_t (*l1_filter_mat)[FILTERED_IMG_WIDTH1] = NULL;
    int l1f_capacity = 0;
    int l1f_row_cnt = 0;
    int l1f_col_cnt = 0;

    // layer1 post-pool stream -- width fixed (CONV_IMG_WIDTH1), height grows dynamically
    uint8_t (*l1_pool_mat)[CONV_IMG_WIDTH1] = NULL;
    int pool_capacity = 0;
    int max_row_cnt = 0;
    int max_col_cnt = 0;

    int i = 0;
    printf("Line Buffer & Convolution layer Test Start\n");
    for (int cycle = 0; cycle < TOTAL_CYCLES; cycle++)
    {
        conv2D(&conv2D0, img[i], test_weight, test_bias, conv_en, &layer_out0, &valid0);
        conv2D(&conv2D1, layer_out0, test_weight, test_bias, valid0, &layer_out1, &valid1);

        // layer1 pre-pool (conv/filter) output, exposed via the public conv2D1.filter fields
        if (conv2D1.filter.filter_valid) {
            if (l1f_row_cnt + 1 > l1f_capacity) {
                l1f_capacity = l1f_row_cnt + 1;
                l1_filter_mat = realloc(l1_filter_mat, l1f_capacity * sizeof(*l1_filter_mat));
            }
            l1_filter_mat[l1f_row_cnt][l1f_col_cnt] = conv2D1.filter.filter_out;
            l1f_col_cnt++;
            if (l1f_col_cnt == FILTERED_IMG_WIDTH1) {
                l1f_row_cnt++;
                l1f_col_cnt = 0;
            }
        }

        // layer1 post-pool (final) output
        if (valid1) {
            if (max_row_cnt + 1 > pool_capacity) {
                pool_capacity = max_row_cnt + 1;
                l1_pool_mat = realloc(l1_pool_mat, pool_capacity * sizeof(*l1_pool_mat));
            }
            l1_pool_mat[max_row_cnt][max_col_cnt] = layer_out1;
            max_col_cnt++;
            if (max_col_cnt == CONV_IMG_WIDTH1) {
                max_row_cnt++;
                max_col_cnt = 0;
            }
        }

        i++;
        if (i >= INPUT_IMG_SIZE)
        {
            i = INPUT_IMG_SIZE;
            conv_en = 0;
        }
    }

    print_matrix((uint8_t *)l1_filter_mat, FILTERED_IMG_WIDTH1, l1f_row_cnt, FILTERED_IMG_HEIGHT1, "Layer1 Conv (pre-pool) output");
    print_matrix((uint8_t *)l1_pool_mat, CONV_IMG_WIDTH1, max_row_cnt, CONV_IMG_HEIGHT1, "Layer1 MaxPool (post-pool) output");

    free(l1_filter_mat);
    free(l1_pool_mat);

    return 0;
}
