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

		input wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_delay,
		input wire [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times,

		output reg csa_calc_logic_inuse = 0,

		input wire csa_calc_logic_request,
		input wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in,

		output reg csa_calc_logic_ready = 0,
		output reg [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out = 0,
		input wire csa_calc_logic_reset
	);

	localparam integer CYPHER_DATA_WIDTH = 8 * 8;

	reg request_fetch = 0;

	reg [AXI_DATA_WIDTH - 1 : 0] times_reg = 0;

	wire [CYPHER_DATA_WIDTH - 1 : 0] cb;

	wire [CYPHER_DATA_WIDTH - 1 : 0] ck_out;

	reg [32 - 1 : 0] delay_reg = 0;
	integer calc_state = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_calc_logic_inuse <= 0;
			request_fetch <= 0;

			times_reg <= 50000;

			csa_calc_logic_ready <= 0;
			csa_calc_logic_out <= 0;

			calc_state <= 0;

			delay_reg <= 0;
		end
		else begin
			request_fetch <= 0;

			case(calc_state)
				0: begin
					if(csa_calc_logic_request == 1) begin
						csa_calc_logic_inuse <= 1;
						if(csa_calc_logic_times < 1) begin
							times_reg <= 1;
						end
						else begin
							times_reg <= csa_calc_logic_times;
						end

						calc_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if(csa_calc_logic_delay > 0) begin
						delay_reg <= csa_calc_logic_delay;

						calc_state <= 2;
					end
					else begin
						delay_reg <= 0;

						calc_state <= 3;
					end

				end
				2: begin
					if(delay_reg > 0) begin
						delay_reg <= delay_reg - 1;

						calc_state <= 2;
					end
					else begin

						calc_state <= 3;
					end
				end
				3: begin
					if(times_reg > 0) begin
						request_fetch <= 1;
					end
					else begin
					end

					calc_state <= 4;
				end
				4: begin
					if(times_reg > 0) begin
						times_reg <= times_reg - 1;

						calc_state <= 1;
					end
					else begin
						csa_calc_logic_out <= cb[CSA_CALC_OUT_WIDTH - 1 : 0];
						csa_calc_logic_ready <= 1;

						calc_state <= 5;
					end
				end
				5: begin
					if(csa_calc_logic_reset == 1) begin
						csa_calc_logic_ready <= 0;
						csa_calc_logic_inuse <= 0;

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

	reg [CYPHER_DATA_WIDTH - 1 : 0] ck = 0;
	always @(negedge clk) begin
		if(rst_n == 0) begin
			ck <= 0;
		end
		else begin
			if(request_fetch == 1) begin
				ck <= ck_out;
			end
			else begin 
			end
		end
	end

	wire [CYPHER_DATA_WIDTH - 1 : 0] sb;

	assign sb = 64'hE613DB6DC11C4524;

	//assign cb = ck;
	stream_cypher stream_cypher_inst(
			.ck(ck),//input
			.sb(sb),//input fixed!
			.cb(cb)//output
		);

	wire [AXI_DATA_WIDTH - 1 : 0] loops;

	assign loops = csa_calc_logic_times - times_reg;

	//assign ck_out = (loops == 0) ? {16'd0, ID[8 - 1 : 0], csa_calc_logic_in} : cb;
	ck_processer #(
			.AXI_DATA_WIDTH(AXI_DATA_WIDTH),
			.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
			.CYPHER_DATA_WIDTH(CYPHER_DATA_WIDTH)
		) ck_processer_inst(
			.in(csa_calc_logic_in),
			.cb(cb),
			.loops(loops),

			.ck_out(ck_out)
		);
endmodule
