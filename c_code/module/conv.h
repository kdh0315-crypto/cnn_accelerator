#include <stdint.h>
#include "parameter.h"

#include "line_buffer.h"
#include "filter.h"
#include "pooling.h"

typedef struct {
    lineBuf_Filter_t lb_filter;
    convFilter_t filter;
    lineBuf_Maxpool_t lb_max;
} conv2D_t;

void conv2D_Init(conv2D_t *ctx, uint8_t conv_width, uint8_t conv_height, uint8_t max_width, uint8_t max_height);
void conv2D_Reset(conv2D_t *ctx);
void conv2D(conv2D_t *ctx, uint8_t img_input, int8_t *weight, int8_t bias, uint8_t conv_en, uint8_t *conv_out, uint8_t *conv_valid);
