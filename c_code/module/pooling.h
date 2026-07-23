#ifndef POOLING_H
#define POOLING_H

#include <stdint.h>
#include "parameter.h"
#include "line_buffer.h"

void max_pool(uint8_t *max_in, uint8_t *max_out);

// ----------------------------------
// 1 Channel Max Pooling Layer
// ----------------------------------
typedef struct {
    lineBuf_Maxpool_t lb_max;
} pooling2D_t;

void max_pooling2D_Init(pooling2D_t *ctx, uint8_t max_width, uint8_t max_height);
void max_pooling2D_Reset(pooling2D_t *ctx);
void max_pooling2D(pooling2D_t *ctx, uint8_t max_in, uint8_t max_pool_en, uint8_t *max_valid, uint8_t *max_pool_out);


#endif