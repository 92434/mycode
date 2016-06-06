`timescale 1ns / 1ps
`include "iic_def.v"

module iic_slave_interface #(
		parameter integer I2C_ADDRESS = 7'h3c
	)
	(
		input wire clk, 
		input wire rst_n, 

		output reg start_stop_det_enable = 1,
		input wire [1 : 0] start_stop_state,

		input wire scl_in,
		input wire sda_in,
		output reg sda_out_en = 0,
		output reg sda_out = 1,

		output reg fifo_wen = 0,
		output reg [7 : 0] fifo_wdata = 8'h00, 
		output reg fifo_wdata_start = 0,//

		output reg fifo_ren = 0,
		input wire [7 : 0] fifo_rdata 
	);

	// stream states
	`define STREAM_STATE_NULL 0
	`define STREAM_STATE_DATA_IN_I2C_ADDRESS 1
	`define STREAM_STATE_DATA_IN_DATA 2
	`define STREAM_STATE_DATA_OUT_DATA 3

	//iic slave state
	`define STATE_START_CONDITION 0
	`define STATE_IN_I2C_ADDRESS 1
	`define STATE_CHECK_I2C_ADDRESS 2
	`define STATE_IN_DATA 3
	`define STATE_CHECK_IN_DATA 4
	`define STATE_OUT_DATA 5
	`define STATE_CHECK_OUT_DATA 6
	`define STATE_DATA_LOOP 7
	`define STATE_ACK 8

	integer stream_state = `STREAM_STATE_NULL;

	reg [2 : 0] data_bit_count = 0;

	reg fifo_rdata_valid = 0;

	reg [7 : 0] fifo_rdata_reg = 0;

	reg wait_first_in_data = 0;

	integer state = `STATE_START_CONDITION;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			start_stop_det_enable <= 1;

			sda_out_en <= 0;
			sda_out <= 1;

			fifo_wen <= 0;
			fifo_wdata <= 8'h00;
			fifo_wdata_start <= 0;

			fifo_ren <= 0;

			stream_state <= `STREAM_STATE_NULL;

			data_bit_count <= 0;

			fifo_rdata_valid <= 0;

			fifo_rdata_reg <= 0;

			wait_first_in_data <= 0;

			state <= `STATE_START_CONDITION;
		end
		else begin
			fifo_wen <= 0;
			fifo_wdata_start <= 0;

			fifo_ren <= 0;
			
			if(start_stop_state == `START_DET || start_stop_state == `STOP_DET) begin
				start_stop_det_enable <= 0;

				state <= `STATE_START_CONDITION;
			end
			else begin
				case(state)
					`STATE_START_CONDITION: begin//repeat start condition
						if(scl_in == 1) begin
							data_bit_count <= 0;

							state <= `STATE_IN_I2C_ADDRESS;
						end
						else begin
						end
					end
					`STATE_IN_I2C_ADDRESS: begin
						if(scl_in == 0) begin
							start_stop_det_enable <= 1;

							sda_out_en <= 0;
							stream_state <= `STREAM_STATE_DATA_IN_I2C_ADDRESS;

							state <= `STATE_DATA_LOOP;
						end
						else begin
						end
					end
					`STATE_CHECK_I2C_ADDRESS: begin
						if(scl_in == 0) begin
							if(fifo_wdata[7 : 1] == I2C_ADDRESS[6 : 0]) begin
								if(fifo_wdata[0] == 1) begin
									stream_state <= `STREAM_STATE_DATA_OUT_DATA;
								end
								else begin
									wait_first_in_data <= 1;
									stream_state <= `STREAM_STATE_DATA_IN_DATA;
								end

								sda_out <= `I2C_ACK;
								sda_out_en <= 1;

								state <= `STATE_ACK;
							end
							else begin
							end
						end
						else begin
						end
					end
					`STATE_IN_DATA: begin
						if(scl_in == 0) begin
							sda_out_en <= 0;//reset ack

							state <= `STATE_DATA_LOOP;
						end
					end
					`STATE_CHECK_IN_DATA: begin
						if(scl_in == 0) begin
							sda_out <= `I2C_ACK;
							sda_out_en <= 1;

							state <= `STATE_ACK;
						end
						else begin
						end
					end
					`STATE_OUT_DATA: begin
						if(scl_in == 0) begin
							if(data_bit_count == 0) begin
								sda_out <= fifo_rdata[7];
								fifo_rdata_reg <= {fifo_rdata[6 : 0], 1'b0};
							end
							else begin
								sda_out <= fifo_rdata_reg[7];
								fifo_rdata_reg <= {fifo_rdata_reg[6 : 0], 1'b0};
							end

							sda_out_en <= 1;

							state <= `STATE_DATA_LOOP;
						end
					end
					`STATE_CHECK_OUT_DATA: begin
						if(scl_in == 0) begin
							sda_out_en <= 0;//prepare for reading ack/nack
							fifo_rdata_valid <= 0;//enable update fifo_rdata

							state <= `STATE_ACK;
						end
						else begin
						end
					end
					`STATE_DATA_LOOP: begin
						if(scl_in == 1) begin
							case(stream_state)
								`STREAM_STATE_DATA_IN_I2C_ADDRESS: begin
									fifo_wdata <= {fifo_wdata[6 : 0], sda_in};

									if(data_bit_count == 7) begin

										state <= `STATE_CHECK_I2C_ADDRESS;
									end
									else begin

										state <= `STATE_IN_I2C_ADDRESS;
									end
								end
								`STREAM_STATE_DATA_IN_DATA: begin
									fifo_wdata <= {fifo_wdata[6 : 0], sda_in};

									if(data_bit_count == 7) begin
										fifo_wen <= 1;
										if(wait_first_in_data == 1) begin
											wait_first_in_data <= 0;

											fifo_wdata_start <= 1;
										end
										else begin
										end

										state <= `STATE_CHECK_IN_DATA;
									end
									else begin

										state <= `STATE_IN_DATA;
									end
								end
								`STREAM_STATE_DATA_OUT_DATA: begin
									if(data_bit_count == 7) begin

										state <= `STATE_CHECK_OUT_DATA;
									end
									else begin

										state <= `STATE_OUT_DATA;
									end
								end
								default: begin
								end
							endcase
							data_bit_count <= data_bit_count + 1;
						end
						else begin
						end
					end
					`STATE_ACK: begin
						if(scl_in == 1) begin
							case(stream_state)
								`STREAM_STATE_DATA_IN_DATA: begin//write ack for in address
									state <= `STATE_IN_DATA;
								end
								`STREAM_STATE_DATA_OUT_DATA: begin
									if(sda_out_en == 1) begin//write ack for in address
										if(fifo_rdata_valid == 0) begin//update fifo_rdata
											fifo_ren <= 1;
											fifo_rdata_valid <= 1;
										end
										else begin
										end
										state <= `STATE_OUT_DATA;
									end
									else begin//read ack for out data
										if(sda_in == `I2C_ACK) begin
											if(fifo_rdata_valid == 0) begin
												fifo_ren <= 1;
												fifo_rdata_valid <= 1;
											end
											else begin
											end

											state <= `STATE_OUT_DATA;
										end
										else begin//I2C_NACK?wait for start/stop
											stream_state <= `STREAM_STATE_NULL;
										end
									end
								end
								default: begin
								end
							endcase
						end
						else begin
						end
					end
					default: begin
					end
				endcase
			end
		end
	end
endmodule
