#include <stdio.h>
#include <stdint.h>
#include "parameter.h"

// module
#include "module/line_buffer.h"
#include "module/conv.h"


void max_pool(uint8_t *max_in, uint8_t lb_valid, uint8_t *max_out)
{
    // ------------------------
    // Combinational
    // ------------------------
    uint8_t w_max0;
    uint8_t w_max1;

    if (lb_valid) {
        w_max0 = (max_in[0] > max_in[1]) ? max_in[0] : max_in[1];
        w_max1 = (max_in[2] > max_in[3]) ? max_in[2] : max_in[3];

        *max_out = (w_max0 > w_max1) ? w_max0 : w_max1;
    }
}

// void conv_layer(uint8_t *img, uint8_t *conv_valid)

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


// Test
static uint8_t img[INPUT_IMG_HEIGHT * INPUT_IMG_WIDTH];

static int8_t test_weight[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE] = {16,16,16, 16,16,16, 16,16,16};
static int8_t test_bias = 0;

int main(void)
{
    // 확인용: 각 픽셀에 (행*100 + 열) 넣기
    for (int y = 0; y < INPUT_IMG_HEIGHT; y++)
        for (int x = 0; x < INPUT_IMG_WIDTH; x++)
            img[y * INPUT_IMG_WIDTH + x] = (uint8_t)(y*10 + x);


    // conv layer 0
    lineBuf_Conv_t lb_conv0;
    conv3x3_t conv0;
    lineBuf_Maxpool_t lb_max0;

    uint8_t lb_conv0_start = 1;

    // window & line buffer valid signal
    uint8_t lb_valid0;
    uint8_t conv_win[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE];
    uint8_t conv_valid;
    uint8_t lb_valid1;
    uint8_t max_win[2 * 2];

    // output
    uint8_t conv_out0;
    uint8_t max_out0;

    // Init
    lineBuf_Conv_init(&lb_conv0, INPUT_IMG_WIDTH, INPUT_IMG_HEIGHT);
    lineBuf_Maxpool_init(&lb_max0, CONV0_IMG_WIDTH, CONV0_IMG_HEIGHT);

    int conv_cnt = 0;
    int conv_row_cnt = 0;
    int conv_col_cnt = 0;
    
    int max_cnt = 0;
    int max_row_cnt = 0;
    int max_col_cnt = 0;
    // Reset
    lineBuf_Conv_reset(&lb_conv0);
    conv_reset(&conv0);
    lineBuf_Maxpool_reset(&lb_max0);
    
    int i = 0;
    printf("Line Buffer & Convolution layer Test Start\n");
    for (;;)
    {
        lineBuf_Conv_op(&lb_conv0, img[i], lb_conv0_start, conv_win, &lb_valid0);
        conv3x3(&conv0, conv_win, test_weight, test_bias, lb_valid0, &conv_out0, &conv_valid);
        lineBuf_Maxpool_op(&lb_max0, conv_out0, conv_valid, max_win, &lb_valid1);
        max_pool(max_win, lb_valid1, &max_out0);

        // if (conv_valid) {
        //     printf("%3d ", conv_out0);
        //     conv_cnt++;
        //     conv_col_cnt++;
        //     if (conv_col_cnt == CONV0_IMG_WIDTH) {
        //         printf("\tlb : %d / conv : %d / max : %d at layer0\n", lb_valid0, conv_valid, lb_valid1);
        //         conv_row_cnt++;
        //         conv_col_cnt = 0;
        //         if (conv_row_cnt == CONV0_IMG_HEIGHT) break;
        //     }
        // }

        if (lb_valid1) {
            printf("%3d ", max_out0);
            max_cnt++;
            max_col_cnt++;
            if (max_col_cnt == MAX_POOL0_IMG_WIDTH) {
                printf("\tlb : %d / conv : %d / max : %d at layer0\n", lb_valid0, conv_valid, lb_valid1);
                max_row_cnt++;
                max_col_cnt = 0;
                if (max_row_cnt == MAX_POOL0_IMG_HEIGHT) break;
            }
        }

        // if (lb_valid1) {
        //     printf("%3d ", max_out0);
        //     conv_cnt++;
        // }
        // printf("\n");

        i++;
        if (i >= INPUT_IMG_HEIGHT * INPUT_IMG_WIDTH) i = INPUT_IMG_HEIGHT * INPUT_IMG_WIDTH;
    }

    printf("\nConvolution Calulate for %d times\n", conv_cnt);
    printf("\nMax pooling Calulate for %d times\n", max_cnt);

    return 0;
}