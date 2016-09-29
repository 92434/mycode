`include "../bench/timescale.v"

// this module do a iteration operation for 2 bit

module stream_iteration(clk, rst_n, init,in1,in2,
                                Ai,Bi,Di,Ei,Fi,Xi,Yi,Zi,pi,qi,ri,
                                Ao,Bo,Do,Eo,Fo,Xo,Yo,Zo,po,qo,ro,
                                op);
input clk;
input rst_n;
input    init;
input [3     :0] in1;
input [3     :0] in2;

input [10*4-1:0] Ai;
input [10*4-1:0] Bi;
input [3     :0] Di;
input [3     :0] Ei;
input [3     :0] Fi;
input [3     :0] Xi;
input [3     :0] Yi;
input [3     :0] Zi;
input            pi;
input            qi;
input            ri;

output [10*4-1:0] Ao;
output [10*4-1:0] Bo;
output [3     :0] Do;
output [3     :0] Eo;
output [3     :0] Fo;
output [3     :0] Xo;
output [3     :0] Yo;
output [3     :0] Zo;
output            po;
output            qo;
output            ro;

output[1     :0] op;

wire    init_reg;
wire [3     :0] in1_reg;
wire [3     :0] in2_reg;
wire [10*4-1:0] Ai_reg;
wire [10*4-1:0] Bi_reg;
wire [3     :0] Di_reg;
wire [3     :0] Ei_reg;
wire [3     :0] Fi_reg;
wire [3     :0] Xi_reg;
wire [3     :0] Yi_reg;
wire [3     :0] Zi_reg;
wire            pi_reg;
wire            qi_reg;
wire            ri_reg;
assign init_reg = init;
assign in1_reg = in1;
assign in2_reg = in2;
assign Ai_reg = Ai;
assign Bi_reg = Bi;
assign Di_reg = Di;
assign Ei_reg = Ei;
assign Fi_reg = Fi;
assign Xi_reg = Xi;
assign Yi_reg = Yi;
assign Zi_reg = Zi;
assign pi_reg = pi;
assign qi_reg = qi;
assign ri_reg = ri;
//always @(posedge clk) begin
//	if(rst_n == 0) begin
//		init_reg <= 0;
//		in1_reg <= 0;
//		in2_reg <= 0;
//		Ai_reg <= 0;
//		Bi_reg <= 0;
//		Di_reg <= 0;
//		Ei_reg <= 0;
//		Fi_reg <= 0;
//		Xi_reg <= 0;
//		Yi_reg <= 0;
//		Zi_reg <= 0;
//		pi_reg <= 0;
//		qi_reg <= 0;
//		ri_reg <= 0;
//	end
//	else begin
//		init_reg <= init;
//		in1_reg <= in1;
//		in2_reg <= in2;
//		Ai_reg <= Ai;
//		Bi_reg <= Bi;
//		Di_reg <= Di;
//		Ei_reg <= Ei;
//		Fi_reg <= Fi;
//		Xi_reg <= Xi;
//		Yi_reg <= Yi;
//		Zi_reg <= Zi;
//		pi_reg <= pi;
//		qi_reg <= qi;
//		ri_reg <= ri;
//	end
//end

wire [1:0] s1;
wire [1:0] s2;
wire [1:0] s3;
wire [1:0] s4;
wire [1:0] s5;
wire [1:0] s6;
wire [1:0] s7;

wire [3:0] extra_B;
wire [3:0] next_A1;
wire [3:0] _next_B1;
wire [3:0] next_B1;
wire [3:0] next_E;

wire [4:0] total;

sboxes b(
		.clk(clk),
		.rst_n(rst_n),
                .A(Ai_reg[9*4-1:0])
               ,.s1(s1)
               ,.s2(s2)
               ,.s3(s3)
               ,.s4(s4)
               ,.s5(s5)
               ,.s6(s6)
               ,.s7(s7)
        );

assign extra_B ={( Bi_reg[(3-1)*4+0] ^ Bi_reg[(6-1)*4+1] ^ Bi_reg[(7-1)*4+2] ^ Bi_reg[(9-1)*4+3]) ,
                 ( Bi_reg[(6-1)*4+0] ^ Bi_reg[(8-1)*4+1] ^ Bi_reg[(3-1)*4+3] ^ Bi_reg[(4-1)*4+2]) ,
                 ( Bi_reg[(5-1)*4+3] ^ Bi_reg[(8-1)*4+2] ^ Bi_reg[(4-1)*4+0] ^ Bi_reg[(5-1)*4+1]) ,
                 ( Bi_reg[(9-1)*4+2] ^ Bi_reg[(6-1)*4+3] ^ Bi_reg[(3-1)*4+1] ^ Bi_reg[(8-1)*4+0]) };

assign next_A1=(init_reg)?   Ai_reg[(10)*4-1:(10-1)*4] ^ Xi_reg ^ Di_reg ^ in2_reg
                        :Ai_reg[(10)*4-1:(10-1)*4] ^ Xi_reg;

assign _next_B1=(init_reg)?  Bi_reg[7*4-1:(7-1)*4] ^ Bi_reg[10*4-1:(10-1)*4] ^ Yi_reg ^ in1_reg
                        :Bi_reg[7*4-1:(7-1)*4] ^ Bi_reg[10*4-1:(10-1)*4] ^ Yi_reg ;

assign next_B1=(pi_reg)?{ _next_B1[2:0], _next_B1[3] }: _next_B1;

assign Do = Ei_reg ^ Zi_reg ^ extra_B;

assign next_E=Fi_reg;

assign total=Zi_reg+Ei_reg+ri_reg;

assign Fo=(qi_reg)? total[3:0]:Ei_reg;
assign ro=(qi_reg)? total[4]:ri_reg;

assign Eo=next_E;

assign Ao[10*4-1:4]=Ai_reg[9*4-1:0];
assign Ao[1*4-1:0] =next_A1;
assign Bo[10*4-1:4]=Bi_reg[9*4-1:0];
assign Bo[1*4-1:0] =next_B1;

assign Xo ={s4[0] , s3[0] , s2[1] , s1[1] };
assign Yo ={s6[0] , s5[0] , s4[1] , s3[1] };
assign Zo ={s2[0] , s1[0] , s6[1] , s5[1] };

assign po=s7[1];
assign qo=s7[0];

assign op = { Do[3] ^ Do[2], Do[1] ^ Do[0]  };
endmodule
