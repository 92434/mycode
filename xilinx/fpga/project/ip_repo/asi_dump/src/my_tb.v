`timescale 1ns / 1ns

module tb #(
		// Users to add parameters here
		parameter integer MPEG_DATA_WIDTH = 10,
		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Master Bus Interface M00_AXIS
		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 32
	)
	(
	);

	localparam integer MPEG_LENGTH = 1316;
	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_M00_AXIS_TDATA_WIDTH / 8);

	reg [7:0] filter1[PACK_BYTE_SIZE - 1 : 0];
	reg [7:0] filter2[PACK_BYTE_SIZE - 1 : 0];
	reg[7:0] mpeg_in[MPEG_LENGTH - 1 : 0];

	reg S_AXI_ARESETN = 0;

	wire ts_clk;
	reg ts_valid = 0;
	reg ts_sync = 0;
	reg [MPEG_DATA_WIDTH - 1 : 0] ts_data = 0;

	clkgen #(.clk_period(2)) xiaofeiclk2(.clk(ts_clk));

	reg [C_M00_AXIS_TDATA_WIDTH-1 : 0] send_valid = 0;
	always @(posedge ts_clk) begin
		if((send_valid >= 0) && (send_valid < 3)) begin
			send_valid <= send_valid + 1;
		end
		else begin
			send_valid <= 0;
		end
	end

	//send ts
	reg [C_M00_AXIS_TDATA_WIDTH-1 : 0] ts_index = 0;
	always @(posedge ts_clk) begin
		if(S_AXI_ARESETN == 0) begin
			ts_data <= 0;
			ts_valid <= 0;
			ts_sync <= 0;
			ts_index <= 0;
		end
		else begin
			if((ts_index >= 0) && (ts_index < MPEG_LENGTH)) begin
				if((send_valid == 3)) begin
					ts_valid <= 1;

					ts_data <= {2'b11, mpeg_in[ts_index]};
					if((ts_index % PACK_BYTE_SIZE) == 0) begin
						ts_sync <= 1;
					end
					else begin
						ts_sync <= 0;
					end
					ts_index <= ts_index + 1;
				end
				else begin
					ts_valid <= 0;
				end
			end
			else begin
				ts_index <= 0;
			end
		end
	end

	// Instantiation of Axi Bus Interface M00_AXIS
	wire r_ready;
	wire error_full;
	wire error_empty;

	reg m00_axis_aresetn = 1;
	wire m00_axis_aclk;
	wire m00_axis_tvalid;
	wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] m00_axis_tdata;
	wire [(C_M00_AXIS_TDATA_WIDTH / 8) - 1 : 0] m00_axis_tstrb;
	wire m00_axis_tlast;
	reg m00_axis_tready = 1;

	wire [9 : 0] data1;
	wire [9 : 0] data2;
	wire [9 : 0] data3;

	assign data1 = m00_axis_tdata[9 -: 10];
	assign data2 = m00_axis_tdata[19 -: 10];
	assign data3 = m00_axis_tdata[29 -: 10];

	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(m00_axis_aclk));

	asi_dump_wrap # ( 
		.MPEG_DATA_WIDTH(MPEG_DATA_WIDTH),

		.C_M00_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M00_AXIS_START_COUNT(C_M00_AXIS_START_COUNT)
	) asi_dump_wrap_inst (
		.ts_clk(ts_valid),
		.ts_data(ts_data),

		.r_ready(r_ready),
		.error_full(error_full),
		.error_empty(error_empty),

		.m00_axis_aclk(m00_axis_aclk),
		.m00_axis_aresetn(m00_axis_aresetn),
		.m00_axis_tvalid(m00_axis_tvalid),
		.m00_axis_tdata(m00_axis_tdata),
		.m00_axis_tstrb(m00_axis_tstrb),
		.m00_axis_tlast(m00_axis_tlast),
		.m00_axis_tready(m00_axis_tready)
	);

	initial begin
		$readmemh("/home/action/vivadoworkspace/ip_repo/tsp_dump/src/filter1.txt", filter1, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/tsp_dump/src/filter2.txt", filter2, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/tsp_dump/src/ts4.txt", mpeg_in,0);
		#2;
		S_AXI_ARESETN = 1;

		m00_axis_aresetn = 0;
		#2
		m00_axis_aresetn = 1;
		m00_axis_tready = 1;
	end
endmodule
