#include <stdint.h>
#include "parameter.h"

typedef struct {
    // input port
    uint8_t lb_start;
    // output port
    uint8_t win_out[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE];
    uint8_t lb_valid;
    
    // inner wire
    uint8_t next_win[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE];
    uint8_t lb0[INPUT_IMG_WIDTH];
    uint8_t next_lb0[INPUT_IMG_WIDTH];
    uint8_t lb1[INPUT_IMG_WIDTH];
    uint8_t next_lb1[INPUT_IMG_WIDTH];

    uint8_t row_cnt;
    uint8_t n_row_cnt;
    uint8_t col_cnt;
    uint8_t n_col_cnt;

    // parameter
    uint8_t img_width;
    uint8_t img_height;
} lineBuf_Filter_t;

typedef struct {
    // input port
    uint8_t lb_start;
    // output port
    uint8_t win_out[MAX_POOL_KERNEL_SIZE * MAX_POOL_KERNEL_SIZE];
    uint8_t lb_valid;
    
    // inner wire
    uint8_t next_win[MAX_POOL_KERNEL_SIZE * MAX_POOL_KERNEL_SIZE];
    uint8_t lb0[INPUT_IMG_WIDTH];
    uint8_t next_lb0[INPUT_IMG_WIDTH];

    uint8_t row_cnt;
    uint8_t n_row_cnt;
    uint8_t col_cnt;
    uint8_t n_col_cnt;

    // parameter
    uint8_t img_width;
    uint8_t img_height;
} lineBuf_Maxpool_t;

void lineBuf_Maxpool_init(lineBuf_Maxpool_t *ctx, uint32_t img_width, uint32_t img_height);
void lineBuf_Maxpool_reset(lineBuf_Maxpool_t *ctx);
void lineBuf_Maxpool(lineBuf_Maxpool_t *ctx, uint8_t img_data_in, uint8_t lb_start, uint8_t *win_out, uint8_t *lb_valid);

void lineBuf_Filter_init(lineBuf_Filter_t *ctx, uint32_t img_width, uint32_t img_height);
void lineBuf_Filter_reset(lineBuf_Filter_t *ctx);
void lineBuf_Filter(lineBuf_Filter_t *ctx, uint8_t img_data_in, uint8_t lb_start, uint8_t *win_out, uint8_t *lb_valid);
