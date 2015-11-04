`timescale 1ns / 1ns

module my_tp #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,
		parameter integer MONITOR_FILTER_NUM = 32,
		parameter integer REPLACER_FILTER_NUM = 32
	)
	(
	);

	localparam integer MPEG_LENGTH = 1316;
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
	localparam integer ADDR_TS_DATA_END = ADDR_TS_DATA_BASE + PACK_WORD_SIZE;

	localparam integer MONITOR_PID_BASE = 0;
	localparam integer REPLACER_PID_BASE = MONITOR_PID_BASE + MONITOR_FILTER_NUM;
	localparam integer ALL_FILTERS_NUM = MONITOR_FILTER_NUM + REPLACER_FILTER_NUM;

	reg [7:0] filter1[PACK_BYTE_SIZE - 1 : 0];
	reg [7:0] filter2[PACK_BYTE_SIZE - 1 : 0];
	reg[7:0] mpeg_in[MPEG_LENGTH - 1:0];

	wire S_AXI_ACLK;
	reg S_AXI_ARESETN = 0;

	reg mem_rden = 0;
	reg mem_wren = 0;
	reg [OPT_MEM_ADDR_BITS:0] mem_address = 0;


	wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB;

	assign S_AXI_WSTRB = {(C_S_AXI_DATA_WIDTH/8){1'b1}};

	reg [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA = 0;
	reg start_test_monitor = 0;
	reg start_test_replacer = 0;

	reg write_replacer_ts_enable = 0;

	reg [C_S_AXI_DATA_WIDTH-1 : 0] read_data_index = 0;

	initial begin
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4-tsp/src/filter1.txt", filter1, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4-tsp/src/filter2.txt", filter2, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4-tsp/src/ts4.txt", mpeg_in,0);
		#2;
		S_AXI_ARESETN = 1;

		//start_test_monitor = 1;
		start_test_replacer = 1;
	end
	
	//write monitor pid
	reg [7:0] mpeg_data = 0;
	wire mpeg_clk;
	reg mpeg_valid = 0;
	reg mpeg_sync = 0;

	wire [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata;
	wire ts_out_clk;
	wire ts_out_valid;
	wire ts_out_sync;
	wire [7:0] ts_out;

	reg [C_S_AXI_DATA_WIDTH-1 : 0] send_valid = 0;
	always @(posedge mpeg_clk) begin
		if((send_valid >= 0) && (send_valid < 3)) begin
			send_valid <= send_valid + 1;
		end
		else begin
			send_valid <= 0;
		end
	end

	//send ts
	reg [C_S_AXI_DATA_WIDTH-1 : 0] ts_index = 0;
	always @(posedge mpeg_clk) begin
		if(S_AXI_ARESETN == 0) begin
			mpeg_data <= 0;
			mpeg_valid <= 0;
			mpeg_sync <= 0;
			ts_index <= 0;
		end
		else begin
			if((ts_index >= 0) && (ts_index < MPEG_LENGTH)) begin
				if((send_valid == 3)) begin
					mpeg_valid <= 1;

					mpeg_data <= mpeg_in[ts_index];
					if((ts_index % PACK_BYTE_SIZE) == 0) begin
						mpeg_sync <= 1;
					end
					else begin
						mpeg_sync <= 0;
					end
					ts_index <= ts_index + 1;
				end
				else begin
					mpeg_valid <= 0;
				end
			end
			else begin
				ts_index <= 0;
			end
		end
	end

	//write monitor pid
	reg [C_S_AXI_DATA_WIDTH-1 : 0] state_read_monitor = 0;
	reg [C_S_AXI_DATA_WIDTH-1 : 0] read_delay = 0;
	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
			state_read_monitor <= 0;
			read_delay <= 0;
		end
		else begin
			if(start_test_monitor == 1) begin
				case(state_read_monitor)
					0: begin
						mem_wren <= 1;
						state_read_monitor <= 1;
					end
					1: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 0;

						state_read_monitor <= 2;
					end
					2: begin
						mem_address <= ADDR_PID;
						S_AXI_WDATA <= 32'h0000157f;

						state_read_monitor <= 3;
					end
					3: begin
						mem_address <= ADDR_RUN_ENABLE;
						S_AXI_WDATA <= 1;

						state_read_monitor <= 4;
					end
					4: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 1;

						state_read_monitor <= 5;
					end
					5: begin
						mem_address <= ADDR_PID;
						S_AXI_WDATA <= 32'h00000191;

						state_read_monitor <= 6;
					end
					6: begin
						mem_address <= ADDR_RUN_ENABLE;
						S_AXI_WDATA <= 1;

						state_read_monitor <= 7;
					end
					7: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 1;
						state_read_monitor <= 8;
					end
					8: begin
						if(mpeg_sync == 1) begin
							read_delay <= 0;
							state_read_monitor <= 9;
						end
						else begin
						end
					end
					9: begin
						if(read_delay == 20) begin
							mem_address <= ADDR_CMD;
							S_AXI_WDATA <= CMD_READ_REQUEST;
							state_read_monitor <= 10;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					10: begin
						mem_wren <= 0;
						mem_rden <= 1;
						mem_address <= ADDR_STATUS;
						state_read_monitor <= 11;
					end
					11: begin
						if(axi_rdata == 1) begin
							state_read_monitor <= 12;
						end
						else begin
						end
					end
					12: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							mem_address <= ADDR_TS_DATA_BASE + read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							mem_wren <= 1;
							mem_rden <= 0;

							state_read_monitor <= 7;
							read_data_index <= 0;
						end
					end
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	reg [C_S_AXI_DATA_WIDTH-1 : 0] state_read_replacer = 0;
	reg [C_S_AXI_DATA_WIDTH-1 : 0] write_replace_data_index = 0;

	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
			state_read_replacer <= 0;
			write_replace_data_index <= 0;
		end
		else begin
			if(start_test_replacer == 1) begin
				case(state_read_replacer)
					0: begin
						mem_wren <= 1;
						state_read_replacer <= 1;
					end
					1: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= REPLACER_PID_BASE;

						state_read_replacer <= 2;
					end
					2: begin
						mem_address <= ADDR_PID;
						S_AXI_WDATA <= 32'h0000157f;

						state_read_replacer <= 3;
					end
					3: begin
						mem_address <= ADDR_CMD;
						S_AXI_WDATA <= CMD_WRITE_REPLACE_DATA;
						write_replace_data_index <= 0;

						state_read_replacer <= 4;
					end
					4: begin
						if((write_replace_data_index >= 0) && (write_replace_data_index < PACK_BYTE_SIZE)) begin
							mem_address <= ADDR_TS_DATA_BASE + write_replace_data_index / 4;
							S_AXI_WDATA <= {filter1[write_replace_data_index + 3], filter1[write_replace_data_index + 2], filter1[write_replace_data_index + 1], filter1[write_replace_data_index + 0]};
							write_replace_data_index = write_replace_data_index + 4;
						end
						else begin
							state_read_replacer <= 5;
						end
					end
					5: begin
						mem_address <= ADDR_RUN_ENABLE;
						S_AXI_WDATA <= 1;

						state_read_replacer <= 6;
					end
					6: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= REPLACER_PID_BASE + 1;

						state_read_replacer <= 7;
					end
					7: begin
						mem_address <= ADDR_PID;
						S_AXI_WDATA <= 32'h00000191;

						state_read_replacer <= 8;
					end
					8: begin
						mem_address <= ADDR_CMD;
						S_AXI_WDATA <= CMD_WRITE_REPLACE_DATA;
						write_replace_data_index <= 0;

						state_read_replacer <= 9;
					end
					9: begin
						if((write_replace_data_index >= 0) && (write_replace_data_index < PACK_BYTE_SIZE)) begin
							mem_address <= ADDR_TS_DATA_BASE + write_replace_data_index / 4;
							S_AXI_WDATA <= {filter2[write_replace_data_index + 3], filter2[write_replace_data_index + 2], filter2[write_replace_data_index + 1], filter2[write_replace_data_index + 0]};
							write_replace_data_index = write_replace_data_index + 4;
						end
						else begin
							state_read_replacer <= 10;
						end
					end
					10: begin
						mem_address <= ADDR_RUN_ENABLE;
						S_AXI_WDATA <= 1;

						state_read_replacer <= 11;
					end
					11: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 32;
						state_read_replacer <= 12;
					end
					12: begin
						mem_address <= ADDR_CMD;
						S_AXI_WDATA <= CMD_READ_REQUEST;
						state_read_replacer <= 13;
					end
					13: begin
						mem_wren <= 0;
						mem_rden <= 1;
						mem_address <= ADDR_STATUS;
						state_read_replacer <= 14;
					end
					14: begin
						if(axi_rdata == 1) begin
							state_read_replacer <= 15;
						end
						else begin
						end
					end
					15: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							mem_address <= ADDR_TS_DATA_BASE + read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							mem_wren <= 1;
							mem_rden <= 0;

							state_read_replacer <= 16;
							read_data_index <= 0;
						end
					end
					16: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 33;
						state_read_replacer <= 17;
					end
					17: begin
						mem_address <= ADDR_CMD;
						S_AXI_WDATA <= CMD_READ_REQUEST;
						state_read_replacer <= 18;
					end
					18: begin
						mem_wren <= 0;
						mem_rden <= 1;
						mem_address <= ADDR_STATUS;
						state_read_replacer <= 19;
					end
					19: begin
						if(axi_rdata == 1) begin
							state_read_replacer <= 20;
						end
						else begin
						end
					end
					20: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							mem_address <= ADDR_TS_DATA_BASE + read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							mem_wren <= 1;
							mem_rden <= 0;

							state_read_replacer <= 21;
							read_data_index <= 0;
						end
					end
					21: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 0;

						state_read_replacer <= 22;
					end
					22: begin
						mem_address <= ADDR_PID;
						S_AXI_WDATA <= 32'h0000157f;

						state_read_replacer <= 23;
					end
					23: begin
						mem_address <= ADDR_RUN_ENABLE;
						S_AXI_WDATA <= 1;

						state_read_replacer <= 24;
					end
					24: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 1;

						state_read_replacer <= 25;
					end
					25: begin
						mem_address <= ADDR_PID;
						S_AXI_WDATA <= 32'h00000191;

						state_read_replacer <= 26;
					end
					26: begin
						mem_address <= ADDR_RUN_ENABLE;
						S_AXI_WDATA <= 1;

						state_read_replacer <= 27;
					end
					27: begin//start
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 0;
						state_read_replacer <= 28;
					end
					28: begin
						if(mpeg_sync == 1) begin
							read_delay <= 0;
							state_read_replacer <= 29;
						end
						else begin
						end
					end
					29: begin
						if(read_delay == 20) begin
							mem_address <= ADDR_CMD;
							S_AXI_WDATA <= CMD_READ_REQUEST;
							state_read_replacer <= 30;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					30: begin
						mem_wren <= 0;
						mem_rden <= 1;
						mem_address <= ADDR_STATUS;
						state_read_replacer <= 31;
					end
					31: begin
						if(axi_rdata == 1) begin
							state_read_replacer <= 32;
						end
						else begin
						end
					end
					32: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							mem_address <= ADDR_TS_DATA_BASE + read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							mem_wren <= 1;
							mem_rden <= 0;

							state_read_replacer <= 33;
							read_data_index <= 0;
						end
					end
					33: begin
						mem_address <= ADDR_INDEX;
						S_AXI_WDATA <= 1;
						state_read_replacer <= 34;
					end
					34: begin
						if(mpeg_sync == 1) begin
							read_delay <= 0;
							state_read_replacer <= 35;
						end
						else begin
						end
					end
					35: begin
						if(read_delay == 20) begin
							mem_address <= ADDR_CMD;
							S_AXI_WDATA <= CMD_READ_REQUEST;
							state_read_replacer <= 36;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					36: begin
						mem_wren <= 0;
						mem_rden <= 1;
						mem_address <= ADDR_STATUS;
						state_read_replacer <= 37;
					end
					37: begin
						if(axi_rdata == 1) begin
							state_read_replacer <= 38;
						end
						else begin
						end
					end
					38: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							mem_address <= ADDR_TS_DATA_BASE + read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							mem_wren <= 1;
							mem_rden <= 0;

							state_read_replacer <= 27;
							read_data_index <= 0;
						end
					end
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	logic_ram #(
			.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),
			.MONITOR_FILTER_NUM(MONITOR_FILTER_NUM),
			.REPLACER_FILTER_NUM(REPLACER_FILTER_NUM)
		)
		ram_inst (
			.S_AXI_ARESETN(S_AXI_ARESETN),
			.S_AXI_ACLK(S_AXI_ACLK),
			.S_AXI_WSTRB(S_AXI_WSTRB),
			.S_AXI_WDATA(S_AXI_WDATA),
			.mem_rden(mem_rden),
			.mem_wren(mem_wren),
			.mem_address(mem_address),

			.mpeg_data(mpeg_data),
			.mpeg_clk(mpeg_clk),
			.mpeg_valid(mpeg_valid),
			.mpeg_sync(mpeg_sync),

			.axi_rdata(axi_rdata),
			.ts_out_clk(ts_out_clk),
			.ts_out_valid(ts_out_valid),
			.ts_out_sync(ts_out_sync),
			.ts_out(ts_out)
		);

	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(S_AXI_ACLK));
	clkgen #(.clk_period(2)) xiaofeiclk2(.clk(mpeg_clk));
endmodule
