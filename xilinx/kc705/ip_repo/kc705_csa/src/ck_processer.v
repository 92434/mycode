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

	assign ck_out[7:0]  =(init==1)?((block[31:30]==2'b01)?(in[7:0]^((loops))):in[7:0]):((cb[7:0])^((loops)));
	assign ck_out[15:8] =(init==1)?((block[31:30]==2'b01)?((in[15:8]^in[55:48])^((loops)>>8)):in[15:8]):((cb[15:8]^cb[55:48])^((loops)>>8));
	assign ck_out[23:16]=(init==1)?(in[23:16]):(cb[23:16]);
	assign ck_out[31:24]=(init==1)?((block[31:30]==2'b01)?((in[7:0]^((loops)))+((in[15:8]^in[55:48])^((loops)>>8))+in[23:16]):(in[7:0]+in[15:8]+in[23:16])):((cb[7:0]^((loops)))+((cb[15:8]^cb[55:48])^((loops)>>8))+cb[23:16]);
	assign ck_out[39:32]=(init==1)?in[31:24]:cb[31:24];
	assign ck_out[47:40]=(init==1)?in[39:32]:cb[39:32];
	assign ck_out[55:48]=(init==1)?((block[31:30]==2'b01)?in[47:40]:0):cb[47:40];
	assign ck_out[63:56]=(init==1)?((block[31:30]==2'b01)?(in[31:24]+in[39:32]+in[47:40]):(in[31:24]+in[39:32])):(cb[31:24]+cb[39:32]+cb[47:40]);
endmodule
