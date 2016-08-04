`timescale 1 ns / 1 ps

module ck_processer #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 8,
		parameter integer CYPHER_DATA_WIDTH = 8 * 8
	)
	(
		input wire init,
		input wire [31 : 0] block,
		input wire [CSA_CALC_IN_WIDTH - 1 : 0] in,
		input wire [CYPHER_DATA_WIDTH - 1 : 0] cb,
		input wire [AXI_DATA_WIDTH - 1 : 0] loops,

		output wire [CYPHER_DATA_WIDTH - 1 : 0] ck_out
	);
	wire[47:0] rout;
	rfunc_bill rfunc(((block[31:30]==2'b01)?((init==1)?in[55:0]:cb[55:0]):cb[55:0]),rout);
	
	
	assign ck_out[7:0]  =(init==1)?((block[31:30]==2'b01)?(rout[7:0]^loops):in[7:0]):rout[7:0]^loops;
	assign ck_out[15:8] =(init==1)?((block[31:30]==2'b01)?(rout[15:8]^(loops>>8)):in[15:8]):rout[15:8]^(loops>>8);
	assign ck_out[23:16]=(init==1)?((block[31:30]==2'b01)?rout[23:16]:in[23:16]):rout[23:16];
	assign ck_out[31:24]=(init==1)?((block[31:30]==2'b01)?((rout[7:0]^loops)+(rout[15:8]^(loops>>8))+rout[23:16]):(in[7:0]+in[15:8]+in[23:16])):((rout[7:0]^loops)+(rout[15:8]^(loops>>8))+rout[23:16]);
	assign ck_out[39:32]=(init==1)?((block[31:30]==2'b01)?rout[31:24]:in[31:24]):rout[31:24];
	assign ck_out[47:40]=(init==1)?((block[31:30]==2'b01)?rout[39:32]:in[39:32]):rout[39:32];
	assign ck_out[55:48]=(init==1)?((block[31:30]==2'b01)?rout[47:40]:0):rout[47:40];
	assign ck_out[63:56]=(init==1)?((block[31:30]==2'b01)?(rout[31:24]+rout[39:32]+rout[47:40]):(in[31:24]+in[39:32])):(rout[31:24]+rout[39:32]+rout[47:40]);
	
	
endmodule
