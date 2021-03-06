`timescale 1 ns / 1 ps

module csa_ram #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,

		parameter integer CSA_CALC_INST_NUM = 4,

		parameter integer CYPHER_DATA_WIDTH = 8 * 8,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 8,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 8,

		parameter integer CSA_IN_PARAMETER_LENGTH = AXI_DATA_WIDTH * 5,
		parameter integer CSA_OUT_PARAMETER_LENGTH = AXI_DATA_WIDTH * 7
	)
	(
		input wire axi_mm_clk,
		input wire s00_axi_aresetn,

		input wire [(AXI_DATA_WIDTH / 8) - 1 : 0] wstrb,
		input wire wen,
		input wire [AXI_DATA_WIDTH - 1 : 0] wdata,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr,

		input wire ren,
		output reg [AXI_DATA_WIDTH - 1 : 0] rdata = 0,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr,

		input wire csa_calc_clk,//csa_calc_clk MUST NOT slower than axi_mm_clk!!!
		output wire user_rst_n,
		input wire rst_n,

		input wire csa_in_r_ready,
		input wire csa_in_error_empty,
		output wire csa_in_rclk,
		output reg csa_in_ren = 0,
		input wire [AXI_DATA_WIDTH - 1 : 0] csa_in_rdata,

		input wire m00_axis_tvalid,
		input wire csa_out_error_full,
		output wire csa_out_wclk,
		output reg csa_out_wen = 0,
		output reg [AXI_DATA_WIDTH - 1 : 0] csa_out_wdata = 0
	);

	localparam integer ADDR_CHANNEL_INDEX = 0;

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
	localparam integer ADDR_OUT_DATA_3 = ADDR_OUT_DATA_2 + 1;
	localparam integer ADDR_OUT_DATA_4 = ADDR_OUT_DATA_3 + 1;
	localparam integer ADDR_OUT_DATA_5 = ADDR_OUT_DATA_4 + 1;
	localparam integer ADDR_OUT_DATA_6 = ADDR_OUT_DATA_5 + 1;
	localparam integer ADDR_DATA_CATCH_COUNT_INDEX = ADDR_OUT_DATA_6 + 1;
	localparam integer ADDR_DATA_CATCH_COUNT_LOW = ADDR_DATA_CATCH_COUNT_INDEX + 1;
	localparam integer ADDR_DATA_CATCH_COUNT_HIGH = ADDR_DATA_CATCH_COUNT_LOW + 1;
	localparam integer ADDR_OUT_MASK_ENABLE = ADDR_DATA_CATCH_COUNT_HIGH + 1;
	localparam integer ADDR_OUT_MASK_LOW = ADDR_OUT_MASK_ENABLE + 1;
	localparam integer ADDR_OUT_MASK_HIGH = ADDR_OUT_MASK_LOW + 1;
	localparam integer ADDR_OUT_VALUE_LOW = ADDR_OUT_MASK_HIGH + 1;
	localparam integer ADDR_OUT_VALUE_HIGH = ADDR_OUT_VALUE_LOW + 1;
	localparam integer ADDR_DEVICE_IDLE = ADDR_OUT_VALUE_HIGH + 1;
	localparam integer ADDR_DEVICE_READY = ADDR_DEVICE_IDLE + 1;
	localparam integer ADDR_RESET = ADDR_DEVICE_READY + 1;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_current_channel = 0;
	reg csa_current_channel_changed = 0;
	reg user_rst_n_reg = 1;
	
	reg [CSA_CALC_OUT_WIDTH - 1 : 0] ram_data_catch_count[0 : 8'hff];
	reg [8 - 1 : 0] data_catch_count_index_reg = 0;
	reg out_mask_enable = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] out_mask = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] out_value = 0;

	wire device_idle;
	assign device_idle = csa_in_error_empty;

	wire device_ready;
	assign device_ready = m00_axis_tvalid;

	wire [AXI_DATA_WIDTH - 1 : 0] data_catch_count_low_wire;
	assign data_catch_count_low_wire = ram_data_catch_count[data_catch_count_index_reg][AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];

	wire [AXI_DATA_WIDTH - 1 : 0] data_catch_count_high_wire;
	assign data_catch_count_high_wire = ram_data_catch_count[data_catch_count_index_reg][AXI_DATA_WIDTH * 2 - 1 : AXI_DATA_WIDTH * 1];

	always @(posedge axi_mm_clk) begin
		if(s00_axi_aresetn == 0) begin
			csa_current_channel <= 0;
			csa_current_channel_changed <= 0;

			user_rst_n_reg <= 1;

			data_catch_count_index_reg <= 0;
			out_mask_enable <= 0;
			out_mask <= 0;
			out_value <= 0;
		end
		else begin
			csa_current_channel_changed <= 0;
			user_rst_n_reg <= 1;

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
					ADDR_DATA_CATCH_COUNT_INDEX: begin
						data_catch_count_index_reg <= wdata[8 - 1 : 0];
					end
					ADDR_OUT_MASK_ENABLE: begin
						out_mask_enable <= wdata[0];
					end
					ADDR_OUT_MASK_LOW: begin
						out_mask[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0] <= wdata;
					end
					ADDR_OUT_MASK_HIGH: begin
						out_mask[AXI_DATA_WIDTH * 2 - 1 : AXI_DATA_WIDTH * 1] <= wdata;
					end
					ADDR_OUT_VALUE_LOW: begin
						out_value[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0] <= wdata;
					end
					ADDR_OUT_VALUE_HIGH: begin
						out_value[AXI_DATA_WIDTH * 2 - 1 : AXI_DATA_WIDTH * 1] <= wdata;
					end
					ADDR_RESET: begin
						user_rst_n_reg <= wdata[0];
					end
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	assign user_rst_n = user_rst_n_reg;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_in_0 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_in_1 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_in_2 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_in_3 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_in_4 = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_out_0 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_out_1 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_out_2 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_out_3 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_out_4 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_out_5 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_out_6 = 0;

	reg csa_in_valid = 0;
	reg csa_out_valid = 0;

	always @(posedge axi_mm_clk) begin
		if(s00_axi_aresetn == 0) begin
			rdata <= 0;
		end
		else begin
			if (ren == 1) begin
				case(raddr)
					ADDR_CHANNEL_INDEX: begin
						rdata <= csa_current_channel;
					end
					ADDR_IN_DATA_VALID: begin
						rdata <= {{(AXI_DATA_WIDTH - 1){1'b0}}, csa_in_valid};
					end
					ADDR_OUT_DATA_VALID: begin
						rdata <= {{(AXI_DATA_WIDTH - 1){1'b0}}, csa_out_valid};
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
					ADDR_OUT_DATA_3: begin
						rdata <= csa_out_3;
					end
					ADDR_OUT_DATA_4: begin
						rdata <= csa_out_4;
					end
					ADDR_OUT_DATA_5: begin
						rdata <= csa_out_5;
					end
					ADDR_OUT_DATA_6: begin
						rdata <= csa_out_6;
					end
					ADDR_DATA_CATCH_COUNT_INDEX: begin
						rdata <= {{(AXI_DATA_WIDTH - 8){1'b0}}, data_catch_count_index_reg};
					end
					ADDR_DATA_CATCH_COUNT_LOW: begin
						rdata <= data_catch_count_low_wire;
					end
					ADDR_DATA_CATCH_COUNT_HIGH: begin
						rdata <= data_catch_count_high_wire;
					end
					ADDR_OUT_MASK_ENABLE: begin
						rdata <= {{(AXI_DATA_WIDTH - 1){1'b0}}, out_mask_enable};
					end
					ADDR_OUT_MASK_LOW: begin
						rdata <= out_mask[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];
					end
					ADDR_OUT_MASK_HIGH: begin
						rdata <= out_mask[AXI_DATA_WIDTH * 2 - 1 : AXI_DATA_WIDTH * 1];
					end
					ADDR_OUT_VALUE_LOW: begin
						rdata <= out_value[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];
					end
					ADDR_OUT_VALUE_HIGH: begin
						rdata <= out_value[AXI_DATA_WIDTH * 2 - 1 : AXI_DATA_WIDTH * 1];
					end
					ADDR_DEVICE_IDLE: begin
						rdata <= {{(AXI_DATA_WIDTH - 1){1'b0}}, device_idle};
					end
					ADDR_DEVICE_READY: begin
						rdata <= {{(AXI_DATA_WIDTH - 1){1'b0}}, device_ready};
					end
					ADDR_RESET: begin
						rdata <= {{(AXI_DATA_WIDTH - 1){1'b0}}, user_rst_n_reg};
					end
					default: begin
						rdata <= {16'hE000, {(16 - OPT_MEM_ADDR_BITS){1'b0}}, raddr};
					end
				endcase
			end
		end
	end
	
	assign csa_in_rclk = csa_calc_clk;
	assign csa_out_wclk = csa_calc_clk;

	localparam integer CSA_CALC_IN_WIDTH_PAD = AXI_DATA_WIDTH * 2 - CSA_CALC_IN_WIDTH;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start = 0;
	wire [CSA_IN_PARAMETER_LENGTH - 1 : 0] csa_in;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_o = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_o = 0;//used for ck_processer init
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_o = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_o = 0;
	reg [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out = 0;
	wire [CSA_OUT_PARAMETER_LENGTH - 1 : 0] csa_out[0 : CSA_CALC_INST_NUM - 1];

	wire [CSA_CALC_INST_NUM - 1 : 0] csa_in_full;
	reg [CSA_CALC_INST_NUM - 1 : 0] csa_in_wen = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] w_index = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] w_state = 0;
	always @(posedge csa_in_rclk) begin
		if(rst_n == 0) begin
			csa_in_ren <= 0;

			csa_in_valid <= 0;
			csa_in_0 <= 0;
			csa_in_1 <= 0;
			csa_in_2 <= 0;
			csa_in_3 <= 0;
			csa_in_4 <= 0;

			csa_calc_logic_block <= 0;
			csa_calc_logic_in <= 0;
			csa_calc_logic_times <= 0;
			csa_calc_logic_times_start <= 0;

			w_state <= 0;
			w_index <= 0;
		end
		else begin
			csa_in_ren <= 0;
			csa_in_wen <= 0;

			if(csa_current_channel_changed == 1) begin
				csa_in_valid <= 0;
			end
			else begin
			end

			case(w_state)
				0: begin
					if(csa_in_r_ready == 1) begin
						csa_in_ren <= 1;

						w_state <= 1;
					end
					else begin
					end
				end
				1: begin
					csa_in_ren <= 1;

					w_state <= 2;
				end
				2: begin
					csa_calc_logic_block <= csa_in_rdata;

					csa_in_ren <= 1;

					w_state <= 3;
				end
				3: begin
					csa_calc_logic_in[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0] <= csa_in_rdata;

					csa_in_ren <= 1;

					w_state <= 4;
				end
				4: begin
					csa_calc_logic_in[AXI_DATA_WIDTH * 2 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1] <= csa_in_rdata[AXI_DATA_WIDTH * 1 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 0];

					csa_in_ren <= 1;

					w_state <= 5;
				end
				5: begin
					csa_calc_logic_times <= csa_in_rdata;

					w_state <= 6;
				end
				6: begin
					csa_calc_logic_times_start <= csa_in_rdata;

					w_state <= 7;
				end
				7: begin
					if(csa_in_full[w_index] == 0) begin

						w_state <= 9;
					end
					else begin

						w_state <= 8;
					end
				end
				8: begin
					if(w_index == CSA_CALC_INST_NUM - 1) begin
						w_index <= 0;
					end
					else begin
						w_index <= w_index + 1;
					end

					w_state <= 7;
				end
				9: begin
					csa_in_wen[w_index] <= 1;
					if(w_index == csa_current_channel) begin
						csa_in_valid <= 1;
						csa_in_0 <= csa_calc_logic_block;
						csa_in_1 <= csa_calc_logic_in[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];
						csa_in_2 <= {{(CSA_CALC_IN_WIDTH_PAD){1'b0}}, csa_calc_logic_in[AXI_DATA_WIDTH * 2 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1]};
						csa_in_3 <= csa_calc_logic_times;
						csa_in_4 <= csa_calc_logic_times_start;
					end
					else begin
					end

					w_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

	assign csa_in = {csa_calc_logic_times_start, csa_calc_logic_times, {(CSA_CALC_IN_WIDTH_PAD){1'b0}}, csa_calc_logic_in, csa_calc_logic_block};


	wire [CSA_CALC_INST_NUM - 1 : 0] csa_out_ready;
	reg [CSA_CALC_INST_NUM - 1 : 0] csa_out_ren = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] r_index = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] r_state = 0;
	reg data_catch_enable = 0;
	always @(posedge csa_out_wclk) begin
		if(rst_n == 0) begin
			csa_out_valid <= 0;
			csa_out_0 <= 0;
			csa_out_1 <= 0;
			csa_out_2 <= 0;
			csa_out_3 <= 0;
			csa_out_4 <= 0;
			csa_out_5 <= 0;
			csa_out_6 <= 0;

			csa_out_wen <= 0;
			csa_out_wdata <= 0;

			r_state <= 0;
			r_index <= 0;

			data_catch_enable <= 0;
		end
		else begin
			csa_out_ren <= 0;
			csa_out_wen <= 0;

			data_catch_enable <= 0;

			if(csa_current_channel_changed == 1) begin
				csa_out_valid <= 0;
			end
			else begin
			end

			case(r_state)
				0: begin
					if(csa_out_ready[r_index] == 1) begin

						r_state <= 2;
					end
					else begin

						r_state <= 1;
					end
				end
				1: begin
					if(r_index == CSA_CALC_INST_NUM - 1) begin
						r_index <= 0;
					end
					else begin
						r_index <= r_index + 1;
					end

					r_state <= 0;
				end
				2 : begin
					csa_out_ren[r_index] <= 1;

					r_state <= 3;
				end
				3: begin
					if(csa_out_ren[r_index] == 1) begin
						r_state <= 4;
					end
					else begin
						r_state <= 0;
					end
				end
				4: begin
					csa_calc_logic_block_o <= csa_out[r_index][AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];
					csa_calc_logic_in_o <= csa_out[r_index][AXI_DATA_WIDTH * 3 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1];
					csa_calc_logic_times_o <= csa_out[r_index][AXI_DATA_WIDTH * 4 - 1 : AXI_DATA_WIDTH * 3];
					csa_calc_logic_times_start_o <= csa_out[r_index][AXI_DATA_WIDTH * 5 - 1 : AXI_DATA_WIDTH * 4];
					csa_calc_logic_out <= csa_out[r_index][AXI_DATA_WIDTH * 7 - 1 : AXI_DATA_WIDTH * 5];

					if(r_index == csa_current_channel) begin
						csa_out_valid <= 1;
						csa_out_0 <= csa_calc_logic_block_o;
						csa_out_1 <= csa_calc_logic_in_o[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];
						csa_out_2 <= {{(CSA_CALC_IN_WIDTH_PAD){1'b0}}, csa_calc_logic_in_o[AXI_DATA_WIDTH * 2 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1]};
						csa_out_3 <= csa_calc_logic_times_o;
						csa_out_4 <= csa_calc_logic_times_start_o;
						csa_out_5 <= csa_calc_logic_out[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];
						csa_out_6 <= csa_calc_logic_out[AXI_DATA_WIDTH * 2 - 1 : AXI_DATA_WIDTH * 1];
					end
					else begin
					end

					r_state <= 5;
				end
				5: begin
					if(csa_out_error_full == 0) begin
						csa_out_wen <= 1;
						csa_out_wdata <= csa_calc_logic_block_o;

						r_state <= 6;
					end
					else begin
						r_state <= 5;
					end
				end
				6: begin
					if(csa_out_error_full == 0) begin
						csa_out_wen <= 1;
						csa_out_wdata <= csa_calc_logic_in_o[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];

						r_state <= 7;
					end
					else begin
						r_state <= 6;
					end
				end
				7: begin
					if(csa_out_error_full == 0) begin
						csa_out_wen <= 1;
						csa_out_wdata <= {{(CSA_CALC_IN_WIDTH_PAD){1'b0}}, csa_calc_logic_in_o[AXI_DATA_WIDTH * 2 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1]};

						r_state <= 8;
					end
					else begin
						r_state <= 7;
					end
				end
				8: begin
					if(csa_out_error_full == 0) begin
						csa_out_wen <= 1;
						csa_out_wdata <= csa_calc_logic_times_o;

						r_state <= 9;
					end
					else begin
						r_state <= 8;
					end
				end
				9: begin
					if(csa_out_error_full == 0) begin
						csa_out_wen <= 1;
						csa_out_wdata <= csa_calc_logic_times_start_o;

						r_state <= 10;
					end
					else begin
						r_state <= 9;
					end
				end
				10: begin
					if(csa_out_error_full == 0) begin
						csa_out_wen <= 1;
						csa_out_wdata <= csa_calc_logic_out[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];

						r_state <= 11;
					end
					else begin
						r_state <= 10;
					end
				end
				11: begin
					if(csa_out_error_full == 0) begin
						csa_out_wen <= 1;
						csa_out_wdata <= csa_calc_logic_out[AXI_DATA_WIDTH * 2 - 1 : AXI_DATA_WIDTH * 1];
						data_catch_enable <= 1;
						
						r_state <= 0;
					end
					else begin
						r_state <= 11;
					end
				end
				default: begin
				end
			endcase
		end
	end

	genvar i;
	generate for (i = 0; i < CSA_CALC_INST_NUM; i = i + 1)
		begin : csa_calc_unit
			//localparam integer id = i;


			csa_calc_logic_wrap #
				(
					.AXI_DATA_WIDTH(AXI_DATA_WIDTH),

					.CYPHER_DATA_WIDTH(CYPHER_DATA_WIDTH),
					.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
					.CSA_CALC_OUT_WIDTH(CSA_CALC_OUT_WIDTH),

					.CSA_IN_PARAMETER_LENGTH(CSA_IN_PARAMETER_LENGTH),
					.CSA_OUT_PARAMETER_LENGTH(CSA_OUT_PARAMETER_LENGTH)

				)
				csa_calc_logic_wrap_inst
				(
					.clk(csa_calc_clk),
					.rst_n(rst_n),

					.mask_enable(out_mask_enable),
					.mask(out_mask),
					.value(out_value),

					.csa_in_full(csa_in_full[i]),
					.csa_in_wen(csa_in_wen[i]),
					.csa_in(csa_in),

					.csa_out_ready(csa_out_ready[i]),
					.csa_out_ren(csa_out_ren[i]),
					.csa_out(csa_out[i])
				);
		end
	endgenerate

	wire [8 - 1 : 0] ram_data_catch_index;
	assign ram_data_catch_index = csa_out_wdata[8 - 1 : 0];

	reg [AXI_DATA_WIDTH - 1 : 0] ram_data_catch_count_index = 0;
	always @(posedge csa_out_wclk) begin
		if(rst_n == 0) begin
			for(ram_data_catch_count_index = 0; ram_data_catch_count_index < 256; ram_data_catch_count_index = ram_data_catch_count_index + 1) begin
				ram_data_catch_count[ram_data_catch_count_index] <= 0;
			end
		end
		else begin
			if(data_catch_enable == 1) begin
				ram_data_catch_count[ram_data_catch_index] <= ram_data_catch_count[ram_data_catch_index] + 1;
			end
			else begin
			end
		end
	end
endmodule
