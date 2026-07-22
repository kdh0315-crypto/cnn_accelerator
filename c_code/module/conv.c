#include "conv.h"

void conv2D_Init(conv2D_t *ctx, uint8_t conv_width, uint8_t conv_height, uint8_t max_width, uint8_t max_height)
{
    // Init
    lineBuf_Filter_init(&ctx->lb_filter, conv_width, conv_height);
    lineBuf_Maxpool_init(&ctx->lb_max, max_width, max_height);
}

void conv2D_Reset(conv2D_t *ctx)
{
    // Reset
    lineBuf_Filter_reset(&ctx->lb_filter);
    convFilter_Reset(&ctx->filter);
    lineBuf_Maxpool_reset(&ctx->lb_max);
}

void conv2D(conv2D_t *ctx, uint8_t img_input, int8_t *weight, int8_t bias, uint8_t conv_en, uint8_t *conv_out, uint8_t *conv_valid)
{
    // -------------------------
    // inner wire
    // -------------------------
    uint8_t filter_out;
    uint8_t max_out;
    
    // window & conv_valid signal
    uint8_t filter_win[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE];
    uint8_t max_win[MAX_POOL_KERNEL_SIZE * MAX_POOL_KERNEL_SIZE];
    
    uint8_t lb_filter_valid;
    uint8_t filter_valid;
    uint8_t lb_max_valid;
    
    // -------------------------
    // Module
    // -------------------------
    lineBuf_Filter(&ctx->lb_filter, img_input, conv_en, filter_win, &lb_filter_valid);
    convFilter(&ctx->filter, filter_win, weight, bias, lb_filter_valid, &filter_out, &filter_valid);
    lineBuf_Maxpool(&ctx->lb_max, filter_out, filter_valid, max_win, &lb_max_valid);
    max_pool(max_win, lb_max_valid, &max_out);

    // -------------------------
    // Output port
    // -------------------------
    *conv_out = max_out;
    *conv_valid = lb_max_valid;
}