#include "convlayer.h"

void ConvLayer_Init(ConvLayer_t *ctx, uint8_t conv_width, uint8_t conv_height, uint8_t max_width, uint8_t max_height)
{
    // Init
    lineBuf_Conv_init(&ctx->lb_conv, conv_width, conv_height);
    lineBuf_Maxpool_init(&ctx->lb_max, max_width, max_height);
}

void ConvLayer_Reset(ConvLayer_t *ctx)
{
    // Reset
    lineBuf_Conv_reset(&ctx->lb_conv);
    conv_reset(&ctx->conv);
    lineBuf_Maxpool_reset(&ctx->lb_max);
}

void ConvLayer(ConvLayer_t *ctx, uint8_t img_input, int8_t *weight, int8_t bias, uint8_t conv_en, uint8_t *layer_out, uint8_t *valid)
{
    // -------------------------
    // inner wire
    // -------------------------
    uint8_t conv_out;
    uint8_t max_out;
    
    // window & valid signal
    uint8_t conv_win[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE];
    uint8_t max_win[MAX_POOL_KERNEL_SIZE * MAX_POOL_KERNEL_SIZE];
    
    uint8_t lb_conv_valid;
    uint8_t conv_valid;
    uint8_t lb_max_valid;
    
    // -------------------------
    // Module
    // -------------------------
    lineBuf_Conv(&ctx->lb_conv, img_input, conv_en, conv_win, &lb_conv_valid);
    conv3x3(&ctx->conv, conv_win, weight, bias, lb_conv_valid, &conv_out, &conv_valid);
    lineBuf_Maxpool(&ctx->lb_max, conv_out, conv_valid, max_win, &lb_max_valid);
    max_pool(max_win, lb_max_valid, &max_out);

    // -------------------------
    // Output port
    // -------------------------
    *layer_out = max_out;
    *valid = lb_max_valid;
}