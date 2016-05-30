`timescale 1 ns / 1 ps

module csa_calc_logic_tb #(
		parameter integer AXI_DATA_WIDTH = 32,

		parameter integer CSA_CALC_IN_WIDTH = 8 * 5,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 6
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
	
	wire csa_calc_logic_inuse;
	reg csa_calc_logic_request = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times = 100;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_delay = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in = 40'h1111111111;

	wire csa_calc_logic_ready;
	wire [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out;

	reg csa_calc_logic_reset = 0;

	csa_calc_logic #(
			.AXI_DATA_WIDTH(AXI_DATA_WIDTH),
			.ID(8),
			.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
			.CSA_CALC_OUT_WIDTH(CSA_CALC_OUT_WIDTH)
		) csa_calc_logic_inst(
			.clk(clk),
			.rst_n(rst_n),

			.csa_calc_logic_inuse(csa_calc_logic_inuse),

			.csa_calc_logic_request(csa_calc_logic_request),
			.csa_calc_logic_delay(csa_calc_logic_delay),
			.csa_calc_logic_times(csa_calc_logic_times),
			.csa_calc_logic_in(csa_calc_logic_in),

			.csa_calc_logic_ready(csa_calc_logic_ready),
			.csa_calc_logic_out(csa_calc_logic_out),
			.csa_calc_logic_reset(csa_calc_logic_reset)
		);

		integer state = 0;
		always @(posedge clk) begin
			if(rst_n == 0) begin
				csa_calc_logic_times <= 100;
				csa_calc_logic_delay <= 0;

				csa_calc_logic_request <= 0;
				csa_calc_logic_reset <= 0;

				csa_calc_logic_in <= 40'h1111111111;

				state <= 0;
			end
			else begin
				csa_calc_logic_request <= 0;
				csa_calc_logic_reset <= 0;

				case(state)
					0: begin
						csa_calc_logic_request <= 1;

						state <= 1;
					end
					1: begin
						if(csa_calc_logic_ready == 1) begin
							csa_calc_logic_reset <= 1;

							if(csa_calc_logic_in <= 0) begin
								csa_calc_logic_in <= csa_calc_logic_in + 1;
							end
							else begin
								csa_calc_logic_in <= 40'h1111111111;
								//csa_calc_logic_times <= csa_calc_logic_times + 1;
							end

							state <= 0;
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
