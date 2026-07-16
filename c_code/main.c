#include <stdio.h>
#include <stdint.h>

#define IMG_WIDTH 28
#define IMG_HEIGHT 28
#define IMG_SIZE (IMG_WIDTH * IMG_HEIGHT)
#define KERNEL_SIZE 3

uint8_t win[KERNEL_SIZE * KERNEL_SIZE];
uint8_t lb0[IMG_WIDTH];
uint8_t lb1[IMG_WIDTH];
uint8_t lb_valid;

uint8_t row_cnt;
uint8_t col_cnt;

void line_buf_reset()
{
    row_cnt = 0;
    col_cnt = 0;
    for (int i = 0; i < IMG_WIDTH; i++) 
    {
        lb0[i] = 0;
    }
    for (int i = 0; i < KERNEL_SIZE*KERNEL_SIZE; i++)
    {
        win[i] = 0;
    }
}

void line_buf_push(uint8_t img_data_in)
{
    // first row
    win[0] = win[1];
    win[1] = win[2];
    win[2] = lb0[col_cnt];
    
    // second row
    win[3] = win[4];
    win[4] = win[5];
    win[5] = lb1[col_cnt];

    // last row
    win[6] = win[7];
    win[7] = win[8];
    win[8] = img_data_in;

    // Update line buffer
    lb0[col_cnt] = lb1[col_cnt];
    lb1[col_cnt] = img_data_in;

    // Make valid signal to check data is available
    lb_valid = (row_cnt >= KERNEL_SIZE-1) && (col_cnt >= KERNEL_SIZE-1);

    // Column count
    col_cnt++;
    if (col_cnt == IMG_WIDTH)
    {
        col_cnt = 0;
        row_cnt++;
    }
}

// int main(void)
// {
//     return 0;
// }

static uint8_t img[IMG_HEIGHT][IMG_WIDTH];

int main(void)
{
    // 확인용: 각 픽셀에 (행*100 + 열) 넣기
    for (int y = 0; y < IMG_HEIGHT; y++)
        for (int x = 0; x < IMG_WIDTH; x++)
            img[y][x] = (uint8_t)(y*10 + x);   // 8bit라 대충

    line_buf_reset();

    for (int y = 0; y < IMG_HEIGHT; y++) {
        for (int x = 0; x < IMG_WIDTH; x++) {
            line_buf_push(img[y][x]);
            if (lb_valid) {
                // 윈도우 중심 = (y-1, x-1)
                if (y == 2 && x == 3) {   // 첫 유효 윈도우
                    printf("center=(%d,%d)\n", y-1, x-1);
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++)
                            printf("%3d ", win[i*3+j]);
                        printf("\n");
                    }
                    return 0;
                }
            }
        }
    }
    return 0;
}