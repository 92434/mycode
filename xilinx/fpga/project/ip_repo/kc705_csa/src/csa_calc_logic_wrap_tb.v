`timescale 1 ns / 1 ps

module csa_calc_logic_wrap_tb #(
		parameter integer AXI_DATA_WIDTH = 32,

		parameter integer CSA_CALC_INST_NUM = 4,

		parameter integer CYPHER_DATA_WIDTH = 8 * 8,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 6,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 8,

		parameter integer CSA_IN_PARAMETER_LENGTH = AXI_DATA_WIDTH * 5,
		parameter integer CSA_OUT_PARAMETER_LENGTH = AXI_DATA_WIDTH * 7
	)
	(
	);

	wire clk;
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));

	reg rst_n = 0;
	initial begin
		rst_n = 0;
		#2
		rst_n = 1;
	end

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

	reg [AXI_DATA_WIDTH - 1 : 0] count = 1;
	reg [AXI_DATA_WIDTH - 1 : 0] w_state = 0;

	wire [CSA_CALC_INST_NUM - 1 : 0] csa_in_full;
	reg [CSA_CALC_INST_NUM - 1 : 0] csa_in_wen = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] w_index = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] current_w_index = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_calc_logic_block <= 0;
			csa_calc_logic_in <= 0;
			csa_calc_logic_times <= 0;
			csa_calc_logic_times_start <= 0;

			count <= 1;
			w_state <= 0;
			w_index <= 0;
			current_w_index <= 0;
		end
		else begin
			csa_in_wen <= 0;

			case(w_state)
				0: begin
					if(csa_in_full[w_index] == 0) begin
						current_w_index <= w_index;

						w_state <= 1;
					end
					else begin
					end

					if(w_index == CSA_CALC_INST_NUM - 1) begin
						w_index <= 0;
					end
					else begin
						w_index <= w_index + 1;
					end
				end
				1 : begin
					csa_in_wen[current_w_index] <= 1;
					csa_calc_logic_block <= count;
					csa_calc_logic_in <= count;
					csa_calc_logic_times <= 10;
					csa_calc_logic_times_start <= 0;

					count <= count + 1;

					w_state <= 2;
				end
				2: begin
					w_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

	localparam integer CSA_CALC_IN_WIDTH_PAD = AXI_DATA_WIDTH * 2 - CSA_CALC_IN_WIDTH;
	assign csa_in = {csa_calc_logic_times_start, csa_calc_logic_times, {(CSA_CALC_IN_WIDTH_PAD){1'b0}}, csa_calc_logic_in, csa_calc_logic_block};


	wire [CSA_CALC_INST_NUM - 1 : 0] csa_out_ready;
	reg [CSA_CALC_INST_NUM - 1 : 0] csa_out_ren = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] r_index = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] current_r_index = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] r_state = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_o_reg = 0;
	reg csa_ready = 0;
	reg csa_error = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_calc_logic_block_o <= 0;
			csa_calc_logic_in_o <= 0;
			csa_calc_logic_times_o <= 0;
			csa_calc_logic_times_start_o <= 0;
			csa_calc_logic_out <= 0;

			r_state <= 0;
			r_index <= 0;
			current_r_index <= 0;
			csa_ready <= 0;
			csa_error <= 0;
			csa_calc_logic_block_o_reg <= 0;
		end
		else begin
			csa_out_ren <= 0;
			csa_ready <= 0;
			csa_error <= 0;

			case(r_state)
				0: begin
					if(csa_out_ready[r_index] == 1) begin
						current_r_index <= r_index;

						r_state <= 1;
					end
					else begin
					end

					if(r_index == CSA_CALC_INST_NUM - 1) begin
						r_index <= 0;
					end
					else begin
						r_index <= r_index + 1;
					end
				end
				1 : begin
					csa_out_ren[current_r_index] <= 1;

					r_state <= 2;
				end
				2: begin
					csa_calc_logic_block_o <= csa_out[current_r_index][AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];
					csa_calc_logic_in_o <= csa_out[current_r_index][AXI_DATA_WIDTH * 3 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1];
					csa_calc_logic_times_o <= csa_out[current_r_index][AXI_DATA_WIDTH * 4 - 1 : AXI_DATA_WIDTH * 3];
					csa_calc_logic_times_start_o <= csa_out[current_r_index][AXI_DATA_WIDTH * 5 - 1 : AXI_DATA_WIDTH * 4];
					csa_calc_logic_out <= csa_out[current_r_index][AXI_DATA_WIDTH * 7 - 1 : AXI_DATA_WIDTH * 5];

					csa_ready <= 1;
					if(csa_calc_logic_block_o_reg + 1 == csa_out[current_r_index][AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0]) begin
					end
					else begin
						csa_error <= 1;
					end

					csa_calc_logic_block_o_reg <= csa_out[current_r_index][AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];

					r_state <= 0;
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
					.CSA_IN_PARAMETER_LENGTH(CSA_IN_PARAMETER_LENGTH),

					.CSA_IN_PARAMETER_LENGTH(CSA_IN_PARAMETER_LENGTH),
					.CSA_OUT_PARAMETER_LENGTH(CSA_OUT_PARAMETER_LENGTH),

					.CSA_CALC_OUT_WIDTH(CSA_CALC_OUT_WIDTH)
				)
				csa_calc_logic_wrap_inst
				(
					.clk(clk),
					.rst_n(rst_n),

					.csa_in_full(csa_in_full[i]),
					.csa_in_wen(csa_in_wen[i]),
					.csa_in(csa_in),

					.csa_out_ready(csa_out_ready[i]),
					.csa_out_ren(csa_out_ren[i]),
					.csa_out(csa_out[i])
				);
		end
	endgenerate
endmodule
