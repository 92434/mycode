`timescale 1 ns / 1 ps

module csa_calc_logic_impl #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer CYPHER_DATA_WIDTH = 8 * 8

	)
	(
		input wire clk,
		input wire rst_n,

		input wire request,
		input wire [AXI_DATA_WIDTH - 1 : 0] times,
		input wire [CYPHER_DATA_WIDTH - 1 : 0] ck_in,

		output reg ready = 0,
		output wire [CYPHER_DATA_WIDTH - 1 : 0] cb,
		
		input wire reset
	);

	reg request_fetch = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] times_reg = 0;

	reg [CYPHER_DATA_WIDTH - 1 : 0] ck = 0;
	reg [CYPHER_DATA_WIDTH - 1 : 0] cb_reg = 0;

	integer calc_state = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			request_fetch <= 0;

			times_reg <= 50000;

			ck <= 0;

			ready <= 0;

			calc_state <= 0;
		end
		else begin
			request_fetch <= 0;

			case(calc_state)
				0: begin
					if(request == 1) begin
						times_reg <= times;
						ck <= ck_in;
						request_fetch <= 1;

						calc_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if(times_reg <= 1) begin
						ready <= 1;

						calc_state <= 2;
					end
					else begin
						ck <= cb_reg;
						request_fetch <= 1;

						calc_state <= 1;
					end
					
					times_reg <= times_reg - 1;
				end
				2: begin
					if(reset == 1) begin
						ready <= 0;

						calc_state <= 0;
					end
					else begin
					end

				end
				default: begin
				end
			endcase
		end
	end

	always @(negedge clk) begin
		if(rst_n == 0) begin
			cb_reg <= 0;
		end
		else begin
			if(request_fetch == 1) begin
				cb_reg <= cb;
			end
			else begin 
			end
		end
	end

	assign cb = ck + 1;
	//stream_cypher stream_cypher_inst(
	//		.ck(ck),//input
	//		.sb(sb),//input fixed!
	//		.cb(cb)//output
	//	);
endmodule
