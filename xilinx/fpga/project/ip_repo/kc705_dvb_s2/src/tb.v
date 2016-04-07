`timescale 1ns / 1ns

module tb #(
		parameter integer C_S_AXI_DATA_WIDTH = 32
	)
	(
	);

	//localparam integer MPEG_LENGTH = 1316;
	localparam integer CRCEncoder_In_Bits_Len = 6040064;
	localparam integer PACK_BYTE_SIZE = 188;
	localparam integer PACK_WORD_SIZE = PACK_BYTE_SIZE / (C_S_AXI_DATA_WIDTH / 8);

	//reg [7:0] filter1[PACK_BYTE_SIZE - 1 : 0];
	//reg [7:0] filter2[PACK_BYTE_SIZE - 1 : 0];
	//reg[7:0] mpeg_in[MPEG_LENGTH - 1 : 0];
	reg CRCEncoder_In_Bits[CRCEncoder_In_Bits_Len - 1 : 0];

	wire clk;
	reg rst_n = 0;

	integer symbol_1x_out_sim_file_pointer;
	integer symbol_1x_out_sim_file_pointer_null;
	initial begin
		//$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/filter1.txt", filter1, 0);
		//$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/filter2.txt", filter2, 0);
		//$readmemh("/home/action/vivadoworkspace/ip_repo/axi4_tsp/src/ts4.txt", mpeg_in,0);
		$readmemb("../../../../testUseCase/Mode_0_0_6_0/anotherSimul/CRCEncoder_In.txt",CRCEncoder_In_Bits);
		symbol_1x_out_sim_file_pointer = $fopen("../../../../testUseCase/Mode_0_0_6_0/anotherSimul/symbol_1x_out_sim.txt", "w");
		symbol_1x_out_sim_file_pointer_null = $fopen("../../../../testUseCase/Mode_0_0_6_0/anotherSimul/symbol_1x_out_sim_null.txt", "w");
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


	wire fs_en_inner;
	wire fs_en2_inner;
	
	reg [7:0] mpeg_data = 0;
	wire mpeg_clk;
	reg mpeg_valid = 0;
	reg mpeg_sync = 0;

	reg [C_S_AXI_DATA_WIDTH-1 : 0] send_valid = 3;
	//always @(posedge mpeg_clk) begin
	//	if((send_valid >= 0) && (send_valid < 3)) begin
	//		send_valid <= send_valid + 1;
	//	end
	//	else begin
	//		send_valid <= 0;
	//	end
	//end

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
			if((ts_index >= 0) && (ts_index < (CRCEncoder_In_Bits_Len / 8)/*MPEG_LENGTH*/)) begin
				if((send_valid == 3) && (ts_clk == 1)) begin
					mpeg_valid <= 1;

					//mpeg_data <= mpeg_in[ts_index];
					mpeg_data <= {
							CRCEncoder_In_Bits[0 + (8 * ts_index)],
							CRCEncoder_In_Bits[1 + (8 * ts_index)],
							CRCEncoder_In_Bits[2 + (8 * ts_index)],
							CRCEncoder_In_Bits[3 + (8 * ts_index)],
							CRCEncoder_In_Bits[4 + (8 * ts_index)],
							CRCEncoder_In_Bits[5 + (8 * ts_index)],
							CRCEncoder_In_Bits[6 + (8 * ts_index)],
							CRCEncoder_In_Bits[7 + (8 * ts_index)]
						};
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
	//clkgen #(.clk_period(2)) xiaofeiclk2(.clk(mpeg_clk));
	assign mpeg_clk = clk;


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

	//always @(negedge clk)begin
	//	if(rst_n == 0) begin
	//	end
	//	else begin
	//		if((symbol_1x_oe == 1)) begin
	//			$fwrite(symbol_1x_out_sim_file_pointer, "%f %f \n", symbol_1x_re_out, symbol_1x_im_out);
	//		end
	//		else begin
	//		end
	//	end
	//	//$fclose(symbol_out_file_pointer);
	//end

	always @(negedge clk)begin
		if(rst_n == 0) begin
		end
		else begin
			if((dvb_s2_inst.dvb_s2_ram_inst.dvb_s2_system_top_inst.symbol_1x_oe == 1)) begin

				case({dvb_s2_inst.dvb_s2_ram_inst.dvb_s2_system_top_inst.symbol_1x_re_out, dvb_s2_inst.dvb_s2_ram_inst.dvb_s2_system_top_inst.symbol_1x_im_out})
					32'hE960E960: begin
						$fwrite(symbol_1x_out_sim_file_pointer, "%s\n", "-0.70710678118654757 - 0.70710678118654757j");
					end
					32'hE96016A0: begin
						$fwrite(symbol_1x_out_sim_file_pointer, "%s\n", "-0.70710678118654757 + 0.70710678118654757j");
					end
					32'h16A0E960: begin
						$fwrite(symbol_1x_out_sim_file_pointer, "%s\n", "0.70710678118654757 - 0.70710678118654757j");
					end
					32'h16A016A0: begin
						$fwrite(symbol_1x_out_sim_file_pointer, "%s\n", "0.70710678118654757 + 0.70710678118654757j");
					end
					default: begin
						$fwrite(symbol_1x_out_sim_file_pointer_null, "%04h%04h \n", dvb_s2_inst.dvb_s2_ram_inst.dvb_s2_system_top_inst.mapper_symbol_re_out, dvb_s2_inst.dvb_s2_ram_inst.dvb_s2_system_top_inst.mapper_symbol_im_out);
					end
				endcase
			end
			else begin
			end
		end
		//$fclose(symbol_out_file_pointer);
	end

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

		.fs_en_inner(fs_en_inner),
		.fs_en2_inner(fs_en2_inner),

		.symbol_1x_oe(symbol_1x_oe),
		.symbol_1x_re_out(symbol_1x_re_out),
		.symbol_1x_im_out(symbol_1x_im_out),

		.symbol_2x_oe(symbol_2x_oe),
		.symbol_2x_re_out(symbol_2x_re_out),
		.symbol_2x_im_out(symbol_2x_im_out)
	);

endmodule
