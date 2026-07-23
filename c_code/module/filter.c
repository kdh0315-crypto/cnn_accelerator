#include "filter.h"

void convFilter_Reset(convFilter_t *ctx)
{
    ctx->c_mac = 0;
    ctx->n_mac = 0;
}

void convFilter(convFilter_t *ctx, uint8_t *filter_in, int8_t *weight, int8_t bias, uint8_t filter_en, uint8_t *filter_out)
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

    // ----- Staging to reduce critical path delay -----

    // Activation function - ReLU
    relu(ctx->c_mac, &relu_out);

    // Scaling
    quantized = (relu_out >> SCALE_FACTOR);
    // Quantize
    if (quantized >= 255) quantized = 255;
    
    // Output logic
    ctx->filter_out = quantized;

    // -------------------------
    // Sequential
    // -------------------------
    ctx->c_mac = ctx->n_mac;
}