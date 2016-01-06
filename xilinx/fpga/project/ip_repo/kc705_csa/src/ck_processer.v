`timescale 1 ns / 1 ps

module ck_processer #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 5,
		parameter integer CYPHER_DATA_WIDTH = 8 * 8

	)
	(
		input wire [CSA_CALC_IN_WIDTH - 1 : 0] in,
		input wire [CYPHER_DATA_WIDTH - 1 : 0] cb,
		input wire [AXI_DATA_WIDTH - 1 : 0] loops,

		output wire [CYPHER_DATA_WIDTH - 1 : 0] ck_out
	);
	
	assign ck_out[7:0]  =(loops==0)?in[7:0]:((cb[7:0]^8'hB5)+loops)%256; 
	assign ck_out[15:8] =(loops==0)?in[15:8]:((cb[15:8]^8'h93)+loops)%256;
	assign ck_out[23:16]=(loops==0)?in[23:16]:((cb[23:16]^8'h5E)+loops)%256;
	assign ck_out[31:24]=(loops==0)?(in[7:0]+in[15:8]+in[23:16]):(((cb[7:0]^8'hB5)+loops)%256+((cb[15:8]^8'h93)+loops)%256+((cb[23:16]^8'h5E)+loops)%256);
	assign ck_out[39:32]=(loops==0)?in[31:24]:((cb[31:24]^8'hD6)+loops)%256;
	assign ck_out[47:40]=(loops==0)?in[39:32]:((cb[39:32]^8'hB0)+loops)%256;
	assign ck_out[55:48]=(loops==0)?0:((cb[47:40]^8'hD7)+loops)%256;
	assign ck_out[63:56]=(loops==0)?(in[31:24]+in[39:32]):(((cb[31:24]^8'hD6)+loops)%256+((cb[39:32]^8'hB0)+loops)%256+((cb[47:40]^8'hD7)+loops)%256);
	
endmodule
