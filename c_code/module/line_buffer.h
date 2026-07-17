#include <stdint.h>

#define IMG_WIDTH 28
#define IMG_HEIGHT 28
#define IMG_SIZE (IMG_WIDTH * IMG_HEIGHT)
#define KERNEL_SIZE 3


void line_buf_reset(void);
void line_buf_push(uint8_t img_data_in, uint8_t *win_out, uint8_t *lb_valid);