`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/09/01 13:55:42
// Design Name: 
// Module Name: testbench
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module testbench #(
		parameter integer DATA_WIDTH = 8,
		parameter integer FIFO_DATA_WIDTH = 9
	)
	(
	);

	reg rst_n;
	wire clk;
	wire din_clk;
	reg valid;
	wire [DATA_WIDTH - 1 : 0] din_8b;
	wire ts_clk;
	wire ts_valid;
	wire [DATA_WIDTH - 1 : 0] ts_data;
	wire asi_out_p;
	wire asi_out_n;
	wire [FIFO_DATA_WIDTH - 1 : 0] din;
	wire [FIFO_DATA_WIDTH - 1 : 0] rdata;
	wire [FIFO_DATA_WIDTH - 1 : 0] dout;
	wire ce_R;
	wire ce;
	wire [4 : 0] ce_sr;
	wire r_ready;
	wire error_full;
	wire error_empty;
	wire start;
	wire [9:0] sout_data;
	wire [9:0] sout_data_1;
	wire [7:0] din_8b_R_debug;

	reg [2 : 0] i = 0;

	initial begin
		rst_n = 0;
		#9
		rst_n = 1;
	end

	assign sout_data_1 = {
			sout_data[0],
			sout_data[1],
			sout_data[2],
			sout_data[3],
			sout_data[4],
			sout_data[5],
			sout_data[6],
			sout_data[7],
			sout_data[8],
			sout_data[9]
		};

	// Add user logic here
	clkgen #(.clk_period(6)) xiaofeiclk_wr(.clk(din_clk));
	clkgen #(.clk_period(1)) xiaofeiclk_rd(.clk(clk));

	data_gen_counter #(
			.DATA_WIDTH(DATA_WIDTH)
		) data_gen(
			.rst_n(rst_n),
			.clk(din_clk),
			.data_source(din_8b)
		);
	
	always @(posedge din_clk) begin
		if(rst_n == 0) begin
			i <= 0;
			valid <= 0;
		end
		else begin
			if(i == 3) begin
				valid <= 0;
			end
			else begin
				valid <= 1;
			end
			i <= i + 1;
		end
	end
	
	wire ts2_clk;
	wire ts2_valid;
	wire ts2_sync;
	wire [7:0] ts2_data;

	ts2asi #(
			.FIFO_DATA_WIDTH(9)
		) ts2asi_inst (
			.ts2_clk(ts2_clk),
			.ts2_valid(ts2_valid),
			.ts2_sync(ts2_sync),
			.ts2_data(ts2_data),

			.rst_n(rst_n),
			.clk(clk),

			.din_clk(din_clk),
			.valid(valid),
			.din_8b(din_8b),

			.asi_out_p(asi_out_p),
			.asi_out_n(asi_out_p),

			.din(din),
			.rdata(rdata),
			.dout(dout),

			.ts_clk(ts_clk),
			.ts_valid(ts_valid),
			.ts_data(ts_data),

			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty),

			.ce_R(ce_R),
			.ce(ce),
			.ce_sr(ce_sr),
			.start(start),
			.sout_data(sout_data),
			.din_8b_R_debug(din_8b_R_debug)
		);
	// User logic ends
endmodule
