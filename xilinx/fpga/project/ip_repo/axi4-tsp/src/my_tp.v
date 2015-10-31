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

	reg [7:0] filter[PACK_BYTE_SIZE * 2 - 1 : 0];
	reg[7:0] mpeg_in[MPEG_LENGTH - 1:0];

	wire S_AXI_ACLK;
	reg S_AXI_ARESETN = 0;

	wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB;

	assign S_AXI_WSTRB = {(C_S_AXI_DATA_WIDTH/8){1'b1}};

	initial begin
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4-tsp/src/filter1.txt", filter, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4-tsp/src/filter2.txt", filter, PACK_BYTE_SIZE);
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4-tsp/src/ts4.txt", mpeg_in,0);
		#2;
		S_AXI_ARESETN = 1;
	end
	
	//write monitor pid
	reg mem_rden = 0;
	reg mem_wren = 0;
	reg [OPT_MEM_ADDR_BITS:0] mem_address;

	reg [7:0] mpeg_data = 0;
	wire mpeg_clk;
	reg mpeg_valid = 0;
	reg mpeg_sync = 0;

	wire [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata;
	wire ts_out_clk;
	wire ts_out_valid;
	wire [7:0] ts_out;

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
				mpeg_data <= mpeg_in[ts_index];
				if((ts_index % PACK_BYTE_SIZE) == 0) begin
					mpeg_sync <= 1;
				end
				else begin
					mpeg_sync <= 0;
				end
				mpeg_valid <= 1;
				ts_index <= ts_index + 1;
			end
			else begin
				ts_index <= 0;
			end
		end
	end

	//write monitor pid
	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
		end
		else begin
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
			.ts_out(ts_out)
		);

	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(S_AXI_ACLK));
	clkgen #(.clk_period(2)) xiaofeiclk2(.clk(mpeg_clk));
endmodule
