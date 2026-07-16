module cnn #(
    parameter IMAGE_SIZE  = 28 * 28,
    parameter FILTER_SIZE = 3
) (
    input       clk,
    input       reset,
    input [7:0] img_data_in
);
    // --------------------------
    // Image Buffer
    // --------------------------
    reg [7:0] img_buffer [0 : IMAGE_SIZE * IMAGE_SIZE];
    reg img_buf_we;
    reg [$clog2(IMAGE_SIZE*IMAGE_SIZE)-1:0] img_buf_addr;

    always @(posedge clk) begin
        
    end

    // --------------------------
    // Filter
    // --------------------------


endmodule
