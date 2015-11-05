`timescale 1ns / 1ps

module replacer #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer REPLACE_MATCH_PID_COUNT = 32,
		parameter integer REPLACE_DATA_GROUPS = 1
	)
	(
		input wire S_AXI_ARESETN,
		input wire S_AXI_ACLK,

		input wire base_data,

		input wire pump_data_enable,

		input wire save_replace_data_enable,
		input wire [C_S_AXI_DATA_WIDTH-1:0] in_data,
		input wire [C_S_AXI_DATA_WIDTH-1:0] in_data_index,


		output reg [C_S_AXI_DATA_WIDTH-1:0] out_data = 0,
		output reg [C_S_AXI_DATA_WIDTH-1:0] out_data_index = 0,
		output reg ready_for_read = 0,

		input wire update_pid_enable,
		input wire [C_S_AXI_DATA_WIDTH-1:0] in_pid,
		output reg [C_S_AXI_DATA_WIDTH-1:0] out_pid = 0,

		input wire [7:0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,

		output reg ts_out_valid = 0,
		output reg [7:0] ts_out = 0,
		output reg ts_out_sync = 0,
		output reg matched_state = 0,

		input wire run_enable
	);

	localparam integer REPLACE_PID_PID_WIDTH = 13;
	localparam integer REPLACE_PID_INDEX_WIDTH = 8;
	localparam integer REPLACE_PID_MATCH_ENABLE_WIDTH = 1;
	localparam integer REPLACE_PID_PAD_WIDTH = C_S_AXI_DATA_WIDTH - REPLACE_PID_PID_WIDTH - REPLACE_PID_INDEX_WIDTH - REPLACE_PID_MATCH_ENABLE_WIDTH;

	localparam integer REPLACE_PID_PID_START = 0;
	localparam integer REPLACE_PID_PID_END = REPLACE_PID_PID_START + REPLACE_PID_PID_WIDTH - 1;

	localparam integer REPLACE_PID_INDEX_START = REPLACE_PID_PID_WIDTH;
	localparam integer REPLACE_PID_INDEX_END = REPLACE_PID_INDEX_START + REPLACE_PID_INDEX_WIDTH - 1;

	localparam integer REPLACE_PID_MATCH_ENABLE_START = REPLACE_PID_PID_WIDTH + REPLACE_PID_INDEX_WIDTH;
	localparam integer REPLACE_PID_MATCH_ENABLE_END = REPLACE_PID_MATCH_ENABLE_START + REPLACE_PID_MATCH_ENABLE_WIDTH - 1;

	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_data[0 : PACK_WORD_SIZE * REPLACE_DATA_GROUPS - 1];
	reg [REPLACE_PID_PID_WIDTH - 1 : 0] ram_for_pid[0 : REPLACE_MATCH_PID_COUNT - 1];
	reg [REPLACE_MATCH_PID_COUNT - 1 : 0] match_pid_enable = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] data_index = 0;

	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
		end
		else begin
			if(save_replace_data_enable == 1) begin
				ram_for_data[in_data_index] <= in_data;
			end
			else begin
			end
		end
	end

	wire [REPLACE_PID_PID_WIDTH - 1 : 0] cur_pid_pid;
	wire [REPLACE_PID_INDEX_WIDTH - 1 : 0] cur_pid_index;
	wire [REPLACE_PID_MATCH_ENABLE_WIDTH - 1 : 0] cur_pid_enable;
	assign cur_pid_pid = in_pid[REPLACE_PID_PID_END : REPLACE_PID_PID_START];
	assign cur_pid_index = in_pid[REPLACE_PID_INDEX_END : REPLACE_PID_INDEX_START];
	assign cur_pid_enable = in_pid[REPLACE_PID_MATCH_ENABLE_END : REPLACE_PID_MATCH_ENABLE_START];

	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
			match_pid_enable <= 0;
			out_pid <= 0;
		end
		else begin
			if(update_pid_enable == 1) begin
				if((cur_pid_index >= 0) && (cur_pid_index < REPLACE_MATCH_PID_COUNT)) begin
					ram_for_pid[cur_pid_index] <= cur_pid_pid;
					match_pid_enable[cur_pid_index] <= cur_pid_enable;
				end
				else begin
				end
			end
			else begin
				out_pid <= {{(REPLACE_PID_PAD_WIDTH){1'b0}}, match_pid_enable[cur_pid_index], cur_pid_index, ram_for_pid[cur_pid_index]};
			end
		end
	end

	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
			ready_for_read <= 0;
			data_index <= 0;
			out_data_index <= 0;
		end
		else begin
			ready_for_read <= 0;
			if(pump_data_enable == 1) begin
				if((data_index >= 0) && (data_index < PACK_WORD_SIZE * REPLACE_DATA_GROUPS)) begin
					out_data_index <= data_index;
					out_data <= ram_for_data[data_index];

					data_index = data_index + 1;
				end
				else begin
					ready_for_read <= 1;
				end
			end
			else begin
				data_index <= 0;
			end
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


	reg ts_out_valid_d1 = 0;
	reg ts_out_valid_d2 = 0;
	reg ts_out_valid_d3 = 0;

	wire [REPLACE_MATCH_PID_COUNT - 1 : 0] match_states;

	genvar i;
	generate for (i = 0; i < REPLACE_MATCH_PID_COUNT; i = i + 1)
		begin : matcher
			assign match_states[i] = (({mpeg_data_d1[5 - 1 : 0], mpeg_data} == ram_for_pid[i]) && match_pid_enable[i]) ? 1 : 0;
		end
	endgenerate

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
					if((matched_index >= (ts_out_group_index * PACK_BYTE_SIZE)) && (matched_index < (ts_out_group_index * PACK_BYTE_SIZE) + PACK_BYTE_SIZE)) begin
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
						if((match_states > 0) && (run_enable == 1)) begin
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
