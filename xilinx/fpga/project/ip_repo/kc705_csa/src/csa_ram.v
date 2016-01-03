`timescale 1 ns / 1 ps

module csa_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10,


		parameter integer CSA_CALC_TIMES = 50000,
		parameter integer CSA_CALC_INST_NUM = 5,
		parameter integer CSA_CALC_IN_WIDTH = 8 * 5,
		parameter integer CSA_CALC_OUT_WIDTH = 8 * 6

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


		input wire csa_in_data_r_ready,
		output wire csa_in_data_rclk,
		output wire csa_in_data_ren,
		input wire [CSA_CALC_IN_WIDTH - 1 : 0] csa_in_data_rdata,

		output wire csa_out_data_wclk,
		output wire csa_out_data_wen,
		output wire [CSA_CALC_OUT_WIDTH - 1 : 0] csa_out_data_wdata
	);

	localparam integer ADDR_CHANNEL_INDEX = 0;
	localparam integer ADDR_IN_DATA_VALID = ADDR_CHANNEL_INDEX + 1;
	localparam integer ADDR_OUT_DATA_VALID = ADDR_IN_DATA_VALID + 1;
	localparam integer ADDR_IN_DATA_0 = ADDR_CHANNEL_INDEX + 1;
	localparam integer ADDR_IN_DATA_1 = ADDR_IN_DATA_0 + 1;
	localparam integer ADDR_IN_DATA_2 = ADDR_IN_DATA_1 + 1;
	localparam integer ADDR_IN_DATA_3 = ADDR_IN_DATA_2 + 1;
	localparam integer ADDR_IN_DATA_4 = ADDR_IN_DATA_3 + 1;

	localparam integer ADDR_OUT_DATA_0 = ADDR_IN_DATA_4 + 1;
	localparam integer ADDR_OUT_DATA_1 = ADDR_OUT_DATA_0 + 1;
	localparam integer ADDR_OUT_DATA_2 = ADDR_OUT_DATA_1 + 1;


	reg [OPT_MEM_ADDR_BITS : 0] current_write_address = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] current_write_data = 0;
	reg current_mem_wen = 0;

	integer index_wstrb;
	//assigning 8 bit data
	always @(posedge clk) begin
		if(rst_n == 0) begin
			current_write_address <= 0;
			current_write_data <= 0;
			current_mem_wen <= 0;
		end
		else begin
			if (wen == 1) begin
				current_write_address <= waddr;
				for(index_wstrb = 0; index_wstrb < (C_S_AXI_DATA_WIDTH / 8); index_wstrb = index_wstrb + 1) begin
					if(wstrb[index_wstrb] == 1) begin
						current_write_data[(8 * index_wstrb + 7) -: 8] <= wdata[(8 * index_wstrb + 7) -: 8];
					end
				end
			end
			current_mem_wen <= wen;
		end
	end


	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_current_channel = 0;
	reg csa_current_channel_changed = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_current_channel <= 0;
			csa_current_channel_changed <= 0;
		end
		else begin
			csa_current_channel_changed <= 0;
			if(current_mem_wen == 1) begin
				case(current_write_address)
					ADDR_CHANNEL_INDEX: begin
						if((current_write_data >= 0) && (current_write_data < CSA_CALC_INST_NUM)) begin
							csa_current_channel <= current_write_data;
							csa_current_channel_changed <= 1;
						end
						else begin
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

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_data_0 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_data_1 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_data_2 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_data_3 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_in_data_4 = 0;

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_out_data_0 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_out_data_1 = 0;
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] csa_out_data_2 = 0;

	reg csa_in_data_valid = 0;
	reg csa_out_data_valid = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			rdata <= 0;
		end
		else begin
			if (ren == 1) begin
				case(raddr)
					ADDR_CHANNEL_INDEX: begin
						rdata <= csa_current_channel;
					end
					ADDR_IN_DATA_VALID: begin
						rdata <= {{(C_S_AXI_DATA_WIDTH - 1){1'b0}}, csa_in_data_valid};
					end
					ADDR_OUT_DATA_VALID: begin
						rdata <= {{(C_S_AXI_DATA_WIDTH - 1){1'b0}}, csa_out_data_valid};
					end
					ADDR_IN_DATA_0: begin
						rdata <= csa_in_data_0;
					end
					ADDR_IN_DATA_1: begin
						rdata <= csa_in_data_1;
					end
					ADDR_IN_DATA_2: begin
						rdata <= csa_in_data_2;
					end
					ADDR_IN_DATA_3: begin
						rdata <= csa_in_data_3;
					end
					ADDR_IN_DATA_4: begin
						rdata <= csa_in_data_4;
					end
					ADDR_OUT_DATA_0: begin
						rdata <= csa_out_data_0;
					end
					ADDR_OUT_DATA_1: begin
						rdata <= csa_out_data_1;
					end
					ADDR_OUT_DATA_2: begin
						rdata <= csa_out_data_2;
					end
					default: begin
						rdata <= {16'hE000, {(16 - OPT_MEM_ADDR_BITS - 1){1'b0}}, raddr};
					end
				endcase
			end
		end
	end
	
	reg [CSA_CALC_IN_WIDTH - 1 : 0] csa_calc_logic_in = 0;
	wire [CSA_CALC_OUT_WIDTH - 1 : 0] csa_calc_logic_out;
	reg csa_calc_logic_calc_request [0 : CSA_CALC_INST_NUM - 1] = 0;
	wire csa_calc_logic_calc_ready [0 : CSA_CALC_INST_NUM - 1] = 0;
	
	genvar i;
	generate for (i = 0; i < CSA_CALC_INST_NUM; i = i + 1)
		begin : csa_calc_unit
			localparam integer id = i;

			csa_calc_logic #(
					.ID(id),
					.CSA_CALC_TIMES(CSA_CALC_TIMES)
				) csa_calc_logic_inst(
					.clk(clk),
					.rst_n(rst_n),

					.csa_calc_logic_in(csa_calc_logic_in),
					.csa_calc_logic_out(csa_calc_logic_out),
					.csa_calc_logic_calc_request(csa_calc_logic_calc_request[i]),
					.csa_calc_logic_calc_release(),
					.csa_calc_logic_calc_inuse(),
					.csa_calc_logic_calc_ready(csa_calc_logic_calc_ready[i])
				);
		end
	endgenerate

	integer csa_channel_in_index = 0;
	integer csa_in_data_state = 0;
	reg csa_in_data_changed = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_calc_logic_calc_request <= 0;
			csa_channel_in_index <= 0;
			csa_in_data_state <= 0;
			csa_calc_logic_in <= 0;
			csa_in_data_changed <= 0;
		end
		else begin
			csa_in_data_ren <= 0;
			csa_calc_logic_calc_request <= 0;

			case(csa_in_data_state)
				0: begin
					if(csa_in_data_r_ready == 1) begin
						csa_in_data_ren <= 1;

						csa_in_data_state <= 1;
					end
					else begin
					end
				end
				1: begin
					csa_calc_logic_in <= csa_in_data_rdata;
					csa_calc_logic_calc_request[csa_channel_in_index] <= 1;

					if(csa_channel_in_index == csa_current_channel) begin
						csa_in_data_0 <= {(C_S_AXI_DATA_WIDTH - 8){1'b0}, csa_in_data_rdata[(0 * 8) +: 8]};
						csa_in_data_1 <= {(C_S_AXI_DATA_WIDTH - 8){1'b0}, csa_in_data_rdata[(1 * 8) +: 8]};
						csa_in_data_2 <= {(C_S_AXI_DATA_WIDTH - 8){1'b0}, csa_in_data_rdata[(2 * 8) +: 8]};
						csa_in_data_3 <= {(C_S_AXI_DATA_WIDTH - 8){1'b0}, csa_in_data_rdata[(3 * 8) +: 8]};
						csa_in_data_4 <= {(C_S_AXI_DATA_WIDTH - 8){1'b0}, csa_in_data_rdata[(4 * 8) +: 8]};
						csa_in_data_changed <= 1;
					end
					else begin
					end

					if((csa_channel_in_index >= 0) && (csa_channel_in_index < CSA_CALC_INST_NUM - 1)) begin
						csa_channel_in_index <= csa_channel_in_index + 1;
					end
					else begin
						csa_channel_in_index <= 0;
					end

					csa_in_data_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

	assign csa_out_data_wclk = clk;
	integer csa_channel_out_index = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_channel_out_index <= 0;

			csa_out_data_wen <= 0;
			csa_out_data_wdata <= 0;
		end
		else begin
			csa_out_data_wen <= 0;
			for(csa_channel_out_index = 0; csa_channel_out_index <= CSA_CALC_INST_NUM; csa_channel_out_index = csa_channel_out_index + 1) begin
				if(csa_calc_logic_calc_ready[csa_channel_out_index] == 1) begin
					csa_out_data_wdata <= csa_calc_logic_out;
					csa_out_data_wen <= 1;
				end
				else begin
				end
			end
		end
	end

	integer csa_data_valid_state = 0
	always @(posedge clk) begin
		if(rst_n == 0) begin
			csa_data_valid_state <= 1;
			csa_in_data_valid <= 0;
			csa_out_data_valid <= 0;
		end
		else begin
			case(csa_data_valid_state)
				0: begin
					if(csa_current_channel_changed == 1) begin
						csa_in_data_valid <= 0;
						csa_out_data_valid <= 0;

						csa_data_valid_state <= 1;
					end
					else begin
					end
				end
				1: begin
					if(csa_in_data_changed == 1) begin
						csa_in_data_valid <= 1;

						csa_data_valid_state <= 2;
					end
					else begin
					end
				end
				2: begin
					if(csa_out_data_changed == 1) begin
						csa_out_data_valid <= 1;

						csa_data_valid_state <= 0;
					end
					else begin
					end
				end
				default: begin
				end
			endcase
		end
	end

endmodule
