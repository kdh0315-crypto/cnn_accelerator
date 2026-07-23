#include "conv.h"


// -----------------------
// 16 Channel Convolution
// -----------------------
typedef struct {
    // value for sequential logic
    uint8_t conv_valid;
    uint8_t n_valid;

    // module
    lineBuf_Filter_t lb_filter;
    convFilter_t filter0;
    convFilter_t filter1;
    convFilter_t filter2;
    convFilter_t filter3;
    convFilter_t filter4;
    convFilter_t filter5;
    convFilter_t filter6;
    convFilter_t filter7;
    convFilter_t filter8;
    convFilter_t filter9;
    convFilter_t filter10;
    convFilter_t filter11;
    convFilter_t filter12;
    convFilter_t filter13;
    convFilter_t filter14;
    convFilter_t filter15;
} conv_16ch_t;

void conv_16ch_Init(conv_16ch_t *ctx, uint8_t filter_width, uint8_t filter_height)
{
    // Init
    lineBuf_Filter_init(&ctx->lb_filter, filter_width, filter_height);
}

void conv_16ch_Reset(conv_16ch_t *ctx)
{
    // sequential value
    ctx->conv_valid = 0;

    lineBuf_Filter_reset(&ctx->lb_filter);
    
    convFilter_Reset(&ctx->filter0);
    convFilter_Reset(&ctx->filter1);
    convFilter_Reset(&ctx->filter2);
    convFilter_Reset(&ctx->filter3);
    convFilter_Reset(&ctx->filter4);
    convFilter_Reset(&ctx->filter5);
    convFilter_Reset(&ctx->filter6);
    convFilter_Reset(&ctx->filter7);
    convFilter_Reset(&ctx->filter8);
    convFilter_Reset(&ctx->filter9);
    convFilter_Reset(&ctx->filter10);
    convFilter_Reset(&ctx->filter11);
    convFilter_Reset(&ctx->filter12);
    convFilter_Reset(&ctx->filter13);
    convFilter_Reset(&ctx->filter14);
    convFilter_Reset(&ctx->filter15);
}

void conv_16ch(conv_16ch_t *ctx, uint8_t img_input, const int8_t weight[][CNN_KERNEL_SIZE*CNN_KERNEL_SIZE], const int8_t bias[], uint8_t conv_en, uint8_t *conv_out, uint8_t *conv_valid)
{
    // -------------------------
    // inner wire
    // -------------------------
    uint8_t temp_valid = 0; // no exist in verilog code - just to make 1 clk delay to valid signal
    
    // filter out values
    uint8_t filter_out0;
    uint8_t filter_out1;
    uint8_t filter_out2;
    uint8_t filter_out3;
    uint8_t filter_out4;
    uint8_t filter_out5;
    uint8_t filter_out6;
    uint8_t filter_out7;
    uint8_t filter_out8;
    uint8_t filter_out9;
    uint8_t filter_out10;
    uint8_t filter_out11;
    uint8_t filter_out12;
    uint8_t filter_out13;
    uint8_t filter_out14;
    uint8_t filter_out15;
    
    // filter window
    uint8_t filter_win[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE];
    
    // filter enable signal
    uint8_t filter_en;
    
    
    lineBuf_Filter(&ctx->lb_filter, img_input, conv_en, filter_win, &filter_en);
    
    convFilter(&ctx->filter0,  filter_win, weight[0],  bias[0],  filter_en, &filter_out0);
    convFilter(&ctx->filter1,  filter_win, weight[1],  bias[1],  filter_en, &filter_out1);
    convFilter(&ctx->filter2,  filter_win, weight[2],  bias[2],  filter_en, &filter_out2);
    convFilter(&ctx->filter3,  filter_win, weight[3],  bias[3],  filter_en, &filter_out3);
    convFilter(&ctx->filter4,  filter_win, weight[4],  bias[4],  filter_en, &filter_out4);
    convFilter(&ctx->filter5,  filter_win, weight[5],  bias[5],  filter_en, &filter_out5);
    convFilter(&ctx->filter6,  filter_win, weight[6],  bias[6],  filter_en, &filter_out6);
    convFilter(&ctx->filter7,  filter_win, weight[7],  bias[7],  filter_en, &filter_out7);
    convFilter(&ctx->filter8,  filter_win, weight[8],  bias[8],  filter_en, &filter_out8);
    convFilter(&ctx->filter9,  filter_win, weight[9],  bias[9],  filter_en, &filter_out9);
    convFilter(&ctx->filter10, filter_win, weight[10], bias[10], filter_en, &filter_out10);
    convFilter(&ctx->filter11, filter_win, weight[11], bias[11], filter_en, &filter_out11);
    convFilter(&ctx->filter12, filter_win, weight[12], bias[12], filter_en, &filter_out12);
    convFilter(&ctx->filter13, filter_win, weight[13], bias[13], filter_en, &filter_out13);
    convFilter(&ctx->filter14, filter_win, weight[14], bias[14], filter_en, &filter_out14);
    convFilter(&ctx->filter15, filter_win, weight[15], bias[15], filter_en, &filter_out15);
    
    // output logic
    *conv_out = filter_out0 + filter_out1 + filter_out2 + filter_out3 + filter_out4
                + filter_out5 + filter_out6 + filter_out7 + filter_out8 + filter_out9
                + filter_out10 + filter_out11 + filter_out12 + filter_out13 + filter_out14
                + filter_out15;

    // valid signal
    ctx->n_valid = conv_en;
    
    // -------------------------
    // Sequential logic 
    // -------------------------
    *conv_valid = temp_valid;
    ctx->conv_valid = *conv_valid;
    temp_valid = ctx->n_valid; // no exist in verilog code for the same reason of temp_valid
}


