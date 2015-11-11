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
		// Inputs
		input wire rst_n, //Synchronous reset
		input wire clk, //half-bit rate clock, 

		input wire din_clk,
		input wire valid,
		input wire [7:0] din_8b, // 8-bit input data to be encoded, serialized, transmitted

		output wire asi_out, //1-bit serialized output data. 8b10b encoded.

		//debug ports
		output wire [FIFO_DATA_WIDTH - 1 : 0] din,
		output wire [FIFO_DATA_WIDTH - 1 : 0] rdata,
		output wire [FIFO_DATA_WIDTH - 1 : 0] dout,
		output wire r_enable,
		output wire error_full,
		output wire error_empty,

		output reg ce, //Clock enable for parallel domain
		output reg [4 : 0] ce_sr,
		output wire start,
 		output wire [9:0] sout_data,
		output wire [7:0] din_8b_R_debug
	);

	reg output_ready_R;


	// Tx clock enable generation
	always @(posedge clk) begin
		if (rst_n == 0) begin
			ce <= 1'b0;
			ce_sr <= 5'b00001;
		end
		else begin
			ce_sr <= {ce_sr[3:0], ce_sr[4]};
			ce <= ce_sr[4];
		end
	end 

	assign din = (valid == 0) ? {1'b1, 8'hBC} : {1'b0, din_8b};
	assign dout = (output_ready_R == 0) ? {1'b1, 8'hBC} : rdata;

	always @(negedge ce) begin
		if (rst_n == 0) begin
			output_ready_R <= 0;
		end
		else begin
			output_ready_R <= r_enable;
		end
	end

	my_fifo #(
			.DATA_WIDTH(FIFO_DATA_WIDTH),
			.BULK_OF_DATA(1)
		) xiaofei_fifo (
			.rst_n(rst_n),
			.wclk(din_clk),
			.rclk(ce),
			.wdata(din),
			.rdata(rdata),
			.w_enable(1),
			.r_enable(r_enable),
			.r_ready(r_enable),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	//reg kchar_in_R = 1'b0;
	//reg [7:0] din_8b_R = 8'h00;

	// Internal signals
	wire [9:0] data_enc10b;

 
	//// Synchronous process
	//always @(posedge clk) begin
	//	if(ce) begin
	//		din_8b_R <= din_8b;
	//		kchar_in_R <= kchar_in;
	//	end
	//end
 
	// 8b10b Encoder
	encoder_8b10b encoder_inst(
		.din_8b_R_debug(din_8b_R_debug),
		.din(dout[7 : 0]),
		.kin(dout[8]),
		.clk(clk),
		.ce(ce),
		.dout(data_enc10b),
		.valid(),
		.code_err());


	// Instantiate the 10:1 serializer
	serializer_10b1b asi_serializer(
		.sout_data(sout_data),
		.start(start),
		.sclk_0(clk),
		.sclk_180(~clk),
		.ce(ce),
		.reset(~rst_n),
		.din_10b(data_enc10b),
		.sdout(asi_out));

endmodule // txdata_path
