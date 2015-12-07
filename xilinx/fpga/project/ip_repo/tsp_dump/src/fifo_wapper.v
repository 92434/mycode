`timescale 1 ns / 1 ps

module fifo_wapper #(
		parameter integer MPEG_DATA_WIDTH = 8,
		parameter integer C_M_AXIS_TDATA_WIDTH = 32
	)
	(
		input wire rst_n,
		input wire clk,

		input wire ts_clk,
		input wire ts_valid,
		input wire ts_sync,
		input wire [MPEG_DATA_WIDTH - 1 : 0] ts_data,

		output wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] rdata,
		input wire r_enable,

		output wire r_ready,
		output wire error_full,
		output wire error_empty
	);
	
	localparam integer FIFO_DATA_WIDTH = C_M_AXIS_TDATA_WIDTH;
	localparam integer BULK_OF_DATA = 16;
	localparam integer BULK_DEPTH = 512;

	localparam integer TS_DATA_NUM_PER_WFIFO = C_M_AXIS_TDATA_WIDTH / MPEG_DATA_WIDTH;


	integer buffer_index = 0;
	integer buffer_index_R1 = 0;
	reg [FIFO_DATA_WIDTH - 1 : 0] buffer_ts_data = 0;
	reg [MPEG_DATA_WIDTH - 1 : 0] ts_data_R1 = 0;
	reg buffer_valid = 0;

	wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] current_endpos;
	assign current_endpos = buffer_index * MPEG_DATA_WIDTH + MPEG_DATA_WIDTH - 1;

	//buffer mpeg data to 32 bit
	always @(posedge ts_clk) begin
		if(rst_n == 0) begin
			ts_data_R1 <= 0;

			buffer_ts_data <= 0;
			buffer_index <= 0;
			buffer_valid <= 0;
		end
		else begin
			buffer_valid <= 0;
			if(ts_valid == 1) begin
				ts_data_R1 <= ts_data;
				buffer_index_R1 <= buffer_index;
				buffer_ts_data[current_endpos -: MPEG_DATA_WIDTH] <= ts_data_R1;

				if((buffer_index >= 0) && (buffer_index < TS_DATA_NUM_PER_WFIFO - 1) && (ts_sync == 0)) begin
					buffer_index <= buffer_index + 1;
				end
				else begin
					buffer_index <= 0;
					buffer_valid <= 1;
				end
			end
			else begin
			end
		end
	end


	reg w_enable = 0;
	reg [FIFO_DATA_WIDTH - 1 : 0] wdata = 0;

	always @(posedge ts_clk) begin
		if(rst_n == 0) begin
			w_enable <= 0;
			wdata <= 0;
		end
		else begin
			w_enable <= 0;
			wdata <= 0;

			if(buffer_valid == 1) begin
				case(buffer_index_R1)
					0: begin
						wdata <= {{(C_M_AXIS_TDATA_WIDTH - 1 * MPEG_DATA_WIDTH){1'b0}}, buffer_ts_data[( 1 * MPEG_DATA_WIDTH - 1) : 0]};
					end
					1: begin
						wdata <= {{(C_M_AXIS_TDATA_WIDTH - 2 * MPEG_DATA_WIDTH){1'b0}}, buffer_ts_data[( 2 * MPEG_DATA_WIDTH - 1) : 0]};
					end
					2: begin
						wdata <= {{(C_M_AXIS_TDATA_WIDTH - 3 * MPEG_DATA_WIDTH){1'b0}}, buffer_ts_data[( 3 * MPEG_DATA_WIDTH - 1) : 0]};
					end
					3: begin
						wdata <= buffer_ts_data;
					end
					default: begin
					end
				endcase

				w_enable <= 1;
			end
			else begin
			end
		end
	end

	my_fifo # (
			.DATA_WIDTH(FIFO_DATA_WIDTH),
			.BULK_OF_DATA(BULK_OF_DATA),
			.BULK_DEPTH(BULK_DEPTH)
		) my_fifo_inst (
			.rst_n(rst_n),
			.wclk(ts_clk),
			.rclk(clk),
			.wdata(wdata),
			.rdata(rdata),
			.w_enable(w_enable),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);
endmodule
