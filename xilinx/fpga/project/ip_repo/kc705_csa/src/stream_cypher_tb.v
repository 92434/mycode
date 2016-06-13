`timescale 1 ns / 1 ps

module stream_cypher_tb #
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

	reg [63 : 0] sb = 64'h0000000000000000;
	reg [63 : 0] ck = 64'h0000000000000000;

	wire [63 : 0] cb;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			sb <= 64'h0000000000000000;
			ck <= 64'h0000000000000000;
		end
		else begin
			sb <= 64'hE613DB6DC11C4524;
			ck <= ck + 1;
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

	stream_cypher #
		(
		)
		stream_cypher_inst (
			.clk(clk),
			.rst_n(rst_n),
			.sb(sb),
			.ck(ck),
			.cb(cb)
		);
endmodule
