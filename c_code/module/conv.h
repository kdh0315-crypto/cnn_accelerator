#include <stdint.h>
#include "parameter.h"
#include "activation_fn.h"

#define SCALE_FACTOR 8

typedef struct {
    // output port
    uint8_t conv_out;
    uint8_t conv_valid;
    
    // inner wire
    int32_t c_mac;
    int32_t n_mac;
    int32_t n_valid;
} conv3x3_t;

void conv_reset(conv3x3_t *ctx);
void conv3x3(conv3x3_t *ctx, uint8_t *cnn_in, int8_t *weight, int8_t bias, uint8_t lb_valid, uint8_t *conv_out, uint8_t *conv_valid);