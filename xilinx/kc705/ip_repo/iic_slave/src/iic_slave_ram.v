`timescale 1ns / 1ps

module iic_slave_ram #(
		parameter integer AXI_C_S_AXI_DATA_WIDTH = 32,
		parameter integer AXI_OPT_MEM_ADDR_BITS = 10,
		parameter integer IIC_IIC_SLAVE_DATA_WIDTH = 8,
		parameter integer IIC_OPT_MEM_ADDR_BITS = 16
	)
	(
		input wire axi_clk,
		input wire axi_rst_n,

		input wire [(AXI_C_S_AXI_DATA_WIDTH / 8) - 1 : 0] axi_wstrb,
		input wire axi_wen,
		input wire [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] axi_wdata,
		input wire [AXI_OPT_MEM_ADDR_BITS - 1 : 0] axi_waddr,

		input wire axi_ren,
		output reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] axi_rdata = 0,
		input wire [AXI_OPT_MEM_ADDR_BITS - 1 : 0] axi_raddr,

		input wire iic_clk,
		input wire iic_rst_n,

		input wire iic_wen,
		input wire [IIC_IIC_SLAVE_DATA_WIDTH - 1 : 0] iic_wdata,
		input wire [IIC_OPT_MEM_ADDR_BITS - 1 : 0] iic_waddr,

		input wire iic_ren,
		output reg [IIC_IIC_SLAVE_DATA_WIDTH - 1 : 0] iic_rdata = 0,
		input wire [IIC_OPT_MEM_ADDR_BITS - 1 : 0] iic_raddr
	);

	reg [AXI_OPT_MEM_ADDR_BITS - 1 : 0] current_write_address = 0;
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] current_write_data = 0;
	reg current_mem_wren = 0;

	// implement Block RAM(s)
	// for write command
	//
	//00
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] power_on_switch_reg = 0;
	//01
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] mod_mode_cfg_reg = 0;
	//02
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] ldpc_mode_cfg_reg = 6;
	//03
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] frame_mode_cfg_reg = 0;
	//04
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] pilot_mode_cfg_reg = 0;
	//05
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] srrc_mode_reg = 2;//00:0.35; 01:0.25; 10:0.20(default) 
	//06
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] dvb_s_convolution_mode_reg = 0;
	//07
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] dvb_s_mode_reg = 1;// 0:dvb-s; 1:dvb-s2
	//08
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] TS_Source_mode_reg = 1;// 00:TS Source inside by ts_clk; 01:TS Source outside input without Empty Frame; 10: TS Source outside input with Empty Frame;
	//09
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] SYS_Freq_Num_reg = 12500;//sys_clk freq 
	//10
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] SYS_Baud_Num_reg = 2500;//32'd2500 --> 25M BaudRate   SYS_Baud_mode,// 00:10M; 01:25M; 
	//11
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] Freq_Inv_mode_reg = 0;// 0:不执行频谱翻转; 1:执行频谱翻转 通过交换I和Q发送基带信号翻转频谱，具体地：Im=sin(ωmt) 及Qm=cos(ωmt);
	//12
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] fs_en2_count_reg = 0;
	//13
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] fs_en_count_reg = 0;
	//14
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] mpeg_bytes_count_reg = 0;
	//15
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] symbol_1x_oe_count_reg = 0;
	//16
	reg [AXI_C_S_AXI_DATA_WIDTH - 1 : 0] symbol_2x_oe_count_reg = 0;

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
	//assigning 8 bit iic_data
	always @(posedge axi_clk) begin
		if(axi_rst_n == 0) begin
			index_wstrb <= 0;
			current_write_address <= 0;
			current_write_data <= 0;
			current_mem_wren <= 0;
		end
		else begin
			if (axi_wen == 1) begin
				current_write_address <= axi_waddr;
				for(index_wstrb = 0; index_wstrb < (AXI_C_S_AXI_DATA_WIDTH / 8); index_wstrb = index_wstrb + 1) begin
					if(axi_wstrb[index_wstrb] == 1) begin
						current_write_data[(8 * index_wstrb + 7) -: 8] <= axi_wdata[(8 * index_wstrb + 7) -: 8];
					end
				end
			end
			current_mem_wren <= axi_wen;
		end
	end

	always @(posedge axi_clk) begin
		if(axi_rst_n == 0) begin
		end
		else begin
			if(current_mem_wren == 1) begin
				case(current_write_address)
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	always @(posedge axi_clk) begin
		if(axi_rst_n == 0) begin
			axi_rdata <= 0;
		end
		else begin
			if (axi_ren == 1) begin
				case(axi_raddr)
					0: begin
						axi_rdata <= power_on_switch_reg;
					end
					1: begin
						axi_rdata <= mod_mode_cfg_reg;
					end
					2: begin
						axi_rdata <= ldpc_mode_cfg_reg;
					end
					3: begin
						axi_rdata <= frame_mode_cfg_reg;
					end
					4: begin
						axi_rdata <= pilot_mode_cfg_reg;
					end
					5: begin
						axi_rdata <= srrc_mode_reg;
					end
					6: begin
						axi_rdata <= dvb_s_convolution_mode_reg;
					end
					7: begin
						axi_rdata <= dvb_s_mode_reg;
					end
					8: begin
						axi_rdata <= TS_Source_mode_reg;
					end
					9: begin
						axi_rdata <= SYS_Freq_Num_reg;
					end
					10: begin
						axi_rdata <= SYS_Baud_Num_reg;
					end
					11: begin
						axi_rdata <= Freq_Inv_mode_reg;
					end
					12: begin
						axi_rdata <= fs_en2_count_reg;
					end
					13: begin
						axi_rdata <= fs_en_count_reg;
					end
					14: begin
						axi_rdata <= mpeg_bytes_count_reg;
					end
					15: begin
						axi_rdata <= symbol_1x_oe_count_reg;
					end
					16: begin
						axi_rdata <= symbol_2x_oe_count_reg;
					end
					default: begin
						axi_rdata <= {16'hE000, {(16 - AXI_OPT_MEM_ADDR_BITS){1'b0}}, axi_raddr};
					end
				endcase
			end
		end
	end

	wire [IIC_OPT_MEM_ADDR_BITS - 1 : 0] iic_waddr_reg_index;
	wire [IIC_OPT_MEM_ADDR_BITS - 1 : 0] iic_waddr_reg_byte_index;
	assign iic_waddr_reg_index = iic_waddr / 4;
	assign iic_waddr_reg_byte_index = iic_waddr % 4;

	always @(posedge iic_clk) begin
		if (iic_rst_n == 1'b0) begin
			power_on_switch_reg <= 0;
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
			if (iic_wen == 1) begin
				case(iic_waddr_reg_index)
					0: begin
						power_on_switch_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					1: begin
						mod_mode_cfg_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					2: begin
						ldpc_mode_cfg_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					3: begin
						frame_mode_cfg_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					4: begin
						pilot_mode_cfg_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					5: begin
						srrc_mode_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					6: begin
						dvb_s_convolution_mode_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					7: begin
						dvb_s_mode_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					8: begin
						TS_Source_mode_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					9: begin
						SYS_Freq_Num_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					10: begin
						SYS_Baud_Num_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					11: begin
						Freq_Inv_mode_reg[iic_waddr_reg_byte_index * 8 +: 8] <= iic_wdata;
					end
					12: begin
					end
					13: begin
					end
					14: begin
					end
					15: begin
					end
					16: begin
					end
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	wire [IIC_OPT_MEM_ADDR_BITS - 1 : 0] iic_raddr_reg_index;
	wire [IIC_OPT_MEM_ADDR_BITS - 1 : 0] iic_raddr_reg_byte_index;
	assign iic_raddr_reg_index = iic_raddr / 4;
	assign iic_raddr_reg_byte_index = iic_raddr % 4;

	always @(posedge iic_clk) begin
		if (iic_rst_n == 1'b0) begin
			iic_rdata <= 0;
		end 
		else begin
			if (iic_ren == 1) begin
				case(iic_raddr_reg_index)
					0: begin
						iic_rdata <= power_on_switch_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					1: begin
						iic_rdata <= mod_mode_cfg_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					2: begin
						iic_rdata <= ldpc_mode_cfg_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					3: begin
						iic_rdata <= frame_mode_cfg_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					4: begin
						iic_rdata <= pilot_mode_cfg_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					5: begin
						iic_rdata <= srrc_mode_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					6: begin
						iic_rdata <= dvb_s_convolution_mode_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					7: begin
						iic_rdata <= dvb_s_mode_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					8: begin
						iic_rdata <= TS_Source_mode_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					9: begin
						iic_rdata <= SYS_Freq_Num_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					10: begin
						iic_rdata <= SYS_Baud_Num_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					11: begin
						iic_rdata <= Freq_Inv_mode_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					12: begin
						iic_rdata <= fs_en2_count_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					13: begin
						iic_rdata <= fs_en_count_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					14: begin
						iic_rdata <= mpeg_bytes_count_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					15: begin
						iic_rdata <= symbol_1x_oe_count_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					16: begin
						iic_rdata <= symbol_2x_oe_count_reg[iic_raddr_reg_byte_index * 8 +: 8];
					end
					default: begin
						case(iic_raddr_reg_byte_index)
							0: begin
								iic_rdata <= iic_raddr[iic_raddr_reg_byte_index * 8 +: 8];
							end
							1: begin
								iic_rdata <= iic_raddr[iic_raddr_reg_byte_index * 8 +: 8];
							end
							2: begin
								iic_rdata <= 8'h00;
							end
							3: begin
								iic_rdata <= 8'hE0;
							end
							default: begin
							end
						endcase
					end
				endcase
			end
			else begin
			end
		end
	end
endmodule
