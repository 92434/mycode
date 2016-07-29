`timescale 1ps / 1ps
module ldpc_encoder_ctrl_top(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_mode,
input	          			[3:0] 		ldpc_mode,
//////////////////////////////////////////////////////////////
input	          							ram_clr_vld,// before byte_in Q_360 cycles but after ldpc_check output
input	          							frame_head,// before byte_in 3cycles
input	          							frame_vld,// before byte_in 3cycles
//////////////////////////////////////////////////////////////
output  	 								shift_vld,
output  		     			[8:0]			shift_index,// cyclic shift derication is up

output  		     			[2:0] 		order_idx,
output  		 								switch_vld,
output       			[5:0] 		switch_array,//0~44
output       			[2:0] 		order_array,//0~7

output       							reg_clr_vld,

output  						[7:0]  		check_ram0_rdaddress,
output  						[7:0]  		check_ram1_rdaddress,
output  						[7:0]  		check_ram2_rdaddress,
output  						[7:0]  		check_ram3_rdaddress,
output  						[7:0]  		check_ram4_rdaddress,
output  						[7:0]  		check_ram5_rdaddress,
output  						[7:0]  		check_ram6_rdaddress,
output  						[7:0]  		check_ram7_rdaddress,

output  										equal_flag,
output  										check_ram_rden,
output  		 				[7:0]  		check_ram_wraddress,
output  										check_ram_wren
//////////////////////////////////////////////////////////////
);

wire	          			[8:0] 		int_Q_360_2pre;
wire	          			[7:0] 		yu_Q_360_2pre;

reg	          			[8:0] 		int_Q_360_1pre;
reg	          			[7:0] 		yu_Q_360_1pre;

reg	          			[8:0] 		int_Q_360;
reg	          			[7:0] 		yu_Q_360;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		int_Q_360_1pre <= 9'h000;
		yu_Q_360_1pre  <= 8'h00;
		int_Q_360 <= 9'h000;
		yu_Q_360  <= 8'h00;
	end
	else if(fs_en == 1'b1)begin
		int_Q_360_1pre <= int_Q_360_2pre;
		yu_Q_360_1pre  <= yu_Q_360_2pre;
		int_Q_360 <= int_Q_360_1pre;
		yu_Q_360  <= yu_Q_360_1pre;
	end
	else begin
	end
end
//////////////////////////////////////////////////////////////

wire		 							 		rd_h_matrix_en;
wire							[12:0]  		rd_h_matrix_addr;
//////////////////////////////////////////////////////////////

//total_yu_Q_360_rom uut_total_yu_Q_360_rom (
//  .clka(sys_clk),    // input wire clka
//  .ena(rd_h_matrix_en),      // input wire ena
//  .addra(rd_h_matrix_addr),  // input wire [12 : 0] addra
//  .douta(yu_Q_360_2pre)  // output wire [7 : 0] douta
//);

total_yu_Q_360_rom_txt uut_total_yu_Q_360_rom_txt(
.address										(rd_h_matrix_addr),
.clock										(sys_clk),
.rden											(rd_h_matrix_en),
.q												(yu_Q_360_2pre));

//total_int_Q_360_rom uut_total_int_Q_360_rom (
//  .clka(sys_clk),    // input wire clka
//  .ena(rd_h_matrix_en),      // input wire ena
//  .addra(rd_h_matrix_addr),  // input wire [12 : 0] addra
//  .douta(int_Q_360_2pre)  // output wire [8 : 0] douta
//);

total_int_Q_360_rom_txt uut_total_int_Q_360_rom_txt(
.address										(rd_h_matrix_addr),
.clock										(sys_clk),
.rden											(rd_h_matrix_en),
.q												(int_Q_360_2pre));

ldpc_ctrl uut_ldpc_ctrl(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),
//////////////////////////////////////////////////////////////
.ram_clr_vld								(ram_clr_vld),// before byte_in Q_360 cycles but after ldpc_check output
.frame_head									(frame_head),// before byte_in 
.frame_vld									(frame_vld),// before byte_in 
//////////////////////////////////////////////////////////////
.int_Q_360									(int_Q_360),
.yu_Q_360									(yu_Q_360),
//////////////////////////////////////////////////////////////
.shift_vld									(shift_vld),
.shift_index								(shift_index),// cyclic shift derication is up

.order_idx									(order_idx),
.switch_vld									(switch_vld),
.switch_array								(switch_array),//0~44
.order_array								(order_array),//0~7

.reg_clr_vld								(reg_clr_vld),

.check_ram0_rdaddress					(check_ram0_rdaddress),
.check_ram1_rdaddress					(check_ram1_rdaddress),
.check_ram2_rdaddress					(check_ram2_rdaddress),
.check_ram3_rdaddress					(check_ram3_rdaddress),
.check_ram4_rdaddress					(check_ram4_rdaddress),
.check_ram5_rdaddress					(check_ram5_rdaddress),
.check_ram6_rdaddress					(check_ram6_rdaddress),
.check_ram7_rdaddress					(check_ram7_rdaddress),

.equal_flag									(equal_flag),
.ram_rden									(check_ram_rden),
.ram_wraddress								(check_ram_wraddress),
.ram_wren									(check_ram_wren),

.rd_h_matrix_en							(rd_h_matrix_en),
.rd_h_matrix_addr							(rd_h_matrix_addr)
//////////////////////////////////////////////////////////////
);

endmodule
