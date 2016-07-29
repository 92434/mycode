`timescale 1ns / 1ps
module randn_input(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	       								ts_rd_head,            // align with start_en_4dly  
input	       								ts_rd_vld,             // align with start_en_4dly
output 				reg					oe_head,
output 				reg					oe,
output 				reg		[7:0]		symbol_out
);

reg								[7:0]		ts_cnt;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ts_cnt <= 8'h00;
	end
	else if(fs_en == 1'b1)begin
		if(ts_rd_vld == 1'b1) begin
			if(ts_cnt == 8'd187)begin
				ts_cnt <= 8'h00;
			end
			else begin
				ts_cnt <= ts_cnt + 8'h01;
			end
		end
		else begin
			ts_cnt <= 8'h00;
		end
	end
	else begin
	end
end

reg								[7:0]		symbol_out_tmp;//1dly

always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_out_tmp <= 8'h00;
	end
	else if(fs_en == 1'b1)begin
		if((ts_rd_vld == 1'b1)&&(ts_cnt == 8'd0)) begin
			symbol_out_tmp <= 8'h47;
		end
		else begin
			symbol_out_tmp <= ts_cnt;
		end
	end
	else begin
	end
end

reg								[7:0]		symbol_out_tmp_1dly;//2dly
reg								[7:0]		symbol_out_tmp_2dly;//3dly
reg								[7:0]		symbol_out_tmp_3dly;//4dly
reg								[7:0]		symbol_out_tmp_4dly;//5dly
reg								[7:0]		symbol_out_tmp_5dly;//6dly
reg								[7:0]		symbol_out_tmp_6dly;//7dly
reg								[7:0]		symbol_out_tmp_7dly;//8dly
reg								[7:0]		symbol_out_tmp_8dly;//9dly


reg											ts_rd_head_1dly;//1dly
reg											ts_rd_head_2dly;//2dly
reg											ts_rd_head_3dly;//3dly
reg											ts_rd_head_4dly;//4dly
reg											ts_rd_head_5dly;//5dly
reg											ts_rd_head_6dly;//6dly
reg											ts_rd_head_7dly;//7dly
reg											ts_rd_head_8dly;//8dly
reg											ts_rd_head_9dly;//9dly

reg											ts_rd_vld_1dly;//1dly
reg											ts_rd_vld_2dly;//2dly
reg											ts_rd_vld_3dly;//3dly
reg											ts_rd_vld_4dly;//4dly
reg											ts_rd_vld_5dly;//5dly
reg											ts_rd_vld_6dly;//6dly
reg											ts_rd_vld_7dly;//7dly
reg											ts_rd_vld_8dly;//8dly
reg											ts_rd_vld_9dly;//9dly

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ts_rd_head_1dly <= 1'b0;//1dly
		ts_rd_head_2dly <= 1'b0;//2dly
		ts_rd_head_3dly <= 1'b0;//3dly
		ts_rd_head_4dly <= 1'b0;//4dly
		ts_rd_head_5dly <= 1'b0;//5dly
		ts_rd_head_6dly <= 1'b0;//6dly
		ts_rd_head_7dly <= 1'b0;//7dly
		ts_rd_head_8dly <= 1'b0;//8dly
		ts_rd_head_9dly <= 1'b0;//9dly
		
		ts_rd_vld_1dly <= 1'b0;//1dly
		ts_rd_vld_2dly <= 1'b0;//2dly
		ts_rd_vld_3dly <= 1'b0;//3dly
		ts_rd_vld_4dly <= 1'b0;//4dly
		ts_rd_vld_5dly <= 1'b0;//5dly
		ts_rd_vld_6dly <= 1'b0;//6dly
		ts_rd_vld_7dly <= 1'b0;//7dly
		ts_rd_vld_8dly <= 1'b0;//8dly
		ts_rd_vld_9dly <= 1'b0;//9dly
		
		symbol_out_tmp_1dly <= 8'h00;//2dly
		symbol_out_tmp_2dly <= 8'h00;//3dly
		symbol_out_tmp_3dly <= 8'h00;//4dly
		symbol_out_tmp_4dly <= 8'h00;//5dly
		symbol_out_tmp_5dly <= 8'h00;//6dly
		symbol_out_tmp_6dly <= 8'h00;//7dly
		symbol_out_tmp_7dly <= 8'h00;//8dly
		symbol_out_tmp_8dly <= 8'h00;//9dly
	end
	else if(fs_en == 1'b1)begin
		ts_rd_head_1dly <= ts_rd_head;//1dly
		ts_rd_head_2dly <= ts_rd_head_1dly;//2dly
		ts_rd_head_3dly <= ts_rd_head_2dly;//3dly
		ts_rd_head_4dly <= ts_rd_head_3dly;//4dly
		ts_rd_head_5dly <= ts_rd_head_4dly;//5dly
		ts_rd_head_6dly <= ts_rd_head_5dly;//6dly
		ts_rd_head_7dly <= ts_rd_head_6dly;//7dly
		ts_rd_head_8dly <= ts_rd_head_7dly;//8dly
		ts_rd_head_9dly <= ts_rd_head_8dly;//9dly
		
		ts_rd_vld_1dly <= ts_rd_vld;//1dly
		ts_rd_vld_2dly <= ts_rd_vld_1dly;//2dly
		ts_rd_vld_3dly <= ts_rd_vld_2dly;//3dly
		ts_rd_vld_4dly <= ts_rd_vld_3dly;//4dly
		ts_rd_vld_5dly <= ts_rd_vld_4dly;//5dly
		ts_rd_vld_6dly <= ts_rd_vld_5dly;//6dly
		ts_rd_vld_7dly <= ts_rd_vld_6dly;//7dly
		ts_rd_vld_8dly <= ts_rd_vld_7dly;//8dly
		ts_rd_vld_9dly <= ts_rd_vld_8dly;//9dly
		
		symbol_out_tmp_1dly <= symbol_out_tmp;//2dly
		symbol_out_tmp_2dly <= symbol_out_tmp_1dly;//3dly
		symbol_out_tmp_3dly <= symbol_out_tmp_2dly;//4dly
		symbol_out_tmp_4dly <= symbol_out_tmp_3dly;//5dly
		symbol_out_tmp_5dly <= symbol_out_tmp_4dly;//6dly
		symbol_out_tmp_6dly <= symbol_out_tmp_5dly;//7dly
		symbol_out_tmp_7dly <= symbol_out_tmp_6dly;//8dly
		symbol_out_tmp_8dly <= symbol_out_tmp_7dly;//9dly
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		oe_head <= 1'b0;
		oe <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		oe_head <= ts_rd_head_9dly;
		oe <= ts_rd_vld_9dly;
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_out <= 8'h00;
	end
	else if(fs_en == 1'b1)begin
		symbol_out <= symbol_out_tmp_8dly;
	end
	else begin
	end
end


endmodule
