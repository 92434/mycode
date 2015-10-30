`timescale 1ns / 1ps

module logic_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,
		parameter integer MONITOR_FILTER_NUM = 32,
		parameter integer REPLACER_FILTER_NUM = 32
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
		output wire ts_out_valid
		output wire [7:0] ts_out,
	);

	localparam integer PACK_BYTE_SIZE = 188;

	localparam integer ADDR_INDEX = 0;

	localparam integer ADDR_PID = ADDR_INDEX + 1;

	localparam integer ADDR_CMD = ADDR_PID + 1;
	localparam integer CMD_WRITE_REPLACE_DATA = 1;
	localparam integer CMD_READ_REQUEST = 2;

	localparam integer ADDR_STATUS = ADDR_CMD + 1;

	localparam integer ADDR_TS_DATA_BASE = 128;
	localparam integer ADDR_TS_DATA_END = ADDR_TS_DATA_BASE + PACK_BYTE_SIZE;

	localparam integer MONITOR_PID_BASE = 0;
	localparam integer REPLACER_PID_BASE = MONITOR_PID_BASE + MONITOR_FILTER_NUM;
	localparam integer ALL_FILTERS_NUM = MONITOR_FILTER_NUM + REPLACER_FILTER_NUM;

	// implement Block RAM(s)
	// for write command
	//
	reg [OPT_MEM_ADDR_BITS:0] current_write_address;
	reg [C_S_AXI_DATA_WIDTH-1:0] current_write_data;
	reg current_mem_wren = 0;

	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_pid [0 : ALL_FILTERS_NUM - 1];
	reg [ALL_FILTERS_NUM - 1 : 0] write_data_enable = 0;
	reg [ALL_FILTERS_NUM - 1 : 0] read_data_enable = 0;
	wire [ALL_FILTERS_NUM - 1 : 0] read_data_ready = 0;

	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_replace_data [0 : (PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8)) - 1];

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
		end
		else begin
			if(current_mem_wren) begin

				read_data_enable <= 0;

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

	always @(posedge S_AXI_ACLK) begin
		if (mem_rden) begin
			case(mem_address)
				ADDR_INDEX:
					axi_rdata <= current_slot;
				ADDR_PID:
					axi_rdata <= ram_for_pid[current_slot];
				ADDR_CMD:
					axi_rdata <= current_cmd;
				ADDR_STATUS:
					axi_rdata <= read_data_ready[current_slot];
				default: begin
					if((mem_address >= ADDR_TS_DATA_BASE) && (mem_address < ADDR_TS_DATA_END) begin
						axi_rdata <= ram_for_replace_data[mem_address - ADDR_TS_DATA_BASE];
					end
					else begin
						axi_rdata <= {(C_S_AXI_DATA_WIDTH / 4){1'b1}, (C_S_AXI_DATA_WIDTH / 4){1'b0}, (C_S_AXI_DATA_WIDTH / 4){1'b1}, (C_S_AXI_DATA_WIDTH / 4){1'b0}};
					end
				end
			endcase
		end
	end


	genvar i;
	generate for (i = 0; i < MONITOR_FILTER_NUM; i = i + 1)
		begin : monitors
			monitor # 
			()
			monitor_inst (
			);
		end
	endgenerate

	genvar j;
	generate for (j=MONITOR_FILTER_NUM; j < MONITOR_FILTER_NUM + REPLACER_FILTER_NUM; j = j + 1)
		begin : replacers

		wire [C_S_AXI_DATA_WIDTH-1:0] out_data;
		wire [C_S_AXI_DATA_WIDTH-1:0] out_data_index;
		reg pump_data_enable_d1 = 0;

		replacer # 
			(
				.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
				.MONITOR_FILTER_NUM(MONITOR_FILTER_NUM),
				.REPLACER_FILTER_NUM(REPLACER_FILTER_NUM)
			)
			replacer_inst (
				S_AXI_ARESETN(S_AXI_ARESETN),
				S_AXI_ACLK(S_AXI_ACLK),

				pump_data_enable(read_data_enable[j]),
				save_replace_data_enable(write_data_enable[j]),

				in_data(current_data),
				in_data_index(current_data_index),

				out_data(out_data),
				out_data_index(out_data_index),
				ready_for_read(read_data_ready[j]),

				pid(ram_for_pid[j]),
				mpeg_data(mpeg_data),
				mpeg_clk(mpeg_clk),
				mpeg_valid(mpeg_valid),
				mpeg_sync(mpeg_sync),

				ts_out_clk(ts_out_clk),
				ts_out_valid(ts_out_valid),
				ts_out(ts_out),
				matched_state()
			);

			always @(posedge S_AXI_ACLK) begin
				if(S_AXI_ARESETN == 0) begin
				end
				else
					pump_data_enable_d1 <= read_data_enable[j];
					if(pump_data_enable_d1 == 1) begin
						ram_for_replace_data[out_data_index] <= out_data;
					end
					else begin
					end
				end
			end
		end
	endgenerate

endmodule
