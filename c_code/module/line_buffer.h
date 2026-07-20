#include <stdint.h>
#include "parameter.h"

typedef enum {
    IDLE,
    START,
    DATA,
    STOP
} line_buf_state_t;

typedef struct {
    // input port
    uint8_t lb_start;
    // output port
    uint8_t win_out[MAX_KERNEL_SIZE * MAX_KERNEL_SIZE];
    uint8_t lb_valid;
    
    // inner wire
    uint8_t next_win[MAX_KERNEL_SIZE * MAX_KERNEL_SIZE];
    uint8_t lb0[MAX_IMG_WIDTH];
    uint8_t next_lb0[MAX_IMG_WIDTH];
    uint8_t lb1[MAX_IMG_WIDTH];
    uint8_t next_lb1[MAX_IMG_WIDTH];

    uint8_t row_cnt;
    uint8_t n_row_cnt;
    uint8_t col_cnt;
    uint8_t n_col_cnt;
    
    // Parameter
    uint8_t img_width;
    uint8_t img_height;
    uint8_t kernel_size;

    // State parameter
    line_buf_state_t c_state;
    line_buf_state_t n_state;
} line_buf_t;

void line_buf_init(line_buf_t *ctx, uint32_t img_width, uint32_t img_height, uint32_t kernel_size);
void line_buf_reset(line_buf_t *ctx);
void line_buf_op(line_buf_t *ctx, uint8_t img_data_in, uint8_t *win_out, uint8_t *lb_valid);