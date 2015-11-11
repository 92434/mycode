`timescale 1 ns / 1 ps

module i2s_receiver_wapper #(
		parameter integer C_M_AXIS_TDATA_WIDTH = 32,
		parameter integer ID_WIDTH = 5,
		parameter integer I2S_RECEIVER_NUM = 32
	)
	(
		input wire rst_n,
		input wire clk,

		input wire [I2S_RECEIVER_NUM - 1 : 0] bclk,
		input wire [I2S_RECEIVER_NUM - 1 : 0] lrclk,
		input wire [I2S_RECEIVER_NUM - 1 : 0] sdata,
		//output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_din,

		output wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] rdata,
		input wire r_enable,

		output wire r_ready,
		output wire error_full,
		output wire error_empty,

		output wire [I2S_RECEIVER_NUM - 1 : 0] local_r_ready,
		output wire [I2S_RECEIVER_NUM - 1 : 0] local_error_full,
		output wire [I2S_RECEIVER_NUM - 1 : 0] local_error_empty
	);
	
	localparam integer I2S_DATA_BIT_WIDTH = 24;
	localparam integer FIFO_DATA_WIDTH = 16;

	reg [I2S_RECEIVER_NUM - 1 : 0] local_r_enable;

	wire [FIFO_DATA_WIDTH - 1 : 0] local_rdata [I2S_RECEIVER_NUM - 1:0];

	genvar i;
	generate for (i = 0; i < I2S_RECEIVER_NUM; i = i + 1)
		begin : i2s_instance

			localparam integer id = i;

			i2s_receiver #(
					.C_M_AXIS_TDATA_WIDTH(C_M_AXIS_TDATA_WIDTH),
					.I2S_DATA_BIT_WIDTH(I2S_DATA_BIT_WIDTH),
					.FIFO_DATA_WIDTH(FIFO_DATA_WIDTH),
					.ID(id),
					.ID_WIDTH(5)
				) receiver_inst (
					.clk(clk),
					.rst_n(rst_n),

					.bclk(bclk[i]),
					.lrclk(lrclk[i]),
					.sdata(sdata[i]),

					.r_enable(local_r_enable[i]),
					.rdata(local_rdata[i]),

					.r_ready(local_r_ready[i]),
					.error_full(local_error_full[i]),
					.error_empty(local_error_empty[i])
				);
		end
	endgenerate


	localparam integer BULK_OF_DATA = 15;
	localparam integer BULK_DEPTH = 128;

	localparam integer LOCAL_BULK_OF_DATA = 15;

	reg [FIFO_DATA_WIDTH - 1 : 0] wdata = 0;
	reg w_enable = 0;
	integer i2s_index = 0;
	integer cache_state = 0;
	integer local_rdata_count = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			cache_state <= 0;
			local_r_enable <= 0;
			i2s_index <= 0;
			local_rdata_count <= 0;
		end
		else begin
			case(cache_state)
				0: begin
					local_r_enable <= 0;

					if(local_r_ready[i2s_index] == 1) begin
						local_rdata_count <= 0;
						local_r_enable[i2s_index] <= 1;
						cache_state <= 1;
					end
					else begin
						if((i2s_index >= 0) && (i2s_index < I2S_RECEIVER_NUM - 1)) begin
							i2s_index <= i2s_index + 1;
						end
						else begin
							i2s_index <= 0;
						end
					end
				end
				1: begin
					if((local_rdata_count >= 0) && (local_rdata_count < LOCAL_BULK_OF_DATA)) begin
						w_enable <= 1;
						wdata <= local_rdata[i2s_index];

						if(local_rdata_count == LOCAL_BULK_OF_DATA - 1) begin
							local_r_enable <= 0;
						end
						else begin
						end

						local_rdata_count <= local_rdata_count + 1;
					end
					else begin
						w_enable <= 0;

						if((i2s_index >= 0) && (i2s_index < I2S_RECEIVER_NUM - 1)) begin
							i2s_index <= i2s_index + 1;
						end
						else begin
							i2s_index <= 0;
						end

						cache_state <= 0;

					end
				end
				default: begin
				end
			endcase
		end
	end

	wire [FIFO_DATA_WIDTH - 1 : 0] fifo_rdata;
	assign rdata = {{(C_M_AXIS_TDATA_WIDTH - FIFO_DATA_WIDTH){1'b0}}, fifo_rdata};

	my_fifo # (
			.DATA_WIDTH(FIFO_DATA_WIDTH),
			.BULK_OF_DATA(BULK_OF_DATA),
			.BULK_DEPTH(BULK_DEPTH)
		) receiver_fifo (
			.rst_n(rst_n),
			.wclk(clk),
			.rclk(clk),
			.wdata(wdata),
			.rdata(fifo_rdata),
			.w_enable(w_enable),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);
endmodule
