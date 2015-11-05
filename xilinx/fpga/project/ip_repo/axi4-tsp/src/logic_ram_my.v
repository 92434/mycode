`timescale 1ns / 1ps

module logic_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,
		parameter integer MONITOR_FILTER_NUM = 32,
		parameter integer REPLACER_FILTER_NUM = 32,
		parameter integer REPLACE_MATCH_PID_COUNT = 32,
		parameter integer REPLACE_DATA_GROUPS = 1
	)
	(
		input wire S_AXI_ARESETN,
		input wire S_AXI_ACLK,
		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		input wire mem_rden,
		input wire mem_wren,
		input wire [OPT_MEM_ADDR_BITS:0] mem_address,

		input wire [7:0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,

		output reg [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata,
		output wire ts_out_clk,
		output wire ts_out_valid,
		output wire ts_out_sync,
		output wire [7:0] ts_out
	);

	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	localparam integer ADDR_INDEX = 0;

	localparam integer ADDR_PID = ADDR_INDEX + 1;

	localparam integer ADDR_RUN_ENABLE = ADDR_PID + 1;

	localparam integer ADDR_CMD = ADDR_RUN_ENABLE + 1;
	localparam integer CMD_WRITE_REPLACE_DATA = 1;
	localparam integer CMD_READ_REQUEST = 2;

	localparam integer ADDR_STATUS = ADDR_CMD + 1;

	localparam integer ADDR_TS_DATA_BASE = 128;
	localparam integer ADDR_TS_DATA_END = ADDR_TS_DATA_BASE + PACK_WORD_SIZE * REPLACE_DATA_GROUPS;

	localparam integer MONITOR_PID_BASE = 0;
	localparam integer REPLACER_PID_BASE = MONITOR_PID_BASE + MONITOR_FILTER_NUM;
	localparam integer ALL_FILTERS_NUM = MONITOR_FILTER_NUM + REPLACER_FILTER_NUM;

	// implement Block RAM(s)
	// for write command
	//
	reg [OPT_MEM_ADDR_BITS:0] current_write_address = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] current_write_data = 0;
	reg current_mem_wren = 0;

	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_pid [0 : ALL_FILTERS_NUM - 1];

	reg [ALL_FILTERS_NUM - 1 : 0] run_enable = 0;

	reg [ALL_FILTERS_NUM - 1 : 0] read_data_enable = 0;
	wire [ALL_FILTERS_NUM - 1 : 0] read_data_ready;

	reg [ALL_FILTERS_NUM - 1 : 0] write_data_enable = 0;

	reg [ALL_FILTERS_NUM - 1 : 0] pid_changing = 0;

	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_data [0 : PACK_WORD_SIZE * REPLACE_DATA_GROUPS - 1];

	reg [C_S_AXI_DATA_WIDTH-1:0] current_slot = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] current_data_index = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] current_data = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] current_cmd = 0;

	integer index_wstrb;
	//assigning 8 bit data
	always @(posedge S_AXI_ACLK) begin
		if (mem_wren) begin
			current_write_address <= mem_address;
			for(index_wstrb = 0; index_wstrb < (C_S_AXI_DATA_WIDTH / 8); index_wstrb = index_wstrb + 1) begin
				if(S_AXI_WSTRB[index_wstrb] == 1) begin
					current_write_data[(8 * index_wstrb + 7) -: 8] <= S_AXI_WDATA[(8 * index_wstrb + 7) -: 8];
				end
			end
		end
		current_mem_wren <= mem_wren;
	end

	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
			write_data_enable <= 0;
			read_data_enable <= 0;
			pid_changing <= 0;
		end
		else begin
			if(current_mem_wren) begin

				read_data_enable <= 0;
				pid_changing <= 0;

				//写数据时，维持write_data_enable状态,否则复位
				//状态
				if((current_write_address >= ADDR_TS_DATA_BASE) && (current_write_address < ADDR_TS_DATA_END)) begin
				end
				else begin
					write_data_enable <= 0;
				end

				case(current_write_address)
					ADDR_INDEX: begin
						if((current_write_data >= 0) && (current_write_data < ALL_FILTERS_NUM)) begin
							current_slot <= current_write_data;
						end
						else begin
						end
					end
					ADDR_PID: begin
						ram_for_pid[current_slot] <= current_write_data;
						pid_changing[current_slot] <= 1;
					end
					ADDR_RUN_ENABLE: begin
						if(current_write_data == 1) begin
							run_enable[current_slot] <= 1;
						end
						else if(current_write_data == 0) begin
							run_enable[current_slot] <= 0;
						end
						else begin
							run_enable[current_slot] <= 0;
						end
					end
					ADDR_CMD: begin
						current_cmd <= current_write_data;
						if(current_write_data == CMD_WRITE_REPLACE_DATA) begin
							if((current_slot >= REPLACER_PID_BASE) && (current_slot < ALL_FILTERS_NUM)) begin
								write_data_enable[current_slot] <= 1;
							end
							else begin
							end
						end
						else if(current_write_data == CMD_READ_REQUEST) begin
							read_data_enable[current_slot] <= 1;
						end
						else begin
						end
					end
					default: begin
						if((current_write_address >= ADDR_TS_DATA_BASE) && (current_write_address < ADDR_TS_DATA_END)) begin
							current_data_index <= current_write_address - ADDR_TS_DATA_BASE;
							current_data <= current_write_data;
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

	wire [C_S_AXI_DATA_WIDTH-1:0] replacers_out_pid [0 : REPLACER_FILTER_NUM];

	always @(posedge S_AXI_ACLK) begin
		if (mem_rden) begin
			case(mem_address)
				ADDR_INDEX:
					axi_rdata <= current_slot;
				ADDR_PID:
					if((current_slot >= 0) && (current_slot < MONITOR_FILTER_NUM)) begin
						axi_rdata <= ram_for_pid[current_slot];
					end
					else begin
						axi_rdata <= replacers_out_pid[current_slot - REPLACER_PID_BASE + 1];
					end
				ADDR_RUN_ENABLE:
					axi_rdata <= run_enable[current_slot];
				ADDR_CMD:
					axi_rdata <= current_cmd;
				ADDR_STATUS:
					axi_rdata <= read_data_ready[current_slot];
				default: begin
					if((mem_address >= ADDR_TS_DATA_BASE) && (mem_address < ADDR_TS_DATA_END)) begin
						axi_rdata <= ram_for_data[mem_address - ADDR_TS_DATA_BASE];
					end
					else begin
						axi_rdata <= {{(C_S_AXI_DATA_WIDTH / 4){1'b1}}, {(C_S_AXI_DATA_WIDTH / 4){1'b0}}, {(C_S_AXI_DATA_WIDTH / 4){1'b1}}, {(C_S_AXI_DATA_WIDTH / 4){1'b0}}};
					end
				end
			endcase
		end
	end


	//for input assign
	wire [MONITOR_FILTER_NUM - 1 : 0] monitors_run_enable;
	wire [MONITOR_FILTER_NUM - 1 : 0] monitors_read_data_enable;
	wire [C_S_AXI_DATA_WIDTH-1:0] ram_for_monitors_pid [0 : REPLACER_FILTER_NUM - 1];

	//for output
	wire [C_S_AXI_DATA_WIDTH-1:0] monitors_out_data[0 : MONITOR_FILTER_NUM - 1];
	wire [C_S_AXI_DATA_WIDTH-1:0] monitors_out_data_index[0 : MONITOR_FILTER_NUM - 1];
	wire [MONITOR_FILTER_NUM - 1 : 0] monitors_read_data_ready;

	//output assign
	assign read_data_ready[MONITOR_FILTER_NUM - 1 : 0] = monitors_read_data_ready;

	genvar i;
	generate for (i = 0; i < MONITOR_FILTER_NUM; i = i + 1)
		begin : monitors

			//input assign
			assign monitors_run_enable[i] = run_enable[i];
			assign monitors_read_data_enable[i] = read_data_enable[i];
			assign ram_for_monitors_pid[i] = ram_for_pid[i];

			monitor # 
				(
					.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH)
				)
				monitor_inst (
					.S_AXI_ARESETN(S_AXI_ARESETN),
					.S_AXI_ACLK(S_AXI_ACLK),

					.pump_data_enable(monitors_read_data_enable[i]),

					.out_data(monitors_out_data[i]),
					.out_data_index(monitors_out_data_index[i]),
					.ready_for_read(monitors_read_data_ready[i]),

					.pid(ram_for_monitors_pid[i]),
					.mpeg_data(mpeg_data),
					.mpeg_clk(mpeg_clk),
					.mpeg_valid(mpeg_valid),
					.mpeg_sync(mpeg_sync),

					.run_enable(monitors_run_enable[i])
				);
		end
	endgenerate

	//for input assign
	wire [REPLACER_FILTER_NUM : 0] replacers_run_enable;

	wire [REPLACER_FILTER_NUM : 0] replacers_read_data_enable;
	wire [REPLACER_FILTER_NUM : 0] replacers_write_data_enable;

	wire [REPLACER_FILTER_NUM : 0] replacers_update_pid_enable;

	wire [C_S_AXI_DATA_WIDTH-1:0] ram_for_replacers_pid [0 : REPLACER_FILTER_NUM];


	//for output
	wire [C_S_AXI_DATA_WIDTH-1:0] replacers_out_data[0 : REPLACER_FILTER_NUM];
	wire [C_S_AXI_DATA_WIDTH-1:0] replacers_out_data_index[0 : REPLACER_FILTER_NUM];

	wire [REPLACER_FILTER_NUM : 0] replacers_read_data_ready;
	wire [REPLACER_FILTER_NUM : 0] replacers_matched_state;
	wire [REPLACER_FILTER_NUM : 0] replacers_base_data;
	wire [REPLACER_FILTER_NUM : 0] replacers_ts_out_valid;
	wire [REPLACER_FILTER_NUM : 0] replacers_ts_out_sync;
	wire [7:0] replacers_ts_out[0 : REPLACER_FILTER_NUM];

	assign replacers_base_data = {{(REPLACER_FILTER_NUM){1'b0}}, 1'b1};

	//output assign
	assign read_data_ready[ALL_FILTERS_NUM - 1 : REPLACER_PID_BASE] = replacers_read_data_ready[REPLACER_FILTER_NUM : 1];
	
	genvar j;
	generate for (j=0; j <= REPLACER_FILTER_NUM; j = j + 1)
		begin : replacers

			//input assign
			assign replacers_run_enable[j] = (j == 0) ? 0 : run_enable[REPLACER_PID_BASE + j - 1];

			assign replacers_read_data_enable[j] = (j == 0) ? 0 : read_data_enable[REPLACER_PID_BASE + j - 1];
			assign replacers_write_data_enable[j] = (j == 0) ? 0 : write_data_enable[REPLACER_PID_BASE + j - 1];
			assign ram_for_replacers_pid[j] = (j == 0) ? 0 : ram_for_pid[REPLACER_PID_BASE + j - 1];
			assign replacers_update_pid_enable[j] = (j == 0) ? 0 : pid_changing[REPLACER_PID_BASE + j - 1];

			replacer # 
				(
					.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
					.REPLACE_MATCH_PID_COUNT(REPLACE_MATCH_PID_COUNT),
					.REPLACE_DATA_GROUPS(REPLACE_DATA_GROUPS)
				)
				replacer_inst (
					.S_AXI_ARESETN(S_AXI_ARESETN),
					.S_AXI_ACLK(S_AXI_ACLK),

					.base_data(replacers_base_data[j]),
					.pump_data_enable(replacers_read_data_enable[j]),

					.save_replace_data_enable(replacers_write_data_enable[j]),
					.in_data(current_data),
					.in_data_index(current_data_index),

					.out_data(replacers_out_data[j]),
					.out_data_index(replacers_out_data_index[j]),
					.ready_for_read(replacers_read_data_ready[j]),

					.update_pid_enable(replacers_update_pid_enable[j]),
					.in_pid(ram_for_replacers_pid[j]),
					.out_pid(replacers_out_pid[j]),

					.mpeg_data(mpeg_data),
					.mpeg_clk(mpeg_clk),
					.mpeg_valid(mpeg_valid),
					.mpeg_sync(mpeg_sync),

					.ts_out_valid(replacers_ts_out_valid[j]),
					.ts_out(replacers_ts_out[j]),
					.ts_out_sync(replacers_ts_out_sync[j]),
					.matched_state(replacers_matched_state[j]),

					.run_enable(replacers_run_enable[j])
				);
		end
	endgenerate

	//write back ram_for_data
	reg [ALL_FILTERS_NUM - 1 : 0] read_data_enable_d1 = 0;
	reg [7 : 0] write_back_index = 0;
	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
			write_back_index <= 0;
		end
		else begin
			read_data_enable_d1 <= read_data_enable;
			for(write_back_index = 0; write_back_index < ALL_FILTERS_NUM; write_back_index = write_back_index + 1) begin
				if(read_data_enable_d1[write_back_index] == 1) begin
					if((write_back_index >= 0) && (write_back_index < MONITOR_FILTER_NUM)) begin
						ram_for_data[monitors_out_data_index[write_back_index]] <= monitors_out_data[write_back_index];
					end
					else if((write_back_index >= REPLACER_PID_BASE) && (write_back_index < ALL_FILTERS_NUM)) begin
						ram_for_data[replacers_out_data_index[write_back_index - REPLACER_PID_BASE + 1]] <= replacers_out_data[write_back_index - REPLACER_PID_BASE + 1];
					end
					else begin
					end
				end
				else begin
				end
			end
		end
	end

	//process ts out
	reg [7 : 0] ts_out_index = 0;
	reg my_ts_out_valid = 0;
	reg [7:0] my_ts_out = 0;
	reg my_ts_out_sync = 0;

	assign ts_out_clk = mpeg_clk;
	assign ts_out_valid = my_ts_out_valid;
	assign ts_out = my_ts_out;
	assign ts_out_sync = my_ts_out_sync;

	always @(posedge mpeg_clk) begin
		if(S_AXI_ARESETN == 0) begin
			ts_out_index <= 0;
			my_ts_out_valid <= 0;
			my_ts_out <= 0;
		end
		else begin
			for(ts_out_index = 0; ts_out_index <= REPLACER_FILTER_NUM; ts_out_index = ts_out_index + 1) begin
				if(replacers_matched_state[ts_out_index] == 1) begin
					my_ts_out_valid <= replacers_ts_out_valid[ts_out_index];
					my_ts_out <= replacers_ts_out[ts_out_index];
					my_ts_out_sync <= replacers_ts_out_sync[ts_out_index];
				end
			end
		end
	end

endmodule
