#include "conv.h"

void conv_reset(conv3x3_t *ctx)
{
    ctx->c_mac = 0;
    ctx->n_mac = 0;

    ctx->conv_valid = 0;
}

void conv3x3(conv3x3_t *ctx, uint8_t *cnn_in, int8_t *weight, int8_t bias, uint8_t lb_valid, uint8_t *conv_out, uint8_t *conv_valid)
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
        ctx->n_mac += (int32_t)cnn_in[i] * (int32_t)weight[i];
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
    ctx->conv_out = quantized;
    *conv_out = ctx->conv_out;
    *conv_valid = ctx->conv_valid;
    
    // -------------------------
    // Sequential
    // -------------------------
    ctx->c_mac = ctx->n_mac;
    ctx->conv_valid = ctx->n_valid;
}