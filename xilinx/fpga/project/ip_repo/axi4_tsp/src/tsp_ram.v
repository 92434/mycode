`timescale 1ns / 1ps

module tsp_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,

		parameter integer MONITOR_FILTER_NUM = 2,
		parameter integer REPLACER_FILTER_NUM = 17,
		parameter integer REPLACE_MATCH_PID_COUNT = 1,
		parameter integer REPLACE_DATA_GROUPS = 1,
		parameter integer COMMON_REPLACER_FILTER_NUM = 1,
		parameter integer COMMON_REPLACE_MATCH_PID_COUNT = 16,
		parameter integer COMMON_REPLACE_DATA_GROUPS = 2
	)
	(
		input wire clk,
		input wire rst_n,

		input wire [(C_S_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb,
		input wire wen,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] wdata,
		input wire [OPT_MEM_ADDR_BITS : 0] waddr,

		input wire ren,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] rdata,
		input wire [OPT_MEM_ADDR_BITS : 0] raddr,

		input wire [7 : 0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,

		output wire ts_out_clk,
		output wire ts_out_valid,
		output wire ts_out_sync,
		output wire [7 : 0] ts_out
	);

	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	localparam integer ADDR_INDEX = 0;

	localparam integer ADDR_PID_INDEX = ADDR_INDEX + 1;

	localparam integer ADDR_PID = ADDR_PID_INDEX + 1;//{{15{1'b0}}, 1'b(pid_enable), {3{1'b0}}, 13'b(pid)}

	localparam integer ADDR_MATCH_ENABLE = ADDR_PID + 1;

	localparam integer ADDR_READ_REQUEST = ADDR_MATCH_ENABLE + 1;

	localparam integer ADDR_TS_DATA_BASE = 128;
	localparam integer ADDR_TS_DATA_END = ADDR_TS_DATA_BASE + PACK_WORD_SIZE * COMMON_REPLACE_DATA_GROUPS;

	localparam integer MONITOR_PID_BASE = 0;
	localparam integer REPLACER_PID_BASE = MONITOR_PID_BASE + MONITOR_FILTER_NUM;
	localparam integer ALL_FILTERS_NUM = MONITOR_FILTER_NUM + REPLACER_FILTER_NUM;

	// implement Block RAM(s)
	// for write command
	//
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] current_slot = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] current_pid_index = 0;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] current_data_index = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] current_data = 0;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] ram_for_pid_index [0 : ALL_FILTERS_NUM - 1];
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] ram_for_pid [0 : ALL_FILTERS_NUM - 1];

	reg [ALL_FILTERS_NUM - 1 : 0] match_enable = 0;

	reg [ALL_FILTERS_NUM - 1 : 0] pump_data_request = 0;
	wire [ALL_FILTERS_NUM - 1 : 0] pump_data_request_ready;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] ram_for_data [0 : PACK_WORD_SIZE * COMMON_REPLACE_DATA_GROUPS - 1];


	reg [ALL_FILTERS_NUM - 1 : 0] update_data_request = 0;

	reg [ALL_FILTERS_NUM - 1 : 0] update_pid_request = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			update_pid_request <= 0;
			pump_data_request <= 0;
			update_data_request <= 0;
		end
		else begin
			update_pid_request <= 0;
			pump_data_request <= 0;
			update_data_request <= 0;

			if(wen == 1) begin
				case(waddr)
					ADDR_INDEX: begin
						if((wdata >= 0) && (wdata < ALL_FILTERS_NUM)) begin
							current_slot <= wdata;
						end
						else begin
						end
					end
					ADDR_PID_INDEX: begin
						if((wdata >= 0) && (wdata < COMMON_REPLACE_MATCH_PID_COUNT)) begin
							current_pid_index <= wdata;
						end
						else begin
						end
					end
					ADDR_PID: begin
						ram_for_pid[current_slot] <= wdata;
						update_pid_request[current_slot] <= 1;
					end
					ADDR_MATCH_ENABLE: begin
						match_enable[current_slot] <= (wdata == 1) ? 1 : 0;
					end
					ADDR_READ_REQUEST: begin
						pump_data_request[current_slot] <= 1;
					end
					default: begin
						if((waddr >= ADDR_TS_DATA_BASE) && (waddr < ADDR_TS_DATA_END)) begin
							current_data_index <= waddr - ADDR_TS_DATA_BASE;
							current_data <= wdata;
							update_data_request[current_slot] <= 1;
						end
						else begin
						end
					end
				endcase
			end
			else begin
			end
		end
	end

	always @(posedge clk) begin
		if (ren == 1) begin
			case(raddr)
				ADDR_INDEX:
					rdata <= current_slot;
				ADDR_PID_INDEX: begin
					rdata <= current_pid_index;
				end
				ADDR_PID:
					if((current_slot >= 0) && (current_slot < REPLACER_PID_BASE)) begin
						rdata <= monitors_out_pid[current_slot];
					end
					else if((current_slot >= REPLACER_PID_BASE) && (current_slot < ALL_FILTERS_NUM))begin
						rdata <= replacers_out_pid[current_slot - REPLACER_PID_BASE + 1];
					end
					else begin
					end
				ADDR_MATCH_ENABLE:
					rdata <= match_enable[current_slot];
				ADDR_READ_REQUEST:
					rdata <= pump_data_request_ready[current_slot];
				default: begin
					if((raddr >= ADDR_TS_DATA_BASE) && (raddr < ADDR_TS_DATA_END)) begin
						rdata <= ram_for_data[raddr - ADDR_TS_DATA_BASE];
					end
					else begin
						rdata <= {16'hE000, {(16 - OPT_MEM_ADDR_BITS - 1){1'b0}}, raddr};
					end
				end
			endcase
		end
	end


	//for input assign
	wire [MONITOR_FILTER_NUM - 1 : 0] monitors_match_enable;
	wire [MONITOR_FILTER_NUM - 1 : 0] monitors_udpate_pid_request;
	wire [MONITOR_FILTER_NUM - 1 : 0] monitors_pump_data_request;
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] monitors_ram_for_pid [0 : MONITOR_FILTER_NUM - 1];

	//for output
	wire [MONITOR_FILTER_NUM - 1 : 0] monitors_pump_data_request_ready;
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] monitors_out_data[0 : MONITOR_FILTER_NUM - 1];
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] monitors_out_data_index[0 : MONITOR_FILTER_NUM - 1];
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] monitors_out_pid [0 : MONITOR_FILTER_NUM - 1];

	//output assign
	assign pump_data_request_ready[MONITOR_FILTER_NUM - 1 : 0] = monitors_pump_data_request_ready;

	genvar i;
	generate for (i = 0; i < MONITOR_FILTER_NUM; i = i + 1)
		begin : monitors

			//input assign
			assign monitors_match_enable[i] = match_enable[i];
			assign monitors_udpate_pid_request[i] = update_pid_request[i];
			assign monitors_pump_data_request[i] = pump_data_request[i];
			assign monitors_ram_for_pid[i] = ram_for_pid[i];

			monitor # 
				(
					.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH)
				)
				monitor_inst (
					.rst_n(rst_n),
					.clk(clk),

					.match_enable(monitors_match_enable[i]),

					.update_pid_request(monitors_udpate_pid_request[i]),
					.pid_index(current_pid_index),
					.pid(monitors_ram_for_pid[i]),

					.out_pid(monitors_out_pid[i]),

					.pump_data_request(monitors_pump_data_request[i]),

					.pump_data_request_ready(monitors_pump_data_request_ready[i]),
					.out_data(monitors_out_data[i]),
					.out_data_index(monitors_out_data_index[i]),

					.mpeg_data(mpeg_data),
					.mpeg_clk(mpeg_clk),
					.mpeg_valid(mpeg_valid),
					.mpeg_sync(mpeg_sync)
				);
		end
	endgenerate

	//for input assign
	wire [REPLACER_FILTER_NUM : 0] replacers_match_enable;
	wire [REPLACER_FILTER_NUM : 0] replacers_update_pid_request;
	wire [REPLACER_FILTER_NUM : 0] replacers_update_data_request;
	wire [REPLACER_FILTER_NUM : 0] replacers_pump_data_request;
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] replacers_ram_for_pid [0 : REPLACER_FILTER_NUM];


	//for output
	wire [REPLACER_FILTER_NUM : 0] replacers_data_request_ready;
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] replacers_out_data[0 : REPLACER_FILTER_NUM];
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] replacers_out_data_index[0 : REPLACER_FILTER_NUM];
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] replacers_out_pid[0 : REPLACER_FILTER_NUM];

	wire [REPLACER_FILTER_NUM : 0] replacers_matched_state;
	wire [REPLACER_FILTER_NUM : 0] replacers_basic_data;
	wire [REPLACER_FILTER_NUM : 0] replacers_ts_out_valid;
	wire [REPLACER_FILTER_NUM : 0] replacers_ts_out_sync;
	wire [7 : 0] replacers_ts_out[0 : REPLACER_FILTER_NUM];

	assign replacers_basic_data = {{(REPLACER_FILTER_NUM){1'b0}}, 1'b1};

	//output assign
	assign pump_data_request_ready[ALL_FILTERS_NUM - 1 : REPLACER_PID_BASE] = replacers_data_request_ready[REPLACER_FILTER_NUM : 1];
	
	genvar j;
	generate for (j=0; j <= REPLACER_FILTER_NUM; j = j + 1)
		begin : replacers

			localparam integer CUR_REPLACE_MATCH_PID_COUNT = (j > REPLACER_FILTER_NUM - COMMON_REPLACER_FILTER_NUM) ? COMMON_REPLACE_MATCH_PID_COUNT : REPLACE_MATCH_PID_COUNT;

			localparam integer CUR_REPLACE_DATA_GROUPS = (j > REPLACER_FILTER_NUM - COMMON_REPLACER_FILTER_NUM) ? COMMON_REPLACE_DATA_GROUPS : REPLACE_DATA_GROUPS;

			//input assign
			assign replacers_match_enable[j] = (j == 0) ? 0 : match_enable[REPLACER_PID_BASE + j - 1];
			assign replacers_update_pid_request[j] = (j == 0) ? 0 : update_pid_request[REPLACER_PID_BASE + j - 1];
			assign replacers_update_data_request[j] = (j == 0) ? 0 : update_data_request[REPLACER_PID_BASE + j - 1];
			assign replacers_pump_data_request[j] = (j == 0) ? 0 : pump_data_request[REPLACER_PID_BASE + j - 1];
			assign replacers_ram_for_pid[j] = (j == 0) ? 0 : ram_for_pid[REPLACER_PID_BASE + j - 1];

			replacer # 
				(
					.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
					.REPLACE_MATCH_PID_COUNT(CUR_REPLACE_MATCH_PID_COUNT),
					.REPLACE_DATA_GROUPS(CUR_REPLACE_DATA_GROUPS)
				)
				replacer_inst (
					.rst_n(rst_n),
					.clk(clk),

					.match_enable(replacers_match_enable[j]),

					.base_data(replacers_basic_data[j]),

					.update_pid_request(replacers_update_pid_request[j]),
					.pid_index(current_pid_index),
					.pid(replacers_ram_for_pid[j]),

					.out_pid(replacers_out_pid[j]),

					.update_data_request(replacers_update_data_request[j]),
					.in_data(current_data),
					.in_data_index(current_data_index),

					.pump_data_request(replacers_pump_data_request[j]),

					.pump_data_request_ready(replacers_data_request_ready[j]),
					.out_data(replacers_out_data[j]),
					.out_data_index(replacers_out_data_index[j]),

					.mpeg_data(mpeg_data),
					.mpeg_clk(mpeg_clk),
					.mpeg_valid(mpeg_valid),
					.mpeg_sync(mpeg_sync),

					.matched_state(replacers_matched_state[j]),
					.ts_out_valid(replacers_ts_out_valid[j]),
					.ts_out(replacers_ts_out[j]),
					.ts_out_sync(replacers_ts_out_sync[j])
				);
		end
	endgenerate

	//pump data to ram_for_data
	integer pump_data_state = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			pump_data_state <= 0;
		end
		else begin
			case(pump_data_state)
				0: begin
					if(pump_data_request[current_slot] == 1) begin
						pump_data_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if(pump_data_request_ready[current_slot] == 0) begin
						if((current_slot >= 0) && (current_slot < REPLACER_PID_BASE)) begin
							ram_for_data[(monitors_out_data_index[current_slot])] <= monitors_out_data[current_slot];
						end
						else if((current_slot >= REPLACER_PID_BASE) && (current_slot < ALL_FILTERS_NUM)) begin
							ram_for_data[(replacers_out_data_index[current_slot - REPLACER_PID_BASE + 1])] <= replacers_out_data[current_slot - REPLACER_PID_BASE + 1];
						end
						else begin
						end
					end
					else begin
						pump_data_state <= 0;
					end
				end
				default: begin
				end
			endcase
		end
	end

	//process ts out
	reg [7 : 0] ts_out_index = 0;
	reg ts_out_valid_reg = 0;
	reg [7 : 0] ts_out_reg = 0;
	reg ts_out_sync_reg = 0;

	assign ts_out_clk = mpeg_clk;
	assign ts_out_valid = ts_out_valid_reg;
	assign ts_out = ts_out_reg;
	assign ts_out_sync = ts_out_sync_reg;

	always @(posedge mpeg_clk) begin
		if(rst_n == 0) begin
			ts_out_index <= 0;
			ts_out_valid_reg <= 0;
			ts_out_reg <= 0;
		end
		else begin
			for(ts_out_index = 0; ts_out_index <= REPLACER_FILTER_NUM; ts_out_index = ts_out_index + 1) begin
				if(replacers_matched_state[ts_out_index] == 1) begin
					ts_out_valid_reg <= replacers_ts_out_valid[ts_out_index];
					ts_out_reg <= replacers_ts_out[ts_out_index];
					ts_out_sync_reg <= replacers_ts_out_sync[ts_out_index];
				end
			end
		end
	end

endmodule
