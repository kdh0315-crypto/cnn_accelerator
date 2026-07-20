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
    for (int i = 0; i < MAX_KERNEL_SIZE; i++)
    {
        for (int j = 0; j < MAX_KERNEL_SIZE; j++)
        {
            printf("%3d ", win_out[i*MAX_KERNEL_SIZE+j]);
        }
        printf("\n");
    }
}


// Test
static uint8_t img[MAX_IMG_HEIGHT][MAX_IMG_WIDTH];

static int8_t test_weight[MAX_KERNEL_SIZE * MAX_KERNEL_SIZE] = {16,16,16, 16,16,16, 16,16,16};
static int8_t test_bias = 0;

int main(void)
{
    // 확인용: 각 픽셀에 (행*100 + 열) 넣기
    for (int y = 0; y < MAX_IMG_HEIGHT; y++)
        for (int x = 0; x < MAX_IMG_WIDTH; x++)
            img[y][x] = (uint8_t)(y*10 + x);


    // conv layer 0
    line_buf_t lb_conv0;
    conv_t conv0;
    line_buf_t lb_max0;

    uint8_t lb_conv0_start = 1;

    // window & line buffer valid signal
    uint8_t lb_valid0;
    uint8_t conv_win[MAX_KERNEL_SIZE * MAX_KERNEL_SIZE];
    uint8_t lb_valid1;
    uint8_t max_win[2 * 2];

    // output
    uint8_t conv_out0;
    uint8_t max_out0;

    // Init
    line_buf_init(&lb_conv0, 28, 28, 3);
    conv_init(&conv0, 3);
    line_buf_init(&lb_max0, 26, 26, 2);

    int conv_cnt = 0;

    // Reset
    line_buf_reset(&lb_conv0);
    conv_reset(&conv0);
    line_buf_reset(&lb_max0);
    
    printf("Line Buffer & Convolution layer Test Start\n");
    for (int y = 0; y < MAX_IMG_HEIGHT; y++)
    {
        for (int x = 0; x < MAX_IMG_WIDTH; x++)
        {
            line_buf_op(&lb_conv0, img[y][x], lb_conv0_start, conv_win, &lb_valid0);
            conv3x3(&conv0, conv_win, test_weight, test_bias, lb_valid0, &conv_out0);
            line_buf_op(&lb_max0, conv_out0, lb_valid0, max_win, &lb_valid1);
            max_pool(max_win, lb_valid1, &max_out0);
            if (lb_valid0) {
                for (int i = 0; i < 9; i++)
                {
                    printf("%3d ", conv_win[i]);
                }
                printf("Convolution result = %d\n", conv_out0);
            }

            if (lb_valid1) {
                for (int i = 0; i < 4; i++)
                {
                    printf("%3d ", max_win[i]);
                }
                printf("\n");
            }

            // if (lb_valid0) {
            //     printf("%3d ", conv_out0);
            //     conv_cnt++;
            // }
            // if (lb_valid1) {
            //     printf("%3d ", max_out0);
            //     conv_cnt++;
            // }
        }
        printf("\tlb for conv : %d / max : %d at layer0\n", lb_valid0, lb_valid1);
        // printf("\n");
    }

    printf("\nConvolution Calulate for %d times\n", conv_cnt);

    return 0;
}