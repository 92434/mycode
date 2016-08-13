`timescale 1ns/1ns

module i2s_receiver # (
		parameter integer FIFO_DATA_WIDTH = 32,
		parameter integer BULK_OF_DATA = 174 / 2,
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

	localparam integer I2S_DATA_BIT_WIDTH = 24;

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

	localparam integer DATA_WIDTH = 8;
	localparam integer I2S_DATA_VALID_BYTE_WIDTH = 2;
	localparam integer I2S_DATA_VALID_BIT_WIDTH = DATA_WIDTH * I2S_DATA_VALID_BYTE_WIDTH;

	localparam integer HEADER_BYTE_COUNT = 10;

	localparam integer BULK_DEPTH = 32;

	wire [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data;
	assign i2s_data = i2s_received_data[DATA_WIDTH * 3 - 1 -: I2S_DATA_VALID_BIT_WIDTH];

	reg [DATA_WIDTH * HEADER_BYTE_COUNT - 1 : 0] i2s_data_cache = 0;
	always @(posedge bclk) begin
		if(rst_n == 0) begin
			i2s_data_cache <= 0;
		end
		else begin
			if(s_data_valid == 1) begin
				i2s_data_cache <= {i2s_data_cache[DATA_WIDTH * HEADER_BYTE_COUNT - I2S_DATA_VALID_BIT_WIDTH - 1 : 0], i2s_data};
			end
			else begin
			end
		end
	end

	reg s_data_valid_reg = 0;
	reg s_data_valid_reg_1 = 0;
	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg = 0;
	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg_1 = 0;
	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg_2 = 0;
	always @(posedge bclk) begin
		if(rst_n == 0) begin
			s_data_valid_reg <= 0;
			s_data_valid_reg_1 <= 0;

			i2s_data_reg <= 0;
			i2s_data_reg_1 <= 0;
			i2s_data_reg_2 <= 0;
		end
		else begin
			s_data_valid_reg <= s_data_valid;
			s_data_valid_reg_1 <= s_data_valid_reg;

			i2s_data_reg <= i2s_data_cache[DATA_WIDTH * HEADER_BYTE_COUNT - 1 -: DATA_WIDTH * 2];//s_data_valid
			i2s_data_reg_1 <= i2s_data_reg;//s_data_valid_reg
			i2s_data_reg_2 <= i2s_data_reg_1;//s_data_valid_reg_1
		end
	end
	
	wire [DATA_WIDTH * HEADER_BYTE_COUNT - 1 : 0]header;
	assign header = {8'h0B, 8'h77, 8'hA1, 8'hDD, 8'h42, 8'h40, 8'h2F, 8'h84, 8'h2B, 8'h03};

	reg need_cache_i2s_data = 0;
	always @(posedge bclk) begin
		if(rst_n == 0) begin
			need_cache_i2s_data <= 0;
		end
		else begin
			need_cache_i2s_data <= 0;
			if(s_data_valid == 1) begin
				if(header[DATA_WIDTH * HEADER_BYTE_COUNT - 1 -: DATA_WIDTH * 2] == i2s_data_cache[DATA_WIDTH * HEADER_BYTE_COUNT - 1 -: DATA_WIDTH * 2]) begin
					need_cache_i2s_data <= 1;
				end
				else begin
				end
			end
			else begin
			end
		end
	end


	reg w_enable = 0;
	integer cache_index = 0;
	integer cache_state = 0;
	always @(posedge bclk) begin
		if(rst_n == 0) begin
			w_enable <= 0;
			cache_index <= 0;

			cache_state <= 0;
		end
		else begin 
			w_enable <= 0;

			case(cache_state)
				0: begin
					if(need_cache_i2s_data == 1) begin
						cache_index <= 0;

						cache_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if((cache_index >= 0) && (cache_index < BULK_OF_DATA)) begin
						if(s_data_valid_reg_1 == 1) begin
							w_enable <= 1;

							cache_index <= cache_index + 1;
						end
						else begin
						end
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

	wire [ID_WIDTH - 1 : 0] id;
	assign id = ID[ID_WIDTH - 1 : 0];

	wire [FIFO_DATA_WIDTH - 1 : 0] wdata;
	assign wdata = {{(FIFO_DATA_WIDTH - ID_WIDTH - DATA_WIDTH){1'b0}}, id, i2s_data_reg_2};

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
