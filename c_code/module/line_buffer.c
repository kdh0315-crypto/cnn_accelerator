#include "line_buffer.h"

// Initialize line buf
void line_buf_init(line_buf_t *ctx, uint32_t img_width, uint32_t img_height, uint32_t kernel_size)
{
    ctx->img_width = img_width;
    ctx->img_height = img_height;
    ctx->kernel_size = kernel_size;
}

// reset operation
void line_buf_reset(line_buf_t *ctx)
{
    ctx->row_cnt = 0;
    ctx->col_cnt = 0;
    for (int i = 0; i < ctx->img_width; i++) 
    {
        ctx->lb0[i] = 0;
        ctx->lb1[i] = 0;
    }
    for (int i = 0; i < ctx->kernel_size*ctx->kernel_size; i++)
    {
        ctx->win[i] = 0;
    }
}

void line_buf_push(line_buf_t *ctx, uint8_t img_data_in, uint8_t *win_out, uint8_t *lb_valid)
{
    // first row
    ctx->win[0] = ctx->win[1];
    ctx->win[1] = ctx->win[2];
    ctx->win[2] = ctx->lb0[ctx->col_cnt];
    
    // second row
    ctx->win[3] = ctx->win[4];
    ctx->win[4] = ctx->win[5];
    ctx->win[5] = ctx->lb1[ctx->col_cnt];

    // last row
    ctx->win[6] = ctx->win[7];
    ctx->win[7] = ctx->win[8];
    ctx->win[8] = img_data_in;

    // Update line buffer
    ctx->lb0[ctx->col_cnt] = ctx->lb1[ctx->col_cnt];
    ctx->lb1[ctx->col_cnt] = img_data_in;

    // Make valid signal to check data is available
    *lb_valid = (ctx->row_cnt >= ctx->kernel_size-1) && (ctx->col_cnt >= ctx->kernel_size-1);

    // Column count
    ctx->col_cnt++;
    if (ctx->col_cnt == ctx->img_width)
    {
        ctx->col_cnt = 0;
        ctx->row_cnt++;
    }

    // Output logic
    for (int i = 0; i < ctx->kernel_size*ctx->kernel_size; i++)
    {
        win_out[i] = ctx->win[i];
    }
}

void line_buf_op(line_buf_t *ctx, uint8_t img_data_in[][DEFAULT_IMG_WIDTH], uint8_t *win_out, uint8_t *lb_valid)
{
    for (int y = 0; y < ctx->img_height; y++)
    {
        for (int x = 0; x < ctx->img_width; x++)
        {
            line_buf_push(ctx, img_data_in[y][x], win_out, lb_valid);
        }
    }
}