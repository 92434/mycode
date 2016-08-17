`timescale 1ns/1ns

module i2s_receiver # (
		parameter integer FIFO_DATA_WIDTH = 32,
		parameter integer BULK_OF_DATA = 174 / 2,
		parameter integer ID = 0
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

	localparam integer BULK_DEPTH = 32;

	localparam integer PACKET_INDEX_WIDTH = 8;

	wire [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data;
	assign i2s_data = i2s_received_data[DATA_WIDTH * 3 - 1 -: I2S_DATA_VALID_BIT_WIDTH];

	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg_0 = 0;
	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg_1 = 0;
	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg_2 = 0;
	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg_3 = 0;
	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg_4 = 0;
	reg [I2S_DATA_VALID_BIT_WIDTH - 1 : 0] i2s_data_reg_5 = 0;

	wire matched;
	assign matched = ((i2s_data_reg_0 == 16'h2b03) && (i2s_data_reg_1 == 16'h2f84) && (i2s_data_reg_2 == 16'h4240) && (i2s_data_reg_3 == 16'ha1dd) && (i2s_data_reg_4 == 16'h0b77)) ? 1 : 0;

	always @(posedge bclk) begin
		if(rst_n == 0) begin
			i2s_data_reg_0 <= 0;
			i2s_data_reg_1 <= 0;
			i2s_data_reg_2 <= 0;
			i2s_data_reg_3 <= 0;
			i2s_data_reg_4 <= 0;
			i2s_data_reg_5 <= 0;
		end
		else begin
			if(s_data_valid == 1) begin
				i2s_data_reg_0 <= i2s_data;
				i2s_data_reg_1 <= i2s_data_reg_0;
				i2s_data_reg_2 <= i2s_data_reg_1;
				i2s_data_reg_3 <= i2s_data_reg_2;
				i2s_data_reg_4 <= i2s_data_reg_3;
				i2s_data_reg_5 <= i2s_data_reg_4;
			end
			else begin
			end
		end
	end

	reg w_enable = 0;
	reg [PACKET_INDEX_WIDTH - 1 : 0] cache_count = 0;
	reg [PACKET_INDEX_WIDTH - 1 : 0] cache_index = 0;
	reg [PACKET_INDEX_WIDTH - 1 : 0] cache_state = 0;
	always @(posedge bclk) begin
		if(rst_n == 0) begin
			w_enable <= 0;

			cache_count <= 0;

			cache_index <= 0;

			cache_state <= 0;
		end
		else begin 
			w_enable <= 0;
			cache_index <= 0;

			case(cache_state)
				0: begin
					if(matched == 1) begin
						cache_count <= 0;

						cache_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if((cache_count >= 0) && (cache_count < BULK_OF_DATA)) begin
						if(s_data_valid == 1) begin
							w_enable <= 1;
							cache_index <= cache_count;

							cache_count <= cache_count + 1;
						end
						else begin
						end
					end
					else begin
						cache_state <= 0;
					end
				end
				default: begin
					cache_state <= 0;
				end
			endcase
		end
	end

	localparam integer ID_WIDTH = 5;

	wire [ID_WIDTH - 1 : 0] id;
	assign id = ID[ID_WIDTH - 1 : 0];

	wire [FIFO_DATA_WIDTH - 1 : 0] wdata;
	assign wdata = {cache_index, {(FIFO_DATA_WIDTH - PACKET_INDEX_WIDTH - ID_WIDTH - I2S_DATA_VALID_BIT_WIDTH){1'b0}}, id, i2s_data_reg_5};

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
