`timescale 1ns / 1ps

module replacer #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
	)
	(
		input wire S_AXI_ARESETN,
		input wire S_AXI_ACLK,

		input wire base_data,

		input wire pump_data_enable,

		input wire save_replace_data_enable,
		input wire [C_S_AXI_DATA_WIDTH-1:0] in_data,
		input wire [C_S_AXI_DATA_WIDTH-1:0] in_data_index,


		output reg [C_S_AXI_DATA_WIDTH-1:0] out_data = 0,
		output reg [C_S_AXI_DATA_WIDTH-1:0] out_data_index = 0,
		output reg ready_for_read = 0,

		input wire [C_S_AXI_DATA_WIDTH-1:0] pid,
		input wire [7:0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,

		output reg ts_out_valid = 0,
		output reg [7:0] ts_out = 0,
		output reg ts_out_sync = 0,
		output reg matched_state = 0,

		input wire run_enable
	);

	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	reg [C_S_AXI_DATA_WIDTH-1:0] ram_for_data [0 : PACK_WORD_SIZE - 1];
	reg [C_S_AXI_DATA_WIDTH-1:0] data_index = 0;

	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
		end
		else begin
			if(save_replace_data_enable == 1) begin
				ram_for_data[in_data_index] <= in_data;
			end
			else begin
			end
		end
	end

	always @(posedge S_AXI_ACLK) begin
		if(S_AXI_ARESETN == 0) begin
			ready_for_read <= 0;
			data_index <= 0;
			out_data_index <= 0;
		end
		else begin
			ready_for_read <= 0;
			if(pump_data_enable == 1) begin
				if((data_index >= 0) && (data_index < PACK_WORD_SIZE)) begin
					out_data_index <= data_index;
					out_data <= ram_for_data[data_index];

					data_index = data_index + 1;
				end
				else begin
					ready_for_read <= 1;
				end
			end
			else begin
				data_index <= 0;
			end
		end
	end

	reg mpeg_sync_d1 = 0;
	reg mpeg_sync_d2 = 0;
	reg mpeg_sync_d3 = 0;
	reg [7:0] mpeg_data_d1 = 0;
	reg [7:0] mpeg_data_d2 = 0;
	reg [7:0] mpeg_data_d3 = 0;

	always @(posedge mpeg_clk) begin
		if(S_AXI_ARESETN == 0) begin
			mpeg_sync_d1 <= 0;
			mpeg_sync_d2 <= 0;
			mpeg_sync_d3 <= 0;
			mpeg_data_d1 <= 0;
			mpeg_data_d2 <= 0;
			mpeg_data_d3 <= 0;
		end
		else begin
			if(mpeg_valid == 1) begin
				mpeg_sync_d1 <= mpeg_sync;
				mpeg_sync_d2 <= mpeg_sync_d1;
				mpeg_sync_d3 <= mpeg_sync_d2;
				mpeg_data_d1 <= mpeg_data;
				mpeg_data_d2 <= mpeg_data_d1;
				mpeg_data_d3 <= mpeg_data_d2;
			end
			else begin
			end
		end
	end

	reg pid_matched = 0;
	reg [C_S_AXI_DATA_WIDTH-1:0] matched_index = 0;

	reg ts_out_valid_d1 = 0;
	reg ts_out_valid_d2 = 0;
	reg ts_out_valid_d3 = 0;
	always @(posedge mpeg_clk) begin
		if(S_AXI_ARESETN == 0) begin
			pid_matched <= 0;
			matched_state <= 0;
			matched_index <= 0;
			ts_out_valid <= 0;
		end
		else begin
			matched_state <= pid_matched;

			if(base_data == 1) begin
				matched_state <= 1;

				ts_out_valid_d1 <= mpeg_valid;
				ts_out_valid_d2 <= ts_out_valid_d1;
				ts_out_valid_d3 <= ts_out_valid_d2;
				ts_out_valid <= ts_out_valid_d3;

				ts_out_sync <= mpeg_sync_d3;
				ts_out <= mpeg_data_d3;
			end
			else begin
				if(pid_matched == 1) begin
					if((matched_index >= 0) && (matched_index < PACK_BYTE_SIZE)) begin
						ts_out_valid <= 1;

						ts_out_sync <= mpeg_sync_d3;
						ts_out <= ram_for_data[matched_index / 4][(8 * (matched_index % 4) + 7) -: 8];
						matched_index <= matched_index + 1;
					end
					else begin
						ts_out_valid <= 0;
					end
				end
				else begin
				end
			end

			if(mpeg_valid == 1) begin
				if(mpeg_sync_d2 == 1) begin
					if(mpeg_data_d2 == 8'h47) begin
						if(({mpeg_data_d1[5 - 1 : 0], mpeg_data} == pid[13 - 1 : 0]) && (run_enable == 1)) begin
							pid_matched <= 1;
							matched_index <= 0;
						end
						else begin
							pid_matched <= 0;
						end
					end
					else begin
					end
				end
				else begin
				end
			end
			else begin
			end

		end
	end

endmodule
