
`timescale 1ns / 1ps
module bchencoder_top(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_mode,
input	          		[3:0] 			ldpc_mode,
input	          							init_vld,
input	          							frame_vld,
input   			 		[7:0]     		byte_in,
//////////////////////////////////////////////////////////////
output  		  								oe,
output  		  			[7:0]     		byte_out
//////////////////////////////////////////////////////////////
);


wire			  								bchencoder_data_align_oe;
wire			 			[191:0]     	bchencoder_data_alignbyte_out;

bchencoder_data_align uut_bchencoder_data_align(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),
.init_vld									(init_vld),
.frame_vld									(frame_vld),
.byte_in										(byte_in),
//////////////////////////////////////////////////////////////
.oe											(bchencoder_data_align_oe),
.byte_out									(bchencoder_data_alignbyte_out)
//////////////////////////////////////////////////////////////
);

reg											init_vld_1dly;
reg											init_vld_2dly;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		init_vld_1dly <= 1'b0;
		init_vld_2dly <= 1'b0;
	end
	else if(fs_en == 1'b1)begin	
		init_vld_1dly <= init_vld;
		init_vld_2dly <= init_vld_1dly;
	end
	else begin
	end
end

wire											bchencoder_data_ps_oe_head;
wire											bchencoder_data_ps_oe;
wire						 [7:0]     		bchencoder_data_ps_byte_out;

bchencoder_data_ps uut_bchencoder_data_ps(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),

.init_vld									(init_vld_2dly),
.frame_vld									(bchencoder_data_align_oe),
.byte_in										(bchencoder_data_alignbyte_out),
//////////////////////////////////////////////////////////////
.oe_head										(bchencoder_data_ps_oe_head),
.oe											(bchencoder_data_ps_oe),
.byte_out									(bchencoder_data_ps_byte_out)
//////////////////////////////////////////////////////////////
);

wire			  								bchencoder_oe;
wire			  			 [7:0]     		bchencoder_byte_out;
wire  					 [1:0]			BCH_Q_Byte_Num_Flag;


bchencoder uut_bchencoder(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),
.init_vld									(init_vld_2dly),
.frame_vld									(bchencoder_data_ps_oe),
.byte_in										(bchencoder_data_ps_byte_out),
//////////////////////////////////////////////////////////////
.oe											(bchencoder_oe),
.BCH_Q_Byte_Num_Flag                (BCH_Q_Byte_Num_Flag),
.byte_out									(bchencoder_byte_out)
//////////////////////////////////////////////////////////////
);

bchencoder_mux uut_bchencoder_mux(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.infor_frame_vld							(frame_vld),
.infor_byte_in								(byte_in),
.check_frame_vld							(bchencoder_oe),
.check_byte_in								(bchencoder_byte_out),
.BCH_Q_Byte_Num_Flag                (BCH_Q_Byte_Num_Flag),
//////////////////////////////////////////////////////////////
.oe											(oe),
.byte_out									(byte_out)
//////////////////////////////////////////////////////////////
);

endmodule

