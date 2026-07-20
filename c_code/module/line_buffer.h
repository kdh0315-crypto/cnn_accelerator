#include <stdint.h>
#include "parameter.h"

typedef struct {
    uint8_t win[MAX_KERNEL_SIZE * MAX_KERNEL_SIZE];
    uint8_t lb0[MAX_IMG_WIDTH];
    uint8_t lb1[MAX_IMG_WIDTH];
    uint8_t row_cnt;
    uint8_t col_cnt;
    uint8_t img_width;
    uint8_t img_height;
    uint8_t kernel_size;
} line_buf_t;

void line_buf_init(line_buf_t *ctx, uint32_t img_width, uint32_t img_height, uint32_t kernel_size);
void line_buf_reset(line_buf_t *ctx);
void line_buf_push(line_buf_t *ctx, uint8_t img_data_in, uint8_t *win_out, uint8_t *lb_valid);
void line_buf_op(line_buf_t *ctx, uint8_t img_data_in[][MAX_IMG_WIDTH], uint8_t *win_out, uint8_t *lb_valid);