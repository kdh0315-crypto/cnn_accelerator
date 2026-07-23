#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include "parameter.h"
#include "activation_fn.h"

#define SCALE_FACTOR 8

typedef struct {
    // output port
    uint8_t filter_out;
    
    // inner wire
    int32_t c_mac;
    int32_t n_mac;
} convFilter_t;

void convFilter_Reset(convFilter_t *ctx);
void convFilter(convFilter_t *ctx, uint8_t *filter_in, int8_t *weight, int8_t bias, uint8_t filter_en, uint8_t *filter_out);

#endif