`timescale 1 ns / 1 ps

module csa_calc_logic_wrap #(
		parameter integer AXI_DATA_WIDTH = 32,

		parameter integer CYPHER_DATA_WIDTH = 8 * 8,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 8,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 8,

		parameter integer CSA_IN_PARAMETER_LENGTH = AXI_DATA_WIDTH * 5,
		parameter integer CSA_OUT_PARAMETER_LENGTH = AXI_DATA_WIDTH * 7

	)
	(
		input wire clk,
		input wire rst_n,

		input wire [CYPHER_DATA_WIDTH - 1 : 0] mask,
		input wire [CYPHER_DATA_WIDTH - 1 : 0] value,

		output wire csa_in_full,
		output wire csa_in_empty,
		input wire csa_in_wen,
		input wire [CSA_IN_PARAMETER_LENGTH - 1 : 0] csa_in,

		output wire csa_out_ready,
		input wire csa_out_ren,
		output wire [CSA_OUT_PARAMETER_LENGTH - 1 : 0] csa_out
	);


	wire fifo_ren;
	wire my_fifo_inst_1_r_ready;
	wire [CSA_IN_PARAMETER_LENGTH - 1 : 0] my_fifo_inst_1_rdata;
	my_fifo #(
			.DATA_WIDTH(CSA_IN_PARAMETER_LENGTH),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(22)
		) my_fifo_inst_1 (
			.rst_n(rst_n),
			.wclk(clk),
			.rclk(clk),
			.wdata(csa_in),
			.rdata(my_fifo_inst_1_rdata),
			.w_enable(csa_in_wen),
			.r_enable(fifo_ren),
			.r_ready(my_fifo_inst_1_r_ready),
			.error_full(csa_in_full),
			.error_empty(csa_in_empty)
		);

	wire [CSA_OUT_PARAMETER_LENGTH - 1 : 0] my_fifo_inst_2_wdata;
	wire csa_calc_logic_ready;
	wire csa_out_error_full;
	wire my_fifo_inst_2_error_empty;
	my_fifo #(
			.DATA_WIDTH(CSA_OUT_PARAMETER_LENGTH),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(23)
		) my_fifo_inst_2 (
			.rst_n(rst_n),
			.wclk(clk),
			.rclk(clk),
			.wdata(my_fifo_inst_2_wdata),
			.rdata(csa_out),
			.w_enable(csa_calc_logic_ready),
			.r_enable(csa_out_ren),
			.r_ready(csa_out_ready),
			.error_full(csa_out_error_full),
			.error_empty(my_fifo_inst_2_error_empty)
		);

	localparam integer CSA_CALC_IN_WIDTH_PAD = AXI_DATA_WIDTH * 2 - CSA_CALC_IN_WIDTH;
	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_wire;
	wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_wire;
	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_wire;
	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_wire;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start = 0;

	reg fifo_ren_reg = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_calc_logic_block <= 0;
			csa_calc_logic_in <= 0;
			csa_calc_logic_times <= 0;
			csa_calc_logic_times_start <= 0;
			fifo_ren_reg <= 0;
		end
		else begin
			csa_calc_logic_block <= 0;
			csa_calc_logic_in <= 0;
			csa_calc_logic_times <= 0;
			csa_calc_logic_times_start <= 0;
			fifo_ren_reg <= fifo_ren;

			if(fifo_ren_reg == 1) begin
				csa_calc_logic_block <= csa_calc_logic_block_wire;
				csa_calc_logic_in <= csa_calc_logic_in_wire;
				csa_calc_logic_times <= csa_calc_logic_times_wire;
				csa_calc_logic_times_start <= csa_calc_logic_times_start_wire;
			end
		end
	end

	wire fifo_ready;
	assign fifo_ready = (my_fifo_inst_1_r_ready == 1 && my_fifo_inst_2_error_empty == 1) ? 1 : 0;
	assign csa_calc_logic_block_wire = my_fifo_inst_1_rdata[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0];
	assign csa_calc_logic_in_wire = my_fifo_inst_1_rdata[AXI_DATA_WIDTH * 3 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1];
	assign csa_calc_logic_times_wire = my_fifo_inst_1_rdata[AXI_DATA_WIDTH * 4 - 1 : AXI_DATA_WIDTH * 3];
	assign csa_calc_logic_times_start_wire = my_fifo_inst_1_rdata[AXI_DATA_WIDTH * 5 - 1 : AXI_DATA_WIDTH * 4];

	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_o;
	wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_o;//used for ck_processer init
	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_o;
	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_o;
	wire [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out;

	assign my_fifo_inst_2_wdata = {csa_calc_logic_out, csa_calc_logic_times_start_o, csa_calc_logic_times_o, {(CSA_CALC_IN_WIDTH_PAD){1'b0}}, csa_calc_logic_in_o, csa_calc_logic_block_o};

	csa_calc_logic #(
			.AXI_DATA_WIDTH(AXI_DATA_WIDTH),
			.ID(8),
			.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
			.CSA_CALC_OUT_WIDTH(CSA_CALC_OUT_WIDTH)
		) csa_calc_logic_inst(
			.clk(clk),
			.rst_n(rst_n),

			.fifo_ready(fifo_ready),
			.fifo_ren(fifo_ren),

			.mask(mask),
			.value(value),

			.csa_out_error_full(csa_out_error_full),

			.csa_calc_logic_block(csa_calc_logic_block),
			.csa_calc_logic_in(csa_calc_logic_in),
			.csa_calc_logic_times(csa_calc_logic_times),
			.csa_calc_logic_times_start(csa_calc_logic_times_start),

			.csa_calc_logic_ready(csa_calc_logic_ready),
			.csa_calc_logic_block_o(csa_calc_logic_block_o),
			.csa_calc_logic_in_o(csa_calc_logic_in_o),
			.csa_calc_logic_times_o(csa_calc_logic_times_o),
			.csa_calc_logic_times_start_o(csa_calc_logic_times_start_o),
			.csa_calc_logic_out(csa_calc_logic_out)
		);
endmodule
