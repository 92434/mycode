`timescale 1 ns / 1 ps

module prepare_csa_ck #(
		parameter integer ID = 0,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 5,
		parameter integer CYPHER_DATA_WIDTH = 8 * 8

	)
	(
		input wire clk,
		input wire rst_n,

		input wire request,
		input wire [CSA_CALC_IN_WIDTH - 1 : 0] in,
		output reg ready = 0,
		output reg [CYPHER_DATA_WIDTH - 1 : 0] ck = 0,
		
		input wire reset
	);

	reg [CSA_CALC_IN_WIDTH - 1 : 0] in_reg = 0;

	integer state = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			ready <= 0;
			ck <= 0;

			in_reg <= 0;

			state <= 0;
		end
		else begin
			case(state)
				0: begin
					if(request == 1) begin
						in_reg <= in;

						state <= 1;
					end
					else begin
					end
				end
				1: begin
					ready <= 1;
					ck <= {16'd0, ID[8 - 1 : 0], in_reg};

					state <= 2;
				end
				2: begin//now wait for reset
					if(reset == 1) begin
						ready <= 0;

						state <= 0;
					end
				end
				default: begin
				end
			endcase
		end
	end
endmodule
