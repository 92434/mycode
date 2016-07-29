
`timescale 1ns / 1ps
module bchencoder(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_mode,
input	          			[3:0] 			ldpc_mode,
input	          							init_vld,
input	          							frame_vld,
input   			 		[7:0]     		byte_in,
//////////////////////////////////////////////////////////////
output  reg  								oe,
output  reg						[1:0]		BCH_Q_Byte_Num_Flag,
output  reg  			   	[7:0]    byte_out
//////////////////////////////////////////////////////////////
);

reg							[4:0]			BCH_Q_Byte_Num;


reg							[4:0]			BCH_Q_Byte_Cnt;

wire						[167:0]			F_short_t12_next_Rom0_rd_q;
wire						[167:0]			F_short_t12_next_Rom1_rd_q;
wire						[167:0]			F_short_t12_next_Rom2_rd_q;
wire						[167:0]			F_short_t12_next_Rom3_rd_q;
wire						[167:0]			F_short_t12_next_Rom4_rd_q;
wire						[167:0]			F_short_t12_next_Rom5_rd_q;
wire						[167:0]			F_short_t12_next_Rom6_rd_q;
wire						[167:0]			F_short_t12_next_Rom7_rd_q;

F_short_t12_next_Rom0 uut_F_short_t12_next_Rom0(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b00)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_short_t12_next_Rom0_rd_q)
);

F_short_t12_next_Rom1 uut_F_short_t12_next_Rom1(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b00)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_short_t12_next_Rom1_rd_q)
);

F_short_t12_next_Rom2 uut_F_short_t12_next_Rom2(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b00)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_short_t12_next_Rom2_rd_q)
);

F_short_t12_next_Rom3 uut_F_short_t12_next_Rom3(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b00)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_short_t12_next_Rom3_rd_q)
);

F_short_t12_next_Rom4 uut_F_short_t12_next_Rom4(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b00)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_short_t12_next_Rom4_rd_q)
);

F_short_t12_next_Rom5 uut_F_short_t12_next_Rom5(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b00)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_short_t12_next_Rom5_rd_q)
);

F_short_t12_next_Rom6 uut_F_short_t12_next_Rom6(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b00)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_short_t12_next_Rom6_rd_q)
);

F_short_t12_next_Rom7 uut_F_short_t12_next_Rom7(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b00)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_short_t12_next_Rom7_rd_q)
);


wire						[191:0]			F_normal_t12_next_Rom0_rd_q;
wire						[191:0]			F_normal_t12_next_Rom1_rd_q;
wire						[191:0]			F_normal_t12_next_Rom2_rd_q;
wire						[191:0]			F_normal_t12_next_Rom3_rd_q;
wire						[191:0]			F_normal_t12_next_Rom4_rd_q;
wire						[191:0]			F_normal_t12_next_Rom5_rd_q;
wire						[191:0]			F_normal_t12_next_Rom6_rd_q;
wire						[191:0]			F_normal_t12_next_Rom7_rd_q;

F_normal_t12_next_Rom0 uut_F_normal_t12_next_Rom0(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b11)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t12_next_Rom0_rd_q)
);

F_normal_t12_next_Rom1 uut_F_normal_t12_next_Rom1(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b11)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t12_next_Rom1_rd_q)
);

F_normal_t12_next_Rom2 uut_F_normal_t12_next_Rom2(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b11)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t12_next_Rom2_rd_q)
);

F_normal_t12_next_Rom3 uut_F_normal_t12_next_Rom3(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b11)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t12_next_Rom3_rd_q)
);

F_normal_t12_next_Rom4 uut_F_normal_t12_next_Rom4(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b11)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t12_next_Rom4_rd_q)
);

F_normal_t12_next_Rom5 uut_F_normal_t12_next_Rom5(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b11)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t12_next_Rom5_rd_q)
);

F_normal_t12_next_Rom6 uut_F_normal_t12_next_Rom6(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b11)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t12_next_Rom6_rd_q)
);

F_normal_t12_next_Rom7 uut_F_normal_t12_next_Rom7(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b11)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t12_next_Rom7_rd_q)
);


wire						[159:0]			F_normal_t10_next_Rom0_rd_q;
wire						[159:0]			F_normal_t10_next_Rom1_rd_q;
wire						[159:0]			F_normal_t10_next_Rom2_rd_q;
wire						[159:0]			F_normal_t10_next_Rom3_rd_q;
wire						[159:0]			F_normal_t10_next_Rom4_rd_q;
wire						[159:0]			F_normal_t10_next_Rom5_rd_q;
wire						[159:0]			F_normal_t10_next_Rom6_rd_q;
wire						[159:0]			F_normal_t10_next_Rom7_rd_q;

F_normal_t10_next_Rom0 uut_F_normal_t10_next_Rom0(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b01)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t10_next_Rom0_rd_q)
);

F_normal_t10_next_Rom1 uut_F_normal_t10_next_Rom1(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b01)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t10_next_Rom1_rd_q)
);

F_normal_t10_next_Rom2 uut_F_normal_t10_next_Rom2(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b01)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t10_next_Rom2_rd_q)
);

F_normal_t10_next_Rom3 uut_F_normal_t10_next_Rom3(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b01)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t10_next_Rom3_rd_q)
);

F_normal_t10_next_Rom4 uut_F_normal_t10_next_Rom4(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b01)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t10_next_Rom4_rd_q)
);

F_normal_t10_next_Rom5 uut_F_normal_t10_next_Rom5(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b01)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t10_next_Rom5_rd_q)
);

F_normal_t10_next_Rom6 uut_F_normal_t10_next_Rom6(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b01)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t10_next_Rom6_rd_q)
);

F_normal_t10_next_Rom7 uut_F_normal_t10_next_Rom7(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b01)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t10_next_Rom7_rd_q)
);

wire						[127:0]			F_normal_t8_next_Rom0_rd_q;
wire						[127:0]			F_normal_t8_next_Rom1_rd_q;
wire						[127:0]			F_normal_t8_next_Rom2_rd_q;
wire						[127:0]			F_normal_t8_next_Rom3_rd_q;
wire						[127:0]			F_normal_t8_next_Rom4_rd_q;
wire						[127:0]			F_normal_t8_next_Rom5_rd_q;
wire						[127:0]			F_normal_t8_next_Rom6_rd_q;
wire						[127:0]			F_normal_t8_next_Rom7_rd_q;

F_normal_t8_next_Rom0 uut_F_normal_t8_next_Rom0(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b10)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t8_next_Rom0_rd_q)
);

F_normal_t8_next_Rom1 uut_F_normal_t8_next_Rom1(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b10)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t8_next_Rom1_rd_q)
);

F_normal_t8_next_Rom2 uut_F_normal_t8_next_Rom2(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b10)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t8_next_Rom2_rd_q)
);

F_normal_t8_next_Rom3 uut_F_normal_t8_next_Rom3(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b10)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t8_next_Rom3_rd_q)
);

F_normal_t8_next_Rom4 uut_F_normal_t8_next_Rom4(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b10)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t8_next_Rom4_rd_q)
);

F_normal_t8_next_Rom5 uut_F_normal_t8_next_Rom5(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b10)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t8_next_Rom5_rd_q)
);

F_normal_t8_next_Rom6 uut_F_normal_t8_next_Rom6(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b10)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t8_next_Rom6_rd_q)
);

F_normal_t8_next_Rom7 uut_F_normal_t8_next_Rom7(
.clk_1x										(sys_clk),
.rst_n										(rst_n),

.rd_en										((BCH_Q_Byte_Num_Flag == 2'b10)&&frame_vld&&fs_en),
.rdaddr										(BCH_Q_Byte_Cnt),
.rd_q										(F_normal_t8_next_Rom7_rd_q)
);


reg			[191:0]			bch_g_rom0_array;
reg			[191:0]			bch_g_rom1_array;
reg			[191:0]			bch_g_rom2_array;
reg			[191:0]			bch_g_rom3_array;
reg			[191:0]			bch_g_rom4_array;
reg			[191:0]			bch_g_rom5_array;
reg			[191:0]			bch_g_rom6_array;
reg			[191:0]			bch_g_rom7_array;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		bch_g_rom0_array <= 192'h0;
		bch_g_rom1_array <= 192'h0;
		bch_g_rom2_array <= 192'h0;
		bch_g_rom3_array <= 192'h0;
		bch_g_rom4_array <= 192'h0;
		bch_g_rom5_array <= 192'h0;
		bch_g_rom6_array <= 192'h0;
		bch_g_rom7_array <= 192'h0;
	end
	else if(fs_en == 1'b1)begin
		case(BCH_Q_Byte_Num_Flag)
		2'b00:begin
			bch_g_rom0_array <= {24'h0,F_short_t12_next_Rom0_rd_q};
			bch_g_rom1_array <= {24'h0,F_short_t12_next_Rom1_rd_q};
			bch_g_rom2_array <= {24'h0,F_short_t12_next_Rom2_rd_q};
			bch_g_rom3_array <= {24'h0,F_short_t12_next_Rom3_rd_q};
			bch_g_rom4_array <= {24'h0,F_short_t12_next_Rom4_rd_q};
			bch_g_rom5_array <= {24'h0,F_short_t12_next_Rom5_rd_q};
			bch_g_rom6_array <= {24'h0,F_short_t12_next_Rom6_rd_q};
			bch_g_rom7_array <= {24'h0,F_short_t12_next_Rom7_rd_q};
		end
		2'b01:begin
			bch_g_rom0_array <= {32'h0,F_normal_t10_next_Rom0_rd_q};
			bch_g_rom1_array <= {32'h0,F_normal_t10_next_Rom1_rd_q};
			bch_g_rom2_array <= {32'h0,F_normal_t10_next_Rom2_rd_q};
			bch_g_rom3_array <= {32'h0,F_normal_t10_next_Rom3_rd_q};
			bch_g_rom4_array <= {32'h0,F_normal_t10_next_Rom4_rd_q};
			bch_g_rom5_array <= {32'h0,F_normal_t10_next_Rom5_rd_q};
			bch_g_rom6_array <= {32'h0,F_normal_t10_next_Rom6_rd_q};
			bch_g_rom7_array <= {32'h0,F_normal_t10_next_Rom7_rd_q};
		end
		2'b10:begin
			bch_g_rom0_array <= {64'h0,F_normal_t8_next_Rom0_rd_q};
			bch_g_rom1_array <= {64'h0,F_normal_t8_next_Rom1_rd_q};
			bch_g_rom2_array <= {64'h0,F_normal_t8_next_Rom2_rd_q};
			bch_g_rom3_array <= {64'h0,F_normal_t8_next_Rom3_rd_q};
			bch_g_rom4_array <= {64'h0,F_normal_t8_next_Rom4_rd_q};
			bch_g_rom5_array <= {64'h0,F_normal_t8_next_Rom5_rd_q};
			bch_g_rom6_array <= {64'h0,F_normal_t8_next_Rom6_rd_q};
			bch_g_rom7_array <= {64'h0,F_normal_t8_next_Rom7_rd_q};
		end
		2'b11:begin
			bch_g_rom0_array <= {F_normal_t12_next_Rom0_rd_q};
			bch_g_rom1_array <= {F_normal_t12_next_Rom1_rd_q};
			bch_g_rom2_array <= {F_normal_t12_next_Rom2_rd_q};
			bch_g_rom3_array <= {F_normal_t12_next_Rom3_rd_q};
			bch_g_rom4_array <= {F_normal_t12_next_Rom4_rd_q};
			bch_g_rom5_array <= {F_normal_t12_next_Rom5_rd_q};
			bch_g_rom6_array <= {F_normal_t12_next_Rom6_rd_q};
			bch_g_rom7_array <= {F_normal_t12_next_Rom7_rd_q};
		end
		endcase
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_Q_Byte_Cnt <= 5'b00000;
	end
	else if(fs_en == 1'b1)begin
		if(frame_vld == 1'b1)begin
			if(BCH_Q_Byte_Cnt == BCH_Q_Byte_Num)begin
				BCH_Q_Byte_Cnt <= 5'b00000;
			end
			else begin
				BCH_Q_Byte_Cnt <= BCH_Q_Byte_Cnt + 5'b00001;
			end
		end
	end
	else begin
	end
end




always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_Q_Byte_Num <= 5'b00000;
		BCH_Q_Byte_Num_Flag <= 2'b00;
	end
	else if(fs_en == 1'b1)begin
		if(frame_mode == 0)begin // 0:normal frame; 1;short frame
			if((ldpc_mode == 5)||(ldpc_mode == 8))begin
				BCH_Q_Byte_Num <= 19;
				BCH_Q_Byte_Num_Flag <= 2'b01;
			end
			else if((ldpc_mode == 9)||(ldpc_mode == 10))begin
				BCH_Q_Byte_Num <= 15;
				BCH_Q_Byte_Num_Flag <= 2'b10;
			end
			else begin
				BCH_Q_Byte_Num <= 23;
				BCH_Q_Byte_Num_Flag <= 2'b11;
			end
		end  
		else begin
			BCH_Q_Byte_Num <= 20;
			BCH_Q_Byte_Num_Flag <= 2'b00;
		end
	end
	else begin
	end
end


reg							[8:0]			BCH_K_BCHq_Num;
reg							[8:0]			BCH_K_BCHq_Cnt;

reg											BCH_ENCODE_READY_FLAG;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_K_BCHq_Cnt <= 9'd0;
		BCH_ENCODE_READY_FLAG <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if((frame_vld == 1'b1)&&(BCH_Q_Byte_Cnt == BCH_Q_Byte_Num)) begin
			if(BCH_K_BCHq_Cnt == BCH_K_BCHq_Num)begin
				BCH_K_BCHq_Cnt <= 9'd0;
				BCH_ENCODE_READY_FLAG <= 1'b1;
			end
			else begin
				BCH_K_BCHq_Cnt <= BCH_K_BCHq_Cnt + 9'd1;
				BCH_ENCODE_READY_FLAG <= 1'b0;
			end
		end
		else begin
			BCH_ENCODE_READY_FLAG <= 1'b0;/// 1 delay
		end
	end
	else begin
	end
end


reg				[4:0]				BCH_Qout_Byte_Cnt;
reg									BCH_Qout_Byte_en;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_Qout_Byte_Cnt <= 5'b00000;
		BCH_Qout_Byte_en  <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if(BCH_ENCODE_READY_FLAG == 1'b1)begin
			BCH_Qout_Byte_Cnt <= 5'b00000;
			BCH_Qout_Byte_en  <= 1'b1;/// 2 delay
		end
		else begin
			if(BCH_Qout_Byte_Cnt == BCH_Q_Byte_Num)begin
				BCH_Qout_Byte_en  <= 1'b0;
			end
			else begin
				BCH_Qout_Byte_Cnt <= BCH_Qout_Byte_Cnt + 5'b00001;
			end
		end
	end
	else begin
	end
end

reg									BCH_Qout_Byte_en_1dly;/// 3 delay
reg									BCH_Qout_Byte_en_2dly;/// 4 delay

always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_Qout_Byte_en_1dly <= 1'b0;
		BCH_Qout_Byte_en_2dly <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		BCH_Qout_Byte_en_1dly <= BCH_Qout_Byte_en;
		BCH_Qout_Byte_en_2dly <= BCH_Qout_Byte_en_1dly;
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_K_BCHq_Num <= 9'd0;
	end
	else if(frame_mode == 0)begin
		case (ldpc_mode)
			4'h0:
				BCH_K_BCHq_Num <= 9'd83;
			4'h1:
				BCH_K_BCHq_Num <= 9'd111;
			4'h2:
				BCH_K_BCHq_Num <= 9'd133;
			4'h3:
				BCH_K_BCHq_Num <= 9'd167;
			4'h4:
				BCH_K_BCHq_Num <= 9'd201;
			4'h5:
				BCH_K_BCHq_Num <= 9'd268;
			4'h6:
				BCH_K_BCHq_Num <= 9'd252;
			4'h7:
				BCH_K_BCHq_Num <= 9'd268;
			4'h8:
				BCH_K_BCHq_Num <= 9'd336;
			4'h9:
				BCH_K_BCHq_Num <= 9'd448;
			4'ha:
				BCH_K_BCHq_Num <= 9'd454;
			default:
				BCH_K_BCHq_Num <= 9'd0;
		endcase
	end
	else begin
		case (ldpc_mode)
			4'h0:
				BCH_K_BCHq_Num <= 9'd18;
			4'h1:
				BCH_K_BCHq_Num <= 9'd31;
			4'h2:
				BCH_K_BCHq_Num <= 9'd37;
			4'h3:
				BCH_K_BCHq_Num <= 9'd41;
			4'h4:
				BCH_K_BCHq_Num <= 9'd56;
			4'h5:
				BCH_K_BCHq_Num <= 9'd63;
			4'h6:
				BCH_K_BCHq_Num <= 9'd69;
			4'h7:
				BCH_K_BCHq_Num <= 9'd73;
			4'h8:
				BCH_K_BCHq_Num <= 9'd78;
			4'h9:
				BCH_K_BCHq_Num <= 9'd84;
			default:
				BCH_K_BCHq_Num <= 9'd0;
		endcase
	end
end


reg											frame_vld_1dly_reg;
reg											frame_head_1dly_reg;
reg							[7:0]			byte_in_1dly_reg;

reg											frame_vld_2dly_reg;
reg											frame_head_2dly_reg;
reg							[7:0]			byte_in_2dly_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		frame_head_1dly_reg <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if((frame_vld == 1'b1)&&(BCH_Q_Byte_Cnt == 5'b00000))begin
			frame_head_1dly_reg <= 1'b1;
		end
		else begin
			frame_head_1dly_reg <= 1'b0;
		end
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_in_1dly_reg <= 8'b00000000;
		frame_vld_1dly_reg <= 1'b0;
		
		byte_in_2dly_reg <= 8'b00000000;
		frame_vld_2dly_reg <= 1'b0;
		frame_head_2dly_reg <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		byte_in_1dly_reg <= byte_in;
		frame_vld_1dly_reg <= frame_vld;
		
		byte_in_2dly_reg <= byte_in_1dly_reg;
		frame_vld_2dly_reg <= frame_vld_1dly_reg;
		frame_head_2dly_reg <= frame_head_1dly_reg;
	end
end


reg							[191:0]		bch_q_reg_next;
wire							[7:0]			bch_q_xor_reg;	

reg							[191:0]		bch_q_reg_shift;

//always @(posedge clk_1x)begin
//	if(~rst_n)begin
//		bch_q_reg_shift <= 192'h0;
//	end
//	else if(frame_head_2dly_reg) begin
//		bch_q_reg_shift <= bch_q_reg_next;
//	end
//	else if(frame_vld_2dly_reg)begin
//		bch_q_reg_shift <= {8'h00,bch_q_reg_shift[191:8]};
//	end
//end

assign  bch_q_xor_reg = (frame_head_2dly_reg == 1'b1)?bch_q_reg_next[7:0]:bch_q_reg_shift[7:0];


wire		[7:0]			bch_xor_reg;

assign	bch_xor_reg = {bch_q_xor_reg[0],bch_q_xor_reg[1],bch_q_xor_reg[2],bch_q_xor_reg[3],bch_q_xor_reg[4],bch_q_xor_reg[5],bch_q_xor_reg[6],bch_q_xor_reg[7]}^byte_in_2dly_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		bch_q_reg_next <= 192'h0;
		bch_q_reg_shift <= 192'h0;
	end
	else if(fs_en == 1'b1)begin
		if(init_vld == 1'b1)begin
			bch_q_reg_next <= 192'h0;
			bch_q_reg_shift <= 192'h0;
		end
		else if(frame_head_2dly_reg == 1'b1)begin
			bch_q_reg_shift <= {8'h00,bch_q_reg_next[191:8]};
			bch_q_reg_next <= ({192{1'b0}})^((({192{bch_xor_reg[7]}})&bch_g_rom7_array)^(({192{bch_xor_reg[6]}})&bch_g_rom6_array)^(({192{bch_xor_reg[5]}})&bch_g_rom5_array)^(({192{bch_xor_reg[4]}})&bch_g_rom4_array)^(({192{bch_xor_reg[3]}})&bch_g_rom3_array)^(({192{bch_xor_reg[2]}})&bch_g_rom2_array)^(({192{bch_xor_reg[1]}})&bch_g_rom1_array)^(({192{bch_xor_reg[0]}})&bch_g_rom0_array));
		end
		else if(frame_vld_2dly_reg == 1'b1) begin
			bch_q_reg_shift <= {8'h00,bch_q_reg_shift[191:8]};
			bch_q_reg_next <= bch_q_reg_next^((({192{bch_xor_reg[7]}})&bch_g_rom7_array)^(({192{bch_xor_reg[6]}})&bch_g_rom6_array)^(({192{bch_xor_reg[5]}})&bch_g_rom5_array)^(({192{bch_xor_reg[4]}})&bch_g_rom4_array)^(({192{bch_xor_reg[3]}})&bch_g_rom3_array)^(({192{bch_xor_reg[2]}})&bch_g_rom2_array)^(({192{bch_xor_reg[1]}})&bch_g_rom1_array)^(({192{bch_xor_reg[0]}})&bch_g_rom0_array));
		end
	end
	else begin
	end
end


reg		[191:0]					bch_q_reg_next_shift;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		bch_q_reg_next_shift <= 192'h0;
	end
	else if(fs_en == 1'b1)begin
		if((BCH_Qout_Byte_en_2dly == 1'b0)&&(BCH_Qout_Byte_en_1dly == 1'b1))begin
			case(BCH_Q_Byte_Num_Flag)
			2'b00:bch_q_reg_next_shift <= {24'h0,bch_q_reg_next[167:0]};
			2'b01:bch_q_reg_next_shift <= {32'h0,bch_q_reg_next[159:0]};
			2'b10:bch_q_reg_next_shift <= {64'h0,bch_q_reg_next[127:0]};
			2'b11:bch_q_reg_next_shift <= bch_q_reg_next;
			default:bch_q_reg_next_shift <= 192'h0;
			endcase
		end
		else begin
			bch_q_reg_next_shift <= {8'b00000000,bch_q_reg_next_shift[191:8]};
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_out <= 8'b00000000;
		oe       <= 1'b0;
	end
	else if(fs_en == 1'b1) begin
		byte_out <= {bch_q_reg_next_shift[0],bch_q_reg_next_shift[1],bch_q_reg_next_shift[2],bch_q_reg_next_shift[3],bch_q_reg_next_shift[4],bch_q_reg_next_shift[5],bch_q_reg_next_shift[6],bch_q_reg_next_shift[7]};
		oe       <= BCH_Qout_Byte_en_2dly;/// 5 delay
	end
	else begin
	end
end


//always @(posedge clk_1x)begin
//	if(~rst_n)begin
//		bch_q_reg_next_shift <= 192'h0;
//	end
//	else if((BCH_Qout_Byte_en_2dly == 1'b0)&&(BCH_Qout_Byte_en_1dly == 1'b1))begin
//		case(BCH_Q_Byte_Num_Flag)
//		2'b00:bch_q_reg_next_shift <= {bch_q_reg_next[167:0],24'h0};
//		2'b01:bch_q_reg_next_shift <= {bch_q_reg_next[159:0],32'h0};
//		2'b10:bch_q_reg_next_shift <= {bch_q_reg_next[127:0],64'h0};
//		2'b11:bch_q_reg_next_shift <= bch_q_reg_next;
//		default:bch_q_reg_next_shift <= 192'h0;
//		endcase
//	end
//	else begin
//		bch_q_reg_next_shift <= {bch_q_reg_next_shift[183:0],8'b00000000};
//	end
//end
//
//always @(posedge clk_1x)begin
//	if(~rst_n)begin
//		byte_out <= 8'b00000000;
//		oe       <= 1'b0;
//	end
//	else begin
//		byte_out <= bch_q_reg_next_shift[191:184];
//		oe       <= BCH_Qout_Byte_en_2dly;/// 5 delay
//	end
//end

endmodule



