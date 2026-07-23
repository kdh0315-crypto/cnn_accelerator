#ifndef CONV_H
#define CONV_H

#include <stdint.h>
#include "parameter.h"

#include "line_buffer.h"
#include "filter.h"
#include "pooling.h"

#include "../weight/weight.h"


// ----------------------------
// 6 Channel Convolution
// ----------------------------
typedef struct {
    // value for sequential logic
    uint8_t conv_valid;
    uint8_t n_valid;

    // module
    lineBuf_Filter_t lb_filter;
    convFilter_t filter0;
    convFilter_t filter1;
    convFilter_t filter2;
    convFilter_t filter3;
    convFilter_t filter4;
    convFilter_t filter5;
} conv_6ch_t;

void conv_6ch_Init(conv_6ch_t *ctx, uint8_t filter_width, uint8_t filter_height);
void conv_6ch_Reset(conv_6ch_t *ctx);
void conv_6ch(conv_6ch_t *ctx, uint8_t img_input, const int8_t weight[][CNN_KERNEL_SIZE*CNN_KERNEL_SIZE], const int8_t bias[], uint8_t conv_en, uint8_t *conv_out, uint8_t *conv_valid);


// ----------------------------
// 1 Channel Convolution
// ----------------------------
typedef struct {
    // value for sequential logic
    uint8_t conv_valid;
    uint8_t n_valid;
    
    // module
    lineBuf_Filter_t lb_filter;
    convFilter_t filter;
} conv2D_t;

void conv2D_Init(conv2D_t *ctx, uint8_t conv_width, uint8_t conv_height);
void conv2D_Reset(conv2D_t *ctx);
void conv2D(conv2D_t *ctx, uint8_t img_input, int8_t *weight, int8_t bias, uint8_t conv_en, uint8_t *conv_out, uint8_t *conv_valid);

#endif