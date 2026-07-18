#include <stdio.h>
#include <stdint.h>
#include "parameter.h"

// module
#include "module/line_buffer.h"
#include "module/conv.h"


void print_window(uint8_t *win_out)
{
    printf("Line Buffer output - Window\n");
    for (int i = 0; i < KERNEL_SIZE; i++)
    {
        for (int j = 0; j < KERNEL_SIZE; j++)
        {
            printf("%d ", win_out[i*KERNEL_SIZE+j]);
        }
        printf("\n");
    }
}


// Test
static uint8_t img[IMG_HEIGHT][IMG_WIDTH];

static int8_t test_weight[KERNEL_SIZE * KERNEL_SIZE] = {16,16,16, 16,16,16, 16,16,16};
static int8_t test_bias = 0;

int main(void)
{
    // window & line buffer valid signal
    uint8_t win_out[KERNEL_SIZE * KERNEL_SIZE];
    uint8_t lb_valid;

    // conv output
    uint8_t conv_out;

    int conv_cnt = 0;

    // 확인용: 각 픽셀에 (행*100 + 열) 넣기
    for (int y = 0; y < IMG_HEIGHT; y++)
        for (int x = 0; x < IMG_WIDTH; x++)
            img[y][x] = (uint8_t)(y*10 + x);

    // Reset
    line_buf_reset();
    conv_reset();

    
    printf("Line Buffer & Convolution layer Test Start\n");
    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            line_buf_push(img[y][x], win_out, &lb_valid);
            conv3x3(win_out, test_weight, test_bias, lb_valid, &conv_out);
            if (lb_valid) {
                printf("%3d ", conv_out);
                conv_cnt++;
            }
        }
        printf("\n");
    }

    printf("\nConvolution Calulate for %d times\n", conv_cnt);

    return 0;
}