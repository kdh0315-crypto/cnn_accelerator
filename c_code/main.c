#include <stdio.h>
#include <stdint.h>

// module
#include "module/line_buffer.h"

void cnn_reset()
{

}

void cnn(uint8_t cnn_in, uint8_t weight, uint8_t bias)
{
    
}


// Test
static uint8_t img[IMG_HEIGHT][IMG_WIDTH];

int main(void)
{
    // window & line buffer valid signal
    uint8_t win_out[KERNEL_SIZE * KERNEL_SIZE];
    uint8_t lb_valid;

    // 확인용: 각 픽셀에 (행*100 + 열) 넣기
    for (int y = 0; y < IMG_HEIGHT; y++)
        for (int x = 0; x < IMG_WIDTH; x++)
            img[y][x] = (uint8_t)(y*10 + x);   // 8bit라 대충

    line_buf_reset();

    for (int y = 0; y < IMG_HEIGHT; y++) {
        for (int x = 0; x < IMG_WIDTH; x++) {
            line_buf_push(img[y][x], win_out, &lb_valid);
            if (lb_valid) {
                // 윈도우 중심 = (y-1, x-1)
                if (y == 2 && x == 3) {   // 첫 유효 윈도우
                    printf("center=(%d,%d)\n", y-1, x-1);
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++)
                            printf("%3d ", win_out[i*3+j]);
                        printf("\n");
                    }
                }
            }
        }
    }
    return 0;
}