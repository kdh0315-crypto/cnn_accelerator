#include <stdint.h>
#include "parameter.h"
#include "activation_fn.h"

#define SCALE_FACTOR 8

typedef struct {
    int32_t mac;
    uint32_t kernel_size;
} conv_t;

void conv_init(conv_t *ctx, uint32_t kernel_size);
void conv_reset(conv_t *ctx);
void conv3x3(conv_t *ctx, uint8_t *cnn_in, int8_t *weight, int8_t bias, uint8_t lb_valid, uint8_t *conv_out);