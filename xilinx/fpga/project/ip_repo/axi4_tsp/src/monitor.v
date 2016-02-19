`timescale 1ns / 1ps

module monitor #(
		parameter integer C_S_AXI_DATA_WIDTH = 32
	)
	(
		input wire rst_n,
		input wire clk,

		input wire match_enable,

		input wire update_pid_request,
		input wire [C_S_AXI_DATA_WIDTH-1:0] pid_index,
		input wire [C_S_AXI_DATA_WIDTH-1:0] pid,

		output wire [C_S_AXI_DATA_WIDTH-1:0] out_pid,

		input wire pump_data_request,

		output reg pump_data_request_ready = 0,
		output reg [C_S_AXI_DATA_WIDTH-1:0] out_data = 0,
		output reg [C_S_AXI_DATA_WIDTH-1:0] out_data_index = 0,

		input wire [7:0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync
	);

	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	localparam integer PID_PID_WIDTH = 13;
	localparam integer PID_PAD0_WIDTH = 3;
	localparam integer PID_MATCH_ENABLE_WIDTH = 1;
	localparam integer PID_PAD1_WIDTH = 15;

	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_data_0 [0 : PACK_WORD_SIZE - 1];
	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_data_1 [0 : PACK_WORD_SIZE - 1];
	reg [C_S_AXI_DATA_WIDTH-1:0] caching_ram_index = 0;
	wire [C_S_AXI_DATA_WIDTH-1:0] cached_ram_index;

	reg [PID_PID_WIDTH - 1 : 0] ram_for_pid = 0;
	reg [PID_MATCH_ENABLE_WIDTH - 1 : 0] pid_match_enable = 0;

	wire [PID_PID_WIDTH - 1 : 0] cur_pid_pid;
	wire [PID_MATCH_ENABLE_WIDTH - 1 : 0] cur_pid_match_enable;

	assign cur_pid_pid = pid[PID_PID_WIDTH - 1 : 0];
	assign cur_pid_match_enable = pid[PID_PID_WIDTH + PID_PAD0_WIDTH + PID_MATCH_ENABLE_WIDTH - 1 : PID_PID_WIDTH + PID_PAD0_WIDTH];
	assign out_pid = {{(PID_PAD1_WIDTH){1'b0}}, pid_match_enable, {(PID_PAD0_WIDTH){1'b0}}, ram_for_pid};

	always @(posedge clk) begin
		if(rst_n == 0) begin
			ram_for_pid <= 0;
			pid_match_enable <= 0;
		end
		else begin
			if(update_pid_request == 1) begin
				if(pid_index == 0) begin
					ram_for_pid <= cur_pid_pid;
					pid_match_enable <= cur_pid_match_enable;
				end
				else begin
				end
			end
			else begin
			end
		end
	end

	assign cached_ram_index = (caching_ram_index == 0) ? 1 : 0;

	integer pump_data_state = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] pump_data_index = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			pump_data_request_ready <= 0;
			out_data_index <= 0;
			out_data <= 0;

			pump_data_state <= 0;
			pump_data_index <= 0;
		end
		else begin
			case(pump_data_state)
				0: begin
					if(pump_data_request == 1) begin
						pump_data_request_ready <= 0;
						pump_data_index <= 0;

						pump_data_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if((pump_data_index >= 0) && (pump_data_index < PACK_WORD_SIZE)) begin
						out_data_index <= pump_data_index;

						case(cached_ram_index)
							0: begin
								out_data <= ram_for_data_0[pump_data_index];
							end
							1: begin
								out_data <= ram_for_data_1[pump_data_index];
							end
							default: begin
							end
						endcase

						pump_data_index <= pump_data_index + 1;
					end
					else begin
						pump_data_request_ready <= 1;

						pump_data_state <= 0;
					end
				end
				default: begin
				end
			endcase
		end
	end

	reg mpeg_sync_d1 = 0;
	reg mpeg_sync_d2 = 0;
	reg mpeg_sync_d3 = 0;
	reg [7:0] mpeg_data_d1 = 0;
	reg [7:0] mpeg_data_d2 = 0;
	reg [7:0] mpeg_data_d3 = 0;

	always @(posedge mpeg_clk) begin
		if(rst_n == 0) begin
			mpeg_sync_d1 <= 0;
			mpeg_sync_d2 <= 0;
			mpeg_sync_d3 <= 0;
			mpeg_data_d1 <= 0;
			mpeg_data_d2 <= 0;
			mpeg_data_d3 <= 0;
		end
		else begin
			if(mpeg_valid == 1) begin
				mpeg_sync_d1 <= mpeg_sync;
				mpeg_sync_d2 <= mpeg_sync_d1;
				mpeg_sync_d3 <= mpeg_sync_d2;
				mpeg_data_d1 <= mpeg_data;
				mpeg_data_d2 <= mpeg_data_d1;
				mpeg_data_d3 <= mpeg_data_d2;
			end
			else begin
			end
		end
	end

	reg matched_pid = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] matched_index = 0;

	wire match_states;
	assign match_states = (({mpeg_data_d1[5 - 1 : 0], mpeg_data} == ram_for_pid) && (pid_match_enable == 1)) ? 1 : 0;

	always @(posedge mpeg_clk) begin
		if(rst_n == 0) begin
			matched_pid <= 0;
			caching_ram_index <= 0;
		end
		else begin
			if((mpeg_valid == 1) && (matched_pid == 1)) begin
				if((matched_index >= 0) && (matched_index < PACK_BYTE_SIZE)) begin
					case(caching_ram_index)
						0: begin
							ram_for_data_0[matched_index / 4][(8 * (matched_index % 4) + 7) -: 8] <= mpeg_data_d3;
						end
						1: begin
							ram_for_data_1[matched_index / 4][(8 * (matched_index % 4) + 7) -: 8] <= mpeg_data_d3;
						end
						default: begin
						end
					endcase

					if(matched_index == (PACK_BYTE_SIZE - 1)) begin
						caching_ram_index <= (caching_ram_index == 0) ? 1 : 0;
					end
					else begin
					end
					matched_index <= matched_index + 1;
				end
				else begin
				end
			end
			else begin
			end

			if((mpeg_valid == 1) && (mpeg_sync_d2 == 1) && (mpeg_data_d2 == 8'h47)) begin
				if((match_states != 0) && (match_enable == 1)) begin
					matched_pid <= 1;

					matched_index <= 0;
				end
				else begin
					matched_pid <= 0;
				end
			end
		end
	end
endmodule
