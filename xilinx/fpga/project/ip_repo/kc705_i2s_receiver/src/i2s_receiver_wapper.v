`timescale 1 ns / 1 ps

module i2s_receiver_wapper #(
		parameter integer I2S_RECEIVER_NUM = 1,
		parameter integer C_M_AXIS_TDATA_WIDTH = 32,
		parameter integer NUMBER_OF_OUTPUT_WORDS = 8,
		parameter integer I2S_DATA_BIT_WIDTH = 24,
		parameter integer ID_WIDTH = 5
	)
	(
		input wire rst,
		input wire clk,

		input wire [I2S_RECEIVER_NUM - 1 : 0] bclk,
		input wire [I2S_RECEIVER_NUM - 1 : 0] lrclk,
		input wire [I2S_RECEIVER_NUM - 1 : 0] sdata,
		//output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_din,
		input wire read_enable,
		input wire [I2S_RECEIVER_NUM - 1 : 0] chip_select,
		output wire [I2S_RECEIVER_NUM - 1 : 0] output_ready,
		output wire [I2S_RECEIVER_NUM - 1 : 0] buffer_full_error,
		output wire [I2S_RECEIVER_NUM - 1 : 0] buffer_empty_error,

		output wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] rdata,

		output wire s_data_valid,
		output wire [I2S_DATA_BIT_WIDTH:0] i2s_received_data,
		output wire [I2S_RECEIVER_NUM - 1:0] local_read_enable
	);
	
	wire [C_M_AXIS_TDATA_WIDTH - 1 : 0] i2s_rdata [I2S_RECEIVER_NUM - 1:0];
	reg [C_M_AXIS_TDATA_WIDTH - 1 : 0] local_rdata = 0;
	wire [I2S_RECEIVER_NUM - 1:0] local_s_data_valid;
	wire [I2S_DATA_BIT_WIDTH:0] local_i2s_received_data [I2S_RECEIVER_NUM - 1:0];

	reg [7 : 0] index;

	assign s_data_valid = local_s_data_valid[0];
	assign i2s_received_data = local_i2s_received_data[0];
	assign rdata = local_rdata;

	always @(posedge clk) begin
		if(rst == 0) begin
			index <= 0;
		end
		else begin
			for(index = 0;index < I2S_RECEIVER_NUM; index=index+1) begin
				if(read_enable == 1 && chip_select[index] == 1) begin
					local_rdata <= i2s_rdata[index];
				end
			end
		end
	end


	genvar i;
	generate for (i=0; i<I2S_RECEIVER_NUM; i=i+1)
		begin : i2s_instance
			assign local_read_enable[i] = ((read_enable == 1) && (chip_select[i] == 1)) ? 1 : 0;
			i2s_receiver #(
					.C_M_AXIS_TDATA_WIDTH(C_M_AXIS_TDATA_WIDTH),
					.NUMBER_OF_OUTPUT_WORDS(NUMBER_OF_OUTPUT_WORDS),
					.I2S_DATA_BIT_WIDTH(I2S_DATA_BIT_WIDTH),
					.ID(i),
					.ID_WIDTH(5)
				) receiver_inst (
					.rst(rst),
					.bclk(bclk[i]),
					.lrclk(lrclk[i]),
					.sdata(sdata[i]),
					.s_data_valid(local_s_data_valid[i]),
					.i2s_received_data(local_i2s_received_data[i]),
					.read_enable(local_read_enable[i]),
					.clk(clk),
					.rdata(i2s_rdata[i]),
					.output_ready(output_ready[i]),
					.buffer_full_error(buffer_full_error[i]),
					.buffer_empty_error(buffer_empty_error[i])
				);
		end
	endgenerate
endmodule
