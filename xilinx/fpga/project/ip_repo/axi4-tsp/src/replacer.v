`timescale 1ns / 1ps

module replacer #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer REPLACE_MATCH_PID_COUNT = 1,
		parameter integer REPLACE_DATA_GROUPS = 1
	)
	(
		input wire S_AXI_ARESETN,
		input wire S_AXI_ACLK,

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
		output reg ts_out_valid = 0,
		output reg [7:0] ts_out = 0,
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
	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
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

	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
		end
		else begin
			if(update_data_request == 1) begin
				ram_for_data[in_data_index] <= in_data;
			end
			else begin
			end
		end
	end

	integer pump_data_state = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] pump_data_index = 0;
	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
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

	always @(posedge mpeg_clk) begin
		if(S_AXI_ARESETN == 0) begin
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

	reg [C_S_AXI_DATA_WIDTH-1:0] ts_out_group_index = 0;

	reg pid_matched = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] matched_index = 0;

	wire [REPLACE_MATCH_PID_COUNT - 1 : 0] match_states;
	genvar i;
	generate for (i = 0; i < REPLACE_MATCH_PID_COUNT; i = i + 1)
		begin : matcher
			assign match_states[i] = (({mpeg_data_d1[5 - 1 : 0], mpeg_data} == ram_for_pid[i]) && pid_match_enable[i]) ? 1 : 0;
		end
	endgenerate

	reg ts_out_valid_d1 = 0;
	reg ts_out_valid_d2 = 0;
	reg ts_out_valid_d3 = 0;

	always @(posedge mpeg_clk) begin
		if(S_AXI_ARESETN == 0) begin
			pid_matched <= 0;
			matched_state <= 0;
			matched_index <= 0;
			ts_out_valid <= 0;
			ts_out_group_index <= 0;
		end
		else begin
			matched_state <= pid_matched;

			if(base_data == 1) begin
				matched_state <= 1;

				ts_out_valid_d1 <= mpeg_valid;
				ts_out_valid_d2 <= ts_out_valid_d1;
				ts_out_valid_d3 <= ts_out_valid_d2;

				ts_out_valid <= ts_out_valid_d3;
				ts_out_sync <= mpeg_sync_d3;
				ts_out <= mpeg_data_d3;
			end
			else begin
				if(pid_matched == 1) begin
					if((matched_index >= ts_out_group_index * PACK_BYTE_SIZE) && (matched_index < ts_out_group_index * PACK_BYTE_SIZE + PACK_BYTE_SIZE)) begin
						ts_out_valid <= 1;
						ts_out_sync <= mpeg_sync_d3;
						ts_out <= ram_for_data[matched_index / 4][(8 * (matched_index % 4) + 7) -: 8];
						matched_index <= matched_index + 1;
					end
					else begin
						ts_out_valid <= 0;
					end
				end
				else begin
				end
			end

			if(mpeg_valid == 1) begin
				if(mpeg_sync_d2 == 1) begin
					if(mpeg_data_d2 == 8'h47) begin
						if((match_states > 0) && (match_enable == 1)) begin
							pid_matched <= 1;
							if((ts_out_group_index >= 0) && (ts_out_group_index < REPLACE_DATA_GROUPS - 1)) begin
								matched_index <= (ts_out_group_index + 1) * PACK_BYTE_SIZE;
								ts_out_group_index <= ts_out_group_index + 1;
							end
							else begin
								matched_index <= 0;
								ts_out_group_index <= 0;
							end
						end
						else begin
							pid_matched <= 0;
						end
					end
					else begin
					end
				end
				else begin
				end
			end
			else begin
			end
		end
	end
endmodule
