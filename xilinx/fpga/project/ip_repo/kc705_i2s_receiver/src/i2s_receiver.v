`timescale 1ns/1ns

module i2s_receiver # (
		parameter integer C_M_AXIS_TDATA_WIDTH = 32,
		parameter integer BULK_OF_DATA = 15,
		parameter integer BULK_DEPTH = 8,
		parameter integer I2S_DATA_BIT_WIDTH = 24,
		parameter integer ID = 0,
		parameter integer ID_WIDTH = 5,
		parameter integer DATA_WIDTH = 8,
		parameter integer DATA_BYTE_COUNT = 15
	) (
		input wire rst_n,
		input wire clk,

		input wire bclk,
		input wire lrclk,
		input wire sdata,
		//output wire i2s_din,
		output wire s_data_valid,
		output wire [I2S_DATA_BIT_WIDTH : 0] i2s_received_data,

		input wire r_enable,
		output wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] rdata,
		output wire r_ready,
		output wire error_full,
		output wire error_empty
	);

 	receive_data_from_i2s # (
			.I2S_DATA_BIT_WIDTH(I2S_DATA_BIT_WIDTH)
		) receiver (
			.rst_n(rst_n),
			.bclk(bclk),
			.lrclk(lrclk),
			.sdata(sdata),

			.i2s_received_data(i2s_received_data),
			.s_data_valid(s_data_valid)
		);


	reg [DATA_WIDTH * DATA_BYTE_COUNT - 1 : 0] i2s_data;

	wire [DATA_WIDTH * 10 - 1 : 0]header;
	assign header = {8b'h03, 8b'h2B, 8b'h84, 8b'h2F, 8b'h40, 8b'h42, 8b'hDD, 8b'hA1, 8b'h77, 8b'h0B};
	reg matched = 0;

	integer i = 0;
	always @(posedge s_data_valid) begin
		if(rst_n == 0) begin
			matched <= 0;
		end
		else begin
			if(header == i2s_data[DATA_WIDTH * 10 - 1 : 0]) begin
				matched <= 1;
			end
			else begin
			end

			i2s_data <= {i2s_data[DATA_WIDTH * DATA_BYTE_COUNT - I2S_DATA_BIT_WIDTH - 1 : 0], i2s_received_data[I2S_DATA_BIT_WIDTH - 1: 0]}
		end
	end

	wire [C_M_AXIS_TDATA_WIDTH - 1:0] wdata;
	reg [ID_WIDTH - 1 : 0] id = ID;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			id <= ID;
		end
	end

	assign wdata = {{(C_M_AXIS_TDATA_WIDTH - ID_WIDTH - I2S_DATA_BIT_WIDTH - 1){1'b0}}, id, i2s_received_data[I2S_DATA_BIT_WIDTH : 0]};

	my_fifo # (
			.DATA_WIDTH(C_M_AXIS_TDATA_WIDTH),
			.BULK_OF_DATA(BULK_OF_DATA),
			.BULK_DEPTH(BULK_DEPTH)
		) xiaofei_fifo (
			.rst_n(rst_n),
			.wclk(s_data_valid),
			.rclk(clk),
			.wdata(wdata),
			.rdata(rdata),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);
endmodule
