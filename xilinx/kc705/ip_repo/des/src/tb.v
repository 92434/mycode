`timescale 1 ns / 1 ps

module tb #
	(
	)
	(
	);

	wire clk;
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));

	reg rst_n = 1;
	initial begin
		#2;
		rst_n = 0;
		#10;
		rst_n = 1;
	end

	reg [63 : 0] data_bus = 64'h8000000000000000;
	reg [63 : 0] key = 64'h0000000000000000;
	reg decrypt = 0;

	wire [63 : 0] data_out;

	reg [1 : 0] state = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			data_bus <= 64'h8000000000000000;
			key <= 64'h0000000000000000;
			decrypt <= 0;

			state <= 0;
		end
		else begin
			case(state)
				0: begin
					data_bus <= 64'h8000000000000000;
					key <= 64'h0000000000000000;
					decrypt <= 0;
				end
				1: begin
					data_bus <= 64'h95f8a5e5dd31d900;
					key <= 64'h0000000000000001;
					decrypt <= 0;
				end
				2: begin
					data_bus <= 64'h8000000000000001;
					key <= 64'h0000000000000002;
					decrypt <= 0;
				end
				3: begin
					data_bus <= 64'h95f8a5e5dd31d901;
					key <= 64'h0000000000000003;
					decrypt <= 0;
				end
				default: begin
				end
			endcase

			state <= state + 1;
		end
	end

	reg [8 : 0] count = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			count <= 0;
		end
		else begin
			count <= count + 1;
		end
	end

	des #
		(
		)
		des0 (
			.reset(~rst_n),
			.clk(clk),
			.data_bus(data_bus),
			.key(key),
			.decrypt(decrypt),
			.data_out(data_out)
		);
endmodule
