#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../module/parameter.h"

// safety upper bound on total main-loop cycles, so a bug that keeps re-asserting
// valid forever cannot hang this test harness (784 pixels + generous drain margin)
#define TOTAL_CYCLES (INPUT_IMG_SIZE + 4*INPUT_IMG_WIDTH)

// debug: capture a conv2D_t layer's pre-pool (conv/filter) stream and post-pool (final) stream
// into dynamically-growing matrices, so the real shape/values can be inspected regardless of
// whether they match the expected (compile-time) dimensions.
typedef struct {
    uint8_t *filter_buf;   // pre-pool stream, row-major, width = filter_width
    int filter_width;
    int filter_rows;       // rows filled so far
    int filter_col_cnt;    // column position within the current row
    int filter_capacity;   // rows allocated

    uint8_t *pool_buf;     // post-pool stream, row-major, width = pool_width
    int pool_width;
    int pool_rows;
    int pool_col_cnt;
    int pool_capacity;
} LayerProbe_t;

void print_window(uint8_t *win_out);

void print_matrix(uint8_t *mat, int width, int height, int expected_height, const char *label);

void LayerProbe_Init(LayerProbe_t *ctx, int filter_width, int pool_width);
void LayerProbe_Step(LayerProbe_t *ctx, uint8_t filter_out, uint8_t filter_valid, uint8_t pool_out, uint8_t pool_valid);
void LayerProbe_Print(LayerProbe_t *ctx, const char *name, int expected_filter_height, int expected_pool_height);
void LayerProbe_Free(LayerProbe_t *ctx);

#endif