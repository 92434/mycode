`timescale 1ns/1ns

module adi_pattern_gen # (
	)
	(
		input wire rst_n,
		input wire sys_clk,

		output reg oe = 0,
		output wire oe1,
		output reg [15 : 0] re = 0,
		output reg [15 : 0] im = 0
	);

	localparam integer N = 10;

	reg [16 * N - 1 : 0] re_ram = {
		16'h53c6,
		16'hf3c8,
		16'h7fff,
		16'hf3c8,
		16'h53c6,
		16'hac3a,
		16'h0c38,
		16'h8001,
		16'h0c38,
		16'hac3a
	};

	reg [16 * N - 1 : 0] im_ram = {
		16'h0321,
		16'h9a63,
		16'h3fff,
		16'he59f,
		16'h7cdd,
		16'hfcdf,
		16'h659d,
		16'hc001,
		16'h1a61,
		16'h8323
	};

	always @(posedge sys_clk) begin
		if(rst_n == 0) begin
			oe <= 0;
		end
		else begin
			oe <= ~oe;
		end
	end

	reg [16 - 1 : 0] sample_index = 0;
	always @(posedge oe) begin
		if(rst_n == 0) begin
			sample_index <= 0;
		end
		else begin
			re <= re_ram[sample_index * 16 +: 16];
			im <= im_ram[sample_index * 16 +: 16];

			if((sample_index >= 0) && (sample_index < (N - 1))) begin
				sample_index <= sample_index + 1;
			end
			else begin
				sample_index <= 0;
			end
		end
	end

	assign oe1 = oe;

endmodule
