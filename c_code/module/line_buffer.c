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
    ctx->ctx->c_state = IDLE;
}

void line_buf_op(line_buf_t *ctx, uint8_t img_data_in, uint8_t *win_out, uint8_t *lb_valid)
{
    // -------------------------------------
    // Combinational
    // -------------------------------------
    // next state logic
    ctx->n_state  = ctx->c_state;
    ctx->next_win = ctx->win_out;
    
    switch (ctx->c_state)
    {
        case IDLE:
        {
            if (ctx->lb_start == 1) {
                ctx->n_state = DATA;
            }
        }
        case DATA:
        {
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
            if (ctx->col_cnt == ctx->img_width) {
                ctx->n_col_cnt = 0;
                ctx->n_row_cnt = ctx->row_cnt + 1;
            }

            if (ctx->row_cnt == ctx->img_height) {
                ctx->n_state = STOP;
            }
        }
        case STOP:
        {
            ctx->n_state = IDLE;
            ctx->n_row_cnt = 0;
            ctx->n_col_cnt = 0;
        }
    }
    
    // Make valid signal to check data is available
    ctx->lb_valid = (ctx->row_cnt >= ctx->kernel_size-1) && (ctx->col_cnt >= ctx->kernel_size-1);
    

    // -------------------------------------
    // Sequential
    // -------------------------------------
    // State update logic
    ctx->c_state = ctx->n_state;

    // line buffer update
    ctx->lb0 = ctx->next_lb0;
    ctx->lb1 = ctx->next_lb1;

    // row & col count update
    ctx->row_cnt = ctx->n_row_cnt;
    ctx->col_cnt = ctx->n_col_cnt;
}


// void line_buf_push(line_buf_t *ctx, uint8_t img_data_in)
// {
//     // first row
//     ctx->win_out[0] = ctx->win_out[1];
//     ctx->win_out[1] = ctx->win_out[2];
//     ctx->win_out[2] = ctx->lb0[ctx->col_cnt];
    
//     // second row
//     ctx->win_out[3] = ctx->win_out[4];
//     ctx->win_out[4] = ctx->win_out[5];
//     ctx->win_out[5] = ctx->lb1[ctx->col_cnt];

//     // last row
//     ctx->win_out[6] = ctx->win_out[7];
//     ctx->win_out[7] = ctx->win_out[8];
//     ctx->win_out[8] = img_data_in;

//     // Update line buffer
//     ctx->lb0[ctx->col_cnt] = ctx->lb1[ctx->col_cnt];
//     ctx->lb1[ctx->col_cnt] = img_data_in;

//     // Make valid signal to check data is available
//     ctx->lb_valid = (ctx->row_cnt >= ctx->kernel_size-1) && (ctx->col_cnt >= ctx->kernel_size-1);

//     // Column count
//     ctx->col_cnt++;
//     if (ctx->col_cnt == ctx->img_width)
//     {
//         ctx->col_cnt = 0;
//         ctx->row_cnt++;
//     }

//     // Output logic
//     for (int i = 0; i < ctx->kernel_size*ctx->kernel_size; i++)
//     {
//         ctx->win_out[i] = ctx->win_out[i];
//     }
// }