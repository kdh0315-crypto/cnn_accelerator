#include "conv.h"

static int32_t mac;

void conv_reset()
{
    mac = 0;
}

void conv3x3(uint8_t *cnn_in, int8_t *weight, int8_t bias, uint8_t lb_valid, uint8_t *conv_out)
{
    int32_t relu_out;
    int32_t quantized;

    if (lb_valid)
    {
        // reset mac value
        mac = 0;

        // MAC calculation
        for(int i = 0; i < KERNEL_SIZE*KERNEL_SIZE; i++)
        {
            mac += (int32_t)cnn_in[i] * (int32_t)weight[i];
        }
        
        // Bias
        mac += bias;
    }

    // Activation function - ReLU
    relu(mac, &relu_out);

    // Scaling
    quantized = (relu_out >> SCALE_FACTOR);
    // Quantize
    if (quantized >= 255) quantized = 255;
    
    // Output logic
    *conv_out = quantized;
}