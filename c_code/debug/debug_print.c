#include "debug_print.h"

// debug: print a row-major uint8_t matrix of fixed width and actual (possibly unexpected) height
void print_matrix(uint8_t *mat, int width, int height, int expected_height, const char *label)
{
    printf("\n--- %s : %d x %d (expected height %d) ---\n", label, width, height, expected_height);
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            printf("%3d ", mat[r*width + c]);
        }
        printf("\n");
    }
}


void LayerProbe_Init(LayerProbe_t *ctx, int filter_width, int pool_width)
{
    ctx->filter_buf = NULL;
    ctx->filter_width = filter_width;
    ctx->filter_rows = 0;
    ctx->filter_col_cnt = 0;
    ctx->filter_capacity = 0;

    ctx->pool_buf = NULL;
    ctx->pool_width = pool_width;
    ctx->pool_rows = 0;
    ctx->pool_col_cnt = 0;
    ctx->pool_capacity = 0;
}

// call once per main-loop cycle for the layer being probed
void LayerProbe_Step(LayerProbe_t *ctx, conv2D_t *layer, uint8_t layer_out, uint8_t layer_valid)
{
    // pre-pool (conv/filter) output, exposed via the public conv2D_t.filter fields
    if (layer->filter.filter_valid) {
        if (ctx->filter_rows + 1 > ctx->filter_capacity) {
            ctx->filter_capacity = ctx->filter_rows + 1;
            ctx->filter_buf = realloc(ctx->filter_buf, ctx->filter_capacity * ctx->filter_width * sizeof(uint8_t));
        }
        ctx->filter_buf[ctx->filter_rows * ctx->filter_width + ctx->filter_col_cnt] = layer->filter.filter_out;
        ctx->filter_col_cnt++;
        if (ctx->filter_col_cnt == ctx->filter_width) {
            ctx->filter_rows++;
            ctx->filter_col_cnt = 0;
        }
    }

    // post-pool (final layer) output
    if (layer_valid) {
        if (ctx->pool_rows + 1 > ctx->pool_capacity) {
            ctx->pool_capacity = ctx->pool_rows + 1;
            ctx->pool_buf = realloc(ctx->pool_buf, ctx->pool_capacity * ctx->pool_width * sizeof(uint8_t));
        }
        ctx->pool_buf[ctx->pool_rows * ctx->pool_width + ctx->pool_col_cnt] = layer_out;
        ctx->pool_col_cnt++;
        if (ctx->pool_col_cnt == ctx->pool_width) {
            ctx->pool_rows++;
            ctx->pool_col_cnt = 0;
        }
    }
}

// call once after the main loop finishes
void LayerProbe_Print(LayerProbe_t *ctx, const char *name, int expected_filter_height, int expected_pool_height)
{
    char label[64];
    snprintf(label, sizeof(label), "%s Conv (pre-pool) output", name);
    print_matrix(ctx->filter_buf, ctx->filter_width, ctx->filter_rows, expected_filter_height, label);

    snprintf(label, sizeof(label), "%s MaxPool (post-pool) output", name);
    print_matrix(ctx->pool_buf, ctx->pool_width, ctx->pool_rows, expected_pool_height, label);
}

void LayerProbe_Free(LayerProbe_t *ctx)
{
    free(ctx->filter_buf);
    free(ctx->pool_buf);
    ctx->filter_buf = NULL;
    ctx->pool_buf = NULL;
}
