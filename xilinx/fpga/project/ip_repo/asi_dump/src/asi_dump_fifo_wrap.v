`timescale 1 ns / 1 ps

module asi_dump_fifo_wrap #(
		parameter integer MPEG_DATA_WIDTH = 10,
		parameter integer C_M_AXIS_TDATA_WIDTH = 32
	)
	(
		input wire rst_n,
		input wire clk,

		input wire ce,
		input wire [MPEG_DATA_WIDTH - 1 : 0] ts_data,

		output wire wclk,
		output reg wen = 0,
		output reg [C_M_AXIS_TDATA_WIDTH - 1 : 0] wdata = 0
	);
	
	localparam integer TS_DATA_NUM_PER_WFIFO = C_M_AXIS_TDATA_WIDTH / MPEG_DATA_WIDTH;


	integer buffer_index = 0;
	integer buffer_index_R = 0;
	reg [C_M_AXIS_TDATA_WIDTH - 1 : 0] buffer_ts_data = 0;
	reg buffer_valid = 0;

	wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] current_endpos;
	assign current_endpos = buffer_index * MPEG_DATA_WIDTH + MPEG_DATA_WIDTH - 1;

	//buffer mpeg data to 32 bit
	
	wire [MPEG_DATA_WIDTH - 1 : 0] k28p5_p;
	wire [MPEG_DATA_WIDTH - 1 : 0] k28p5_n;

	assign k28p5_p = {10'b0101111100};
	assign k28p5_n = {10'b1010000011};

	always @(posedge clk) begin
		if(rst_n == 0) begin
			buffer_index_R <= 0;
			buffer_ts_data <= 0;
			buffer_index <= 0;
			buffer_valid <= 0;
		end
		else begin
			buffer_valid <= 0;
			if(ce == 1) begin
				if(ts_data == k28p5_p) begin
				end
				else if(ts_data == k28p5_n) begin
				end
				else begin
					buffer_index_R <= buffer_index;
					buffer_ts_data[current_endpos -: MPEG_DATA_WIDTH] <= ts_data;

					if((buffer_index >= 0) && (buffer_index < TS_DATA_NUM_PER_WFIFO - 1)) begin
						buffer_index <= buffer_index + 1;
					end
					else begin
						buffer_index <= 0;
						buffer_valid <= 1;
					end
				end
			end
			else begin
			end
		end
	end


	always @(posedge clk) begin
		if(rst_n == 0) begin
			wen <= 0;
			wdata <= 0;
		end
		else begin
			wen <= 0;
			wdata <= 0;

			if(buffer_valid == 1) begin
				case(buffer_index_R)
					0: begin
						wdata <= {{(C_M_AXIS_TDATA_WIDTH - 1 * MPEG_DATA_WIDTH){1'b0}}, buffer_ts_data[( 1 * MPEG_DATA_WIDTH - 1) : 0]};
					end
					1: begin
						wdata <= {{(C_M_AXIS_TDATA_WIDTH - 2 * MPEG_DATA_WIDTH){1'b0}}, buffer_ts_data[( 2 * MPEG_DATA_WIDTH - 1) : 0]};
					end
					2: begin
						wdata <= {{(C_M_AXIS_TDATA_WIDTH - 3 * MPEG_DATA_WIDTH){1'b0}}, buffer_ts_data[( 3 * MPEG_DATA_WIDTH - 1) : 0]};
					end
					default: begin
					end
				endcase

				wen <= 1;
			end
			else begin
			end
		end
	end

	assign wclk = clk;
endmodule
