`timescale 1ns/1ns

module tb #
	(
		parameter integer BYTE_WIDTH = 8,
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10
	)
	();


	localparam integer C_S_AXI_DATA_BYTE_WIDTH = C_S_AXI_DATA_WIDTH / BYTE_WIDTH;

	wire clk;
	reg rst_n = 0;
	initial begin
		#5;
		rst_n = 1;
	end

	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));

	always @(posedge clk) begin
		if(rst_n == 0) begin
		end
		else begin
		end
	end

	reg [C_S_AXI_DATA_BYTE_WIDTH - 1 : 0] wstrb = 0;
	reg wen = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] wdata = 0;
	reg [OPT_MEM_ADDR_BITS - 1 : 0] waddr = 0;

	reg ren;
	wire [C_S_AXI_DATA_WIDTH - 1 : 0] rdata;
	reg [OPT_MEM_ADDR_BITS - 1 : 0] raddr;

	localparam integer LCM_WIDTH_BITS = 640;
	localparam integer LCM_HEIGHT_BITS = 480;
	localparam integer LCM_WIDTH_BYTES = LCM_WIDTH_BITS / BYTE_WIDTH;
	localparam integer LCM_WIDTH_WORDS = LCM_WIDTH_BITS / C_S_AXI_DATA_WIDTH;
	localparam integer LCM_DATA_BYTES = LCM_WIDTH_BYTES * LCM_HEIGHT_BITS;
	localparam integer REG_RANGE_DATA_WORD_SIZE = (1 << OPT_MEM_ADDR_BITS);
	localparam integer REG_RANGE_DATA_BYTE_SIZE = (1 << (OPT_MEM_ADDR_BITS + 2));

	localparam integer ADDR_LCM_CLK_LEVEL_DELAY = 0;
	localparam integer ADDR_BYTE_ORDER = ADDR_LCM_CLK_LEVEL_DELAY + 1;
	localparam integer ADDR_READ_CHANNEL_INDEX = ADDR_BYTE_ORDER + 1;
	localparam integer ADDR_WRITE_CHANNEL_INDEX = ADDR_READ_CHANNEL_INDEX + 1;

	integer state = 0;
	integer index = 0;

	wire [BYTE_WIDTH - 1 : 0] channel_index;
	assign channel_index = index[(OPT_MEM_ADDR_BITS + 2) +: BYTE_WIDTH] + 1;

	wire [BYTE_WIDTH - 1 : 0] wstrb_index;
	assign wstrb_index = (index % C_S_AXI_DATA_BYTE_WIDTH);
	wire [BYTE_WIDTH - 1 : 0] data;
	assign data = ((index % LCM_WIDTH_BYTES) / 5);

	integer delay = 0;
	reg byte_order = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			index <= 0;
			state <= 0;

			wen <= 0;
			wstrb <= 0;
			waddr <= 0;
			wdata <= 0;
			
			ren <= 0;
			raddr <= 0;

			delay <= 0;
			byte_order <= 0;
		end
		else begin
			wen <= 0;
			ren <= 0;
			wstrb <= 4'b0000;
			case(state)
				0: begin
					wen <= 1;
					wstrb <= 4'b1111;
					waddr <= ADDR_LCM_CLK_LEVEL_DELAY;
					wdata <= 1;

					state <= 1;
				end
				1: begin
					wen <= 1;
					wstrb <= 4'b1111;
					waddr <= ADDR_WRITE_CHANNEL_INDEX;
					wdata <= channel_index;

					state <= 2;
				end
				2: begin
					if((index >= 0) && (index < LCM_DATA_BYTES)) begin
						wen <= 1;
						case(wstrb_index)
							0: begin
								wstrb <= 4'b0001;
							end
							1: begin
								wstrb <= 4'b0010;
							end
							2: begin
								wstrb <= 4'b0100;
							end
							3: begin
								wstrb <= 4'b1000;
							end
							default: begin
							end
						endcase
						waddr <= index[(OPT_MEM_ADDR_BITS - 1 + 2) : 2];
						wdata[BYTE_WIDTH * wstrb_index +: BYTE_WIDTH] <= data;

						if((index > 0) && ((index + 1) % REG_RANGE_DATA_BYTE_SIZE) == 0) begin
							state <= 1;
						end
						else begin
						end

						index <= index + 1;
					end
					else begin
						index <= 0;
						state <= 3;
					end
				end
				3: begin
					wen <= 1;
					wstrb <= 4'b1111;
					waddr <= ADDR_READ_CHANNEL_INDEX;
					wdata <= channel_index;

					state <= 4;
				end
				4: begin
					if((index >= 0) && (index < LCM_DATA_BYTES)) begin
						ren <= 1;
						raddr <= index[(OPT_MEM_ADDR_BITS - 1 + 2) : 2];

						if((index > 0) && ((index + C_S_AXI_DATA_BYTE_WIDTH) % REG_RANGE_DATA_BYTE_SIZE == 0)) begin
							state <= 3;
						end
						else begin
						end

						index <= index + C_S_AXI_DATA_BYTE_WIDTH;
					end
					else begin
						index <= 0;
						state <= 5;
					end
				end
				5: begin
					wen <= 1;
					wstrb <= 4'b1111;
					waddr <= ADDR_READ_CHANNEL_INDEX;
					wdata <= 0;

					state <= 6;
				end
				6: begin
					ren <= 1;
					raddr <= ADDR_LCM_CLK_LEVEL_DELAY;
					state <= 7;
				end
				7: begin
					ren <= 1;
					raddr <= ADDR_BYTE_ORDER;
					state <= 8;
				end
				8: begin
					ren <= 1;
					raddr <= ADDR_READ_CHANNEL_INDEX;
					state <= 9;
				end
				9: begin
					ren <= 1;
					raddr <= ADDR_WRITE_CHANNEL_INDEX;
					state <= 10;
				end
				10: begin
					if((delay >= 0) && (delay < 100000)) begin

						delay <= delay + 1;
					end
					else begin
						delay <= 0;
						state <= 11;
					end
				end
				11: begin
					wen <= 1;
					wstrb <= 4'b1111;
					waddr <= ADDR_BYTE_ORDER;
					wdata <= {{(32 - 1){1'b0}}, ~byte_order};
					byte_order <= ~byte_order;

					state <= 1;
				end
				default: begin
				end
			endcase
		end
	end

	wire [BYTE_WIDTH - 1 : 0] lcm_data_origin;
	wire [BYTE_WIDTH - 1 : 0] lcm_data_reverse;
	wire lcm_din;
	wire lcm_lp;
	wire lcm_xscl;
	wire [BYTE_WIDTH - 1 : 0] lcm_data;

	lcm_controler_eg9013f_nz_ram #(
			.BYTE_WIDTH(BYTE_WIDTH),
			.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS)
		) lcm_controler_eg9013f_nz_ram_inst (
			.clk(clk),
			.rst_n(rst_n),

			.wstrb(wstrb),
			.wen(wen),
			.wdata(wdata),
			.waddr(waddr),

			.ren(ren),
			.rdata(rdata),
			.raddr(raddr),

			.lcm_data_origin(lcm_data_origin),
			.lcm_data_reverse(lcm_data_reverse),
			.lcm_din(lcm_din),
			.lcm_lp(lcm_lp),
			.lcm_xscl(lcm_xscl),
			.lcm_data(lcm_data)
		);
endmodule
