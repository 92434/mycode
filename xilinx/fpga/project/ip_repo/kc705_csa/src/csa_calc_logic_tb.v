`timescale 1 ns / 1 ps

module csa_calc_logic_tb #(
		parameter integer AXI_DATA_WIDTH = 32,

		parameter integer CYPHER_DATA_WIDTH = 8 * 8,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 6,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 8
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

	reg fifo_ready = 0;
	reg fifo_ready_reg = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] fifo_ready_delay_count = 0;
	wire csa_calc_logic_ready;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			fifo_ready <= 0;
			fifo_ready_reg <= 0;
			fifo_ready_delay_count <= 0;
		end
		else begin
			fifo_ready <= 0;
			fifo_ready_reg <= fifo_ready;

			if(fifo_ready_delay_count <= 0) begin
				fifo_ready <= 1;
			end
			else begin
				fifo_ready_delay_count <= fifo_ready_delay_count - 1;
			end

			if(csa_calc_logic_ready == 1) begin
				fifo_ready_delay_count <= 50;
			end
			else begin
			end
		end
	end

	wire fifo_ren;

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_calc_logic_block <= 0;
			csa_calc_logic_in <= 0;
			csa_calc_logic_times <= 0;
			csa_calc_logic_times_start <= 0;
		end
		else begin
			if(fifo_ready_reg == 1 && fifo_ren == 1) begin
				csa_calc_logic_block <= 32'h40000001;//csa_calc_logic_block + 1;
				csa_calc_logic_in <= csa_calc_logic_in + 1;
				//csa_calc_logic_times <= csa_calc_logic_times + 1;
				csa_calc_logic_times <= 9;
				csa_calc_logic_times_start <= 4;
				//if(csa_calc_logic_times > 5) begin
				//	csa_calc_logic_times_start <= 5;
				//end
				//else begin
				//end
			end
			else begin
			end
		end
	end

	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_o;
	wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_o;//used for ck_processer init
	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_o;
	wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_o;
	wire [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out;

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

	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_o_reg = 0;
	reg error_reg = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_calc_logic_in_o_reg <= 0;
			error_reg <= 0;
		end
		else begin
			error_reg <= 0;

			if(csa_calc_logic_ready == 1) begin
				if(csa_calc_logic_in_o_reg + 1 == csa_calc_logic_in_o) begin
				end
				else begin
					error_reg <= 1;
				end

				csa_calc_logic_in_o_reg <= csa_calc_logic_in_o;
			end
			else begin
			end
		end
	end

endmodule
