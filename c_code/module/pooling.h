#ifndef POOLING_H
#define POOLING_H

#include <stdint.h>
#include "parameter.h"

void max_pool(uint8_t *max_in, uint8_t lb_valid, uint8_t *max_out);

#endif