#include <stdio.h>
#include <stdint.h>
#include "parameter.h"

// module
#include "module/convlayer.h"

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


    uint8_t conv_en = 1;

    ConvLayer_t conv_layer1;
    ConvLayer_t conv_layer2;

    ConvLayer_Init(&conv_layer1, INPUT_IMG_WIDTH, INPUT_IMG_HEIGHT, CONV0_IMG_WIDTH, CONV0_IMG_HEIGHT);
    ConvLayer_Init(&conv_layer2, MAX_POOL0_IMG_WIDTH, MAX_POOL0_IMG_HEIGHT, CONV1_IMG_WIDTH, CONV1_IMG_HEIGHT);
    ConvLayer_Reset(&conv_layer1);
    ConvLayer_Reset(&conv_layer2);

    uint8_t layer_out1;
    uint8_t layer_out2;
    uint8_t valid1;
    uint8_t valid2;

    int conv_cnt = 0;
    int conv_row_cnt = 0;
    int conv_col_cnt = 0;
    
    int max_cnt = 0;
    int max_row_cnt = 0;
    int max_col_cnt = 0;

    
    int i = 0;
    printf("Line Buffer & Convolution layer Test Start\n");
    for (;;)
    {
        ConvLayer(&conv_layer1, img[i], test_weight, test_bias, conv_en, &layer_out1, &valid1);
        ConvLayer(&conv_layer2, layer_out1, test_weight, test_bias, valid1, &layer_out2, &valid2);


        // if (conv_valid0) {
        //     printf("%3d ", conv_out0);
        //     conv_cnt++;
        //     conv_col_cnt++;
        //     if (conv_col_cnt == CONV0_IMG_WIDTH) {
        //         printf("\tlb : %d / conv : %d / max : %d at layer0\n", lb_conv_valid0, conv_valid0, lb_max_valid0);
        //         conv_row_cnt++;
        //         conv_col_cnt = 0;
        //         if (conv_row_cnt == CONV0_IMG_HEIGHT) break;
        //     }
        // }

        if (valid2) {
            printf("%3d ", layer_out1);
            max_cnt++;
            max_col_cnt++;
            if (max_col_cnt == MAX_POOL0_IMG_WIDTH) {
                // printf("\tlb : %d / conv : %d / max : %d at layer0\n", lb_conv_valid0, conv_valid0, lb_max_valid0);
                printf("\n");
                max_row_cnt++;
                max_col_cnt = 0;
                if (max_row_cnt == MAX_POOL0_IMG_HEIGHT) break;
            }
        }

        i++;
        if (i >= INPUT_IMG_SIZE)
        {
            i = INPUT_IMG_SIZE;
            conv_en = 0;
        }
    }

    // printf("\nConvolution Calulate for %d times\n", conv_cnt);
    printf("\nMax pooling Calulate for %d times\n", max_cnt);

    return 0;
}