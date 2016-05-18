`timescale 1ns / 1ps

module dvb_s2_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10
	)
	(
		input wire clk,
		input wire rst_n,

		input wire [(C_S_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb,
		input wire wen,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] wdata,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr,

		input wire ren,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] rdata,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr,

		input wire hard_rst_n,

		input wire ts_clk_h264out,// clock from h.264 encoder
		input wire [7 : 0] ts_din_h264out,// @ ts_clk_out
		input wire ts_syn_h264out,// @ ts_clk_out ts stream head
		input wire ts_valid_h264out,// @ ts_clk_out

		input wire sys_clk,
		output wire ts_clk,// @ sys_clk
		input wire fs_0p5_en,

		output wire [7 : 0] ts_din,// @ sys_clk
		output wire ts_syn,// @ sys_clk
		output wire ts_head,// @ sys_clk

		output wire fs_en_1cycle,

		output wire symbol_1x_oe,
		output wire signed [15 : 0] symbol_1x_re_out,
		output wire signed [15 : 0] symbol_1x_im_out,

		output wire symbol_2x_oe,
		output wire signed [15 : 0] symbol_2x_re_out,
		output wire signed [15 : 0] symbol_2x_im_out
	);

	reg [OPT_MEM_ADDR_BITS - 1 : 0] current_write_address = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] current_write_data = 0;
	reg current_mem_wren = 0;

	// implement Block RAM(s)
	// for write command
	//
	//00
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] mod_mode_cfg_reg = 0;
	//01
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] ldpc_mode_cfg_reg = 6;
	//02
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] frame_mode_cfg_reg = 0;
	//03
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] pilot_mode_cfg_reg = 0;
	//04
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] srrc_mode_reg = 2;//00:0.35; 01:0.25; 10:0.20(default) 
	//05
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] dvb_s_convolution_mode_reg = 0;
	//06
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] dvb_s_mode_reg = 1;// 0:dvb-s; 1:dvb-s2
	//07
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] TS_Source_mode_reg = 1;// 00:TS Source inside by ts_clk; 01:TS Source outside input without Empty Frame; 10: TS Source outside input with Empty Frame;
	//08
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] SYS_Freq_Num_reg = 12500;//sys_clk freq 
	//09
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] SYS_Baud_Num_reg = 2500;//32'd2500 --> 25M BaudRate   SYS_Baud_mode,// 00:10M; 01:25M; 
	//10
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] Freq_Inv_mode_reg = 0;// 0:不执行频谱翻转; 1:执行频谱翻转 通过交换I和Q发送基带信号翻转频谱，具体地：Im=sin(ωmt) 及Qm=cos(ωmt);
	//11
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] fs_en2_count_reg = 0;
	//12
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] symbol_2x_oe_count_reg = 0;
	//13
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] mpeg_bytes_count_reg = 0;

	wire [1 : 0] mod_mode_cfg;
	wire [3 : 0] ldpc_mode_cfg;
	wire frame_mode_cfg;
	wire pilot_mode_cfg;
	wire [1 : 0] srrc_mode;//00:0.35; 01:0.25; 10:0.20(default) 
	wire [2 : 0] dvb_s_convolution_mode;
	wire dvb_s_mode;// 0:dvb-s; 1:dvb-s2
	wire [1 : 0] TS_Source_mode;// 00:TS Source inside by ts_clk; 01:TS Source outside input without Empty Frame; 10: TS Source outside input with Empty Frame;
	wire [31 : 0] SYS_Freq_Num;//sys_clk freq
	wire [31 : 0] SYS_Baud_Num;//32'd2500 --> 25M BaudRate   SYS_Baud_mode,// 00:10M; 01:25M; 
	wire Freq_Inv_mode;// 0:不执行频谱翻转; 1:执行频谱翻转 通过交换I和Q发送基带信号翻转频谱，具体地：Im=sin(ωmt) 及Qm=cos(ωmt);

	assign mod_mode_cfg = mod_mode_cfg_reg[1 : 0];
	assign ldpc_mode_cfg = ldpc_mode_cfg_reg[3 : 0];
	assign frame_mode_cfg = frame_mode_cfg_reg[0];
	assign pilot_mode_cfg = pilot_mode_cfg_reg[0];
	assign srrc_mode = srrc_mode_reg[1 : 0];
	assign dvb_s_convolution_mode = dvb_s_convolution_mode_reg[2 : 0];
	assign dvb_s_mode = dvb_s_mode_reg[0];
	assign TS_Source_mode = TS_Source_mode_reg[1 : 0];
	assign SYS_Freq_Num = SYS_Freq_Num_reg;
	assign SYS_Baud_Num = SYS_Baud_Num_reg;
	assign Freq_Inv_mode = Freq_Inv_mode_reg[0];

	integer index_wstrb;
	//assigning 8 bit data
	always @(posedge clk) begin
		if(rst_n == 0) begin
			index_wstrb <= 0;
			current_write_address <= 0;
			current_write_data <= 0;
			current_mem_wren <= 0;
		end
		else begin
			if (wen == 1) begin
				current_write_address <= waddr;
				for(index_wstrb = 0; index_wstrb < (C_S_AXI_DATA_WIDTH / 8); index_wstrb = index_wstrb + 1) begin
					if(wstrb[index_wstrb] == 1) begin
						current_write_data[(8 * index_wstrb + 7) -: 8] <= wdata[(8 * index_wstrb + 7) -: 8];
					end
				end
			end
			current_mem_wren <= wen;
		end
	end

	always @(posedge clk) begin
		if(rst_n == 0) begin
			mod_mode_cfg_reg <= 0;
			ldpc_mode_cfg_reg <= 6;
			frame_mode_cfg_reg <= 0;
			pilot_mode_cfg_reg <= 0;
			srrc_mode_reg <= 2;
			dvb_s_convolution_mode_reg <= 0;
			dvb_s_mode_reg <= 1;
			TS_Source_mode_reg <= 1;
			SYS_Freq_Num_reg <= 12500;
			SYS_Baud_Num_reg <= 2500;
			Freq_Inv_mode_reg <= 0;
		end
		else begin
			if(current_mem_wren == 1) begin
				case(current_write_address)
					0: begin
						mod_mode_cfg_reg <= current_write_data;
					end
					1: begin
						ldpc_mode_cfg_reg <= current_write_data;
					end
					2: begin
						frame_mode_cfg_reg <= current_write_data;
					end
					3: begin
						pilot_mode_cfg_reg <= current_write_data;
					end
					4: begin
						srrc_mode_reg <= current_write_data;
					end
					5: begin
						dvb_s_convolution_mode_reg <= current_write_data;
					end
					6: begin
						dvb_s_mode_reg <= current_write_data;
					end
					7: begin
						TS_Source_mode_reg <= current_write_data;
					end
					8: begin
						SYS_Freq_Num_reg <= current_write_data;
					end
					9: begin
						SYS_Baud_Num_reg <= current_write_data;
					end
					10: begin
						Freq_Inv_mode_reg <= current_write_data;
					end
					11: begin
					end
					12: begin
					end
					13: begin
					end
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	always @(posedge clk) begin
		if(rst_n == 0) begin
		end
		else begin
			if (ren == 1) begin
				case(raddr)
					0: begin
						rdata <= mod_mode_cfg;
					end
					1: begin
						rdata <= ldpc_mode_cfg;
					end
					2: begin
						rdata <= frame_mode_cfg;
					end
					3: begin
						rdata <= pilot_mode_cfg;
					end
					4: begin
						rdata <= srrc_mode;
					end
					5: begin
						rdata <= dvb_s_convolution_mode;
					end
					6: begin
						rdata <= dvb_s_mode;
					end
					7: begin
						rdata <= TS_Source_mode;
					end
					8: begin
						rdata <= SYS_Freq_Num_reg;
					end
					9: begin
						rdata <= SYS_Baud_Num;
					end
					10: begin
						rdata <= Freq_Inv_mode;
					end
					11: begin
						rdata <= fs_en2_count_reg;
					end
					12: begin
						rdata <= symbol_2x_oe_count_reg;
					end
					13: begin
						rdata <= mpeg_bytes_count_reg;
					end
					default: begin
						rdata <= {16'hE000, {(16 - OPT_MEM_ADDR_BITS){1'b0}}, raddr};
					end
				endcase
			end
		end
	end

	dvb_s2_system_top #(
		) dvb_s2_system_top_inst(
			.hard_rst_n(hard_rst_n),// modified by 2014.09.22
			.mod_mode_cfg(mod_mode_cfg),
			.ldpc_mode_cfg(ldpc_mode_cfg),
			.frame_mode_cfg(frame_mode_cfg),
			.pilot_mode_cfg(pilot_mode_cfg),
			.srrc_mode(srrc_mode),//00:0.35; 01:0.25; 10:0.20(default) 
			.dvb_s_convolution_mode(dvb_s_convolution_mode),
			.dvb_s_mode(dvb_s_mode),// 0:dvb-s; 1:dvb-s2
			.TS_Source_mode(TS_Source_mode),// 00:TS Source inside by ts_clk; 01:TS Source outside without Empty Frame; 10: TS Source outside with Empty Frame;
			.SYS_Baud_Num(SYS_Baud_Num),//32'd2500 --> 25M BaudRate SYS_Baud_mode,// 00:10M; 01:25M; 
			.SYS_Freq_Num(SYS_Freq_Num),//sys_clk freq
			.Freq_Inv_mode(Freq_Inv_mode),// 0:不执行频谱翻转; 1:执行频谱翻转 通过交换I和Q发送基带信号翻转频谱，具体地：Im=sin(ωmt) 及Qm=cos(ωmt);

			.ts_clk_h264out(ts_clk_h264out),// clock from h.264 encoder
			.ts_din_h264out(ts_din_h264out),// @ ts_clk_out
			.ts_syn_h264out(ts_syn_h264out),// @ ts_clk_out ts stream head
			.ts_valid_h264out(ts_valid_h264out),// @ ts_clk_out

			.sys_clk(sys_clk),
			.ts_clk(ts_clk),// @ sys_clk

			.fs_0p5_en(fs_0p5_en),

			.ts_din(ts_din),// @ sys_clk
			.ts_syn(ts_syn),// @ sys_clk
			.ts_head(ts_head),// @ sys_clk

			.fs_en_1cycle(fs_en_1cycle),

			.symbol_1x_oe(symbol_1x_oe),
			.symbol_1x_re_out(symbol_1x_re_out),
			.symbol_1x_im_out(symbol_1x_im_out)
		);

	wire symbol_2x_oe_origin;
	wire signed [15 : 0] symbol_2x_re_out_origin;
	wire signed [15 : 0] symbol_2x_im_out_origin;
	dvb_s2_srrc_filter dvb_s2_srrc_filter_inst(
			.hard_rst_n(hard_rst_n),// modified by 2014.09.22

			.sys_clk(sys_clk),

			.symbol_1x_oe(symbol_1x_oe),
			.symbol_1x_re_out(symbol_1x_re_out),
			.symbol_1x_im_out(symbol_1x_im_out),

			.symbol_2x_oe(symbol_2x_oe_origin),
			.symbol_2x_re_out(symbol_2x_re_out_origin),
			.symbol_2x_im_out(symbol_2x_im_out_origin)
		);

	always @(posedge fs_0p5_en) begin
		if(hard_rst_n == 0) begin
			fs_en2_count_reg <= 0;
		end
		else begin
			fs_en2_count_reg <= fs_en2_count_reg + 1;
		end
	end

	always @(posedge sys_clk) begin
		if(hard_rst_n == 0) begin
			symbol_2x_oe_count_reg <= 0;
		end
		else begin
			if(fs_en_1cycle == 1) begin
				symbol_2x_oe_count_reg <= symbol_2x_oe_count_reg + 1;
			end
			else begin
			end
		end
	end

	always @(posedge ts_clk_h264out) begin
		if(hard_rst_n == 0) begin
			mpeg_bytes_count_reg <= 0;
		end
		else begin
			if(ts_valid_h264out == 1) begin
				mpeg_bytes_count_reg <= mpeg_bytes_count_reg + 1;
			end
			else begin
			end
		end
	end

	symbol_2x_process #(
		) symbol_2x_process_inst(
			.rst_n(hard_rst_n),
			.sys_clk(sys_clk),

			.fs_en2(fs_0p5_en),

			.symbol_2x_oe_origin(symbol_2x_oe_origin),
			.symbol_2x_re_out_origin(symbol_2x_re_out_origin),
			.symbol_2x_im_out_origin(symbol_2x_im_out_origin),

			.symbol_2x_oe(symbol_2x_oe),
			.symbol_2x_re_out(symbol_2x_re_out),
			.symbol_2x_im_out(symbol_2x_im_out)
		);
endmodule
