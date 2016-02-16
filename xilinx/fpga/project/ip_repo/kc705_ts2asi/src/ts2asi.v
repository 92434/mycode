`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/07/24 17:51:26
// Design Name: 
// Module Name: kc705_pcie_ext
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

/* Module Description:
This module implements the complete data path for the DVB ASI
 */

module ts2asi #(
		parameter integer FIFO_DATA_WIDTH = 9
	)
	(
		output wire ts2_clk,
		output wire ts2_valid,
		output wire ts2_sync,
		output wire [7:0] ts2_data,

		// Inputs
		input wire rst_n, //Synchronous reset
		input wire clk, //half-bit rate clock, 

		input wire din_clk,
		input wire valid,
		input wire sync,
		input wire [7:0] din_8b, // 8-bit input data to be encoded, serialized, transmitted

		output wire asi_out_p, //1-bit serialized output data. 8b10b encoded.
		output wire asi_out_n, //1-bit serialized output data. 8b10b encoded.

		//debug ports
		output wire [FIFO_DATA_WIDTH - 1 : 0] din,
		output wire [FIFO_DATA_WIDTH - 1 : 0] rdata,
		output reg [FIFO_DATA_WIDTH - 1 : 0] dout,

		output wire ts_clk,
		output wire ts_valid,
		output wire ts_sync,
		output wire [7:0] ts_data,

		output reg r_enable,
		output reg r_enable_R = 0,
		output wire r_ready,
		output wire error_full,
		output wire error_empty,

		output reg ce_R0 = 0,
		output reg ce_R1 = 0,
		output reg ce, //Clock enable for parallel domain
		output reg [4 : 0] ce_sr,
		output wire start,
 		output wire [9:0] sout_data,
		output wire [7:0] din_8b_R_debug
	);


	// Tx clock enable generation
	always @(posedge clk) begin
		if (rst_n == 0) begin
			ce_sr <= 5'b00001;
			ce_R0 <= 0;
			ce_R1 <= 0;
			ce <= 1'b0;
		end
		else begin
			ce_R0 <= ce;
			ce_R1 <= ce_R0;
			ce <= ce_sr[4];
			ce_sr <= {ce_sr[3:0], ce_sr[4]};
		end
	end 

	assign ts_clk = din_clk;
	assign ts_valid = valid;
	assign ts_sync = sync;
	assign ts_data = din_8b;

	assign din = {1'b0, din_8b};

	always @(posedge clk) begin
		if (rst_n == 0) begin
			r_enable_R <= 0;
		end
		else begin
			r_enable <= 0;

			if((ce == 1) && (r_ready == 1)) begin
				r_enable <= 1;
			end
			else begin
			end
			r_enable_R <= r_enable;
		end
	end

	always @(posedge clk) begin
		if (rst_n == 0) begin
			dout <= 0;
		end
		else begin
			if(r_enable == 1) begin
				dout <= rdata;
			end
			else begin
				dout <= {1'b1, 8'hBC};
			end
		end
	end

	assign ts2_clk = clk;
	assign ts2_valid = (r_enable_R == 1) ? 1 : 0;
	assign ts2_sync = 0;
	assign ts2_data = rdata[7 : 0];

	my_fifo #(
			.DATA_WIDTH(FIFO_DATA_WIDTH),
			.BULK_OF_DATA(1)
		) xiaofei_fifo (
			.rst_n(rst_n),
			.wclk(din_clk),
			.rclk(clk),
			.wdata(din),
			.rdata(rdata),
			.w_enable(valid),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	// Internal signals
	wire [9:0] data_enc10b;

 
	// 8b10b Encoder
	encoder_8b10b encoder_inst(
		.din_8b_R_debug(din_8b_R_debug),
		.din(dout[7 : 0]),
		.kin(dout[8]),
		.clk(clk),
		.ce(ce_R1),
		.dout(data_enc10b),
		.valid(),
		.code_err());

	assign sout_data = data_enc10b;

	// Instantiate the 10:1 serializer
	serializer_10b1b asi_serializer(
		//.sout_data(sout_data),
		.start(start),
		.sclk_0(clk),
		.sclk_180(~clk),
		.ce(ce_R1),
		.reset(~rst_n),
		.din_10b(data_enc10b),
		.sdout_p(asi_out_p),
		.sdout_n(asi_out_n));

endmodule // txdata_path
