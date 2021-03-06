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
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr,

		input wire ren,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] rdata,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr,

		output wire [BYTE_WIDTH - 1 : 0] lcm_data_origin,
		output wire [BYTE_WIDTH - 1 : 0] lcm_data_reverse,
		output wire lcm_din,
		output wire lcm_lp,
		output wire lcm_xscl,
		output wire [BYTE_WIDTH - 1 : 0] lcm_data
	);

	localparam integer C_S_AXI_DATA_BYTE_WIDTH = C_S_AXI_DATA_WIDTH / BYTE_WIDTH;

	localparam integer REG_RANGE_DATA_WORD_SIZE = (1 << OPT_MEM_ADDR_BITS);
	localparam integer RAM_PAGES = (LCM_DATA_WORD_SIZE + REG_RANGE_DATA_WORD_SIZE + REG_RANGE_DATA_WORD_SIZE - 1) / REG_RANGE_DATA_WORD_SIZE;

	localparam integer LCM_WIDTH_BITS = 640;
	localparam integer LCM_HEIGHT_BITS = 480;
	localparam integer LCM_WIDTH_WORDS = LCM_WIDTH_BITS / C_S_AXI_DATA_WIDTH;
	localparam integer LCM_DATA_WORD_SIZE = LCM_WIDTH_WORDS * LCM_HEIGHT_BITS;

	localparam integer ADDR_LCM_CLK_LEVEL_DELAY = 0;
	localparam integer ADDR_BYTE_ORDER = ADDR_LCM_CLK_LEVEL_DELAY + 1;
	localparam integer ADDR_READ_CHANNEL_INDEX = ADDR_BYTE_ORDER + 1;
	localparam integer ADDR_WRITE_CHANNEL_INDEX = ADDR_READ_CHANNEL_INDEX + 1;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_ram [0 : LCM_DATA_WORD_SIZE - 1];

	localparam integer LCM_CLK_LEVEL_DELAY = 1;//65;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_clk_level_delay = LCM_CLK_LEVEL_DELAY;
	reg out_data_byte_order = 0;
	reg [BYTE_WIDTH - 1 : 0] read_channel_index = 0;
	reg [BYTE_WIDTH - 1 : 0] write_channel_index = 0;

	wire [BYTE_WIDTH - 1 : 0] read_ram_channel_index;
	assign read_ram_channel_index = (read_channel_index >= 1) ? read_channel_index - 1 : 0;
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] ram_raddr;
	assign ram_raddr = {{(C_S_AXI_DATA_WIDTH - BYTE_WIDTH - OPT_MEM_ADDR_BITS){1'b0}}, read_ram_channel_index, raddr};

	wire [BYTE_WIDTH - 1 : 0] write_ram_channel_index;
	assign write_ram_channel_index = (write_channel_index >= 1) ? write_channel_index - 1 : 0;
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] ram_waddr;
	assign ram_waddr = {{(C_S_AXI_DATA_WIDTH - BYTE_WIDTH - OPT_MEM_ADDR_BITS){1'b0}}, write_ram_channel_index, waddr};

	reg [BYTE_WIDTH - 1 : 0] index_wstrb = 0;
	reg request_update = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			index_wstrb <= 0;

			lcm_clk_level_delay <= LCM_CLK_LEVEL_DELAY;
			out_data_byte_order <= 0;
			write_channel_index <= 0;
			read_channel_index <= 0;

			request_update <= 0;
		end
		else begin
			request_update <= 0;
			if (wen == 1) begin
				case(write_channel_index)
					0: begin
						case(waddr)
							ADDR_LCM_CLK_LEVEL_DELAY: begin
								if(wdata <= LCM_CLK_LEVEL_DELAY) begin
									lcm_clk_level_delay <= LCM_CLK_LEVEL_DELAY;
								end
								else begin
									lcm_clk_level_delay <= wdata;
								end
							end
							ADDR_BYTE_ORDER: begin
								out_data_byte_order <= wdata[0];
							end
							ADDR_READ_CHANNEL_INDEX: begin
								if((wdata >= 0) && (wdata < RAM_PAGES)) begin
									read_channel_index <= wdata[BYTE_WIDTH - 1 : 0];
								end
								else begin
								end
							end
							ADDR_WRITE_CHANNEL_INDEX: begin
								if((wdata >= 0) && (wdata < RAM_PAGES)) begin
									write_channel_index <= wdata[BYTE_WIDTH - 1 : 0];
								end
								else begin
								end
							end
							default: begin
							end
						endcase
					end
					default: begin
						if((ram_waddr >= 0) && (ram_waddr < LCM_DATA_WORD_SIZE)) begin
							for(index_wstrb = 0; index_wstrb < C_S_AXI_DATA_BYTE_WIDTH; index_wstrb = index_wstrb + 1) begin
								if(wstrb[index_wstrb] == 1) begin
									lcm_ram[ram_waddr][(8 * index_wstrb) +: 8] <= wdata[(8 * index_wstrb) +: 8];
								end
							end

							if((ram_waddr == (LCM_DATA_WORD_SIZE - 1)) && (wstrb[C_S_AXI_DATA_BYTE_WIDTH - 1] == 1)) begin
								write_channel_index <= 0;
								request_update <= 1;
							end
							else if((ram_waddr > 0) && ((ram_waddr + 1) % REG_RANGE_DATA_WORD_SIZE == 0) && (wstrb[C_S_AXI_DATA_BYTE_WIDTH - 1] == 1)) begin
								write_channel_index <= 0;
							end
							else begin
							end
						end
						else begin
						end
					end
				endcase
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
				case(read_channel_index)
					0: begin
						case(raddr)
							ADDR_LCM_CLK_LEVEL_DELAY: begin
								rdata <= lcm_clk_level_delay;
							end
							ADDR_BYTE_ORDER: begin
								rdata <= {{(C_S_AXI_DATA_WIDTH - 1){1'b0}}, out_data_byte_order};
							end
							ADDR_READ_CHANNEL_INDEX: begin
								rdata <= {{(C_S_AXI_DATA_WIDTH - BYTE_WIDTH){1'b0}}, read_channel_index};
							end
							ADDR_WRITE_CHANNEL_INDEX: begin
								rdata <= {{(C_S_AXI_DATA_WIDTH - BYTE_WIDTH){1'b0}}, write_channel_index};
							end
							default: begin
								rdata <= {8'hE0, ram_raddr[C_S_AXI_DATA_WIDTH - 8 - 1 : 0]};
							end
						endcase
					end
					default: begin
						if((ram_raddr >= 0) && (ram_raddr <  LCM_DATA_WORD_SIZE)) begin
							rdata <= lcm_ram[ram_raddr];
						end
						else begin
							rdata <= {8'hE0, ram_raddr[C_S_AXI_DATA_WIDTH - 8 - 1 : 0]};
						end
					end
				endcase
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

			.request_update(request_update),

			.lcm_clk_event(lcm_clk_event),
			.lcm_clk(lcm_clk),

			.lcm_din(lcm_din),
			.lcm_lp(lcm_lp),
			.lcm_xscl(lcm_xscl),
			.lcm_data_index(lcm_data_index)
		);

	wire [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_ram_word_index;
	assign lcm_ram_word_index = lcm_data_index / C_S_AXI_DATA_BYTE_WIDTH;

	wire [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_ram_byte_index;
	assign lcm_ram_byte_index = lcm_data_index % C_S_AXI_DATA_BYTE_WIDTH;

	assign lcm_data_origin = lcm_ram[lcm_ram_word_index][BYTE_WIDTH * lcm_ram_byte_index +: BYTE_WIDTH];
	assign lcm_data_reverse = {
			lcm_data_origin[0],
			lcm_data_origin[1],
			lcm_data_origin[2],
			lcm_data_origin[3],
			lcm_data_origin[4],
			lcm_data_origin[5],
			lcm_data_origin[6],
			lcm_data_origin[7]
		};

	assign lcm_data = (out_data_byte_order == 0) ? lcm_data_origin : lcm_data_reverse;

endmodule
