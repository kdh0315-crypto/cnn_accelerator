#include "conv.h"

void conv_init(conv_t *ctx, uint32_t kernel_size)
{
    ctx->kernel_size = kernel_size;
}

void conv_reset(conv_t *ctx)
{
    ctx->mac = 0;
}

void conv3x3(conv_t *ctx, uint8_t *cnn_in, int8_t *weight, int8_t bias, uint8_t lb_valid, uint8_t *conv_out)
{
    int32_t relu_out;
    int32_t quantized;

    if (lb_valid)
    {
        // reset mac value
        ctx->mac = 0;

        // MAC calculation
        for(int i = 0; i < ctx->kernel_size*ctx->kernel_size; i++)
        {
            ctx->mac += (int32_t)cnn_in[i] * (int32_t)weight[i];
        }
        
        // Bias
        ctx->mac += bias;
    }

    // Activation function - ReLU
    relu(ctx->mac, &relu_out);

    // Scaling
    quantized = (relu_out >> SCALE_FACTOR);
    // Quantize
    if (quantized >= 255) quantized = 255;
    
    // Output logic
    *conv_out = quantized;
}