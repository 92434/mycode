`timescale 1 ns / 1 ps

module ts_s2p #(
	)
	(
		input wire mpeg_clk_in,
		input wire mpeg_valid_in,
		input wire mpeg_sync_in,
		input wire mpeg_data_in,

		output wire mpeg_clk_out,
		output wire mpeg_valid_out,
		output wire mpeg_sync_out,
		output wire [7 : 0] mpeg_data_out
	);

	reg ts_valid = 0;
	reg [7 : 0] ts_data = 0;
	reg [2 : 0] index = 0;
	reg reset_ts_data_valid = 0;
	reg ts_data_sync = 0;
	reg ts_data_sync_reg = 0;
	reg [7 : 0] ts_data_reg = 0;
	always @(negedge mpeg_clk_in) begin
		if(reset_ts_data_valid == 1) begin
			ts_data_sync_reg <= 0;
			ts_valid <= 0;
		end
		else begin
		end

		if(mpeg_valid_in == 1) begin
			if(mpeg_sync_in == 1) begin
				ts_data[0] <= mpeg_data_in;
				ts_data_sync <= 1;

				index <= 1;
			end
			else begin
				if(index == 0) begin
					ts_data[8] <= 0;
				end
				else begin
				end

				ts_data[index] <= mpeg_data_in;

				if(index == 3'b111) begin
					ts_valid <= 1;
					if(ts_data_sync == 1) begin
						ts_data_sync <= 0;
						ts_data_sync_reg <= 1;
					end
					else begin
					end
					ts_data_reg <= {mpeg_data_in, ts_data[6 : 0]};
				end
				else begin
				end

				index <= index + 1;
			end
		end
		else begin
		end
	end

	reg mpeg_clk = 0;
	reg [1 : 0] div_count = 0;
	always @(negedge mpeg_clk_in) begin
		if(div_count == 0) begin
			mpeg_clk <= ~mpeg_clk;
		end
		else begin
		end

		div_count <= div_count + 1;
	end

	assign mpeg_clk_out = mpeg_clk;

	localparam integer PACKAGE_BULK_DEPTH = 188 * 2;

	reg mpeg_valid = 0;
	reg mpeg_sync;
	reg [7 : 0] mpeg_data = 0;
	always @(posedge mpeg_clk_out) begin
		reset_ts_data_valid <= 0;
		mpeg_valid <= 0;
		mpeg_sync <= 0;

		if(ts_valid == 1) begin
			reset_ts_data_valid <= 1;

			mpeg_valid <= 1;
			if(ts_data_sync_reg == 1) begin
				mpeg_sync <= 1;
			end
			else begin
			end	
			mpeg_data <= ts_data_reg;
		end
		else begin
		end
	end

	assign mpeg_valid_out = mpeg_valid;
	assign mpeg_sync_out = mpeg_sync;
	assign mpeg_data_out = mpeg_data;

endmodule
