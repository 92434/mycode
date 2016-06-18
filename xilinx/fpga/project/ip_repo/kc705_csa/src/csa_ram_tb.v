`timescale 1 ns / 1 ps

module csa_ram_tb #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,

		parameter integer CSA_CALC_INST_NUM = 4,

		parameter integer CYPHER_DATA_WIDTH = 8 * 8,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 6,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 8,

		parameter integer CSA_IN_PARAMETER_LENGTH = AXI_DATA_WIDTH * 5,
		parameter integer CSA_OUT_PARAMETER_LENGTH = AXI_DATA_WIDTH * 7
	)
	(
	);

	localparam integer CSA_CALC_IN_WIDTH_PAD = AXI_DATA_WIDTH * 2 - CSA_CALC_IN_WIDTH;

	wire axi_mm_clk;//input
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(axi_mm_clk));

	reg rst_n = 0;//input
	initial begin
		rst_n = 0;
		#2
		rst_n = 1;
	end

	wire [(AXI_DATA_WIDTH / 8) - 1 : 0] wstrb;//input
	assign wstrb = {(AXI_DATA_WIDTH / 8){1'b1}};

	reg wen = 0;//input
	reg [AXI_DATA_WIDTH - 1 : 0] wdata = 0;//input
	reg [OPT_MEM_ADDR_BITS - 1 : 0] waddr = 0;//input

	reg ren = 0;//input
	wire [AXI_DATA_WIDTH - 1 : 0] rdata;//output
	reg [OPT_MEM_ADDR_BITS - 1 : 0] raddr = 0;//input

	wire csa_calc_clk;//input --csa_calc_clk MUST NOT slower than axi_mm_clk!!!
	assign csa_calc_clk = axi_mm_clk;


	wire csa_in_wclk;
	assign csa_in_wclk = axi_mm_clk;
	reg csa_in_wen = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_in_wdata = 0;

	wire csa_in_rclk;
	wire csa_in_ren;
	wire [AXI_DATA_WIDTH - 1 : 0] csa_in_rdata;

	wire csa_in_r_ready;
	wire csa_in_error_full;
	wire csa_in_error_empty;
	my_fifo #(
			.DATA_WIDTH(AXI_DATA_WIDTH),
			.BULK_OF_DATA(5),
			.BULK_DEPTH(256)
		) my_fifo_inst_1 (
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

	wire csa_out_wclk;
	wire csa_out_wen;
	wire [AXI_DATA_WIDTH - 1 : 0] csa_out_wdata;

	wire csa_out_rclk;
	assign csa_out_rclk = axi_mm_clk;
	reg csa_out_ren = 0;
	wire [AXI_DATA_WIDTH - 1 : 0] csa_out_rdata;

	wire csa_out_r_ready;
	wire csa_out_error_full;
	wire csa_out_error_empty;
	my_fifo #(
			.DATA_WIDTH(AXI_DATA_WIDTH),
			.BULK_OF_DATA(7),
			.BULK_DEPTH(256)
		) my_fifo_inst_2 (
			.rst_n(rst_n),
			.wclk(csa_out_wclk),
			.rclk(csa_out_rclk),
			.wdata(csa_out_wdata),
			.rdata(csa_out_rdata),
			.w_enable(csa_out_wen),
			.r_enable(csa_out_ren),
			.r_ready(csa_out_r_ready),
			.error_full(csa_out_error_full),
			.error_empty(csa_out_error_empty)
		);

	wire user_rst_n;
	csa_ram #(
			.AXI_DATA_WIDTH(AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),

			.CSA_CALC_INST_NUM(CSA_CALC_INST_NUM),

			.CYPHER_DATA_WIDTH(CYPHER_DATA_WIDTH),
			.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
			.CSA_CALC_OUT_WIDTH(CSA_CALC_OUT_WIDTH),

			.CSA_IN_PARAMETER_LENGTH(CSA_IN_PARAMETER_LENGTH),
			.CSA_OUT_PARAMETER_LENGTH(CSA_OUT_PARAMETER_LENGTH)
		) csa_ram_inst(
			.axi_mm_clk(axi_mm_clk),
			.s00_axi_aresetn(rst_n),

			.wstrb(wstrb),
			.wen(wen),
			.wdata(wdata),
			.waddr(waddr),

			.ren(ren),
			.rdata(rdata),
			.raddr(raddr),

			.csa_calc_clk(csa_calc_clk),
			.user_rst_n(user_rst_n),
			.rst_n(rst_n),

			.csa_in_r_ready(csa_in_r_ready),
			.csa_in_rclk(csa_in_rclk),
			.csa_in_ren(csa_in_ren),
			.csa_in_rdata(csa_in_rdata),

			.csa_out_error_full(csa_out_error_full),
			.csa_out_wclk(csa_out_wclk),
			.csa_out_wen(csa_out_wen),
			.csa_out_wdata(csa_out_wdata)
		);

	reg [AXI_DATA_WIDTH -1 : 0] count = 1;
	reg [AXI_DATA_WIDTH -1 : 0] w_state = 0;
	always @(posedge csa_in_wclk) begin
		if(rst_n == 0) begin
			csa_in_wen <= 0;
			csa_in_wdata <= 0;

			count <= 1;

			w_state <= 0;
		end
		else begin
			csa_in_wen <= 0;

			case(w_state)
				0: begin
					csa_in_wen <= 1;
					csa_in_wdata <= count;//csa_calc_logic_block

					w_state <= 1;
				end
				1: begin
					csa_in_wen <= 1;
					csa_in_wdata <= count;//csa_calc_logic_in[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0]

					w_state <= 2;
				end
				2: begin
					csa_in_wen <= 1;
					csa_in_wdata <= 0;//csa_calc_logic_in[AXI_DATA_WIDTH * 2 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1]

					w_state <= 3;
				end
				3: begin
					csa_in_wen <= 1;
					csa_in_wdata <= 10;//csa_calc_logic_times

					w_state <= 4;
				end
				4: begin
					if(csa_in_error_full == 0) begin
						csa_in_wen <= 1;
						csa_in_wdata <= 0;//csa_calc_logic_times_start

						w_state <= 5;
					end
					else begin
					end
				end
				5: begin
					count <= count + 1;

					w_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_o = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_block_o_reg = 0;
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in_o = 0;//used for ck_processer init
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_o = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] csa_calc_logic_times_start_o = 0;
	reg [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out = 0;
	reg csa_ready = 0;
	reg csa_error = 0;

	reg [AXI_DATA_WIDTH -1 : 0] r_state = 0;
	always @(posedge csa_out_rclk) begin
		if(rst_n == 0) begin
			csa_out_ren <= 0;

			csa_calc_logic_block_o <= 0;
			csa_calc_logic_block_o_reg <= 0;
			csa_calc_logic_in_o <= 0;
			csa_calc_logic_times_o <= 0;
			csa_calc_logic_times_start_o <= 0;
			csa_calc_logic_out <= 0;
			csa_ready <= 0;
			csa_error <= 0;

			r_state <= 0;
		end
		else begin
			csa_out_ren <= 0;

			csa_ready <= 0;
			csa_error <= 0;

			case(r_state)
				0: begin
					if(csa_out_r_ready == 1) begin
						csa_out_ren <= 1;

						r_state <= 1;
					end
					else begin
					end
				end
				1: begin
					csa_out_ren <= 1;
					csa_calc_logic_block_o <= csa_out_rdata;

					r_state <= 2;
				end
				2: begin
					csa_out_ren <= 1;
					csa_calc_logic_in_o[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0] <= csa_out_rdata;

					r_state <= 3;
				end
				3: begin
					csa_out_ren <= 1;
					csa_calc_logic_in_o[AXI_DATA_WIDTH * 2 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 1] <= csa_out_rdata[AXI_DATA_WIDTH * 1 - 1 - CSA_CALC_IN_WIDTH_PAD : AXI_DATA_WIDTH * 0];

					r_state <= 4;
				end
				4: begin
					csa_out_ren <= 1;
					csa_calc_logic_times_o <= csa_out_rdata;

					r_state <= 5;
				end
				5: begin
					csa_out_ren <= 1;
					csa_calc_logic_times_start_o <= csa_out_rdata;

					r_state <= 6;
				end
				6: begin
					csa_out_ren <= 1;
					csa_calc_logic_out[AXI_DATA_WIDTH * 1 - 1 : AXI_DATA_WIDTH * 0] <= csa_out_rdata;

					r_state <= 7;
				end
				7: begin
					csa_calc_logic_out[AXI_DATA_WIDTH * 2 - 1 : AXI_DATA_WIDTH * 1] <= csa_out_rdata;
					csa_ready <= 1;
					if(csa_calc_logic_block_o_reg + 1 == csa_calc_logic_block_o) begin
					end
					else begin
						csa_error <= 1;
					end

					csa_calc_logic_block_o_reg <= csa_calc_logic_block_o;

					r_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

	localparam integer ADDR_CHANNEL_INDEX = 0;

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
	localparam integer ADDR_OUT_DATA_3 = ADDR_OUT_DATA_2 + 1;
	localparam integer ADDR_OUT_DATA_4 = ADDR_OUT_DATA_3 + 1;
	localparam integer ADDR_OUT_DATA_5 = ADDR_OUT_DATA_4 + 1;
	localparam integer ADDR_OUT_DATA_6 = ADDR_OUT_DATA_5 + 1;

	integer state = 0;
	reg in_ready = 0;
	reg out_ready = 0;
	always @(posedge axi_mm_clk) begin
		if(rst_n == 0) begin
			state <= 0;

			wen <= 0;
			waddr <= 0;
			wdata <= 0;

			ren <= 0;
			raddr <= 0;

			in_ready <= 0;
			out_ready <= 0;
		end
		else begin 
			wen <= 0;
			ren <= 0;

			in_ready <= 0;
			out_ready <= 0;

			case(state)
				0: begin
					wen <= 1;

					waddr <= ADDR_CHANNEL_INDEX;
					wdata <= 1;

					state <= 1;
				end
				1: begin
					ren <= 1;

					raddr <= ADDR_IN_DATA_VALID;
					
					state <= 2;
				end
				2: begin

					state <= 3;
				end
				3: begin
					if(rdata == 1) begin
						ren <= 1;

						in_ready <= 1;
						raddr <= ADDR_IN_DATA_0;
						
						state <= 4;
					end
					else begin

						state <= 1;
					end
				end
				4: begin
					ren <= 1;

					raddr <= ADDR_IN_DATA_1;
					
					state <= 5;
				end
				5: begin
					ren <= 1;

					raddr <= ADDR_IN_DATA_2;
					
					state <= 6;
				end
				6: begin
					ren <= 1;

					raddr <= ADDR_IN_DATA_3;
					
					state <= 7;
				end
				7: begin
					ren <= 1;

					raddr <= ADDR_IN_DATA_4;
					
					state <= 8;
				end
				8: begin
					ren <= 1;

					raddr <= ADDR_OUT_DATA_VALID;
					
					state <= 9;
				end
				9: begin
					state <= 10;
				end
				10: begin
					if(rdata == 1) begin
						ren <= 1;

						out_ready <= 1;
						raddr <= ADDR_OUT_DATA_0;
						
						state <= 11;
					end
					else begin
						state <= 8;
					end
				end
				11: begin
					ren <= 1;

					raddr <= ADDR_OUT_DATA_1;
					
					state <= 12;
				end
				12: begin
					ren <= 1;

					raddr <= ADDR_OUT_DATA_2;
					
					state <= 13;
				end
				13: begin
					ren <= 1;

					raddr <= ADDR_OUT_DATA_3;
					
					state <= 14;
				end
				14: begin
					ren <= 1;

					raddr <= ADDR_OUT_DATA_4;
					
					state <= 15;
				end
				15: begin
					ren <= 1;

					raddr <= ADDR_OUT_DATA_5;
					
					state <= 16;
				end
				16: begin
					ren <= 1;

					raddr <= ADDR_OUT_DATA_6;
					
					state <= 0;
				end
				default: begin
				end
			endcase
		end
	end
endmodule
