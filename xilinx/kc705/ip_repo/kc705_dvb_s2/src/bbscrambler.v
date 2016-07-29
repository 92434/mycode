`timescale 1ns / 1ps
module bbscrambler(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
input											bbscrambler_en,
//////////////////////////////////////////////////////////////
input	          							init_vld,
input	          							frame_vld,
input   			 		[7:0]     		byte_in,
//////////////////////////////////////////////////////////////
output  reg  								oe,
output  reg  			   	[7:0]     		byte_out
//////////////////////////////////////////////////////////////
);

parameter reg_init_val = 15'b100101010000000;

//parameter reg_init_val = 15'b000000010101001;

reg											frame_vld_1dly_reg;
reg							[7:0]			byte_in_1dly_reg;
reg							[14:0]			scramble_reg;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		scramble_reg            <= 15'b000000000000000;
	end
	else if(fs_en == 1'b1)begin
		if(init_vld == 1'b1)begin
			scramble_reg            <= reg_init_val;
		end
		else if(frame_vld_1dly_reg == 1'b1)begin
			scramble_reg			<= {scramble_reg[8]^scramble_reg[7],scramble_reg[7]^scramble_reg[6],scramble_reg[6]^scramble_reg[5],scramble_reg[5]^scramble_reg[4],scramble_reg[4]^scramble_reg[3],scramble_reg[3]^scramble_reg[2],scramble_reg[2]^scramble_reg[1],scramble_reg[1]^scramble_reg[0],scramble_reg[14:8]};
		end
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_in_1dly_reg <= 8'b00000000;
		frame_vld_1dly_reg <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		byte_in_1dly_reg <= byte_in;
		frame_vld_1dly_reg <= frame_vld;
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_out <= 8'b00000000;
		oe <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if(bbscrambler_en) begin
			byte_out <= {scramble_reg[1]^scramble_reg[0]^byte_in_1dly_reg[7],scramble_reg[2]^scramble_reg[1]^byte_in_1dly_reg[6],scramble_reg[3]^scramble_reg[2]^byte_in_1dly_reg[5],scramble_reg[4]^scramble_reg[3]^byte_in_1dly_reg[4],scramble_reg[5]^scramble_reg[4]^byte_in_1dly_reg[3],scramble_reg[6]^scramble_reg[5]^byte_in_1dly_reg[2],scramble_reg[7]^scramble_reg[6]^byte_in_1dly_reg[1],scramble_reg[8]^scramble_reg[7]^byte_in_1dly_reg[0]};
			oe <= frame_vld_1dly_reg;
		end
		else begin
			byte_out <= byte_in_1dly_reg;
			oe <= frame_vld_1dly_reg;
		end
	end
	else begin
	end
end


endmodule
