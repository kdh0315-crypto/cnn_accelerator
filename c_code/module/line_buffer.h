#include <stdint.h>
#include "parameter.h"

void line_buf_reset(void);
void line_buf_push(uint8_t img_data_in, uint8_t *win_out, uint8_t *lb_valid);
void line_buf_op(uint8_t img_data_in[][IMG_WIDTH], uint8_t *win_out, uint8_t *lb_valid);