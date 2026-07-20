#include <stdio.h>
#include <stdint.h>
#include "parameter.h"

// module
#include "module/line_buffer.h"
#include "module/conv.h"

void max_pool_reset()
{

}

void max_pool()
{

}


void print_window(uint8_t *win_out)
{
    printf("Line Buffer output - Window\n");
    for (int i = 0; i < MAX_KERNEL_SIZE; i++)
    {
        for (int j = 0; j < MAX_KERNEL_SIZE; j++)
        {
            printf("%d ", win_out[i*MAX_KERNEL_SIZE+j]);
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
    // conv layer 0
    line_buf_t lb_conv0;
    conv_t conv0;
    line_buf_t lb_max0;

    // window & line buffer valid signal
    uint8_t win_out[MAX_KERNEL_SIZE * MAX_KERNEL_SIZE];
    uint8_t lb_valid;

    // conv output
    uint8_t conv_out0;

    // Init
    line_buf_init(&lb_conv0, 28, 28, 3);
    conv_init(&conv0, 3);

    int conv_cnt = 0;

    // 확인용: 각 픽셀에 (행*100 + 열) 넣기
    for (int y = 0; y < MAX_IMG_HEIGHT; y++)
        for (int x = 0; x < MAX_IMG_WIDTH; x++)
            img[y][x] = (uint8_t)(y*10 + x);

    // Reset
    line_buf_reset(&lb_conv0);
    conv_reset(&conv0);
    
    printf("Line Buffer & Convolution layer Test Start\n");
    for (int y = 0; y < MAX_IMG_HEIGHT; y++)
    {
        for (int x = 0; x < MAX_IMG_WIDTH; x++)
        {
            line_buf_push(&lb_conv0, img[y][x], win_out, &lb_valid);
            conv3x3(&conv0, win_out, test_weight, test_bias, lb_valid, &conv_out0);
            if (lb_valid) {
                printf("%3d ", conv_out0);
                conv_cnt++;
            }
        }
        printf("\n");
    }

    printf("\nConvolution Calulate for %d times\n", conv_cnt);

    return 0;
}