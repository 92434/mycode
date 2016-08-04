`timescale 1ns / 1ns

module tb #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,
		parameter integer MONITOR_FILTER_NUM = 1,
		parameter integer REPLACER_FILTER_NUM = 9,
		parameter integer REPLACE_MATCH_PID_COUNT = 1,
		parameter integer REPLACE_DATA_GROUPS = 1,
		parameter integer COMMON_REPLACER_FILTER_NUM = 1,
		parameter integer COMMON_REPLACE_MATCH_PID_COUNT = 16,
		parameter integer COMMON_REPLACE_DATA_GROUPS = 2
	)
	(
	);

	localparam integer MPEG_LENGTH = 1316;
	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	localparam integer ADDR_INDEX = 0;

	localparam integer ADDR_PID_INDEX = ADDR_INDEX + 1;

	localparam integer ADDR_PID = ADDR_PID_INDEX + 1;//{{15{1'b0}}, 1'b(pid_enable), {3{1'b0}}, 13'b(pid)}

	localparam integer ADDR_MATCH_ENABLE = ADDR_PID + 1;

	localparam integer ADDR_READ_REQUEST = ADDR_MATCH_ENABLE + 1;

	localparam integer ADDR_TS_DATA_BASE = 128;
	localparam integer ADDR_TS_DATA_END = ADDR_TS_DATA_BASE + PACK_WORD_SIZE;

	localparam integer MONITOR_PID_BASE = 0;
	localparam integer REPLACER_PID_BASE = MONITOR_PID_BASE + MONITOR_FILTER_NUM;
	localparam integer ALL_FILTERS_NUM = MONITOR_FILTER_NUM + REPLACER_FILTER_NUM;

	reg [7:0] filter1[PACK_BYTE_SIZE - 1 : 0];
	reg [7:0] filter2[PACK_BYTE_SIZE - 1 : 0];
	reg[7:0] mpeg_in[MPEG_LENGTH - 1:0];

	wire clk;
	reg rst_n = 0;

	reg ren = 0;
	reg wen = 0;
	reg [OPT_MEM_ADDR_BITS - 1 : 0] waddr = 0;
	reg [OPT_MEM_ADDR_BITS - 1 : 0] raddr = 0;


	wire [(C_S_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb;

	assign wstrb = {(C_S_AXI_DATA_WIDTH / 8){1'b1}};

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] wdata = 0;
	reg start_test_replacer = 0;

	initial begin
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/filter1.txt", filter1, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/filter2.txt", filter2, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/ts4.txt", mpeg_in,0);
		#2;
		rst_n = 1;

		start_test_replacer = 1;
	end
	
	reg [7:0] mpeg_data = 0;
	wire mpeg_clk;
	reg mpeg_valid = 0;
	reg mpeg_sync = 0;

	wire [C_S_AXI_DATA_WIDTH - 1 : 0] rdata;
	wire ts_out_clk;
	wire ts_out_valid;
	wire ts_out_sync;
	wire [7:0] ts_out;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] send_valid = 3;
	//always @(posedge mpeg_clk) begin
	//	if((send_valid >= 0) && (send_valid < 3)) begin
	//		send_valid <= send_valid + 1;
	//	end
	//	else begin
	//		send_valid <= 0;
	//	end
	//end

	//send ts
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] ts_index = 0;
	always @(posedge mpeg_clk) begin
		if(rst_n == 0) begin
			mpeg_data <= 0;
			mpeg_valid <= 0;
			mpeg_sync <= 0;
			ts_index <= 0;
		end
		else begin
			mpeg_sync <= 0;
			mpeg_valid <= 0;
			if((ts_index >= 0) && (ts_index < MPEG_LENGTH)) begin
				if((send_valid == 3)) begin
					mpeg_valid <= 1;

					mpeg_data <= mpeg_in[ts_index];
					if((ts_index % PACK_BYTE_SIZE) == 0) begin
						mpeg_sync <= 1;
					end
					else begin
					end
					ts_index <= ts_index + 1;
				end
				else begin
				end
			end
			else begin
				ts_index <= 0;
			end
		end
	end

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] read_delay = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] state_test = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] write_data_index = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] read_data_index = 0;


	always @(posedge clk) begin
		if(rst_n == 0) begin
			state_test <= 0;
			write_data_index <= 0;
		end
		else begin
			if(start_test_replacer == 1) begin
				case(state_test)
					0: begin
						state_test <= 1;
					end
					1: begin
						waddr <= ADDR_INDEX;
						wdata <= REPLACER_PID_BASE + 0;
						wen <= 1;
						ren <= 0;

						state_test <= 2;
					end
					2: begin
						waddr <= ADDR_PID_INDEX;
						wdata <= 0;

						state_test <= 3;
					end
					3: begin
						waddr <= ADDR_PID;
						wdata <= {{15{1'b0}}, 1'b1, {3{1'b0}}, 13'h157f};

						write_data_index <= 0;
						state_test <= 4;
					end
					4: begin
						if((write_data_index >= 0) && (write_data_index < PACK_BYTE_SIZE)) begin
							waddr <= ADDR_TS_DATA_BASE + write_data_index / 4;
							wdata <= {filter1[write_data_index + 3], filter1[write_data_index + 2], filter1[write_data_index + 1], filter1[write_data_index + 0]};
							write_data_index <= write_data_index + 4;
						end
						else begin
							state_test <= 5;
						end
					end
					5: begin
						waddr <= ADDR_MATCH_ENABLE;
						wdata <= 1;

						state_test <= 6;
					end
					6: begin
						waddr <= ADDR_INDEX;
						wdata <= REPLACER_PID_BASE + 1;

						state_test <= 7;
					end
					7: begin
						waddr <= ADDR_PID_INDEX;
						wdata <= 0;

						state_test <= 8;
					end
					8: begin
						waddr <= ADDR_PID;
						wdata <= {{15{1'b0}}, 1'b1, {3{1'b0}}, 13'h0191};

						write_data_index <= 0;
						state_test <= 9;
					end
					9: begin
						if((write_data_index >= 0) && (write_data_index < PACK_BYTE_SIZE)) begin
							waddr <= ADDR_TS_DATA_BASE/* + PACK_BYTE_SIZE / 4*/ + write_data_index / 4;
							wdata <= {filter2[write_data_index + 3], filter2[write_data_index + 2], filter2[write_data_index + 1], filter2[write_data_index + 0]};
							write_data_index <= write_data_index + 4;
						end
						else begin
							state_test <= 10;
						end
					end
					10: begin
						waddr <= ADDR_MATCH_ENABLE;
						wdata <= 1;

						state_test <= 11;
					end
					11: begin
						waddr <= ADDR_INDEX;
						wdata <= REPLACER_PID_BASE + 0;
						state_test <= 12;
					end
					12: begin
						waddr <= ADDR_READ_REQUEST;
						wdata <= 0;
						state_test <= 13;
					end
					13: begin
						raddr <= ADDR_READ_REQUEST;
						wen <= 0;
						ren <= 1;

						state_test <= 14;
						read_delay <= 0;
					end
					14: begin
						if(read_delay == 5) begin
							raddr <= ADDR_READ_REQUEST;
							state_test <= 15;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					15: begin
						if(rdata == 1) begin
							state_test <= 16;
						end
						else begin
						end
					end
					16: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							raddr <= ADDR_TS_DATA_BASE + read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							state_test <= 17;
							read_data_index <= 0;
						end
					end
					17: begin
						wen <= 1;
						ren <= 0;
						waddr <= ADDR_INDEX;
						wdata <= REPLACER_PID_BASE + 1;
						state_test <= 18;
					end
					18: begin
						waddr <= ADDR_READ_REQUEST;
						wdata <= 0;
						state_test <= 19;
					end
					19: begin
						wen <= 0;
						ren <= 1;
						raddr <= ADDR_READ_REQUEST;
						state_test <= 20;
						read_delay <= 0;
					end
					20: begin
						if(read_delay == 5) begin
							raddr <= ADDR_READ_REQUEST;
							state_test <= 21;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					21: begin
						if(rdata == 1) begin
							state_test <= 22;
						end
						else begin
						end
					end
					22: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							raddr <= ADDR_TS_DATA_BASE + /*PACK_BYTE_SIZE / 4 + */read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							state_test <= 23;
							read_data_index <= 0;
						end
					end
					23: begin
						wen <= 1;
						ren <= 0;
						waddr <= ADDR_INDEX;
						wdata <= 0;

						state_test <= 24;
					end
					24: begin
						waddr <= ADDR_PID_INDEX;
						wdata <= 0;

						state_test <= 25;
					end
					25: begin
						waddr <= ADDR_PID;
						wdata <= {{15{1'b0}}, 1'b1, {3{1'b0}}, 13'h157f};

						state_test <= 26;
					end
					26: begin
						waddr <= ADDR_MATCH_ENABLE;
						wdata <= 1;

						state_test <= 27;
					end
					27: begin
						waddr <= ADDR_INDEX;
						wdata <= 1;

						state_test <= 28;
					end
					28: begin
						waddr <= ADDR_PID_INDEX;
						wdata <= 0;

						state_test <= 29;
					end
					29: begin
						waddr <= ADDR_PID;
						wdata <= {{15{1'b0}}, 1'b1, {3{1'b0}}, 13'h0191};

						state_test <= 30;
					end
					30: begin
						waddr <= ADDR_MATCH_ENABLE;
						wdata <= 1;

						state_test <= 31;
					end
					31: begin//start
						wen <= 1;
						ren <= 0;

						waddr <= ADDR_INDEX;
						wdata <= 0;
						state_test <= 32;
					end
					32: begin
						if(mpeg_sync == 1) begin
							read_delay <= 0;
							state_test <= 33;
						end
						else begin
						end
					end
					33: begin
						if(read_delay == 20) begin
							waddr <= ADDR_READ_REQUEST;
							wdata <= 0;
							state_test <= 34;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					34: begin
						wen <= 0;
						ren <= 1;
						raddr <= ADDR_READ_REQUEST;
						state_test <= 35;
						read_delay <= 0;
					end
					35: begin
						if(read_delay == 5) begin
							raddr <= ADDR_READ_REQUEST;
							state_test <= 36;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					36: begin
						if(rdata == 1) begin
							state_test <= 37;
						end
						else begin
						end
					end
					37: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							raddr <= ADDR_TS_DATA_BASE + read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							state_test <= 38;
							read_data_index <= 0;
						end
					end
					38: begin
						wen <= 1;
						ren <= 0;
						waddr <= ADDR_INDEX;
						wdata <= 1;
						state_test <= 39;
					end
					39: begin
						if(mpeg_sync == 1) begin
							read_delay <= 0;
							state_test <= 40;
						end
						else begin
						end
					end
					40: begin
						if(read_delay == 20) begin
							waddr <= ADDR_READ_REQUEST;
							wdata <= 0;
							state_test <= 41;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					41: begin
						wen <= 0;
						ren <= 1;
						raddr <= ADDR_READ_REQUEST;
						state_test <= 42;
						read_delay <= 0;
					end
					42: begin
						if(read_delay == 5) begin
							raddr <= ADDR_READ_REQUEST;
							state_test <= 43;
						end
						else begin
							read_delay <= read_delay + 1;
						end
					end
					43: begin
						if(rdata == 1) begin
							state_test <= 44;
						end
						else begin
						end
					end
					44: begin
						if((read_data_index >= 0) && (read_data_index < PACK_WORD_SIZE)) begin
							raddr <= ADDR_TS_DATA_BASE + read_data_index;
							read_data_index <= read_data_index + 1;
						end
						else begin
							state_test <= 31;
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

	tsp_ram #(
			.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),
			.MONITOR_FILTER_NUM(MONITOR_FILTER_NUM),
			.REPLACER_FILTER_NUM(REPLACER_FILTER_NUM),
			.REPLACE_MATCH_PID_COUNT(REPLACE_MATCH_PID_COUNT),
			.REPLACE_DATA_GROUPS(REPLACE_DATA_GROUPS),
			.COMMON_REPLACER_FILTER_NUM(COMMON_REPLACER_FILTER_NUM),
			.COMMON_REPLACE_MATCH_PID_COUNT(COMMON_REPLACE_MATCH_PID_COUNT),
			.COMMON_REPLACE_DATA_GROUPS(COMMON_REPLACE_DATA_GROUPS)
		)
		tsp_ram_inst (
			.clk(clk),
			.rst_n(rst_n),

			.wstrb(wstrb),
			.wen(wen),
			.wdata(wdata),
			.waddr(waddr),

			.ren(ren),
			.rdata(rdata),
			.raddr(raddr),

			.mpeg_data(mpeg_data),
			.mpeg_clk(mpeg_clk),
			.mpeg_valid(mpeg_valid),
			.mpeg_sync(mpeg_sync),

			.ts_out_clk(ts_out_clk),
			.ts_out_valid(ts_out_valid),
			.ts_out_sync(ts_out_sync),
			.ts_out(ts_out)
		);

	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));
	clkgen #(.clk_period(2)) xiaofeiclk2(.clk(mpeg_clk));
endmodule
