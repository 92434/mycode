`timescale 1ns / 1ps
module ldpc_encoder_top(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_mode,
input	          			[3:0] 		ldpc_mode,
//////////////////////////////////////////////////////////////
input	          							init_vld,
input	          							frame_vld,
input   			 			[7:0]    	byte_in,

input	          							ram_clr_vld,// before byte_in Q_360 cycles but after ldpc_check output
//input	          							frame_head,// before byte_in 
//input	          							frame_vld,// before byte_in 
//////////////////////////////////////////////////////////////
//input   			 			[359:0]     byte_in,
//////////////////////////////////////////////////////////////
output  		 								oe,
output  		  			  	[7:0]    	byte_out
//////////////////////////////////////////////////////////////
);


//////////////////////////////////////////////////////////////
//input	          							ram_clr_vld,// before byte_in Q_360 cycles but after ldpc_check output
//input	          							frame_head,// before byte_in 3cycles
//input	          							frame_vld,// before byte_in 3cycles
//////////////////////////////////////////////////////////////
wire  	 									shift_vld;
wire    		     			[8:0]			shift_index;// cyclic shift derication is up

wire    		     			[2:0] 		order_idx;
wire    		 								switch_vld;
wire         				[5:0] 		switch_array;//0~44
wire         				[2:0] 		order_array;//0~7

wire         								reg_clr_vld;

wire    						[7:0]  		check_ram0_rdaddress;
wire    						[7:0]  		check_ram1_rdaddress;
wire    						[7:0]  		check_ram2_rdaddress;
wire    						[7:0]  		check_ram3_rdaddress;
wire    						[7:0]  		check_ram4_rdaddress;
wire    						[7:0]  		check_ram5_rdaddress;
wire    						[7:0]  		check_ram6_rdaddress;
wire    						[7:0]  		check_ram7_rdaddress;

wire    										equal_flag;
wire    										check_ram_rden;
wire    		 				[7:0]  		check_ram_wraddress;
wire    										check_ram_wren;
//////////////////////////////////////////////////////////////
wire        	   		[359:0] 		data_align;
wire                          		data_align_oe;


ldpc_encoder_ctrl_top uut_ldpc_encoder_ctrl_top(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),
//////////////////////////////////////////////////////////////
.ram_clr_vld								(ram_clr_vld),// before byte_in Q_360 cycles but after ldpc_check output
.frame_head									(data_align_oe),// before byte_in 3 cycles
.frame_vld									(data_align_oe),// before byte_in  3 cycles
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
.check_ram_rden							(check_ram_rden),
.check_ram_wraddress						(check_ram_wraddress),
.check_ram_wren							(check_ram_wren)
//////////////////////////////////////////////////////////////
);

wire	      	   		[359:0] 		adder_out;
wire	      	   		[359:0] 		ram_rd_dataout_1pre;
wire							  [44:0]		  check_ram0_q;
wire							  [44:0]		  check_ram1_q;
wire							  [44:0]		  check_ram2_q;
wire							  [44:0]		  check_ram3_q;
wire							  [44:0]		  check_ram4_q;
wire							  [44:0]		  check_ram5_q;
wire							  [44:0]		  check_ram6_q;
wire							  [44:0]		  check_ram7_q;

//assign adder_out = {check_ram0_data,check_ram1_data,check_ram2_data,check_ram3_data,check_ram4_data,check_ram5_data,check_ram6_data,check_ram7_data};

assign ram_rd_dataout_1pre = {check_ram0_q,check_ram1_q,check_ram2_q,check_ram3_q,check_ram4_q,check_ram5_q,check_ram6_q,check_ram7_q};

reg	      	   		[359:0] 		ram_rd_dataout;
reg                                switch_vld_1dly; 


always @(posedge sys_clk)begin
	if(~rst_n)begin
		ram_rd_dataout <= 360'h0;
		switch_vld_1dly <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
	   if(ram_clr_vld)begin//if(ram_clr_vld||((switch_vld_1dly == 1)&&(switch_vld == 0)))begin
	       ram_rd_dataout <= 360'h0;
	   end
	   else begin
		  ram_rd_dataout <= ram_rd_dataout_1pre;
	   end
	   switch_vld_1dly <= switch_vld;
	end
	else begin
	end
end

ldpc_encoder_0ut uut_ldpc_encoder_0ut(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),
.order_idx									(order_idx),
.init_vld									(init_vld),
.frame_vld									(frame_vld),
.ram_clr_vld								(ram_clr_vld),
//////////////////////////////////////////////////////////////
.shift_index								(shift_index),// cyclic shift derication is up
.byte_in										(byte_in),
//////////////////////////////////////////////////////////////
.ram_rd_dataout							(ram_rd_dataout),
.ram_wr_datain								(adder_out),
.equal_flag									(equal_flag),
//////////////////////////////////////////////////////////////
.adder_out									(adder_out),
.data_align									(data_align),
.data_align_oe								(data_align_oe)
);


wire							[44:0]		check_ram0_data;
wire							[44:0]		check_ram1_data;
wire							[44:0]		check_ram2_data;
wire							[44:0]		check_ram3_data;
wire							[44:0]		check_ram4_data;
wire							[44:0]		check_ram5_data;
wire							[44:0]		check_ram6_data;
wire							[44:0]		check_ram7_data;

assign check_ram0_data				= 	adder_out[359:315];
assign check_ram1_data				= 	adder_out[314:270];
assign check_ram2_data				= 	adder_out[269:225];
assign check_ram3_data				= 	adder_out[224:180];
assign check_ram4_data				= 	adder_out[179:135];
assign check_ram5_data				= 	adder_out[134:90];
assign check_ram6_data				= 	adder_out[89:45];
assign check_ram7_data				= 	adder_out[44:0];


ldpc_encoder_1ut uut_ldpc_encoder_1ut(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(rst_n),
//////////////////////////////////////////////////////////////
.switch_vld									(switch_vld),
.ram_data									(ram_rd_dataout),
.switch_array								(switch_array),//0~44
.order_array								(order_array),//0~7
.reg_clr_vld								(reg_clr_vld),
//////////////////////////////////////////////////////////////
.oe											(oe),
.byte_out									(byte_out)
);

//ldpc_check_ram uut_ldpc_check_ram0 (
//  .clka(sys_clk),    // input wire clka
//  .ena(check_ram_wren&&fs_en),      // input wire ena
//  .wea(check_ram_wren&&fs_en),      // input wire [0 : 0] wea
//  .addra(check_ram_wraddress),  // input wire [7 : 0] addra
//  .dina(check_ram0_data),    // input wire [44 : 0] dina
//  .clkb(sys_clk),    // input wire clkb
//  .rstb(ram_clr_vld),    // input wire rstb
//  .enb(check_ram_rden&&fs_en),      // input wire enb
//  .addrb(check_ram0_rdaddress),  // input wire [7 : 0] addrb
//  .doutb(check_ram0_q)  // output wire [44 : 0] doutb
//);


ldpc_check_ram_noip uut_ldpc_check_ram0(
.aclr											(ram_clr_vld),//1:en
.clock										(sys_clk),
.data											(check_ram0_data),
.rdaddress									(check_ram0_rdaddress),
.rden											(check_ram_rden&&fs_en),
.wraddress									(check_ram_wraddress),
.wren											(check_ram_wren&&fs_en),
.q												(check_ram0_q)
);

//ldpc_check_ram uut_ldpc_check_ram1 (
//  .clka(sys_clk),    // input wire clka
//  .ena(check_ram_wren&&fs_en),      // input wire ena
//  .wea(check_ram_wren&&fs_en),      // input wire [0 : 0] wea
//  .addra(check_ram_wraddress),  // input wire [7 : 0] addra
//  .dina(check_ram1_data),    // input wire [44 : 0] dina
//  .clkb(sys_clk),    // input wire clkb
//  .rstb(ram_clr_vld),    // input wire rstb
//  .enb(check_ram_rden&&fs_en),      // input wire enb
//  .addrb(check_ram1_rdaddress),  // input wire [7 : 0] addrb
//  .doutb(check_ram1_q)  // output wire [44 : 0] doutb
//);

ldpc_check_ram_noip uut_ldpc_check_ram1(
.aclr											(ram_clr_vld),//1:en
.clock										(sys_clk),
.data											(check_ram1_data),
.rdaddress									(check_ram1_rdaddress),
.rden											(check_ram_rden&&fs_en),
.wraddress									(check_ram_wraddress),
.wren											(check_ram_wren&&fs_en),
.q												(check_ram1_q)
);

//ldpc_check_ram uut_ldpc_check_ram2 (
//  .clka(sys_clk),    // input wire clka
//  .ena(check_ram_wren&&fs_en),      // input wire ena
//  .wea(check_ram_wren&&fs_en),      // input wire [0 : 0] wea
//  .addra(check_ram_wraddress),  // input wire [7 : 0] addra
//  .dina(check_ram2_data),    // input wire [44 : 0] dina
//  .clkb(sys_clk),    // input wire clkb
//  .rstb(ram_clr_vld),    // input wire rstb
//  .enb(check_ram_rden&&fs_en),      // input wire enb
//  .addrb(check_ram2_rdaddress),  // input wire [7 : 0] addrb
//  .doutb(check_ram2_q)  // output wire [44 : 0] doutb
//);

ldpc_check_ram_noip uut_ldpc_check_ram2(
.aclr											(ram_clr_vld),//1:en
.clock										(sys_clk),
.data											(check_ram2_data),
.rdaddress									(check_ram2_rdaddress),
.rden											(check_ram_rden&&fs_en),
.wraddress									(check_ram_wraddress),
.wren											(check_ram_wren&&fs_en),
.q												(check_ram2_q)
);

//ldpc_check_ram uut_ldpc_check_ram3 (
//  .clka(sys_clk),    // input wire clka
//  .ena(check_ram_wren&&fs_en),      // input wire ena
//  .wea(check_ram_wren&&fs_en),      // input wire [0 : 0] wea
//  .addra(check_ram_wraddress),  // input wire [7 : 0] addra
//  .dina(check_ram3_data),    // input wire [44 : 0] dina
//  .clkb(sys_clk),    // input wire clkb
//  .rstb(ram_clr_vld),    // input wire rstb
//  .enb(check_ram_rden&&fs_en),      // input wire enb
//  .addrb(check_ram3_rdaddress),  // input wire [7 : 0] addrb
//  .doutb(check_ram3_q)  // output wire [44 : 0] doutb
//);

ldpc_check_ram_noip uut_ldpc_check_ram3(
.aclr											(ram_clr_vld),//1:en
.clock										(sys_clk),
.data											(check_ram3_data),
.rdaddress									(check_ram3_rdaddress),
.rden											(check_ram_rden&&fs_en),
.wraddress									(check_ram_wraddress),
.wren											(check_ram_wren&&fs_en),
.q												(check_ram3_q)
);

//ldpc_check_ram uut_ldpc_check_ram4 (
//  .clka(sys_clk),    // input wire clka
//  .ena(check_ram_wren&&fs_en),      // input wire ena
//  .wea(check_ram_wren&&fs_en),      // input wire [0 : 0] wea
//  .addra(check_ram_wraddress),  // input wire [7 : 0] addra
//  .dina(check_ram4_data),    // input wire [44 : 0] dina
//  .clkb(sys_clk),    // input wire clkb
//  .rstb(ram_clr_vld),    // input wire rstb
//  .enb(check_ram_rden&&fs_en),      // input wire enb
//  .addrb(check_ram4_rdaddress),  // input wire [7 : 0] addrb
//  .doutb(check_ram4_q)  // output wire [44 : 0] doutb
//);

ldpc_check_ram_noip uut_ldpc_check_ram4(
.aclr											(ram_clr_vld),//1:en
.clock										(sys_clk),
.data											(check_ram4_data),
.rdaddress									(check_ram4_rdaddress),
.rden											(check_ram_rden&&fs_en),
.wraddress									(check_ram_wraddress),
.wren											(check_ram_wren&&fs_en),
.q												(check_ram4_q)
);

//ldpc_check_ram uut_ldpc_check_ram5 (
//  .clka(sys_clk),    // input wire clka
//  .ena(check_ram_wren&&fs_en),      // input wire ena
//  .wea(check_ram_wren&&fs_en),      // input wire [0 : 0] wea
//  .addra(check_ram_wraddress),  // input wire [7 : 0] addra
//  .dina(check_ram5_data),    // input wire [44 : 0] dina
//  .clkb(sys_clk),    // input wire clkb
//  .rstb(ram_clr_vld),    // input wire rstb
//  .enb(check_ram_rden&&fs_en),      // input wire enb
//  .addrb(check_ram5_rdaddress),  // input wire [7 : 0] addrb
//  .doutb(check_ram5_q)  // output wire [44 : 0] doutb
//);

ldpc_check_ram_noip uut_ldpc_check_ram5(
.aclr											(ram_clr_vld),//1:en
.clock										(sys_clk),
.data											(check_ram5_data),
.rdaddress									(check_ram5_rdaddress),
.rden											(check_ram_rden&&fs_en),
.wraddress									(check_ram_wraddress),
.wren											(check_ram_wren&&fs_en),
.q												(check_ram5_q)
);

//ldpc_check_ram uut_ldpc_check_ram6 (
//  .clka(sys_clk),    // input wire clka
//  .ena(check_ram_wren&&fs_en),      // input wire ena
//  .wea(check_ram_wren&&fs_en),      // input wire [0 : 0] wea
//  .addra(check_ram_wraddress),  // input wire [7 : 0] addra
//  .dina(check_ram6_data),    // input wire [44 : 0] dina
//  .clkb(sys_clk),    // input wire clkb
//  .rstb(ram_clr_vld),    // input wire rstb
//  .enb(check_ram_rden&&fs_en),      // input wire enb
//  .addrb(check_ram6_rdaddress),  // input wire [7 : 0] addrb
//  .doutb(check_ram6_q)  // output wire [44 : 0] doutb
//);

ldpc_check_ram_noip uut_ldpc_check_ram6(
.aclr											(ram_clr_vld),//1:en
.clock										(sys_clk),
.data											(check_ram6_data),
.rdaddress									(check_ram6_rdaddress),
.rden											(check_ram_rden&&fs_en),
.wraddress									(check_ram_wraddress),
.wren											(check_ram_wren&&fs_en),
.q												(check_ram6_q)
);

//ldpc_check_ram uut_ldpc_check_ram7 (
//  .clka(sys_clk),    // input wire clka
//  .ena(check_ram_wren&&fs_en),      // input wire ena
//  .wea(check_ram_wren&&fs_en),      // input wire [0 : 0] wea
//  .addra(check_ram_wraddress),  // input wire [7 : 0] addra
//  .dina(check_ram7_data),    // input wire [44 : 0] dina
//  .clkb(sys_clk),    // input wire clkb
//  .rstb(ram_clr_vld),    // input wire rstb
//  .enb(check_ram_rden&&fs_en),      // input wire enb
//  .addrb(check_ram7_rdaddress),  // input wire [7 : 0] addrb
//  .doutb(check_ram7_q)  // output wire [44 : 0] doutb
//);

ldpc_check_ram_noip uut_ldpc_check_ram7(
.aclr											(ram_clr_vld),//1:en
.clock										(sys_clk),
.data											(check_ram7_data),
.rdaddress									(check_ram7_rdaddress),
.rden											(check_ram_rden&&fs_en),
.wraddress									(check_ram_wraddress),
.wren											(check_ram_wren&&fs_en),
.q												(check_ram7_q)
);

endmodule
