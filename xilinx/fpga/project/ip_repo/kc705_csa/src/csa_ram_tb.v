`timescale 1 ns / 1 ps

module csa_ram_tb #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,


		parameter integer CSA_CALC_INST_NUM = 4,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 5,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 6
	)
	(
	);

	wire axi_mm_clk;//input
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(axi_mm_clk));

	reg rst_n = 0;//input
	initial begin
		rst_n = 0;
		#2
		rst_n = 1;
	end

	wire [(C_S_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb;//input
	assign wstrb = {(C_S_AXI_DATA_WIDTH / 8){1'b1}};

	reg wen = 0;//input
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] wdata = 0;//input
	reg [OPT_MEM_ADDR_BITS - 1 : 0] waddr = 0;//input

	reg ren = 0;//input
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] rdata;//output
	reg [OPT_MEM_ADDR_BITS - 1 : 0] raddr = 0;//input

	wire csa_calc_clk;//input --csa_calc_clk MUST NOT slower than axi_mm_clk!!!
	assign csa_calc_clk = axi_mm_clk;

	wire csa_in_r_ready;//input
	wire csa_in_rclk;//output
	wire csa_in_ren;//output
	wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_in_rdata;//input


	wire csa_in_wclk;
	assign csa_in_wclk = axi_mm_clk;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_in_wdata = 40'h1234567890;
	reg csa_in_wen = 0;
	wire csa_in_error_full;
	wire csa_in_error_empty;

	my_fifo #(
			.DATA_WIDTH(CSA_CALC_IN_WIDTH),
			.BULK_OF_DATA(4),
			.BULK_DEPTH(256)
		) my_fifo_inst (
			.rst_n(rst_n),
			.wclk(csa_in_wclk),
			.rclk(csa_in_rclk),
			.wdata(csa_in_wdata),
			.rdata(csa_in_rdata),
			.w_enable(csa_in_wen),
			.r_enable(csa_in_ren),
			.r_ready(csa_in_r_ready),
			.error_full(csa_in_error_full),
			.error_empty(csa_in_error_empty)
		);

	wire csa_out_error_full;//input
	assign csa_out_error_full = 0;
	wire csa_out_wclk;//output
	wire csa_out_wen;//output
	wire [CSA_CALC_OUT_WIDTH - 1 : 0] csa_out_wdata;//output
	wire axis_m_r_ready;

	assign axis_m_r_ready = 1;

	csa_ram #(
			.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),

			.CSA_CALC_INST_NUM(CSA_CALC_INST_NUM),
			.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
			.CSA_CALC_OUT_WIDTH(CSA_CALC_OUT_WIDTH)
		) csa_ram_inst(
			.axi_mm_clk(axi_mm_clk),
			.rst_n(rst_n),

			.wstrb(wstrb),
			.wen(wen),
			.wdata(wdata),
			.waddr(waddr),

			.ren(ren),
			.rdata(rdata),
			.raddr(raddr),

			.csa_calc_clk(csa_calc_clk),

			.csa_in_r_ready(csa_in_r_ready),
			.csa_in_rclk(csa_in_rclk),
			.csa_in_ren(csa_in_ren),
			.csa_in_rdata(csa_in_rdata),

			.csa_out_error_full(csa_out_error_full),
			.csa_out_wclk(csa_out_wclk),
			.csa_out_wen(csa_out_wen),
			.csa_out_wdata(csa_out_wdata),

			.axis_m_r_ready(axis_m_r_ready)
		);

	localparam integer ADDR_CSA_BUSY = 0;
	localparam integer ADDR_CSA_READY = ADDR_CSA_BUSY + 1;

	localparam integer ADDR_CHANNEL_INDEX = ADDR_CSA_READY + 1;

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

	localparam integer ADDR_CALC_TIMES = ADDR_OUT_DATA_2 + 1;

	integer state = 0;
	always @(posedge csa_in_wclk) begin
		if(rst_n == 0) begin
			state <= 0;

			wen <= 0;
			waddr <= 0;
			wdata <= 0;

			ren <= 0;
			raddr <= 0;

			csa_in_wen <= 0;
			//csa_in_wdata <= 40'h1234567890;
			csa_in_wdata <= 0;
		end
		else begin 
			wen <= 0;
			ren <= 0;
			csa_in_wen <= 0;

			case(state)
				0: begin
					wen <= 1;
					waddr <= ADDR_CALC_TIMES;
					wdata <= 2;

					state <= 1;
				end
				1: begin
					wen <= 1;
					waddr <= ADDR_CHANNEL_INDEX;
					wdata <= 1;

					state <= 2;
				end
				2: begin
					raddr <= ADDR_CSA_BUSY;
					ren <= 1;
					
					state <= 3;
				end
				3: begin

					state <= 4;
				end
				4: begin
					if(rdata == 0) begin

						state <= 5;
					end
				end
				5: begin
					if(csa_in_error_full == 0) begin
						csa_in_wen <= 1;

						state <= 6;
					end
				end
				6: begin
					if(csa_in_wdata == 1) begin
						csa_in_wdata <= 0;
						//state <= 5; //test calc logic
						state <= 7; //test ram
					end
					else begin
						csa_in_wdata <= csa_in_wdata + 1;
						state <= 5;
					end


				end
				7: begin
					raddr <= ADDR_IN_DATA_VALID;
					ren <= 1;
					
					state <= 8;
				end
				8: begin
					state <= 9;
				end
				9: begin
					if(rdata == 1) begin
						raddr <= ADDR_IN_DATA_0;
						ren <= 1;
						
						state <= 10;
					end
					else begin
						state <= 1;
					end
				end
				10: begin
					raddr <= ADDR_IN_DATA_1;
					ren <= 1;
					
					state <= 11;
				end
				11: begin
					raddr <= ADDR_IN_DATA_2;
					ren <= 1;
					
					state <= 12;
				end
				12: begin
					raddr <= ADDR_IN_DATA_3;
					ren <= 1;
					
					state <= 13;
				end
				13: begin
					raddr <= ADDR_IN_DATA_4;
					ren <= 1;
					
					state <= 14;
				end
				14: begin
					raddr <= ADDR_OUT_DATA_VALID;
					ren <= 1;
					
					state <= 15;
				end
				15: begin
					state <= 16;
				end
				16: begin
					if(rdata == 1) begin
						raddr <= ADDR_OUT_DATA_0;
						ren <= 1;
						
						state <= 17;
					end
					else begin
						state <= 1;
					end
				end
				17: begin
					raddr <= ADDR_OUT_DATA_1;
					ren <= 1;
					
					state <= 18;
				end
				18: begin
					raddr <= ADDR_OUT_DATA_2;
					ren <= 1;
					
					state <= 1;
				end
				default: begin
				end
			endcase
		end
	end
endmodule
