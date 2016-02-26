`timescale 1ns/1ns

module lcm_controler_eg9013f_nz_ram #(
		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer BYTE_WIDTH = 8,
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10
	)
	(
		input wire clk,
		input wire rst_n,

		input wire [(C_S_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb,
		input wire wen,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] wdata,
		input wire [OPT_MEM_ADDR_BITS : 0] waddr,

		input wire ren,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] rdata,
		input wire [OPT_MEM_ADDR_BITS : 0] raddr,

		output wire [BYTE_WIDTH - 1 : 0] lcm_data_origin,
		output wire lcm_din,
		output wire lcm_lp,
		output wire xscl,
		output wire [BYTE_WIDTH - 1 : 0] lcm_data
	);

	localparam integer C_S_AXI_DATA_BYTE_WIDTH = C_S_AXI_DATA_WIDTH / BYTE_WIDTH;

	//localparam integer LCM_WIDTH_BITS = 640;
	//localparam integer LCM_HEIGHT_BITS = 480;
	localparam integer LCM_WIDTH_BITS = 32;
	localparam integer LCM_HEIGHT_BITS = 3;
	localparam integer LCM_WIDTH_WORDS = LCM_WIDTH_BITS / C_S_AXI_DATA_WIDTH;
	localparam integer LCM_DATA_WORD_SIZE = LCM_WIDTH_WORDS * LCM_HEIGHT_BITS;
	localparam integer ADDR_LCM_CLK_LEVEL_DELAY = LCM_DATA_WORD_SIZE;

	localparam integer LCM_CLK_LEVEL_DELAY = 1;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_ram [0 : LCM_DATA_WORD_SIZE - 1];

	reg [BYTE_WIDTH - 1 : 0] index_wstrb = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_clk_level_delay = LCM_CLK_LEVEL_DELAY;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			index_wstrb <= 0;
			lcm_clk_level_delay <= LCM_CLK_LEVEL_DELAY;
			lcm_ram[0][8 * 0 +: 8] = 8'h00;
			lcm_ram[0][8 * 1 +: 8] = 8'h01;
			lcm_ram[0][8 * 2 +: 8] = 8'h02;
			lcm_ram[0][8 * 3 +: 8] = 8'h03;
			lcm_ram[1][8 * 0 +: 8] = 8'h04;
			lcm_ram[1][8 * 1 +: 8] = 8'h05;
			lcm_ram[1][8 * 2 +: 8] = 8'h06;
			lcm_ram[1][8 * 3 +: 8] = 8'h07;
			lcm_ram[2][8 * 0 +: 8] = 8'h08;
			lcm_ram[2][8 * 1 +: 8] = 8'h09;
			lcm_ram[2][8 * 2 +: 8] = 8'h0a;
			lcm_ram[2][8 * 3 +: 8] = 8'h0b;
		end
		else begin
			if (wen == 1) begin
				if((waddr >= 0) && (waddr < LCM_DATA_WORD_SIZE)) begin
					for(index_wstrb = 0; index_wstrb < C_S_AXI_DATA_BYTE_WIDTH; index_wstrb = index_wstrb + 1) begin
						if(wstrb[index_wstrb] == 1) begin
							lcm_ram[waddr][(8 * index_wstrb) +: 8] <= wdata[(8 * index_wstrb) +: 8];
						end
					end
				end
				else if(waddr == ADDR_LCM_CLK_LEVEL_DELAY) begin
					if(wdata <= LCM_CLK_LEVEL_DELAY) begin
						lcm_clk_level_delay <= LCM_CLK_LEVEL_DELAY;
					end
					else begin
						lcm_clk_level_delay <= wdata;
					end
				end
				else begin
				end
			end
			else begin
			end
		end
	end

	always @(posedge clk) begin
		if(rst_n == 0) begin
			rdata <= 0;
		end
		else begin
			if (ren == 1) begin
				if((raddr >= 0) && (raddr <  LCM_DATA_WORD_SIZE)) begin
					rdata <= lcm_ram[raddr];
				end
				else if(raddr == ADDR_LCM_CLK_LEVEL_DELAY) begin
					rdata <= lcm_clk_level_delay;
				end
				else begin
					rdata <= {16'hE000, {(16 - OPT_MEM_ADDR_BITS - 1){1'b0}}, raddr};
				end
			end
			else begin
			end
		end
	end

	reg lcm_clk = 0;
	reg lcm_clk_event = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] clk_delay_counter = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			lcm_clk_event <= 0;
			lcm_clk <= 0;
			clk_delay_counter <= 0;
		end
		else begin
			lcm_clk_event <= 0;
			if(clk_delay_counter >= lcm_clk_level_delay - 1) begin
				lcm_clk_event <= 1;
				lcm_clk <= ~lcm_clk;
				clk_delay_counter <= 0;
			end
			else begin
				clk_delay_counter <= clk_delay_counter + 1;
			end
		end
	end

	wire [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_data_index;
	lcm_controler_eg9013f_nz #(
			.BYTE_WIDTH(BYTE_WIDTH),
			.LCM_WIDTH_BITS(LCM_WIDTH_BITS),
			.LCM_HEIGHT_BITS(LCM_HEIGHT_BITS),

			.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH)
		) lcm_controler_eg9013f_nz_inst(
			.clk(clk),
			.rst_n(rst_n),

			.lcm_clk_event(lcm_clk_event),
			.lcm_clk(lcm_clk),

			.lcm_din(lcm_din),
			.lcm_lp(lcm_lp),
			.xscl(xscl),
			.lcm_data_index(lcm_data_index)
		);

	wire [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_ram_word_index;
	assign lcm_ram_word_index = lcm_data_index / C_S_AXI_DATA_BYTE_WIDTH;

	wire [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_ram_byte_index;
	assign lcm_ram_byte_index = lcm_data_index % C_S_AXI_DATA_BYTE_WIDTH;

	assign lcm_data_origin = lcm_ram[lcm_ram_word_index][BYTE_WIDTH * lcm_ram_byte_index +: BYTE_WIDTH];
	assign lcm_data = {
			lcm_data_origin[0],
			lcm_data_origin[1],
			lcm_data_origin[2],
			lcm_data_origin[3],
			lcm_data_origin[4],
			lcm_data_origin[5],
			lcm_data_origin[6],
			lcm_data_origin[7]
		};

endmodule
