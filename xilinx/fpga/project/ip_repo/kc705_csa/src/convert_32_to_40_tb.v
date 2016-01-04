`timescale 1 ns / 1 ps

module convert_32_to_40_tb #()
	();
	
	wire clk;
	reg rst_n = 0;
	
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));

	initial begin
		rst_n = 0;
		#2
		rst_n = 1;
	end

	reg [32 - 1 : 0] i = 32'h12345678;
	integer count = 0;
	reg wen_32 = 0;
	reg [32 - 1 : 0] wdata_32 = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			i <= 32'h12345678;
			count <= 0;
			wen_32 <= 0;
			wdata_32 <= 0;
		end
		else begin
			wen_32 <= 0;

			if(error_full == 0) begin
				if(count == 5) begin
					count <= 0;

					wdata_32 <= i;
					wen_32 <= 1;

					i <= i + 1;
				end
				else begin
					count <= count + 1;
				end
			end
		end
	end

	wire ren_32;
	wire [32 - 1 : 0] rdata_32;
	wire r_ready;
	wire error_full;
	wire error_empty;

	wire wclk_32;
	wire rclk_32;

	assign wclk_32 = clk;

	my_fifo #(
			.DATA_WIDTH(32),
			.BULK_OF_DATA(5),
			.BULK_DEPTH(256)
		) my_fifo_inst (
			.rst_n(rst_n),
			.wclk(wclk_32),
			.rclk(rclk_32),
			.wdata(wdata_32),
			.rdata(rdata_32),
			.w_enable(wen_32),
			.r_enable(ren_32),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	wire csa_in_rclk;
	reg csa_in_ren;
	wire [40 - 1 : 0] csa_in_rdata;
	wire csa_in_r_ready;
	wire csa_in_error_full;
	wire csa_in_error_empty;

	assign csa_in_rclk = clk;

	convert_32_to_40 convert_32_to_40_inst(
			.clk(clk),
			.rst_n(rst_n),

			.axis_s_r_ready(r_ready),

			.axis_s_rclk(rclk_32),
			.axis_s_ren(ren_32),
			.axis_s_rdata(rdata_32),

			.csa_in_rclk(csa_in_rclk),
			.csa_in_ren(csa_in_ren),
			.csa_in_rdata(csa_in_rdata),

			.csa_in_r_ready(csa_in_r_ready),
			.csa_in_error_full(csa_in_error_full),
			.csa_in_error_empty(csa_in_error_empty)
		);

	integer state = 0;
	integer delay = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_in_ren <= 0;
			state <= 0;
			delay <= 0;
		end
		else begin
			csa_in_ren <= 0;
			case(state)
				0: begin
					if(delay == 100) begin
						if(csa_in_r_ready) begin
							csa_in_ren <= 1;
						end
						else begin
						end

						delay <= 0;
					end
					else begin
						delay <= delay + 1;
					end

					state <= 0;
				end
				default: begin
				end
			endcase
		end
	end
endmodule
