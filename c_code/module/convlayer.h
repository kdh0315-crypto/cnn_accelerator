#include <stdint.h>
#include "parameter.h"

#include "line_buffer.h"
#include "conv.h"
#include "pooling.h"

typedef struct {
    lineBuf_Conv_t lb_conv;
    conv3x3_t conv;
    lineBuf_Maxpool_t lb_max;
} ConvLayer_t;

void ConvLayer_Init(ConvLayer_t *ctx, uint8_t conv_width, uint8_t conv_height, uint8_t max_width, uint8_t max_height);
void ConvLayer_Reset(ConvLayer_t *ctx);
void ConvLayer(ConvLayer_t *ctx, uint8_t img_input, int8_t *weight, int8_t bias, uint8_t conv_en, uint8_t *layer_out, uint8_t *valid);
