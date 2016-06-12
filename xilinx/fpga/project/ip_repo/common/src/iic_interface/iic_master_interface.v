`timescale 1ns / 1ps
`include "iic_def.v"

module iic_master_interface #
	(
	)
	(
		input wire clk,
		input wire rst_n,

		input wire scl_out,
		input wire scl_out_delay,

		input wire sda_in,
		output reg sda_out_en = 0,
		output reg sda_out = 1,

		input wire [7 : 0] slave_addr,

		output reg fifo_wen = 0,
		output reg [7 : 0] fifo_wdata = 0,

		output reg fifo_ren = 0,
		input wire [7 : 0] fifo_rdata,

		wire [7 : 0] count,

		input wire start,

		output reg stop_request = 0,
		output reg [8 : 0] status = `I2C_NO_ERR

	);
	
	// stream states
	`define STREAM_STATE_NULL 0
	`define STREAM_STATE_DATA_OUT_I2C_ADDRESS 1
	`define STREAM_STATE_DATA_IN_DATA 2
	`define STREAM_STATE_DATA_OUT_DATA 3

	//iic master state
	`define STATE_NULL 0
	`define STATE_START 1
	`define STATE_START_OUT 2
	`define STATE_STOP 3
	`define STATE_STOP_WAIT 4
	`define STATE_STOP_OUT 5
	`define STATE_OUT_I2C_ADDRESS 6
	`define STATE_CHECK_OUT_I2C_ADDRESS 7
	`define STATE_IN_DATA 8
	`define STATE_CHECK_IN_DATA 9
	`define STATE_OUT_DATA 10
	`define STATE_CHECK_OUT_DATA 11
	`define STATE_DATA_LOOP 12
	`define STATE_ACK 13

	integer stream_state = `STREAM_STATE_NULL;
	reg [2 : 0] data_bit_count = 0;
	reg [7 : 0] fifo_rdata_reg = 0;
	integer state = `STATE_NULL;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			sda_out_en <= 0;
			sda_out <= 1;

			fifo_wen <= 0;
			fifo_wdata <= 0;

			fifo_ren <= 0;

			stop_request <= 0;
			status <= `I2C_NO_ERR;

			stream_state <= `STREAM_STATE_NULL;

			data_bit_count <= 0;

			fifo_rdata_reg <= 0;

			state <= `STATE_NULL;
		end
		else begin
			fifo_wen <= 0;
			fifo_ren <= 0;

			stop_request <= 0;
			status <= `I2C_NO_ERR;

			case(state)
				`STATE_START: begin
					if(scl_out == 1) begin
						state <= `STATE_START_OUT;
					end
					else begin
					end
				end
				`STATE_START_OUT: begin
					if(scl_out_delay == 1) begin
						sda_out <= 0;

						data_bit_count <= 0;

						stream_state <= `STREAM_STATE_DATA_OUT_I2C_ADDRESS;

						state <= `STATE_OUT_I2C_ADDRESS;
					end
					else begin
					end
				end
				`STATE_STOP_WAIT: begin
					if(scl_out == 0) begin
						sda_out <= 0;
						sda_out_en = 1;

						state <= `STATE_STOP;
					end
					else begin
					end
				end
				`STATE_STOP: begin
					if(scl_out == 1) begin

						state <= `STATE_STOP_OUT;
					end
					else begin
					end
				end
				`STATE_STOP_OUT: begin
					if(scl_out_delay == 1) begin
						sda_out <= 1;

						state <= `STATE_NULL;
					end
					else begin
					end
				end
				`STATE_OUT_I2C_ADDRESS: begin
					if(scl_out == 0) begin
						if(data_bit_count == 0) begin
							sda_out <= slave_addr[7];
							fifo_rdata_reg <= {slave_addr[6 : 0], 1'b0};
						end
						else begin
							sda_out <= fifo_rdata_reg[7];
							fifo_rdata_reg <= {fifo_rdata_reg[6 : 0], 1'b0};
						end

						sda_out_en <= 1;

						state <= `STATE_DATA_LOOP;
					end
					else begin
					end
				end
				`STATE_CHECK_OUT_I2C_ADDRESS: begin
					if(scl_out == 0) begin
						sda_out_en <= 0;//prepare for reading ack/nack

						state <= `STATE_ACK;
					end
					else begin
					end
				end
				`STATE_IN_DATA: begin
					if(scl_out == 0) begin
						sda_out_en <= 0;//reset ack

						state <= `STATE_DATA_LOOP;
					end
					else begin
					end
				end
				`STATE_CHECK_IN_DATA: begin
					if(scl_out == 0) begin
						if(count == 0) begin
							sda_out <= `I2C_NAK;
						end
						else begin
							sda_out <= `I2C_ACK;
						end
						sda_out_en <= 1;

						state <= `STATE_ACK;
					end
					else begin
					end
				end
				`STATE_OUT_DATA: begin
					if(scl_out == 0) begin
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
					if(scl_out == 0) begin
						sda_out_en <= 0;//prepare for reading ack/nack

						state <= `STATE_ACK;
					end
					else begin
					end
				end
				`STATE_DATA_LOOP: begin
					if(scl_out == 1) begin
						case(stream_state)
							`STREAM_STATE_DATA_OUT_I2C_ADDRESS: begin
								if(data_bit_count == 7) begin

									state <= `STATE_CHECK_OUT_I2C_ADDRESS;
								end
								else begin

									state <= `STATE_OUT_I2C_ADDRESS;
								end
							end
							`STREAM_STATE_DATA_IN_DATA: begin
								fifo_wdata <= {fifo_wdata[6 : 0], sda_in};

								if(data_bit_count == 7) begin
									fifo_wen <= 1;

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
					if(scl_out == 1) begin
						case(stream_state)
							`STREAM_STATE_DATA_OUT_I2C_ADDRESS: begin
								if(sda_in == `I2C_ACK) begin
									if(slave_addr[0] == 1) begin

										stream_state <= `STREAM_STATE_DATA_IN_DATA;

										state <= `STATE_IN_DATA;
									end
									else begin
										fifo_ren <= 1;

										stream_state <= `STREAM_STATE_DATA_OUT_DATA;

										state <= `STATE_OUT_DATA;
									end
								end
								else begin//I2C_NACK?wait for start/stop
									stop_request <= 1;
									status <= `I2C_ERR_NO_ADDR_ACK;

									stream_state <= `STREAM_STATE_NULL;

									state <= `STATE_STOP_WAIT;
								end
							end
							`STREAM_STATE_DATA_IN_DATA: begin
								if(count == 0) begin
									stop_request <= 1;

									stream_state <= `STREAM_STATE_NULL;

									state <= `STATE_STOP_WAIT;
								end
								else begin

									state <= `STATE_IN_DATA;
								end
							end
							`STREAM_STATE_DATA_OUT_DATA: begin
								if(count == 0) begin
										stop_request <= 1;

										stream_state <= `STREAM_STATE_NULL;

										state <= `STATE_STOP_WAIT;
								end
								else begin
									if(sda_in == `I2C_ACK) begin
										fifo_ren <= 1;

										state <= `STATE_OUT_DATA;
									end
									else begin//I2C_NACK?wait for start/stop
										stop_request <= 1;
										status <= `I2C_ERR_NO_DATA_ACK;

										stream_state <= `STREAM_STATE_NULL;

										state <= `STATE_STOP_WAIT;
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

			if(scl_out == 0) begin//start stop must assert at negedge of scl_out
				if(start == 1)begin
					sda_out <= 1;
					sda_out_en = 1;

					state <= `STATE_START;
				end
				else begin
				end
			end
			else begin
			end
		end
	end

endmodule
