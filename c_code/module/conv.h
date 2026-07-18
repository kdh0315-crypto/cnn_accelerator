#include <stdint.h>
#include "parameter.h"
#include "activation_fn.h"

#define SCALE_FACTOR 8

void conv_reset();
void conv3x3(uint8_t *cnn_in, int8_t *weight, int8_t bias, uint8_t lb_valid, uint8_t *conv_out);