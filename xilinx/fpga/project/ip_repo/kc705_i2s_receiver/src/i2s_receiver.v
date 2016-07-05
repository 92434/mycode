`timescale 1ns/1ns

module i2s_receiver # (
		parameter integer C_M_AXIS_TDATA_WIDTH = 32,
		parameter integer I2S_DATA_BIT_WIDTH = 24,
		parameter integer FIFO_DATA_WIDTH = 16,
		parameter integer ID = 0,
		parameter integer ID_WIDTH = 5
	) (
		input wire rst_n,
		input wire clk,

		input wire bclk,
		input wire lrclk,
		input wire sdata,
		//output wire i2s_din,

		input wire r_enable,
		output wire [FIFO_DATA_WIDTH - 1 : 0] rdata,

		output wire r_ready,
		output wire error_full,
		output wire error_empty
	);

	wire s_data_valid;
	wire [I2S_DATA_BIT_WIDTH : 0] i2s_received_data;
 	i2s_slave_interface # (
			.I2S_DATA_BIT_WIDTH(I2S_DATA_BIT_WIDTH)
		) i2s_slave_interface_inst (
			.rst_n(rst_n),
			.bclk(bclk),
			.lrclk(lrclk),
			.sdata(sdata),

			.i2s_received_data(i2s_received_data),
			.s_data_valid(s_data_valid)
		);

	localparam integer BULK_OF_DATA = 15;
	localparam integer BULK_DEPTH = 32;

	localparam integer DATA_WIDTH = 8;
	localparam integer I2S_DATA_VALID_BYTE_WIDTH = 2;
	localparam integer I2S_DATA_VALID_BIT_WIDTH = DATA_WIDTH * I2S_DATA_VALID_BYTE_WIDTH;

	localparam integer HEADER_BYTE_COUNT = 10;
	localparam integer DATA_BYTE_COUNT = 15;
	localparam integer PACKAGE_BYTE_COUNT = 174;

	wire [I2S_DATA_VALID_BIT_WIDTH - 1 : 0]data;
	assign data = i2s_received_data[DATA_WIDTH * 3 - 1 -: I2S_DATA_VALID_BIT_WIDTH];

	reg [DATA_WIDTH * DATA_BYTE_COUNT - 1 : 0] i2s_data = 0;
	always @(posedge bclk) begin
		if(rst_n == 0) begin
			i2s_data <= 0;
		end
		else begin
			if(s_data_valid == 1) begin
				i2s_data <= {i2s_data[DATA_WIDTH * DATA_BYTE_COUNT - I2S_DATA_VALID_BIT_WIDTH - 1 : 0], data};
			end
			else begin
			end
		end
	end
	
	wire [DATA_WIDTH * HEADER_BYTE_COUNT - 1 : 0]header;
	assign header = {8'h0B, 8'h77, 8'hA1, 8'hDD, 8'h42, 8'h40, 8'h2F, 8'h84, 8'h2B, 8'h03};

	reg [DATA_WIDTH * DATA_BYTE_COUNT - 1 : 0] i2s_data_prev = 0;

	integer match_index = 0;
	reg need_cache_i2s_data = 0;

	integer match_state = 0;
	always @(posedge bclk) begin
		if(rst_n == 0) begin
			i2s_data_prev <= 0;
			match_index <= 0;
			need_cache_i2s_data <= 0;

			match_state <= 0;
		end
		else begin
			need_cache_i2s_data <= 0;

			if(s_data_valid == 1) begin
				case(match_state)
					0: begin
						if(header == i2s_data[DATA_WIDTH * HEADER_BYTE_COUNT - 1 : 0]) begin
							match_index <= HEADER_BYTE_COUNT + I2S_DATA_VALID_BYTE_WIDTH;//set cached bytes count

							match_state <= 1;
						end
						else begin
						end
					end
					1: begin
						if((match_index >= HEADER_BYTE_COUNT + I2S_DATA_VALID_BYTE_WIDTH) && (match_index < PACKAGE_BYTE_COUNT)) begin
							match_index <= match_index + I2S_DATA_VALID_BYTE_WIDTH;
						end
						else begin// match_index >= PACKAGE_BYTE_COUNT
							if(i2s_data == i2s_data_prev) begin
							end
							else begin
								i2s_data_prev <= i2s_data;
								need_cache_i2s_data <= 1;
							end

							match_state <= 0;
						end
					end
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	reg w_enable = 0;
	integer cache_index = 0;
	reg [DATA_WIDTH - 1 : 0] fifo_data = 0;

	integer cache_state = 0;
	always @(posedge bclk) begin
		if(rst_n == 0) begin
			w_enable <= 0;
			cache_index <= 0;
			fifo_data <= 0;

			cache_state <= 0;
		end
		else begin 
			w_enable <= 0;

			case(cache_state)
				0: begin
					if(need_cache_i2s_data == 1) begin
						cache_index <= DATA_BYTE_COUNT;
						cache_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if((cache_index > 0) && (cache_index <= DATA_BYTE_COUNT)) begin
						w_enable <= 1;
						fifo_data <= i2s_data_prev[DATA_WIDTH * cache_index - 1 -: DATA_WIDTH];

						cache_index <= cache_index - 1;
					end
					else begin
						cache_state <= 0;
					end
				end
				default: begin
				end
			endcase
		end
	end

	wire [FIFO_DATA_WIDTH - 1 : 0] wdata;
	reg [ID_WIDTH - 1 : 0] id = ID;

	assign wdata = {{(FIFO_DATA_WIDTH - ID_WIDTH - DATA_WIDTH){1'b0}}, id, fifo_data};

	always @(posedge bclk) begin
		if(rst_n == 0) begin
			id <= ID;
		end
	end

	my_fifo # (
			.DATA_WIDTH(FIFO_DATA_WIDTH),
			.BULK_OF_DATA(BULK_OF_DATA),
			.BULK_DEPTH(BULK_DEPTH)
		) i2s_fifo (
			.rst_n(rst_n),
			.wclk(bclk),
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
