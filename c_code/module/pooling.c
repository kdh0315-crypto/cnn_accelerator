#include "pooling.h"

void max_pool(uint8_t *max_in, uint8_t *max_out)
{
    // ------------------------
    // Combinational
    // ------------------------
    uint8_t w_max0;
    uint8_t w_max1;

    w_max0 = (max_in[0] > max_in[1]) ? max_in[0] : max_in[1];
    w_max1 = (max_in[2] > max_in[3]) ? max_in[2] : max_in[3];

    *max_out = (w_max0 > w_max1) ? w_max0 : w_max1;
}


void max_pooling2D_Init(pooling2D_t *ctx, uint8_t max_width, uint8_t max_height)
{
    lineBuf_Maxpool_init(&ctx->lb_max, max_width, max_height);
}

void max_pooling2D_Reset(pooling2D_t *ctx)
{
    lineBuf_Maxpool_reset(&ctx->lb_max);
}

void max_pooling2D(pooling2D_t *ctx, uint8_t max_in, uint8_t max_pool_en, uint8_t *max_valid, uint8_t *max_pool_out)
{
    uint8_t max_win[MAX_POOL_KERNEL_SIZE * MAX_POOL_KERNEL_SIZE];

    lineBuf_Maxpool(&ctx->lb_max, max_in, max_pool_en, max_win, max_valid);
    max_pool(max_win, max_pool_out);
}