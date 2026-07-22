#include "filter.h"

void convFilter_Reset(convFilter_t *ctx)
{
    ctx->c_mac = 0;
    ctx->n_mac = 0;

    ctx->filter_valid = 0;
}

void convFilter(convFilter_t *ctx, uint8_t *filter_in, int8_t *weight, int8_t bias, uint8_t lb_valid, uint8_t *filter_out, uint8_t *filter_valid)
{
    // -------------------------
    // Combinational
    // -------------------------
    int32_t relu_out;
    int32_t quantized;

    // reset n_mac value - not needed at verilog
    ctx->n_mac = 0;
    
    // MAC calculation
    for(int i = 0; i < CNN_KERNEL_SIZE*CNN_KERNEL_SIZE; i++)
    {
        ctx->n_mac += (int32_t)filter_in[i] * (int32_t)weight[i];
    }
    
    // Bias
    ctx->n_mac += bias;

    // valid signal
    ctx->n_valid = lb_valid;

    // ----- Staging to reduce critical path delay -----

    // Activation function - ReLU
    relu(ctx->c_mac, &relu_out);

    // Scaling
    quantized = (relu_out >> SCALE_FACTOR);
    // Quantize
    if (quantized >= 255) quantized = 255;
    
    // Output logic
    ctx->filter_out = quantized;
    *filter_out = ctx->filter_out;
    *filter_valid = ctx->filter_valid;
    
    // -------------------------
    // Sequential
    // -------------------------
    ctx->c_mac = ctx->n_mac;
    ctx->filter_valid = ctx->n_valid;
}