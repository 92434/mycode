`timescale 1ns / 1ps
module ldpc_encoder_0ut(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          			[8:0]			shift_index,// cyclic shift derication is up
input	          							frame_mode,
input	          			[3:0] 		ldpc_mode,
input	          			[2:0] 		order_idx,
input	          							init_vld,
input	          							frame_vld,
input													ram_clr_vld,
input   			 			[7:0]    	byte_in,
//////////////////////////////////////////////////////////////
input	          			[359:0] 		ram_rd_dataout,
input	          			[359:0] 		ram_wr_datain,
input	          							equal_flag,
//////////////////////////////////////////////////////////////
output      	   		[359:0] 		adder_out,
output        	   		[359:0] 		data_align,
output                          		data_align_oe
);


//reg			          					shift_vld;
//reg                             		data_align_oe_1dly;
//reg                             		data_align_oe_2dly;
//reg                             		data_align_oe_3dly;

reg	        	   		[359:0] 		data_align_1dly;
reg	        	   		[359:0] 		data_align_2dly;
reg	        	   		[359:0] 		data_align_3dly;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		//data_align_oe_1dly <= 1'b0;
		//data_align_oe_2dly <= 1'b0;
		//data_align_oe_3dly <= 1'b0;
		//shift_vld <= 1'b0;
		
		data_align_1dly <= 360'h0;
		data_align_2dly <= 360'h0;
		data_align_3dly <= 360'h0;
	end
	else if(fs_en == 1'b1)begin
		//data_align_oe_1dly <= data_align_oe;
		//data_align_oe_2dly <= data_align_oe_1dly;
		//data_align_oe_3dly <= data_align_oe_2dly;
		//shift_vld <= data_align_oe_3dly;//data_align_oe_2dly;
		
		data_align_1dly <= data_align;
		data_align_2dly <= data_align_1dly;
		data_align_3dly <= data_align_2dly;
	end
	else begin
	end
end


ldpcencoder_data_align uut_ldpcencoder_data_align(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.init_vld									(init_vld),
.frame_vld									(frame_vld),
.byte_in										(byte_in),
//////////////////////////////////////////////////////////////
.oe											(data_align_oe),
.byte_out									(data_align)
//////////////////////////////////////////////////////////////
);

wire	          			[359:0] 		shift_out;

cyclic_shift //#(parameter IND = 1,//Dwidth of channel // Module delay: 1 cycles at clk_1x
				 //	  parameter CHD = 360)//Channel of data
					  uut_cyclic_shift(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
//input	          									shift_vld,
.shift_index								(shift_index),// cyclic shift derication is up
.din											(data_align_3dly),// MSB is the first Bit

//output	  	     								shift_oe,
.shift_out									(shift_out)
);

wire	          			[359:0] 		order_out;

order_ctrl	uut_order_ctrl(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),
.order_idx									(order_idx),
.shift_vld									((ram_clr_vld == 1'b0)),
//////////////////////////////////////////////////////////////
.order_in									(shift_out),
.order_out									(order_out)
);


adder_xor uut_adder_xor(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.data_in										(order_out),
.ram_rd_dataout							(ram_rd_dataout),
.ram_wr_datain								(ram_wr_datain),
.equal_flag									(equal_flag),
//////////////////////////////////////////////////////////////

.adder_out									(adder_out)
);

endmodule
