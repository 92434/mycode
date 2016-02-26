`timescale 1ns/1ns

module lcm_controler_eg9013f_nz #(
		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer BYTE_WIDTH = 8,
		parameter integer LCM_WIDTH_BITS = 640,
		parameter integer LCM_HEIGHT_BITS = 480,

		parameter integer C_S_AXI_DATA_WIDTH = 32
	)
	(
		input wire clk,
		input wire rst_n,

		input wire lcm_clk_event,
		input wire lcm_clk,

		output reg lcm_din = 0,
		output reg lcm_lp = 0,
		output wire xscl,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] lcm_data_index = 0
	);

	localparam integer LCM_WIDTH_BYTES = LCM_WIDTH_BITS / BYTE_WIDTH;
	localparam integer LCM_DATA_BYTES = LCM_WIDTH_BYTES * LCM_HEIGHT_BITS;
	localparam integer LCM_LP_DELAY = 10;

	integer state = 0;
	integer lp_delay_count = 0;
	reg lcm_clk_enable = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			state <= 0;
			lcm_clk_enable <= 0;
			lcm_lp <= 0;
			lcm_data_index <= 0;
			lp_delay_count <= 0;
			lcm_din <= 0;
		end
		else begin
			lcm_lp <= 0;
			case(state)
				0: begin
					if((lcm_clk_event == 1) && (lcm_clk == 1)) begin//start
						lcm_clk_enable <= 1;
						lcm_data_index <= 0;

						state <= 1;
					end
					else begin
					end
				end
				1: begin
					if((lcm_clk_event == 1) && (lcm_clk == 1)) begin//wait for posedge lcm_clk
						if((((lcm_data_index + 1) + 1) % LCM_WIDTH_BYTES) == 0) begin//write one line
							if(((lcm_data_index + 1) + 1) == LCM_WIDTH_BYTES) begin
								lcm_din <= 1;
							end
							else begin
							end

							state <= 2;//delay for lp
						end
						else begin
						end

						if((lcm_data_index >= 0) && (lcm_data_index < LCM_DATA_BYTES - 1)) begin
							lcm_data_index <= lcm_data_index + 1;
						end
						else begin
							lcm_data_index <= 0;
						end
					end
					else begin
					end

				end
				2: begin
					if((lcm_clk_event == 1) && (lcm_clk == 0)) begin//wait for negedge lcm_clk
						lcm_clk_enable <= 0;//disable xscl
						lcm_lp <= 1;//set lcm_lp to 1
						lp_delay_count <= 0;

						state <= 3;
					end
					else begin
					end
				end
				3: begin//keep lp to 1
					lcm_lp <= 1;
					if((lcm_clk_event == 1) && (lcm_clk == 1)) begin//wait for posedge lcm_clk
						if((lp_delay_count >= 0) && (lp_delay_count < LCM_LP_DELAY - 1)) begin
							lp_delay_count <= lp_delay_count + 1;
						end
						else begin
							lp_delay_count <= 0;

							state <= 4;
						end
					end
					else begin
					end
				end
				4: begin//reset lcm_lp
					if((lcm_clk_event == 1) && (lcm_clk == 0)) begin//wait for negedge lcm_clk
						lcm_clk_enable <= 1;//enable xscl
						
						state <= 5;
					end
					else begin
					end
				end
				5: begin
					if((lcm_clk_event == 1) && (lcm_clk == 1)) begin//wait for posedge lcm_clk
						lcm_din <= 0;
						
						state <= 1;
					end
					else begin
					end
				end
				default: begin
				end
			endcase
		end
	end

	reg xscl_reg = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			xscl_reg <= 0;
		end
		else begin
			if(lcm_clk_event == 1) begin
				xscl_reg <= lcm_clk;
			end
			else begin
			end
		end
	end

	assign xscl = (lcm_clk_enable == 1) ? xscl_reg : 0;
endmodule
