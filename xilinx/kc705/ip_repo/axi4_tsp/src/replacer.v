`timescale 1ns / 1ps

module replacer #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer REPLACE_MATCH_PID_COUNT = 1,
		parameter integer REPLACE_DATA_GROUPS = 1,
		parameter integer PTS_DATA_WIDTH = 64

	)
	(
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] matched_count = 0,

		input wire rst_n,
		input wire clk,

		input wire match_enable,

		input wire base_data,

		input wire update_pid_request,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] pid_index,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] pid,

		output wire [C_S_AXI_DATA_WIDTH - 1 : 0] out_pid,

		input wire update_pts_request,
		input wire [PTS_DATA_WIDTH - 1 : 0] pts,

		output wire [PTS_DATA_WIDTH - 1 : 0] out_pts,

		input wire update_data_request,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] in_data,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] in_data_index,

		input wire pump_data_request,

		output reg pump_data_request_ready = 0,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] out_data = 0,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] out_data_index = 0,

		input wire [7 : 0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,

		output reg matched_state = 0,
		output reg [7 : 0] ts_out = 0,
		output reg ts_out_valid = 0,
		output reg ts_out_sync = 0
	);

	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	localparam integer PID_PID_WIDTH = 13;
	localparam integer PID_PAD0_WIDTH = 3;
	localparam integer PID_MATCH_ENABLE_WIDTH = 1;
	localparam integer PID_CHANGE_ENABLE_WIDTH = 1;
	localparam integer PID_PAD1_WIDTH = 15;

	reg [PID_PID_WIDTH - 1 : 0] ram_for_pid[0 : REPLACE_MATCH_PID_COUNT - 1];
	reg [PID_MATCH_ENABLE_WIDTH - 1 : 0] pid_match_enable[0 : REPLACE_MATCH_PID_COUNT - 1];
	reg [PID_CHANGE_ENABLE_WIDTH - 1 : 0] pid_change_enable[0 : REPLACE_MATCH_PID_COUNT - 1];
	wire [PID_PID_WIDTH - 1 : 0] cur_pid_pid;
	wire [PID_MATCH_ENABLE_WIDTH - 1 : 0] cur_pid_match_enable;
	wire [PID_CHANGE_ENABLE_WIDTH - 1 : 0] cur_pid_change_enable;

	assign cur_pid_pid = pid[PID_PID_WIDTH - 1 : 0];
	assign cur_pid_match_enable = pid[PID_PID_WIDTH + PID_PAD0_WIDTH + PID_MATCH_ENABLE_WIDTH - 1 : PID_PID_WIDTH + PID_PAD0_WIDTH];
	assign cur_pid_change_enable = pid[PID_PID_WIDTH + PID_PAD0_WIDTH + PID_MATCH_ENABLE_WIDTH + PID_CHANGE_ENABLE_WIDTH - 1 : PID_PID_WIDTH + PID_PAD0_WIDTH + PID_MATCH_ENABLE_WIDTH];

	assign out_pid = {{(PID_PAD1_WIDTH){1'b0}}, pid_change_enable[pid_index], pid_match_enable[pid_index], {(PID_PAD0_WIDTH){1'b0}}, ram_for_pid[pid_index]};

	reg [PTS_DATA_WIDTH - 1 : 0] pts_data_per_pid[0 : REPLACE_MATCH_PID_COUNT - 1];

	assign out_pts = pts_data_per_pid[pid_index];

	integer update_pid_index = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			update_pid_index <= 0;

			for(update_pid_index = 0; update_pid_index < REPLACE_MATCH_PID_COUNT; update_pid_index = update_pid_index + 1) begin
				ram_for_pid[update_pid_index] <= 0;
				pid_match_enable[update_pid_index] <= 0;
				pid_change_enable[update_pid_index] <= 0;
			end
		end
		else begin
			if(update_pid_request == 1) begin
				if((pid_index >= 0) && (pid_index < REPLACE_MATCH_PID_COUNT)) begin
					ram_for_pid[pid_index] <= cur_pid_pid;
					pid_match_enable[pid_index] <= cur_pid_match_enable;
					pid_change_enable[pid_index] <= cur_pid_change_enable;
				end
				else begin
				end
			end
			else begin
			end
		end
	end

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] pts_delta = 1389;
	reg inc_pts = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			pts_delta <= 1389;
			inc_pts <= 0;
		end
		else begin
			inc_pts <= 0;

			if(pts_delta > 0) begin
				pts_delta <= pts_delta - 1;
			end
			else begin
				pts_delta <= 1389;
				inc_pts <= 1;
			end
		end
	end

	integer update_pts_index = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			update_pts_index <= 0;
		end
		else begin
			if(update_pts_request == 1) begin
				if((pid_index >= 0) && (pid_index < REPLACE_MATCH_PID_COUNT)) begin
					pts_data_per_pid[pid_index] <= pts;
				end
				else begin
				end
			end
			else if(inc_pts == 1) begin
				for(update_pts_index = 0; update_pts_index < REPLACE_MATCH_PID_COUNT; update_pts_index = update_pts_index + 1) begin
					pts_data_per_pid[update_pts_index] <= pts_data_per_pid[update_pts_index] + 1;
				end
			end
			else begin
			end
		end
	end

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] ram_for_data[0 : PACK_WORD_SIZE * REPLACE_DATA_GROUPS - 1];

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
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] pump_data_index = 0;
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
	reg [7 : 0] mpeg_data_d1 = 0;
	reg [7 : 0] mpeg_data_d2 = 0;
	reg [7 : 0] mpeg_data_d3 = 0;

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

	wire [REPLACE_MATCH_PID_COUNT - 1 : 0] match_states;
	wire [REPLACE_MATCH_PID_COUNT - 1 : 0] change_pid_states;
	genvar i;
	generate for (i = 0; i < REPLACE_MATCH_PID_COUNT; i = i + 1)
		begin : matcher
			assign match_states[i] = (({mpeg_data_d1[5 - 1 : 0], mpeg_data} == ram_for_pid[i]) && (pid_match_enable[i] == 1)) ? 1 : 0;
			assign change_pid_states[i] = (({mpeg_data_d1[5 - 1 : 0], mpeg_data} == ram_for_pid[i]) && (pid_match_enable[i] == 1) && (pid_change_enable[i] == 1)) ? 1 : 0;
		end
	endgenerate

	reg matched_pid = 0;
	reg change_pid = 0;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] pid_slot_index = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] ts_out_group_index_per_pid[0 : REPLACE_MATCH_PID_COUNT - 1];
	
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] ts_out_group_index = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] matched_packet_index = 0;

	wire [C_S_AXI_DATA_WIDTH - 1 : 0] ram_match_index;
	assign ram_match_index = (ts_out_group_index * PACK_BYTE_SIZE) + matched_packet_index;

	wire [8 - 1 : 0] cur_ram_data;
	assign cur_ram_data = (matched_packet_index == PACK_BYTE_SIZE) ? 8'b00 : (ram_for_data[ram_match_index / 4][(8 * (ram_match_index % 4) + 7) -: 8]);

	wire payload_unit_start_indicator;
	assign payload_unit_start_indicator = (ts_out_group_index == 0) ? 1'b1 : 1'b0;

	//wire [1 : 0] transport_scrambling_control;
	//assign transport_scrambling_control = (ts_out_group_index == 0) ? 2'b00 : 2'b01;

	wire [8 - 1 : 0] pts_0;
	wire [8 - 1 : 0] pts_1;
	wire [8 - 1 : 0] pts_2;
	wire [8 - 1 : 0] pts_3;
	wire [8 - 1 : 0] pts_4;
	assign pts_0 = (ts_out_group_index > 0) ? cur_ram_data : {4'b0010, pts_data[32 : 30], 1'b1};
	assign pts_1 = (ts_out_group_index > 0) ? cur_ram_data : pts_data[29 : 22];
	assign pts_2 = (ts_out_group_index > 0) ? cur_ram_data : {pts_data[21 : 15], 1'b1};
	assign pts_3 = (ts_out_group_index > 0) ? cur_ram_data : pts_data[14 : 7];
	assign pts_4 = (ts_out_group_index > 0) ? cur_ram_data : {pts_data[8 : 2], 1'b1};
	
	reg [PTS_DATA_WIDTH - 1 : 0] pts_data = 0;

	wire [1 : 0] adaption_field_control;
	assign adaption_field_control = 2'b11;

	always @(posedge mpeg_clk) begin
		if(rst_n == 0) begin
			matched_state <= 0;

			matched_pid <= 0;
			change_pid <= 0;

			pid_slot_index <= 0;
			for(pid_slot_index = 0; pid_slot_index < REPLACE_MATCH_PID_COUNT; pid_slot_index = pid_slot_index + 1) begin
				ts_out_group_index_per_pid[pid_slot_index] <= 0;
			end

			ts_out_group_index <= 0;
			matched_packet_index <= 0;

			pts_data <= 0;

			ts_out <= 0;
			ts_out_valid <= 0;
			ts_out_sync <= 0;

			matched_count <= 0;
		end
		else begin
			ts_out_valid <= 0;
			matched_state <= matched_pid;

			if(mpeg_valid == 1) begin
				if(matched_pid == 1) begin
					if(base_data == 1) begin
						ts_out_valid <= 1;
						ts_out_sync <= mpeg_sync_d3;
						ts_out <= mpeg_data_d3;
					end
					else begin
						//typedef struct TS_packet_header
						//{
						//    unsigned sync_byte                        : 8; //同步字节, 固定为0x47,表示后面的是一个TS分组
						//    unsigned transport_error_indicator        : 1; //传输误码指示符
						//    unsigned payload_unit_start_indicator    : 1; //有效荷载单元起始指示符
						//   
						//    unsigned transport_priority              : 1; //传输优先, 1表示高优先级,传输机制可能用到，解码用不着
						//    unsigned PID                            : 13; //PID
						//    unsigned transport_scrambling_control    : 2; //传输加扰控制 
						//    unsigned adaption_field_control            : 2; //自适应控制 01仅含有效负载，10仅含调整字段，11含有调整字段和有效负载。为00解码器不进行处理
						//    unsigned continuity_counter                : 4; //连续计数器 一个4bit的计数器，范围0-15
						//} TS_packet_header;
						//
						//需要修改的
						//payload_unit_start_indicator 1->1 2->0 1-6
						//PID 2-0,1,2,3,4 3-0,1,2,3,4,5,6,7
						//transport_scrambling_control 1->00 2->01 3-6,7
						//adaption_field_control 1->11 2->11 3-4,5
						if((matched_packet_index >= 0) && (matched_packet_index < PACK_BYTE_SIZE)) begin
							ts_out_valid <= 1;
							ts_out_sync <= mpeg_sync_d3;

							if(matched_packet_index == 1) begin
								//in common replacer, packet pid reserved!
								if(change_pid == 0) begin
									ts_out <= {mpeg_data_d3[7], payload_unit_start_indicator, mpeg_data_d3[5], mpeg_data_d3[5 - 1 : 0]};
								end
								else begin
									ts_out <= {mpeg_data_d3[7], payload_unit_start_indicator, mpeg_data_d3[5], cur_ram_data[5 - 1 : 0]};
								end
							end
							else if(matched_packet_index == 2) begin
								if(change_pid == 0) begin
									ts_out <= mpeg_data_d3;
								end
								else begin
									ts_out <= cur_ram_data;
								end
							end
							else if(matched_packet_index == 3) begin
								//ts_out <= {transport_scrambling_control, adaption_field_control, mpeg_data_d3[3 : 0]};
								ts_out <= {mpeg_data_d3[7 : 6], adaption_field_control, mpeg_data_d3[3 : 0]};
							end
							else if(matched_packet_index == 24) begin
								ts_out <= pts_0;
							end
							else if(matched_packet_index == 25) begin
								ts_out <= pts_1;
							end
							else if(matched_packet_index == 26) begin
								ts_out <= pts_2;
							end
							else if(matched_packet_index == 27) begin
								ts_out <= pts_3;
							end
							else if(matched_packet_index == 28) begin
								ts_out <= pts_4;
							end
							else begin
								ts_out <= cur_ram_data;
							end
							
							matched_packet_index <= matched_packet_index + 1;
						end
						else begin
						end
					end

				end
				else begin
				end

				if(base_data == 1) begin
					matched_pid <= 1;
				end
				else begin
					if((mpeg_sync_d2 == 1) && (mpeg_data_d2 == 8'h47)) begin
						if((match_states != 0) && (match_enable == 1)) begin
							matched_pid <= 1;

							if(change_pid_states != 0) begin
								change_pid <= 1;
							end
							else begin
								change_pid <= 0;
							end

							for(pid_slot_index = 0; pid_slot_index < REPLACE_MATCH_PID_COUNT; pid_slot_index = pid_slot_index + 1) begin
								if(match_states[pid_slot_index] == 1) begin
									ts_out_group_index <= ts_out_group_index_per_pid[pid_slot_index];

									if((ts_out_group_index_per_pid[pid_slot_index] >= 0) && (ts_out_group_index_per_pid[pid_slot_index] < REPLACE_DATA_GROUPS - 1)) begin
										ts_out_group_index_per_pid[pid_slot_index] <= ts_out_group_index_per_pid[pid_slot_index] + 1;
									end
									else begin
										ts_out_group_index_per_pid[pid_slot_index] <= 0;
									end

									pts_data <= pts_data_per_pid[pid_slot_index];
								end
								else begin
								end
							end

							matched_packet_index <= 0;
							matched_count <= matched_count + 1;
						end
						else begin
							matched_pid <= 0;
						end
					end
					else begin
					end
				end
			end
			else begin
			end
		end
	end
endmodule
