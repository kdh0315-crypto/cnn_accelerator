#include "pooling.h"

void max_pool(uint8_t *max_in, uint8_t lb_valid, uint8_t *max_out)
{
    // ------------------------
    // Combinational
    // ------------------------
    uint8_t w_max0;
    uint8_t w_max1;

    if (lb_valid) {
        w_max0 = (max_in[0] > max_in[1]) ? max_in[0] : max_in[1];
        w_max1 = (max_in[2] > max_in[3]) ? max_in[2] : max_in[3];

        *max_out = (w_max0 > w_max1) ? w_max0 : w_max1;
    }
}