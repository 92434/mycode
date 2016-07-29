`timescale 1ns / 1ps
module crc8(
input	          							sys_clk,
input	          							glb_rst_n,
//////////////////////////////////////////////////////////////
input						[7:0]       	ts_in,// @ sys_clk
input                  					ts_syn,// @ sys_clk
input                  					ts_head,// @ sys_clk
//////////////////////////////////////////////////////////////
output  reg  								crc8_cync_en,
output  reg  								crc8_oe,
output  reg  			 [7:0]     		crc,
output  reg  			 [7:0]     		crc_byte_out
//////////////////////////////////////////////////////////////
);

parameter reg_init_val = 8'b00000000;

wire										 	tmp0;
wire										 	tmp1;
wire										 	tmp2;
wire										 	tmp3;
wire										 	tmp4;
wire										 	tmp5;
wire										 	tmp6;
wire										 	tmp7;

assign  tmp0 = crc[7]^crc[6]^crc[1]^crc[3]^crc[0]^ts_in[7]^ts_in[6]^ts_in[3]^ts_in[1]^ts_in[0];
assign  tmp1 = crc[7]^crc[1]^crc[4]^crc[2]^ts_in[7]^ts_in[4]^ts_in[2]^ts_in[1];
assign  tmp2 = crc[7]^crc[6]^crc[5]^crc[1]^crc[2]^crc[0]^ts_in[7]^ts_in[6]^ts_in[5]^ts_in[2]^ts_in[1]^ts_in[0];
assign  tmp3 = crc[7]^crc[6]^crc[1]^crc[3]^crc[2]^ts_in[7]^ts_in[6]^ts_in[3]^ts_in[2]^ts_in[1];
assign  tmp4 = crc[4]^crc[6]^crc[1]^crc[2]^crc[0]^ts_in[6]^ts_in[4]^ts_in[2]^ts_in[1]^ts_in[0];///
assign  tmp5 = crc[7]^crc[1]^crc[3]^crc[2]^crc[5]^ts_in[7]^ts_in[5]^ts_in[3]^ts_in[2]^ts_in[1];
assign  tmp6 = crc[7]^crc[4]^crc[1]^crc[2]^crc[0]^ts_in[7]^ts_in[4]^ts_in[2]^ts_in[1]^ts_in[0];
assign  tmp7 = crc[7]^crc[6]^crc[2]^crc[5]^crc[0]^ts_in[7]^ts_in[6]^ts_in[5]^ts_in[2]^ts_in[0];


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		crc8_oe <= 1'b0;
		crc8_cync_en <= 1'b0;
	end
	else begin
		crc8_cync_en <= ts_head;
		crc8_oe <= ts_syn;
	end
end


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		crc <= reg_init_val;
	end
	else if(ts_head)begin
		crc <= reg_init_val;
	end
	else if(ts_syn)begin
		crc <= {tmp7,tmp6,tmp5,tmp4,tmp3,tmp2,tmp1,tmp0};
	end
end


reg					first_ts_head;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		first_ts_head <= 1'b0;
	end
	else if(ts_head)begin
		first_ts_head <= 1'b1;
	end
end


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		crc_byte_out <= reg_init_val;
	end
	else if(ts_head)begin
		if(first_ts_head == 1'b1)begin
			crc_byte_out <= crc;
		end
		else begin
			crc_byte_out <= ts_in;
		end
	end
	else begin
		crc_byte_out <= ts_in;
	end
end




endmodule
