`timescale 1ns/1ns

module i2s_receiver # (
		parameter integer C_M_AXIS_TDATA_WIDTH = 32,
		parameter integer NUMBER_OF_OUTPUT_WORDS = 8,
		parameter integer I2S_DATA_BIT_WIDTH = 24,
		parameter integer ID = 0,
		parameter integer ID_WIDTH = 5
	) (
		input wire rst,
		input wire clk,

		input wire bclk,
		input wire lrclk,
		input wire sdata,
		//output wire i2s_din,
		output wire s_data_valid,
		output wire [I2S_DATA_BIT_WIDTH:0] i2s_received_data,

		input wire read_enable,
		output wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] rdata,
		output wire output_ready,
		output wire buffer_full_error,
		output wire buffer_empty_error
	);

	wire [C_M_AXIS_TDATA_WIDTH - 1:0] wdata;
	reg [ID_WIDTH - 1 : 0] id = ID;

	assign wdata = {id, {(C_M_AXIS_TDATA_WIDTH - ID_WIDTH - 1 - I2S_DATA_BIT_WIDTH){1'b0}}, i2s_received_data[I2S_DATA_BIT_WIDTH : 0]};

 	receive_data_from_i2s # (
			.I2S_DATA_BIT_WIDTH(I2S_DATA_BIT_WIDTH)
		) receiver (
			.rst(rst),
			.bclk(bclk),
			.lrclk(lrclk),
			.sdata(sdata),
			.i2s_received_data(i2s_received_data),
			.s_data_valid(s_data_valid)
		);


	my_fifo # (
			.DATA_WIDTH(C_M_AXIS_TDATA_WIDTH),
			.NUMBER_OF_OUTPUT_WORDS(NUMBER_OF_OUTPUT_WORDS)
		) xiaofei_fifo (
			.rst(rst),
			.wclk(s_data_valid),
			.rclk(clk),
			.wdata(wdata),
			.rdata(rdata),
			.read_enable(read_enable),
			.output_ready(output_ready),
			.buffer_full_error(buffer_full_error),
			.buffer_empty_error(buffer_empty_error)
		);
	always @(posedge clk) begin
		if(rst == 0) begin
			id <= ID;
		end
	end
endmodule
