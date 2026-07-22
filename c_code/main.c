#include <stdio.h>
#include <stdint.h>
#include "parameter.h"

// module
#include "module/conv.h"

// print function to print
#include "debug/debug_print.h"

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
static uint8_t img[INPUT_IMG_SIZE];

static int8_t test_weight[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE] = {16,16,16, 16,16,16, 16,16,16};
static int8_t test_bias = 0;


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

    LayerProbe_t probe1;
    LayerProbe_Init(&probe1, FILTERED_IMG_WIDTH1, CONV_IMG_WIDTH1);

    int i = 0;
    printf("Line Buffer & Convolution layer Test Start\n");
    for (int cycle = 0; cycle < TOTAL_CYCLES; cycle++)
    {
        conv2D(&conv2D0, img[i], test_weight, test_bias, conv_en, &layer_out0, &valid0);
        conv2D(&conv2D1, layer_out0, test_weight, test_bias, valid0, &layer_out1, &valid1);

        LayerProbe_Step(&probe1, &conv2D1, layer_out1, valid1);

        i++;
        if (i >= INPUT_IMG_SIZE)
        {
            i = INPUT_IMG_SIZE;
            conv_en = 0;
        }
    }

    LayerProbe_Print(&probe1, "Layer1", FILTERED_IMG_HEIGHT1, CONV_IMG_HEIGHT1);
    LayerProbe_Free(&probe1);

    return 0;
}
