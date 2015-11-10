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
	wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] i2s_received_data;
 	receive_data_from_i2s # (
			.I2S_DATA_BIT_WIDTH(I2S_DATA_BIT_WIDTH)
		) receiver (
			.rst_n(rst_n),
			.bclk(bclk),
			.lrclk(lrclk),
			.sdata(sdata),

			.i2s_received_data(i2s_received_data),
			.s_data_valid(s_data_valid)
		);

	localparam integer BULK_OF_DATA = 15;
	localparam integer BULK_DEPTH = 8;

	localparam integer DATA_WIDTH = 8;
	localparam integer I2S_DATA_VALID_BYTE_WIDTH = 2;
	localparam integer I2S_DATA_VALID_BIT_WIDTH = DATA_WIDTH * I2S_DATA_VALID_BYTE_WIDTH;

	localparam integer HEADER_BYTE_COUNT = 10;
	localparam integer DATA_BYTE_COUNT = 15;
	//localparam integer PACKAGE_BYTE_COUNT = 174;
	localparam integer PACKAGE_BYTE_COUNT = 42;

	reg [DATA_WIDTH * DATA_BYTE_COUNT - 1 : 0] i2s_data;
	reg [DATA_WIDTH * DATA_BYTE_COUNT - 1 : 0] i2s_data_prev;
	wire [DATA_WIDTH * HEADER_BYTE_COUNT - 1 : 0]header;
	wire [I2S_DATA_VALID_BIT_WIDTH - 1 : 0]data;

	assign header = {8'h0B, 8'h77, 8'hA1, 8'hDD, 8'h42, 8'h40, 8'h2F, 8'h84, 8'h2B, 8'h03};
	assign data = i2s_received_data[DATA_WIDTH * 3 - 1 -: I2S_DATA_VALID_BIT_WIDTH];

	integer match_state = 0;
	integer i = 0;
	reg need_cache_i2s_data = 0;
	reg i2s_data_cached = 0;
	always @(posedge s_data_valid) begin
		if(rst_n == 0) begin
			match_state <= 0;
			need_cache_i2s_data <= 0;
			i <= 0;
		end
		else begin
			case(match_state)
				0: begin
					if(i2s_data_cached == 1) begin
						need_cache_i2s_data <= 0;
					end
					else begin
					end

					if(header == i2s_data[DATA_WIDTH * HEADER_BYTE_COUNT - 1 : 0]) begin
						i <= HEADER_BYTE_COUNT + I2S_DATA_VALID_BIT_WIDTH;
						match_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if((i >= HEADER_BYTE_COUNT + I2S_DATA_VALID_BIT_WIDTH) && (i < PACKAGE_BYTE_COUNT)) begin
						i <= i + I2S_DATA_VALID_BIT_WIDTH;
					end
					else begin// hit 174 bytes
						if(i2s_data == i2s_data_prev) begin
						end
						else begin
							i2s_data_prev <= i2s_data;
						end

						need_cache_i2s_data <= 1;
						match_state <= 0;
					end
				end
				default: begin
					match_state <= 0;
				end
			endcase

			i2s_data <= {i2s_data[DATA_WIDTH * DATA_BYTE_COUNT - I2S_DATA_VALID_BIT_WIDTH - 1 : 0], data};
		end
	end

	integer cache_state = 0;
	integer data_index = 0;
	reg w_enable = 0;
	reg [DATA_WIDTH - 1 : 0] fifo_data = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			cache_state <= 0;
			data_index <= 0;
			w_enable <= 0;
			i2s_data_cached <= 0;
		end
		else begin 
			case(cache_state)
				0: begin
					w_enable <= 0;

					if(need_cache_i2s_data == 1) begin
						data_index <= 15;
						cache_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if((data_index > 0) && (data_index <= 15)) begin
						w_enable <= 1;
						fifo_data <= i2s_data_prev[DATA_WIDTH * data_index - 1 -: DATA_WIDTH];
						data_index <= data_index - 1;
					end
					else begin
						w_enable <= 0;
						i2s_data_cached <= 1;

						if(need_cache_i2s_data == 0) begin//wait for reset need_cache_i2s_data
							i2s_data_cached <= 0;
							cache_state <= 0;
						end
					end
				end
				default: begin
					cache_state <= 0;
				end
			endcase
		end
	end

	wire [FIFO_DATA_WIDTH - 1 : 0] wdata;
	reg [ID_WIDTH - 1 : 0] id = ID;

	assign wdata = {{(FIFO_DATA_WIDTH - ID_WIDTH - DATA_WIDTH){1'b0}}, id, fifo_data};

	always @(posedge clk) begin
		if(rst_n == 0) begin
			id <= ID;
		end
	end

	my_fifo # (
			.DATA_WIDTH(FIFO_DATA_WIDTH),
			.BULK_OF_DATA(BULK_OF_DATA),
			.BULK_DEPTH(BULK_DEPTH)
		) xiaofei_fifo (
			.rst_n(rst_n),
			.wclk(clk),
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
