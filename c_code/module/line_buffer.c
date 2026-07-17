#include "line_buffer.h"

// inner wire
static uint8_t win[KERNEL_SIZE * KERNEL_SIZE];
static uint8_t lb0[IMG_WIDTH];
static uint8_t lb1[IMG_WIDTH];
static uint8_t row_cnt;
static uint8_t col_cnt;

// reset operation
void line_buf_reset(void)
{
    row_cnt = 0;
    col_cnt = 0;
    for (int i = 0; i < IMG_WIDTH; i++) 
    {
        lb0[i] = 0;
        lb1[i] = 0;
    }
    for (int i = 0; i < KERNEL_SIZE*KERNEL_SIZE; i++)
    {
        win[i] = 0;
    }
}

// Main opeeration
void line_buf_push(uint8_t img_data_in, uint8_t *win_out, uint8_t *lb_valid)
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
    *lb_valid = (row_cnt >= KERNEL_SIZE-1) && (col_cnt >= KERNEL_SIZE-1);

    // Column count
    col_cnt++;
    if (col_cnt == IMG_WIDTH)
    {
        col_cnt = 0;
        row_cnt++;
    }

    // Output logic
    for (int i = 0; i < KERNEL_SIZE*KERNEL_SIZE; i++)
    {
        win_out[i] = win[i];
    }
}