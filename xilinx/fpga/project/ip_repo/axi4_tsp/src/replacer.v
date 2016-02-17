`timescale 1ns / 1ps

module replacer #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer REPLACE_MATCH_PID_COUNT = 1,
		parameter integer REPLACE_DATA_GROUPS = 1
	)
	(
		input wire rst_n,
		input wire clk,

		input wire match_enable,

		input wire base_data,

		input wire update_pid_request,
		input wire [C_S_AXI_DATA_WIDTH-1:0] pid_index,
		input wire [C_S_AXI_DATA_WIDTH-1:0] pid,

		output wire [C_S_AXI_DATA_WIDTH-1:0] out_pid,

		input wire update_data_request,
		input wire [C_S_AXI_DATA_WIDTH-1:0] in_data,
		input wire [C_S_AXI_DATA_WIDTH-1:0] in_data_index,

		input wire pump_data_request,

		output reg pump_data_request_ready = 0,
		output reg [C_S_AXI_DATA_WIDTH-1:0] out_data = 0,
		output reg [C_S_AXI_DATA_WIDTH-1:0] out_data_index = 0,

		input wire [7:0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,

		output reg matched_state = 0,
		output reg [7:0] ts_out = 0,
		output reg ts_out_valid = 0,
		output reg ts_out_sync = 0
	);

	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	localparam integer PID_PID_WIDTH = 13;
	localparam integer PID_PAD0_WIDTH = 3;
	localparam integer PID_MATCH_ENABLE_WIDTH = 1;
	localparam integer PID_PAD1_WIDTH = 15;

	reg [PID_PID_WIDTH - 1 : 0] ram_for_pid[0 : REPLACE_MATCH_PID_COUNT - 1];
	reg [PID_MATCH_ENABLE_WIDTH - 1 : 0] pid_match_enable[0 : REPLACE_MATCH_PID_COUNT - 1];
	wire [PID_PID_WIDTH - 1 : 0] cur_pid_pid;
	wire [PID_MATCH_ENABLE_WIDTH - 1 : 0] cur_pid_match_enable;

	assign cur_pid_pid = pid[PID_PID_WIDTH - 1 : 0];
	assign cur_pid_match_enable = pid[PID_PID_WIDTH + PID_PAD0_WIDTH + PID_MATCH_ENABLE_WIDTH - 1 : PID_PID_WIDTH + PID_PAD0_WIDTH];

	assign out_pid = {{(PID_PAD1_WIDTH){1'b0}}, pid_match_enable[pid_index], {(PID_PAD0_WIDTH){1'b0}}, ram_for_pid[pid_index]};

	integer index = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			for(index = 0; index < REPLACE_MATCH_PID_COUNT; index = index + 1) begin
				ram_for_pid[index] <= 0;
				pid_match_enable[index] <= 0;
			end
		end
		else begin
			if(update_pid_request == 1) begin
				if((pid_index >= 0) && (pid_index < REPLACE_MATCH_PID_COUNT)) begin
					ram_for_pid[pid_index] <= cur_pid_pid;
					pid_match_enable[pid_index] <= cur_pid_match_enable;
				end
				else begin
				end
			end
			else begin
			end
		end
	end

	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_data[0 : PACK_WORD_SIZE * REPLACE_DATA_GROUPS - 1];

	always @(posedge clk) begin
		if(rst_n == 0) begin
		end
		else begin
			if(update_data_request == 1) begin
				if((in_data_index >= 0) && (in_data_index < PACK_WORD_SIZE * REPLACE_DATA_GROUPS)) begin
					ram_for_data[in_data_index] <= in_data;
				end
				else begin
				end
			end
			else begin
			end
		end
	end

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
					if((pump_data_index >= 0) && (pump_data_index < PACK_WORD_SIZE * REPLACE_DATA_GROUPS)) begin
						out_data_index <= pump_data_index;
						out_data <= ram_for_data[pump_data_index];

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
	reg updated = 0;

	always @(posedge mpeg_clk) begin
		if(rst_n == 0) begin
			mpeg_sync_d1 <= 0;
			mpeg_sync_d2 <= 0;
			mpeg_sync_d3 <= 0;
			mpeg_data_d1 <= 0;
			mpeg_data_d2 <= 0;
			mpeg_data_d3 <= 0;
			updated <= 0;
		end
		else begin
			updated <= 0;
			if(mpeg_valid == 1) begin
				mpeg_sync_d1 <= mpeg_sync;
				mpeg_sync_d2 <= mpeg_sync_d1;
				mpeg_sync_d3 <= mpeg_sync_d2;
				mpeg_data_d1 <= mpeg_data;
				mpeg_data_d2 <= mpeg_data_d1;
				mpeg_data_d3 <= mpeg_data_d2;
				updated <= 1;
			end
			else begin
			end
		end
	end

	wire [REPLACE_MATCH_PID_COUNT - 1 : 0] match_states;
	genvar i;
	generate for (i = 0; i < REPLACE_MATCH_PID_COUNT; i = i + 1)
		begin : matcher
			assign match_states[i] = (({mpeg_data_d1[5 - 1 : 0], mpeg_data} == ram_for_pid[i]) && pid_match_enable[i]) ? 1 : 0;
		end
	endgenerate

	reg [C_S_AXI_DATA_WIDTH-1:0] ts_out_group_index = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] matched_index = 0;
	reg matched_pid = 0;
	
	wire [C_S_AXI_DATA_WIDTH-1:0] ram_match_index;
	assign ram_match_index = (ts_out_group_index * PACK_BYTE_SIZE) + matched_index;

	always @(posedge mpeg_clk) begin
		if(rst_n == 0) begin
			matched_pid <= 0;
			matched_state <= 0;

			ts_out_group_index <= 0;
			matched_index <= 0;
			ts_out <= 0;
			ts_out_valid <= 0;
			ts_out_sync <= 0;
		end
		else begin
			matched_state <= matched_pid;

			if(matched_pid == 1) begin
				if(base_data == 1) begin
					ts_out <= mpeg_data_d3;
					ts_out_valid <= (updated == 1) ? 1 : 0;
					ts_out_sync <= (updated == 1) ? mpeg_sync_d3 : 0;
				end
				else begin
					if((matched_index >= 0) && (matched_index < PACK_BYTE_SIZE)) begin
						ts_out <= ram_for_data[ram_match_index / 4][(8 * (ram_match_index % 4) + 7) -: 8];
						ts_out_valid <= 1;
						ts_out_sync <= (matched_index == 0) ? 1 : 0;

						matched_index <= matched_index + 1;
					end
					else begin
						ts_out_valid <= 0;
					end
				end
			end
			else begin
			end

			if(base_data == 1) begin
				matched_pid <= 1;
			end
			else begin
				if((mpeg_valid == 1) && (mpeg_sync_d2 == 1) && (mpeg_data_d2 == 8'h47)) begin
					if((match_states > 0) && (match_enable == 1)) begin
						matched_pid <= 1;

						matched_index <= 0;

						if((ts_out_group_index >= 0) && (ts_out_group_index < REPLACE_DATA_GROUPS - 1)) begin
							ts_out_group_index <= ts_out_group_index + 1;
						end
						else begin
							ts_out_group_index <= 0;
						end
					end
					else begin
						matched_pid <= 0;
					end
				end
				else begin
				end
			end

		end
	end
endmodule
