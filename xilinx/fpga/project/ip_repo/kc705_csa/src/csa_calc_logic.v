`timescale 1 ns / 1 ps
module csa_calc_logic #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer ID = 0,

		parameter integer CYPHER_DATA_WIDTH = 8 * 8,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 5,
		//parameter integer CSA_CALC_OUT_WIDTH = 8 * 6
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 8
	)
	(
		input wire clk,
		input wire rst_n,

		input wire fifo_ready,
		output reg fifo_ren = 0,

		input wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block,
		input wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in,//used for ck_processer
		input wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times,
		input wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start,

		output reg csa_calc_logic_ready = 0,
		output reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_o = 0,
		output reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_o = 0,//used for ck_processer init
		output reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_o = 0,
		output reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_o = 0,
		output reg [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out = 0
	);


	reg [AXI_DATA_WIDTH - 1 : 0] count = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			count <= 0;
		end
		else begin
			count <= count + 1;
		end
	end

	reg fifo_ready_reg_21 = 0;
	reg fifo_ready_reg_22 = 0;

	wire [CYPHER_DATA_WIDTH - 1 : 0] cb_out;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_1 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_2 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_3 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_4 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_5 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_6 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_7 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_8 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_9 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_10 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_11 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_12 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_13 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_14 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_15 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_16 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_17 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_18 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_19 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_20 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_21 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_reg_22 = 0;

	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_1 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_2 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_3 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_4 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_5 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_6 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_7 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_8 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_9 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_10 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_11 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_12 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_13 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_14 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_15 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_16 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_17 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_18 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_19 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_20 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_21 = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_reg_22 = 0;

	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_1 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_2 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_3 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_4 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_5 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_6 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_7 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_8 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_9 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_10 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_11 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_12 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_13 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_14 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_15 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_16 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_17 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_18 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_19 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_20 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_21 = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_out_reg_22 = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_1 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_2 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_3 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_4 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_5 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_6 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_7 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_8 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_9 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_10 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_11 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_12 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_13 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_14 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_15 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_16 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_17 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_18 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_19 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_20 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_21 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_reg_22 = 0;


	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_1 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_2 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_3 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_4 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_5 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_6 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_7 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_8 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_9 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_10 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_11 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_12 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_13 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_14 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_15 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_16 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_17 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_18 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_19 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_20 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_21 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_left_reg_22 = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_1 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_2 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_3 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_4 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_5 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_6 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_7 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_8 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_9 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_10 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_11 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_12 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_13 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_14 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_15 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_16 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_17 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_18 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_19 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_20 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_21 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_reg_22 = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			fifo_ready_reg_21 <= 0;
			fifo_ready_reg_22 <= 0;

			fifo_ren <= 0;

			csa_calc_logic_ready <= 0;

			csa_calc_logic_block_o <= 0;

			csa_calc_logic_in_o <= 0;

			csa_calc_logic_times_o <= 0;

			csa_calc_logic_times_start_o <= 0;

			csa_calc_logic_out <= 0;

			csa_calc_logic_block_reg <= 0;
			csa_calc_logic_block_reg_1 <= 0;
			csa_calc_logic_block_reg_2 <= 0;
			csa_calc_logic_block_reg_3 <= 0;
			csa_calc_logic_block_reg_4 <= 0;
			csa_calc_logic_block_reg_5 <= 0;
			csa_calc_logic_block_reg_6 <= 0;
			csa_calc_logic_block_reg_7 <= 0;
			csa_calc_logic_block_reg_8 <= 0;
			csa_calc_logic_block_reg_9 <= 0;
			csa_calc_logic_block_reg_10 <= 0;
			csa_calc_logic_block_reg_11 <= 0;
			csa_calc_logic_block_reg_12 <= 0;
			csa_calc_logic_block_reg_13 <= 0;
			csa_calc_logic_block_reg_14 <= 0;
			csa_calc_logic_block_reg_15 <= 0;
			csa_calc_logic_block_reg_16 <= 0;
			csa_calc_logic_block_reg_17 <= 0;
			csa_calc_logic_block_reg_18 <= 0;
			csa_calc_logic_block_reg_19 <= 0;
			csa_calc_logic_block_reg_20 <= 0;
			csa_calc_logic_block_reg_21 <= 0;
			csa_calc_logic_block_reg_22 <= 0;

			csa_calc_logic_in_reg <= 0;
			csa_calc_logic_in_reg_1 <= 0;
			csa_calc_logic_in_reg_2 <= 0;
			csa_calc_logic_in_reg_3 <= 0;
			csa_calc_logic_in_reg_4 <= 0;
			csa_calc_logic_in_reg_5 <= 0;
			csa_calc_logic_in_reg_6 <= 0;
			csa_calc_logic_in_reg_7 <= 0;
			csa_calc_logic_in_reg_8 <= 0;
			csa_calc_logic_in_reg_9 <= 0;
			csa_calc_logic_in_reg_10 <= 0;
			csa_calc_logic_in_reg_11 <= 0;
			csa_calc_logic_in_reg_12 <= 0;
			csa_calc_logic_in_reg_13 <= 0;
			csa_calc_logic_in_reg_14 <= 0;
			csa_calc_logic_in_reg_15 <= 0;
			csa_calc_logic_in_reg_16 <= 0;
			csa_calc_logic_in_reg_17 <= 0;
			csa_calc_logic_in_reg_18 <= 0;
			csa_calc_logic_in_reg_19 <= 0;
			csa_calc_logic_in_reg_20 <= 0;
			csa_calc_logic_in_reg_21 <= 0;
			csa_calc_logic_in_reg_22 <= 0;

			cb_out_reg <= 0;
			cb_out_reg_1 <= 0;
			cb_out_reg_2 <= 0;
			cb_out_reg_3 <= 0;
			cb_out_reg_4 <= 0;
			cb_out_reg_5 <= 0;
			cb_out_reg_6 <= 0;
			cb_out_reg_7 <= 0;
			cb_out_reg_8 <= 0;
			cb_out_reg_9 <= 0;
			cb_out_reg_10 <= 0;
			cb_out_reg_11 <= 0;
			cb_out_reg_12 <= 0;
			cb_out_reg_13 <= 0;
			cb_out_reg_14 <= 0;
			cb_out_reg_15 <= 0;
			cb_out_reg_16 <= 0;
			cb_out_reg_17 <= 0;
			cb_out_reg_18 <= 0;
			cb_out_reg_19 <= 0;
			cb_out_reg_20 <= 0;
			cb_out_reg_21 <= 0;
			cb_out_reg_22 <= 0;

			csa_calc_logic_times_reg <= 0;
			csa_calc_logic_times_reg_1 <= 0;
			csa_calc_logic_times_reg_2 <= 0;
			csa_calc_logic_times_reg_3 <= 0;
			csa_calc_logic_times_reg_4 <= 0;
			csa_calc_logic_times_reg_5 <= 0;
			csa_calc_logic_times_reg_6 <= 0;
			csa_calc_logic_times_reg_7 <= 0;
			csa_calc_logic_times_reg_8 <= 0;
			csa_calc_logic_times_reg_9 <= 0;
			csa_calc_logic_times_reg_10 <= 0;
			csa_calc_logic_times_reg_11 <= 0;
			csa_calc_logic_times_reg_12 <= 0;
			csa_calc_logic_times_reg_13 <= 0;
			csa_calc_logic_times_reg_14 <= 0;
			csa_calc_logic_times_reg_15 <= 0;
			csa_calc_logic_times_reg_16 <= 0;
			csa_calc_logic_times_reg_17 <= 0;
			csa_calc_logic_times_reg_18 <= 0;
			csa_calc_logic_times_reg_19 <= 0;
			csa_calc_logic_times_reg_20 <= 0;
			csa_calc_logic_times_reg_21 <= 0;
			csa_calc_logic_times_reg_22 <= 0;

			csa_calc_logic_times_left_reg <= 0;
			csa_calc_logic_times_left_reg_1 <= 0;
			csa_calc_logic_times_left_reg_2 <= 0;
			csa_calc_logic_times_left_reg_3 <= 0;
			csa_calc_logic_times_left_reg_4 <= 0;
			csa_calc_logic_times_left_reg_5 <= 0;
			csa_calc_logic_times_left_reg_6 <= 0;
			csa_calc_logic_times_left_reg_7 <= 0;
			csa_calc_logic_times_left_reg_8 <= 0;
			csa_calc_logic_times_left_reg_9 <= 0;
			csa_calc_logic_times_left_reg_10 <= 0;
			csa_calc_logic_times_left_reg_11 <= 0;
			csa_calc_logic_times_left_reg_12 <= 0;
			csa_calc_logic_times_left_reg_13 <= 0;
			csa_calc_logic_times_left_reg_14 <= 0;
			csa_calc_logic_times_left_reg_15 <= 0;
			csa_calc_logic_times_left_reg_16 <= 0;
			csa_calc_logic_times_left_reg_17 <= 0;
			csa_calc_logic_times_left_reg_18 <= 0;
			csa_calc_logic_times_left_reg_19 <= 0;
			csa_calc_logic_times_left_reg_20 <= 0;
			csa_calc_logic_times_left_reg_21 <= 0;
			csa_calc_logic_times_left_reg_22 <= 0;

			csa_calc_logic_times_start_reg <= 0;
			csa_calc_logic_times_start_reg_1 <= 0;
			csa_calc_logic_times_start_reg_2 <= 0;
			csa_calc_logic_times_start_reg_3 <= 0;
			csa_calc_logic_times_start_reg_4 <= 0;
			csa_calc_logic_times_start_reg_5 <= 0;
			csa_calc_logic_times_start_reg_6 <= 0;
			csa_calc_logic_times_start_reg_7 <= 0;
			csa_calc_logic_times_start_reg_8 <= 0;
			csa_calc_logic_times_start_reg_9 <= 0;
			csa_calc_logic_times_start_reg_10 <= 0;
			csa_calc_logic_times_start_reg_11 <= 0;
			csa_calc_logic_times_start_reg_12 <= 0;
			csa_calc_logic_times_start_reg_13 <= 0;
			csa_calc_logic_times_start_reg_14 <= 0;
			csa_calc_logic_times_start_reg_15 <= 0;
			csa_calc_logic_times_start_reg_16 <= 0;
			csa_calc_logic_times_start_reg_17 <= 0;
			csa_calc_logic_times_start_reg_18 <= 0;
			csa_calc_logic_times_start_reg_19 <= 0;
			csa_calc_logic_times_start_reg_20 <= 0;
			csa_calc_logic_times_start_reg_21 <= 0;
			csa_calc_logic_times_start_reg_22 <= 0;
		end
		else begin
			fifo_ready_reg_21 <= fifo_ready;
			fifo_ready_reg_22 <= fifo_ready_reg_21;

			fifo_ren <= 0;

			csa_calc_logic_ready <= 0;

			csa_calc_logic_block_reg <= csa_calc_logic_block;
			csa_calc_logic_block_reg_1 <= csa_calc_logic_block_reg;
			csa_calc_logic_block_reg_2 <= csa_calc_logic_block_reg_1;
			csa_calc_logic_block_reg_3 <= csa_calc_logic_block_reg_2;
			csa_calc_logic_block_reg_4 <= csa_calc_logic_block_reg_3;
			csa_calc_logic_block_reg_5 <= csa_calc_logic_block_reg_4;
			csa_calc_logic_block_reg_6 <= csa_calc_logic_block_reg_5;
			csa_calc_logic_block_reg_7 <= csa_calc_logic_block_reg_6;
			csa_calc_logic_block_reg_8 <= csa_calc_logic_block_reg_7;
			csa_calc_logic_block_reg_9 <= csa_calc_logic_block_reg_8;
			csa_calc_logic_block_reg_10 <= csa_calc_logic_block_reg_9;
			csa_calc_logic_block_reg_11 <= csa_calc_logic_block_reg_10;
			csa_calc_logic_block_reg_12 <= csa_calc_logic_block_reg_11;
			csa_calc_logic_block_reg_13 <= csa_calc_logic_block_reg_12;
			csa_calc_logic_block_reg_14 <= csa_calc_logic_block_reg_13;
			csa_calc_logic_block_reg_15 <= csa_calc_logic_block_reg_14;
			csa_calc_logic_block_reg_16 <= csa_calc_logic_block_reg_15;
			csa_calc_logic_block_reg_17 <= csa_calc_logic_block_reg_16;
			csa_calc_logic_block_reg_18 <= csa_calc_logic_block_reg_17;
			csa_calc_logic_block_reg_19 <= csa_calc_logic_block_reg_18;
			csa_calc_logic_block_reg_20 <= csa_calc_logic_block_reg_19;
			csa_calc_logic_block_reg_21 <= csa_calc_logic_block_reg_20;
			csa_calc_logic_block_reg_22 <= csa_calc_logic_block_reg_21;

			csa_calc_logic_in_reg <= csa_calc_logic_in;
			csa_calc_logic_in_reg_1 <= csa_calc_logic_in_reg;
			csa_calc_logic_in_reg_2 <= csa_calc_logic_in_reg_1;
			csa_calc_logic_in_reg_3 <= csa_calc_logic_in_reg_2;
			csa_calc_logic_in_reg_4 <= csa_calc_logic_in_reg_3;
			csa_calc_logic_in_reg_5 <= csa_calc_logic_in_reg_4;
			csa_calc_logic_in_reg_6 <= csa_calc_logic_in_reg_5;
			csa_calc_logic_in_reg_7 <= csa_calc_logic_in_reg_6;
			csa_calc_logic_in_reg_8 <= csa_calc_logic_in_reg_7;
			csa_calc_logic_in_reg_9 <= csa_calc_logic_in_reg_8;
			csa_calc_logic_in_reg_10 <= csa_calc_logic_in_reg_9;
			csa_calc_logic_in_reg_11 <= csa_calc_logic_in_reg_10;
			csa_calc_logic_in_reg_12 <= csa_calc_logic_in_reg_11;
			csa_calc_logic_in_reg_13 <= csa_calc_logic_in_reg_12;
			csa_calc_logic_in_reg_14 <= csa_calc_logic_in_reg_13;
			csa_calc_logic_in_reg_15 <= csa_calc_logic_in_reg_14;
			csa_calc_logic_in_reg_16 <= csa_calc_logic_in_reg_15;
			csa_calc_logic_in_reg_17 <= csa_calc_logic_in_reg_16;
			csa_calc_logic_in_reg_18 <= csa_calc_logic_in_reg_17;
			csa_calc_logic_in_reg_19 <= csa_calc_logic_in_reg_18;
			csa_calc_logic_in_reg_20 <= csa_calc_logic_in_reg_19;
			csa_calc_logic_in_reg_21 <= csa_calc_logic_in_reg_20;
			csa_calc_logic_in_reg_22 <= csa_calc_logic_in_reg_21;

			cb_out_reg <= csa_calc_logic_in_reg;//entry
			cb_out_reg_1 <= cb_out_reg;
			cb_out_reg_2 <= cb_out_reg_1;
			cb_out_reg_3 <= cb_out_reg_2;
			cb_out_reg_4 <= cb_out_reg_3;
			cb_out_reg_5 <= cb_out_reg_4;
			cb_out_reg_6 <= cb_out_reg_5;
			cb_out_reg_7 <= cb_out_reg_6;
			cb_out_reg_8 <= cb_out_reg_7;
			cb_out_reg_9 <= cb_out_reg_8;
			cb_out_reg_10 <= cb_out_reg_9;
			cb_out_reg_11 <= cb_out_reg_10;
			cb_out_reg_12 <= cb_out_reg_11;
			cb_out_reg_13 <= cb_out_reg_12;
			cb_out_reg_14 <= cb_out_reg_13;
			cb_out_reg_15 <= cb_out_reg_14;
			cb_out_reg_16 <= cb_out_reg_15;
			cb_out_reg_17 <= cb_out_reg_16;
			cb_out_reg_18 <= cb_out_reg_17;
			cb_out_reg_19 <= cb_out_reg_18;
			cb_out_reg_20 <= cb_out_reg_19;
			cb_out_reg_21 <= cb_out_reg_20;
			cb_out_reg_22 <= cb_out_reg_21;
			//cb_out_reg_22 -->result

			csa_calc_logic_times_reg <= csa_calc_logic_times;//entry
			csa_calc_logic_times_reg_1 <= csa_calc_logic_times_reg;
			csa_calc_logic_times_reg_2 <= csa_calc_logic_times_reg_1;
			csa_calc_logic_times_reg_3 <= csa_calc_logic_times_reg_2;
			csa_calc_logic_times_reg_4 <= csa_calc_logic_times_reg_3;
			csa_calc_logic_times_reg_5 <= csa_calc_logic_times_reg_4;
			csa_calc_logic_times_reg_6 <= csa_calc_logic_times_reg_5;
			csa_calc_logic_times_reg_7 <= csa_calc_logic_times_reg_6;
			csa_calc_logic_times_reg_8 <= csa_calc_logic_times_reg_7;
			csa_calc_logic_times_reg_9 <= csa_calc_logic_times_reg_8;
			csa_calc_logic_times_reg_10 <= csa_calc_logic_times_reg_9;
			csa_calc_logic_times_reg_11 <= csa_calc_logic_times_reg_10;
			csa_calc_logic_times_reg_12 <= csa_calc_logic_times_reg_11;
			csa_calc_logic_times_reg_13 <= csa_calc_logic_times_reg_12;
			csa_calc_logic_times_reg_14 <= csa_calc_logic_times_reg_13;
			csa_calc_logic_times_reg_15 <= csa_calc_logic_times_reg_14;
			csa_calc_logic_times_reg_16 <= csa_calc_logic_times_reg_15;
			csa_calc_logic_times_reg_17 <= csa_calc_logic_times_reg_16;
			csa_calc_logic_times_reg_18 <= csa_calc_logic_times_reg_17;
			csa_calc_logic_times_reg_19 <= csa_calc_logic_times_reg_18;
			csa_calc_logic_times_reg_20 <= csa_calc_logic_times_reg_19;
			csa_calc_logic_times_reg_21 <= csa_calc_logic_times_reg_20;
			csa_calc_logic_times_reg_22 <= csa_calc_logic_times_reg_21;

			csa_calc_logic_times_start_reg <= csa_calc_logic_times_start;
			csa_calc_logic_times_start_reg_1 <= csa_calc_logic_times_start_reg;
			csa_calc_logic_times_start_reg_2 <= csa_calc_logic_times_start_reg_1;
			csa_calc_logic_times_start_reg_3 <= csa_calc_logic_times_start_reg_2;
			csa_calc_logic_times_start_reg_4 <= csa_calc_logic_times_start_reg_3;
			csa_calc_logic_times_start_reg_5 <= csa_calc_logic_times_start_reg_4;
			csa_calc_logic_times_start_reg_6 <= csa_calc_logic_times_start_reg_5;
			csa_calc_logic_times_start_reg_7 <= csa_calc_logic_times_start_reg_6;
			csa_calc_logic_times_start_reg_8 <= csa_calc_logic_times_start_reg_7;
			csa_calc_logic_times_start_reg_9 <= csa_calc_logic_times_start_reg_8;
			csa_calc_logic_times_start_reg_10 <= csa_calc_logic_times_start_reg_9;
			csa_calc_logic_times_start_reg_11 <= csa_calc_logic_times_start_reg_10;
			csa_calc_logic_times_start_reg_12 <= csa_calc_logic_times_start_reg_11;
			csa_calc_logic_times_start_reg_13 <= csa_calc_logic_times_start_reg_12;
			csa_calc_logic_times_start_reg_14 <= csa_calc_logic_times_start_reg_13;
			csa_calc_logic_times_start_reg_15 <= csa_calc_logic_times_start_reg_14;
			csa_calc_logic_times_start_reg_16 <= csa_calc_logic_times_start_reg_15;
			csa_calc_logic_times_start_reg_17 <= csa_calc_logic_times_start_reg_16;
			csa_calc_logic_times_start_reg_18 <= csa_calc_logic_times_start_reg_17;
			csa_calc_logic_times_start_reg_19 <= csa_calc_logic_times_start_reg_18;
			csa_calc_logic_times_start_reg_20 <= csa_calc_logic_times_start_reg_19;
			csa_calc_logic_times_start_reg_21 <= csa_calc_logic_times_start_reg_20;
			csa_calc_logic_times_start_reg_22 <= csa_calc_logic_times_start_reg_21;

			csa_calc_logic_times_left_reg <= csa_calc_logic_times - csa_calc_logic_times_start;//entry
			csa_calc_logic_times_left_reg_1 <= csa_calc_logic_times_left_reg;
			csa_calc_logic_times_left_reg_2 <= csa_calc_logic_times_left_reg_1;
			csa_calc_logic_times_left_reg_3 <= csa_calc_logic_times_left_reg_2;
			csa_calc_logic_times_left_reg_4 <= csa_calc_logic_times_left_reg_3;
			csa_calc_logic_times_left_reg_5 <= csa_calc_logic_times_left_reg_4;
			csa_calc_logic_times_left_reg_6 <= csa_calc_logic_times_left_reg_5;
			csa_calc_logic_times_left_reg_7 <= csa_calc_logic_times_left_reg_6;
			csa_calc_logic_times_left_reg_8 <= csa_calc_logic_times_left_reg_7;
			csa_calc_logic_times_left_reg_9 <= csa_calc_logic_times_left_reg_8;
			csa_calc_logic_times_left_reg_10 <= csa_calc_logic_times_left_reg_9;
			csa_calc_logic_times_left_reg_11 <= csa_calc_logic_times_left_reg_10;
			csa_calc_logic_times_left_reg_12 <= csa_calc_logic_times_left_reg_11;
			csa_calc_logic_times_left_reg_13 <= csa_calc_logic_times_left_reg_12;
			csa_calc_logic_times_left_reg_14 <= csa_calc_logic_times_left_reg_13;
			csa_calc_logic_times_left_reg_15 <= csa_calc_logic_times_left_reg_14;
			csa_calc_logic_times_left_reg_16 <= csa_calc_logic_times_left_reg_15;
			csa_calc_logic_times_left_reg_17 <= csa_calc_logic_times_left_reg_16;
			csa_calc_logic_times_left_reg_18 <= csa_calc_logic_times_left_reg_17;
			csa_calc_logic_times_left_reg_19 <= csa_calc_logic_times_left_reg_18;
			csa_calc_logic_times_left_reg_20 <= csa_calc_logic_times_left_reg_19;
			csa_calc_logic_times_left_reg_21 <= csa_calc_logic_times_left_reg_20;
			csa_calc_logic_times_left_reg_22 <= csa_calc_logic_times_left_reg_21;

			if(csa_calc_logic_times_left_reg > 0) begin
				csa_calc_logic_times_left_reg_1 <= csa_calc_logic_times_left_reg - 1;
			end
			else begin
			end

			if(csa_calc_logic_times_left_reg_20 <= 0) begin
				if(fifo_ready == 1) begin//data is ready
					fifo_ren <= 1;
				end
				else begin
				end
			end
			else begin
			end

			if(csa_calc_logic_times_reg_22 > 0) begin
				if(csa_calc_logic_times_left_reg_22 > 0) begin
					csa_calc_logic_block_reg <= csa_calc_logic_block_reg_22;
					csa_calc_logic_in_reg <= csa_calc_logic_in_reg_22;
					csa_calc_logic_times_reg <= csa_calc_logic_times_reg_22;
					csa_calc_logic_times_start_reg <= csa_calc_logic_times_start_reg_22;
					csa_calc_logic_times_left_reg <= csa_calc_logic_times_left_reg_22;
					//cb_out_reg <= cb_out;//for debug
					cb_out_reg <= cb_out_reg_22;//for debug
				end
				else begin//loop over? out!!!
					csa_calc_logic_ready <= 1;
					csa_calc_logic_block_o <= csa_calc_logic_block_reg_22;
					csa_calc_logic_in_o <= csa_calc_logic_in_reg_22;
					csa_calc_logic_times_o <= csa_calc_logic_times_reg_22;
					csa_calc_logic_times_start_o <= csa_calc_logic_times_start_reg_22;
					csa_calc_logic_out <= cb_out;
				end
			end
			else begin
			end

			if(csa_calc_logic_times_left_reg_22 <= 0) begin
				if(fifo_ready_reg_22 == 0) begin//data is not ready
					csa_calc_logic_block_reg <= 0;
					csa_calc_logic_in_reg <= 0;
					csa_calc_logic_times_reg <= 0;
					csa_calc_logic_times_start_reg <= 0;
					csa_calc_logic_times_left_reg <= 0;
					cb_out_reg <= 0;//for debug
				end
				else begin
				end
			end
			else begin
			end
		end
	end

	wire [AXI_DATA_WIDTH - 1 : 0] loops;
	assign loops = csa_calc_logic_times_reg - csa_calc_logic_times_left_reg;
	wire init;
	assign init = (csa_calc_logic_times_start_reg == loops) ? 1 : 0;
	wire [CYPHER_DATA_WIDTH - 1 : 0] ck_out;
	//assign ck_out = (loops == 0) ? {16'd0, ID[8 - 1 : 0], csa_calc_logic_in_reg} : cb_out_reg;
	assign ck_out = cb_out_reg;
	//ck_processer #(
	//		.AXI_DATA_WIDTH(AXI_DATA_WIDTH),
	//		.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
	//		.CYPHER_DATA_WIDTH(CYPHER_DATA_WIDTH)
	//	) ck_processer_inst(
	//		.init(init);
	//		.in(csa_calc_logic_in_reg),
	//		.cb(cb_out_reg),
	//		.loops(loops),

	//		.ck_out(ck_out)
	//	);

	reg [CYPHER_DATA_WIDTH - 1 : 0] ck_out_reg = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			ck_out_reg <= 0;
		end
		else begin
			ck_out_reg <= ck_out;
		end
	end

	//assign cb_out = ck_out_reg;
	wire [CYPHER_DATA_WIDTH - 1 : 0] sb;
	assign sb = 64'hE613DB6DC11C4524;
	stream_cypher stream_cypher_inst(
			.clk(clk),
			.rst_n(rst_n),

			.sb(sb),//input fixed!
			.ck(ck_out_reg),//input
			.cb(cb_out)//output
		);
endmodule
