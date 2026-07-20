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
        ctx->win_out[i] = 0;
    }
}

void line_buf_op(line_buf_t *ctx, uint8_t img_data_in, uint8_t lb_start, uint8_t *win_out, uint8_t *lb_valid)
{
    // -------------------------------------
    // Combinational
    // -------------------------------------
    ctx->lb_start  = lb_start;
    ctx->n_row_cnt = ctx->row_cnt;
    ctx->n_col_cnt = ctx->col_cnt;
    
    // next window value
    // first row
    ctx->next_win[0] = ctx->win_out[1];
    ctx->next_win[1] = ctx->win_out[2];
    ctx->next_win[2] = ctx->lb0[ctx->col_cnt];
    // second row
    ctx->next_win[3] = ctx->win_out[4];
    ctx->next_win[4] = ctx->win_out[5];
    ctx->next_win[5] = ctx->lb1[ctx->col_cnt];
    // last row
    ctx->next_win[6] = ctx->win_out[7];
    ctx->next_win[7] = ctx->win_out[8];
    ctx->next_win[8] = img_data_in;
    
    // next line buffer
    ctx->next_lb0[ctx->col_cnt] = ctx->lb1[ctx->col_cnt];
    ctx->next_lb1[ctx->col_cnt] = img_data_in;
    
    // column count
    ctx->n_col_cnt = ctx->col_cnt + 1;
    if (ctx->col_cnt == ctx->img_width - 1) {
        ctx->n_col_cnt = 0;
        ctx->n_row_cnt = ctx->row_cnt + 1;
    }
    
    if (ctx->row_cnt == ctx->img_height - 1) {
        ctx->n_row_cnt = 0;
        ctx->n_col_cnt = 0;
    }
    
    // Make valid signal to check data is available
    ctx->lb_valid = (ctx->row_cnt >= ctx->kernel_size-1) && (ctx->col_cnt >= ctx->kernel_size-1);
    *lb_valid = ctx->lb_valid;


    // -------------------------------------
    // Sequential
    // -------------------------------------
    if (ctx->lb_start == 1) {
        // line buffer update
        for (int i = 0; i < ctx->img_width; i++)
        {
            ctx->lb0[i] = ctx->next_lb0[i];
            ctx->lb1[i] = ctx->next_lb1[i];
        }

        // row & col count update
        ctx->row_cnt = ctx->n_row_cnt;
        ctx->col_cnt = ctx->n_col_cnt;

        // window update
        for (int i = 0; i < ctx->kernel_size*ctx->kernel_size; i++)
        {
            ctx->win_out[i] = ctx->next_win[i];
            win_out[i] = ctx->win_out[i];
        }
    }
}