module ts_s2p_tb #(
	)
	(
	);

	reg rst_n = 0;
	initial begin
		rst_n = 0;
		#20;
		rst_n = 1;
	end

	wire clk;
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));

	reg [7 : 0] data = 1;
	reg [2 : 0] index = 0;

	wire mpeg_clk_in;
	wire mpeg_sync_in;
	reg mpeg_valid_in = 0;
	wire mpeg_data_in;

	assign mpeg_clk_in = clk;


	localparam integer DIV_COUNT = 3;
	reg [7 : 0] div = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			div <= 0;
		end
		else begin
			mpeg_valid_in <= 0;

			if(div == DIV_COUNT) begin
				mpeg_valid_in <= 1;

				div <= 0;
			end
			else begin
				div <= div + 1;
			end

		end
	end

	assign mpeg_sync_in = ((data == 1) && (mpeg_valid_in == 1) && (index == 0)) ? 1 : 0;
	assign mpeg_data_in = data[index];

	always @(posedge clk) begin
		if(rst_n == 0) begin
			index <= 0;
		end
		else begin
			if(mpeg_valid_in == 1) begin
				if(index == 3'b111) begin
					if(data == 188) begin
						data <= 1;
					end
					else begin
						data <= data + 1;
					end
				end
				else begin
				end

				index <= index + 1;
			end
			else begin
			end
		end
	end

	wire mpeg_clk_out;
	wire mpeg_valid_out;
	wire mpeg_sync_out;
	wire [7 : 0] mpeg_data_out;

	ts_s2p ts_sp2_inst(
		.mpeg_clk_in(mpeg_clk_in),
		.mpeg_valid_in(mpeg_valid_in),
		.mpeg_sync_in(mpeg_sync_in),
		.mpeg_data_in(mpeg_data_in),
		.mpeg_clk_out(mpeg_clk_out),
		.mpeg_valid_out(mpeg_valid_out),
		.mpeg_sync_out(mpeg_sync_out),
		.mpeg_data_out(mpeg_data_out)
		);

endmodule
