
`timescale 1ns / 1ps
module ldpc_encoder_1ut(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							switch_vld,
input	          			[359:0] 		ram_data,
input	          			[5:0] 		switch_array,//0~44
input	          			[2:0] 		order_array,//0~7
input	          							reg_clr_vld,
//////////////////////////////////////////////////////////////
output  			  							oe,
output      	   		[7:0] 		byte_out
);


wire				  							order_oe;
wire				   		[7:0] 		order_out;

data_switch_order uut_data_switch_order(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.switch_vld									(switch_vld),
.ram_data									(ram_data),
.switch_array								(switch_array),//0~44
.order_array								(order_array),//0~7
//////////////////////////////////////////////////////////////
.order_oe									(order_oe),
.byte_out									(order_out)
);


Parallel_Accum uut_Parallel_Accum(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.Accum_en									(order_oe),
.reg_clr_vld								(reg_clr_vld),
.Parallel_Accum_in						(order_out),
//////////////////////////////////////////////////////////////
.Accum_oe									(oe),
.Parallel_Accum_out						(byte_out)
);

endmodule

