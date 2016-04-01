`timescale 1ns/1ns

module adi_pattern_gen # (
	)
	(
		input wire rst_n,
		input wire clk,

		output reg oe = 0,
		output wire oe1,
		output reg [15 : 0] re = 0,
		output reg [15 : 0] im = 0
	);

	reg [16 * 8 - 1 : 0] re_ram = {
		16'h7fff,
		16'h5a82,
		16'h0000,
		16'hda82,
		16'hffff,
		16'hda82,
		16'h0000,
		16'h5a82
	};

	reg [16 * 8 - 1 : 0] im_ram = {
		16'h0000,
		16'hda82,
		16'hffff,
		16'hda82,
		16'h0000,
		16'h5a82,
		16'h7fff,
		16'h5a82
	};

	always @(posedge clk) begin
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

			if((sample_index >= 0) && (sample_index < (8 - 1))) begin
				sample_index <= sample_index + 1;
			end
			else begin
				sample_index <= 0;
			end
		end
	end

	assign oe1 = oe;

endmodule
