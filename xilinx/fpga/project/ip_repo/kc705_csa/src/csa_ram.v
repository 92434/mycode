`timescale 1 ns / 1 ps

module csa_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,


		parameter integer CSA_CALC_INST_NUM = 4,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 5,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 6

	)
	(
		input wire axi_mm_clk,
		input wire rst_n,

		input wire [(C_S_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb,
		input wire wen,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] wdata,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr,

		input wire ren,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] rdata,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr,

		input wire csa_calc_clk,//csa_calc_clk MUST NOT slower than axi_mm_clk!!!

		input wire csa_in_r_ready,
		output wire csa_in_rclk,
		output reg csa_in_ren,
		input wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_in_rdata,

		input wire csa_out_error_full,
		output wire csa_out_wclk,
		output reg csa_out_wen,
		output reg [CSA_CALC_OUT_WIDTH - 1 : 0] csa_out_wdata = 0,

		input wire axis_m_r_ready
	);

	localparam integer ADDR_CSA_BUSY = 0;
	localparam integer ADDR_CSA_READY = ADDR_CSA_BUSY + 1;

	localparam integer ADDR_CHANNEL_INDEX = ADDR_CSA_READY + 1;

	localparam integer ADDR_IN_DATA_VALID = ADDR_CHANNEL_INDEX + 1;
	localparam integer ADDR_IN_DATA_0 = ADDR_IN_DATA_VALID + 1;
	localparam integer ADDR_IN_DATA_1 = ADDR_IN_DATA_0 + 1;
	localparam integer ADDR_IN_DATA_2 = ADDR_IN_DATA_1 + 1;
	localparam integer ADDR_IN_DATA_3 = ADDR_IN_DATA_2 + 1;
	localparam integer ADDR_IN_DATA_4 = ADDR_IN_DATA_3 + 1;

	localparam integer ADDR_OUT_DATA_VALID = ADDR_IN_DATA_4 + 1;
	localparam integer ADDR_OUT_DATA_0 = ADDR_OUT_DATA_VALID + 1;
	localparam integer ADDR_OUT_DATA_1 = ADDR_OUT_DATA_0 + 1;
	localparam integer ADDR_OUT_DATA_2 = ADDR_OUT_DATA_1 + 1;

	localparam integer ADDR_CALC_TIMES = ADDR_OUT_DATA_2 + 1;
	localparam integer ADDR_CALC_DELAY = ADDR_CALC_TIMES + 1;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_current_channel = 0;
	reg csa_current_channel_changed = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times = 50000;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_delay = 0;

	always @(posedge axi_mm_clk) begin
		if(rst_n == 0) begin
			csa_current_channel <= 0;
			csa_current_channel_changed <= 0;
			csa_calc_logic_times <= 50000;
			csa_calc_logic_delay <= 0;
		end
		else begin
			csa_current_channel_changed <= 0;
			if(wen == 1) begin
				case(waddr)
					ADDR_CHANNEL_INDEX: begin
						if((wdata >= 0) && (wdata < CSA_CALC_INST_NUM)) begin
							csa_current_channel <= wdata;
							csa_current_channel_changed <= 1;
						end
						else begin
						end
					end
					ADDR_CALC_TIMES: begin
						if(wdata <= 0) begin
							csa_calc_logic_times <= 50000;
						end
						else begin
							csa_calc_logic_times <= wdata;
						end
					end
					ADDR_CALC_DELAY: begin
						if(wdata >= 0) begin
							csa_calc_logic_delay <= wdata;
						end
						else begin
							csa_calc_logic_delay <= 0;
						end
					end
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_0 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_1 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_2 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_3 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_4 = 0;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_out_0 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_out_1 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_out_2 = 0;

	reg csa_in_valid = 0;
	reg csa_out_valid = 0;

	always @(posedge axi_mm_clk) begin
		if(rst_n == 0) begin
			rdata <= 0;
		end
		else begin
			if (ren == 1) begin
				case(raddr)
					ADDR_CSA_BUSY: begin
						rdata <= {{(C_S_AXI_DATA_WIDTH - 1){1'b0}}, csa_in_r_ready};
					end
					ADDR_CSA_READY: begin
						rdata <= {{(C_S_AXI_DATA_WIDTH - 1){1'b0}}, axis_m_r_ready};
					end
					ADDR_CHANNEL_INDEX: begin
						rdata <= csa_current_channel;
					end
					ADDR_IN_DATA_VALID: begin
						rdata <= {{(C_S_AXI_DATA_WIDTH - 1){1'b0}}, csa_in_valid};
					end
					ADDR_OUT_DATA_VALID: begin
						rdata <= {{(C_S_AXI_DATA_WIDTH - 1){1'b0}}, csa_out_valid};
					end
					ADDR_IN_DATA_0: begin
						rdata <= csa_in_0;
					end
					ADDR_IN_DATA_1: begin
						rdata <= csa_in_1;
					end
					ADDR_IN_DATA_2: begin
						rdata <= csa_in_2;
					end
					ADDR_IN_DATA_3: begin
						rdata <= csa_in_3;
					end
					ADDR_IN_DATA_4: begin
						rdata <= csa_in_4;
					end
					ADDR_OUT_DATA_0: begin
						rdata <= csa_out_0;
					end
					ADDR_OUT_DATA_1: begin
						rdata <= csa_out_1;
					end
					ADDR_OUT_DATA_2: begin
						rdata <= csa_out_2;
					end
					ADDR_CALC_TIMES: begin
						rdata <= csa_calc_logic_times;
					end
					ADDR_CALC_DELAY: begin
						rdata <= csa_calc_logic_delay;
					end
					default: begin
						rdata <= {16'hE000, {(16 - OPT_MEM_ADDR_BITS){1'b0}}, raddr};
					end
				endcase
			end
		end
	end
	
	wire [CSA_CALC_INST_NUM - 1 : 0] csa_calc_logic_inuse;

	reg [CSA_CALC_INST_NUM - 1 : 0] csa_calc_logic_request = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in = 0;

	wire [CSA_CALC_INST_NUM - 1 : 0] csa_calc_logic_ready;
	wire [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out [0 : CSA_CALC_INST_NUM - 1];
	reg [CSA_CALC_INST_NUM - 1 : 0] csa_calc_logic_reset = 0;

	genvar i;
	generate for (i = 0; i < CSA_CALC_INST_NUM; i = i + 1)
		begin : csa_calc_unit
			localparam integer id = i;

			csa_calc_logic #(
					.AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
					.ID(id),
					.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
					.CSA_CALC_OUT_WIDTH(CSA_CALC_OUT_WIDTH)
				) csa_calc_logic_inst(
					.clk(csa_calc_clk),
					.rst_n(rst_n),

					.csa_calc_logic_delay(csa_calc_logic_delay),
					.csa_calc_logic_times(csa_calc_logic_times),

					.csa_calc_logic_inuse(csa_calc_logic_inuse[i]),

					.csa_calc_logic_request(csa_calc_logic_request[i]),
					.csa_calc_logic_in(csa_calc_logic_in),

					.csa_calc_logic_ready(csa_calc_logic_ready[i]),
					.csa_calc_logic_out(csa_calc_logic_out[i]),
					.csa_calc_logic_reset(csa_calc_logic_reset[i])
				);
		end
	endgenerate

	assign csa_in_rclk = csa_calc_clk;
	assign csa_out_wclk = csa_calc_clk;

	integer csa_channel_in_index = 0;
	integer csa_in_state = 0;
	reg csa_in_changed = 0;
	always @(posedge csa_in_rclk) begin
		if(rst_n == 0) begin
			csa_calc_logic_in <= 0;
			csa_calc_logic_request <= 0;

			csa_in_ren <= 0;

			csa_channel_in_index <= 0;
			csa_in_state <= 0;

			csa_in_changed <= 0;
		end
		else begin
			csa_calc_logic_request <= 0;
			csa_in_ren <= 0;

			csa_in_changed <= 0;

			case(csa_in_state)
				0: begin
					if(csa_calc_logic_inuse[csa_channel_in_index] == 0) begin

						csa_in_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if(csa_in_r_ready == 1) begin
						csa_in_ren <= 1;

						csa_in_state <= 2;
					end
					else begin
					end
				end
				2: begin
					csa_calc_logic_in <= csa_in_rdata;
					csa_calc_logic_request[csa_channel_in_index] <= 1;

					if(csa_channel_in_index == csa_current_channel) begin
						csa_in_0 <= {{(C_S_AXI_DATA_WIDTH - 8){1'b0}}, csa_in_rdata[(0 * 8) +: 8]};
						csa_in_1 <= {{(C_S_AXI_DATA_WIDTH - 8){1'b0}}, csa_in_rdata[(1 * 8) +: 8]};
						csa_in_2 <= {{(C_S_AXI_DATA_WIDTH - 8){1'b0}}, csa_in_rdata[(2 * 8) +: 8]};
						csa_in_3 <= {{(C_S_AXI_DATA_WIDTH - 8){1'b0}}, csa_in_rdata[(3 * 8) +: 8]};
						csa_in_4 <= {{(C_S_AXI_DATA_WIDTH - 8){1'b0}}, csa_in_rdata[(4 * 8) +: 8]};
						csa_in_changed <= 1;
					end
					else begin
					end

					if((csa_channel_in_index >= 0) && (csa_channel_in_index < CSA_CALC_INST_NUM - 1)) begin
						csa_channel_in_index <= csa_channel_in_index + 1;
					end
					else begin
						csa_channel_in_index <= 0;
					end

					csa_in_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

	integer csa_channel_out_index = 0;
	integer csa_out_state = 0;
	reg csa_out_changed = 0;
	always @(posedge csa_out_wclk) begin
		if(rst_n == 0) begin
			csa_calc_logic_reset <= 0;

			csa_out_wen <= 0;
			csa_out_wdata <= 0;

			csa_channel_out_index <= 0;
			csa_out_state <= 0;

			csa_out_changed <= 0;
		end
		else begin
			csa_calc_logic_reset <= 0;
			csa_out_wen <= 0;

			csa_out_changed <= 0;

			case(csa_out_state)
				0: begin
					if(csa_out_error_full == 0) begin

						csa_out_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if(csa_calc_logic_ready[csa_channel_out_index] == 1) begin
						
						csa_out_wen <= 1;
						csa_out_wdata <= csa_calc_logic_out[csa_channel_out_index];
						csa_calc_logic_reset[csa_channel_out_index] <= 1;

						if(csa_channel_out_index == csa_current_channel) begin
							csa_out_0 <= {{(C_S_AXI_DATA_WIDTH - 16){1'b0}}, csa_calc_logic_out[csa_channel_out_index][(0 * 16) +: 16]};
							csa_out_1 <= {{(C_S_AXI_DATA_WIDTH - 16){1'b0}}, csa_calc_logic_out[csa_channel_out_index][(1 * 16) +: 16]};
							csa_out_2 <= {{(C_S_AXI_DATA_WIDTH - 16){1'b0}}, csa_calc_logic_out[csa_channel_out_index][(2 * 16) +: 16]};
							csa_out_changed <= 1;
						end
						else begin
						end

						if((csa_channel_out_index >= 0) && (csa_channel_out_index < CSA_CALC_INST_NUM - 1)) begin
							csa_channel_out_index <= csa_channel_out_index + 1;
						end
						else begin
							csa_channel_out_index <= 0;
						end

						csa_out_state <= 0;
					end
					else begin
					end
				end
				default: begin
				end
			endcase
		end
	end

	integer csa_valid_state = 0;
	always @(posedge csa_calc_clk) begin
		if(rst_n == 0) begin
			csa_in_valid <= 0;
			csa_out_valid <= 0;

			csa_valid_state <= 0;
		end
		else begin
			case(csa_valid_state)
				0: begin
					if(csa_current_channel_changed == 1) begin
						csa_in_valid <= 0;
						csa_out_valid <= 0;

						csa_valid_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if(csa_in_changed == 1) begin
						csa_in_valid <= 1;

						csa_valid_state <= 2;
					end
					else begin
					end
				end
				2: begin
					if(csa_out_changed == 1) begin
						csa_out_valid <= 1;

						csa_valid_state <= 0;
					end
					else begin
					end
				end
				default: begin
				end
			endcase
		end
	end
endmodule