// -----------------------
// 6 Channel Convolution
// -----------------------

typedef struct {
    // value for sequential logic
    uint8_t conv_valid;
    uint8_t n_valid;
    
    // module
    lineBuf_Filter_t lb_filter;
    convFilter_t filter0;
    convFilter_t filter1;
    convFilter_t filter2;
    convFilter_t filter3;
    convFilter_t filter4;
    convFilter_t filter5;
} conv_6ch_t;


void conv_6ch_Init(conv_6ch_t *ctx, uint8_t filter_width, uint8_t filter_height)
{
    // Init
    lineBuf_Filter_init(&ctx->lb_filter, filter_width, filter_height);
}

void conv_6ch_Reset(conv_6ch_t *ctx)
{
    // sequential value
    ctx->conv_valid = 0;

    lineBuf_Filter_reset(&ctx->lb_filter);
    
    convFilter_Reset(&ctx->filter0);
    convFilter_Reset(&ctx->filter1);
    convFilter_Reset(&ctx->filter2);
    convFilter_Reset(&ctx->filter3);
    convFilter_Reset(&ctx->filter4);
    convFilter_Reset(&ctx->filter5);
}

void conv_6ch(conv_6ch_t *ctx, uint8_t img_input, const int8_t weight[][CNN_KERNEL_SIZE*CNN_KERNEL_SIZE], const int8_t bias[], uint8_t conv_en,
                uint8_t *conv_out0, uint8_t *conv_out1, uint8_t *conv_out2, uint8_t *conv_out3, uint8_t *conv_out4, uint8_t *conv_out5, uint8_t *conv_valid)
{
    // -------------------------
    // inner wire
    // -------------------------
    uint8_t temp_valid = 0; // no exist in verilog code - just to make 1 clk delay to valid signal
    
    // filter window
    uint8_t filter_win[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE];
    
    // filter enable signal
    uint8_t filter_en;
    
    
    lineBuf_Filter(&ctx->lb_filter, img_input, conv_en, filter_win, &filter_en);
    
    convFilter(&ctx->filter0, filter_win, weight[0], bias[0], filter_en, conv_out0);
    convFilter(&ctx->filter1, filter_win, weight[1], bias[1], filter_en, conv_out1);
    convFilter(&ctx->filter2, filter_win, weight[2], bias[2], filter_en, conv_out2);
    convFilter(&ctx->filter3, filter_win, weight[3], bias[3], filter_en, conv_out3);
    convFilter(&ctx->filter4, filter_win, weight[4], bias[4], filter_en, conv_out4);
    convFilter(&ctx->filter5, filter_win, weight[5], bias[5], filter_en, conv_out5);

    // valid signal
    ctx->n_valid = conv_en;
    
    // -------------------------
    // Sequential logic 
    // -------------------------
    *conv_valid = temp_valid;
    ctx->conv_valid = *conv_valid;
    temp_valid = ctx->n_valid; // no exist in verilog code for the same reason of temp_valid
}


// -----------------------
// 1 Channel Convolution
// -----------------------
void conv2D_Init(conv2D_t *ctx, uint8_t conv_width, uint8_t conv_height)
{
    // Init
    lineBuf_Filter_init(&ctx->lb_filter, conv_width, conv_height);
}

void conv2D_Reset(conv2D_t *ctx)
{
    // Reset
    lineBuf_Filter_reset(&ctx->lb_filter);
    convFilter_Reset(&ctx->filter);
}

void conv2D(conv2D_t *ctx, uint8_t img_input, int8_t *weight, int8_t bias, uint8_t conv_en, uint8_t *conv_out, uint8_t *conv_valid)
{
    // -------------------------
    // inner wire
    // -------------------------
    // window & conv_valid signal
    uint8_t filter_win[CNN_KERNEL_SIZE * CNN_KERNEL_SIZE];
    uint8_t filter_en;

    uint8_t temp_valid; // no exist in verilog code - just to make 1 clk delay to valid signal
    
    // -------------------------
    // Module
    // -------------------------
    lineBuf_Filter(&ctx->lb_filter, img_input, conv_en, filter_win, &filter_en);
    convFilter(&ctx->filter, filter_win, weight, bias, filter_en, conv_out);

    // -------------------------
    // Output port
    // -------------------------
    // valid signal
    ctx->n_valid = conv_en;
    
    // -------------------------
    // Sequential logic
    // -------------------------
    *conv_valid = temp_valid;
    ctx->conv_valid = *conv_valid;
    temp_valid = ctx->n_valid; // no exist in verilog code for the same reason of temp_valid
}