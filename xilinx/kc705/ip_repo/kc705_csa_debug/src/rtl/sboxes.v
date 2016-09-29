`include "../bench/timescale.v"

//this moudule preform the s-boxes transform

module sboxes(clk, rst_n, A, s1, s2, s3, s4, s5, s6, s7);
input clk;
input rst_n;
input [9*4-1:0] A;

output [2-1:0] s1;
output [2-1:0] s2;
output [2-1:0] s3;
output [2-1:0] s4;
output [2-1:0] s5;
output [2-1:0] s6;
output [2-1:0] s7;


wire [9*4-1:0] A_reg;
assign A_reg = A;
//always @(posedge clk) begin
//	if(rst_n == 0) begin
//		A_reg <= 0;
//	end
//	else begin
//		A_reg <= A;
//	end
//end

sbox1 b1({A_reg[(4-1)*4+0], A_reg[(1-1)*4+2], A_reg[(6-1)*4+1], A_reg[(7-1)*4+3], A_reg[(9-1)*4+0]}, s1);
sbox2 b2({A_reg[(2-1)*4+1], A_reg[(3-1)*4+2], A_reg[(6-1)*4+3], A_reg[(7-1)*4+0], A_reg[(9-1)*4+1]}, s2);
sbox3 b3({A_reg[(1-1)*4+3], A_reg[(2-1)*4+0], A_reg[(5-1)*4+1], A_reg[(5-1)*4+3], A_reg[(6-1)*4+2]}, s3);
sbox4 b4({A_reg[(3-1)*4+3], A_reg[(1-1)*4+1], A_reg[(2-1)*4+3], A_reg[(4-1)*4+2], A_reg[(8-1)*4+0]}, s4);
sbox5 b5({A_reg[(5-1)*4+2], A_reg[(4-1)*4+3], A_reg[(6-1)*4+0], A_reg[(8-1)*4+1], A_reg[(9-1)*4+2]}, s5);
sbox6 b6({A_reg[(3-1)*4+1], A_reg[(4-1)*4+1], A_reg[(5-1)*4+0], A_reg[(7-1)*4+2], A_reg[(9-1)*4+3]}, s6);
sbox7 b7({A_reg[(2-1)*4+2], A_reg[(3-1)*4+0], A_reg[(7-1)*4+1], A_reg[(8-1)*4+2], A_reg[(8-1)*4+3]}, s7);

endmodule
