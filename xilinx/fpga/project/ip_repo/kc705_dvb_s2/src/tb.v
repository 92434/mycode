`timescale 1ns / 1ns

module tb #(
		parameter integer C_S_AXI_DATA_WIDTH = 32
	)
	(
	);

	localparam integer MPEG_LENGTH = 1316;
	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	reg [7:0] filter1[PACK_BYTE_SIZE - 1 : 0];
	reg [7:0] filter2[PACK_BYTE_SIZE - 1 : 0];
	reg[7:0] mpeg_in[MPEG_LENGTH - 1 : 0];

	wire clk;
	reg rst_n = 0;

	initial begin
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/filter1.txt", filter1, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/filter2.txt", filter2, 0);
		$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/ts4.txt", mpeg_in,0);
		#2;
		rst_n = 1;
	end

	reg fs_en2_outer = 0;
	integer delay = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			fs_en2_outer <= 0;
			delay <= 0;
		end
		else begin
			//fs_en2_outer <= ~fs_en2_outer;
			fs_en2_outer <= 0;
			if(delay >= 4 - 1) begin
				delay <= 0;
			end
			else begin
				delay <= delay + 1;
			end
		end
	end

	wire fs_en_outer;
	//assign fs_en_outer = (delay == 1) ? fs_en2_outer : 0;
	assign fs_en_outer = 0;
	
	reg [7:0] mpeg_data = 0;
	wire mpeg_clk;
	reg mpeg_valid = 0;
	reg mpeg_sync = 0;

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

	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));
	clkgen #(.clk_period(2)) xiaofeiclk2(.clk(mpeg_clk));


	wire ts_clk;
	wire [7 : 0] ts_din;// @ sys_clk
	wire ts_syn;// @ sys_clk
	wire ts_head;// @ sys_clk

	wire symbol_1x_oe;
	wire signed [15 : 0] symbol_1x_re_out;
	wire signed [15 : 0] symbol_1x_im_out;

	wire symbol_2x_oe;
	wire signed [15 : 0] symbol_2x_re_out;
	wire signed [15 : 0] symbol_2x_im_out;

	dvb_s2_wrap #() dvb_s2_inst (
		.hard_rst_n(rst_n),

		.ts_clk_h264out(mpeg_clk),// clock from h.264 encoder
		.ts_valid_h264out(mpeg_valid),// @ ts_clk_out
		.ts_syn_h264out(mpeg_sync),// @ ts_clk_out ts stream head
		.ts_din_h264out(mpeg_data),// @ ts_clk_out

		.sys_clk(clk),
		.ts_clk(ts_clk),// @ sys_clk
		.fs_en_outer(fs_en_outer),
		.fs_en2_outer(fs_en2_outer),

		.ts_din(ts_din),// @ sys_clk
		.ts_syn(ts_syn),// @ sys_clk
		.ts_head(ts_head),// @ sys_clk

		.symbol_1x_oe(symbol_1x_oe),
		.symbol_1x_re_out(symbol_1x_re_out),
		.symbol_1x_im_out(symbol_1x_im_out),

		.symbol_2x_oe(symbol_2x_oe),
		.symbol_2x_re_out(symbol_2x_re_out),
		.symbol_2x_im_out(symbol_2x_im_out)
	);
endmodule
