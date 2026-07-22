#include <stdint.h>
#include "parameter.h"
#include "activation_fn.h"

#define SCALE_FACTOR 8

typedef struct {
    // output port
    uint8_t filter_out;
    uint8_t filter_valid;
    
    // inner wire
    int32_t c_mac;
    int32_t n_mac;
    int32_t n_valid;
} convFilter_t;

void convFilter_Reset(convFilter_t *ctx);
void convFilter(convFilter_t *ctx, uint8_t *filter_in, int8_t *weight, int8_t bias, uint8_t lb_valid, uint8_t *filter_out, uint8_t *filter_valid);
