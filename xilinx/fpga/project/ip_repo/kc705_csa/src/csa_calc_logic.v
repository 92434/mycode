`timescale 1 ns / 1 ps
module csa_calc_logic #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer ID = 0,

		parameter integer CSA_CALC_IN_WIDTH = 8 * 5,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 6
	)
	(
		input wire clk,
		input wire rst_n,

		output reg csa_calc_logic_inuse = 0,

		input wire csa_calc_logic_request,
		input wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times,
		input wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in,

		output reg csa_calc_logic_ready = 0,
		output reg [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out = 0,
		input wire csa_calc_logic_reset
	);

	localparam integer CYPHER_DATA_WIDTH = 8 * 8;

	integer state = 0;

	wire prepare_csa_calc_ready;
	reg prepare_csa_calc_reset = 0;

	reg csa_calc_logic_impl_request = 0;
	wire csa_calc_logic_impl_ready;
	reg csa_calc_logic_impl_reset = 0;

	wire [CYPHER_DATA_WIDTH - 1 : 0] ck;
	wire [CYPHER_DATA_WIDTH - 1 : 0] cb;

	reg [CYPHER_DATA_WIDTH - 1 : 0] ck_in = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_calc_logic_inuse <= 0;

			prepare_csa_calc_reset <= 0;

			csa_calc_logic_impl_request <= 0;
			csa_calc_logic_impl_reset <= 0;

			csa_calc_logic_ready <= 0;
			csa_calc_logic_out <= 0;

			ck_in <= 0;

			state <= 0;
		end
		else begin
			prepare_csa_calc_reset <= 0;

			csa_calc_logic_impl_request <= 0;
			csa_calc_logic_impl_reset <= 0;

			case(state)
				0: begin
					if(csa_calc_logic_request == 1) begin
						csa_calc_logic_inuse <= 1;

						state <= 1;
					end
					else begin
					end
				end
				1: begin//wait for prepare_csa_calc
					if(prepare_csa_calc_ready == 1) begin
						ck_in <= ck;
						prepare_csa_calc_reset <= 1;

						csa_calc_logic_impl_request <= 1;

						state <= 2;
					end
					else begin
					end
				end
				2: begin
					if(csa_calc_logic_impl_ready == 1) begin
						csa_calc_logic_impl_reset <= 1;

						csa_calc_logic_ready <= 1;
						csa_calc_logic_out <= cb[CSA_CALC_OUT_WIDTH - 1 : 0];

						state <= 3;
					end
					else begin
					end
				end
				3: begin
					if(csa_calc_logic_reset == 1) begin
						csa_calc_logic_ready <= 0;
						csa_calc_logic_inuse <= 0;

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

	prepare_csa_ck #(
			.ID(ID),
			.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
			.CYPHER_DATA_WIDTH(CYPHER_DATA_WIDTH)
		) prepare_csa_ck_inst(
			.clk(clk),//input
			.rst_n(rst_n),//input

			.request(csa_calc_logic_request),//input
			.in(csa_calc_logic_in),//input
			.ready(prepare_csa_calc_ready),//output --default is 0
			.ck(ck),//output
			.reset(prepare_csa_calc_reset)//input
		);

	csa_calc_logic_impl #(
			.AXI_DATA_WIDTH(AXI_DATA_WIDTH),
			.CYPHER_DATA_WIDTH(CYPHER_DATA_WIDTH)
		) csa_calc_logic_impl_inst(
			.clk(clk),//input
			.rst_n(rst_n),//input

			.request(csa_calc_logic_impl_request),//input
			.times(csa_calc_logic_times),//input
			.ck_in(ck_in),//input
			.ready(csa_calc_logic_impl_ready),//output --default is 0
			.cb(cb),//output
			.reset(csa_calc_logic_impl_reset)//input
		);
endmodule
