`include "../bench/timescale.v"


// this file implement the stream cypher module

module stream_cypher(clk, rst_n, sb,ck,cb);
//input                 clk;
//input                 init;   // hi enable
//input                 en;      // hi enable
input clk;
input rst_n;
input  [8 *8-1:0]     ck;
input  [8 *8-1:0]     sb;
output [8 *8-1:0]     cb;

reg [8 *8-1:0] ck_reg;
reg [8 *8-1:0] sb_reg;
//assign ck_reg = ck;
//assign sb_reg = sb;
always @(posedge clk) begin
	if(rst_n == 0) begin
		ck_reg <= 0;
		sb_reg <= 0;
	end
	else begin
		ck_reg <= ck;
		sb_reg <= sb;
	end
end


// intermediate variable
wire    [10*4-1 : 0]A;
wire    [10*4-1 : 0]B;
wire    [4-1    : 0]X;
wire    [4-1    : 0]Y;
wire    [4-1    : 0]Z;
wire    [4-1    : 0]D;
wire    [4-1    : 0]E;
wire    [4-1    : 0]F;
wire                p;
wire                q;
wire                r;

wire   [10*4-1 : 0]Ao;
wire   [10*4-1 : 0]Ainit;
wire   [10*4-1 : 0]Bo;
wire   [10*4-1 : 0]Binit;
wire   [4-1    : 0]Xo;
wire   [4-1    : 0]Yo;
wire   [4-1    : 0]Zo;
wire   [4-1    : 0]Do;
wire   [4-1    : 0]Eo;
wire   [4-1    : 0]Fo;
wire               po;
wire               qo;
wire               ro;
wire   [8 *8-1 : 0]cbo;

//reg init0,init1;
assign Ainit = { 
                4'b0,         4'b0,
        ck_reg[7*4-1:6*4],ck_reg[8*4-1:7*4], 
        ck_reg[5*4-1:4*4],ck_reg[6*4-1:5*4], 
        ck_reg[3*4-1:2*4],ck_reg[4*4-1:3*4], 
        ck_reg[1*4-1:0*4],ck_reg[2*4-1:1*4] 
};

assign Binit = { 
                   4'b0,           4'b0,
        ck_reg[15*4-1:14*4],ck_reg[16*4-1:15*4], 
        ck_reg[13*4-1:12*4],ck_reg[14*4-1:13*4], 
        ck_reg[11*4-1:10*4],ck_reg[12*4-1:11*4], 
        ck_reg[ 9*4-1: 8*4],ck_reg[10*4-1: 9*4]
};
/*
always @(posedge clk)
begin
        if(init) begin
          init0=0;
          init1=1;
          sb =64'hE613DB6DC11C4524;//constant header 8B
        end
end
*/
stream_8bytes stream_8bytes_init(
                         .clk(clk),
                         .rst_n(rst_n),
                        .init(1'b1)
                       ,.sb(sb_reg)
                       ,.Ai(Ainit)
                       ,.Bi(Binit)
                       ,.Di(4'b0)
                       ,.Ei(4'b0)
                       ,.Fi(4'b0)
                       ,.Xi(4'b0)
                       ,.Yi(4'b0)
                       ,.Zi(4'b0)
                       ,.pi(1'b0)
                       ,.qi(1'b0)
                       ,.ri(1'b0)

                       ,.Ao(A)
                       ,.Bo(B)
                       ,.Do(D)
                       ,.Eo(E)
                       ,.Fo(F)
                       ,.Xo(X)
                       ,.Yo(Y)
                       ,.Zo(Z)
                       ,.po(p)
                       ,.qo(q)
                       ,.ro(r)
                       ,.cb()
                );
stream_8bytes stream_8bytes_calc(
                         .clk(clk),
                         .rst_n(rst_n),
                        .init(1'b0)
                       ,.sb()
                       ,.Ai(A)
                       ,.Bi(B)
                       ,.Di(D)
                       ,.Ei(E)
                       ,.Fi(F)
                       ,.Xi(X)
                       ,.Yi(Y)
                       ,.Zi(Z)
                       ,.pi(p)
                       ,.qi(q)
                       ,.ri(r)

                       ,.Ao(Ao)
                       ,.Bo(Bo)
                       ,.Do(Do)
                       ,.Eo(Eo)
                       ,.Fo(Fo)
                       ,.Xo(Xo)
                       ,.Yo(Yo)
                       ,.Zo(Zo)
                       ,.po(po)
                       ,.qo(qo)
                       ,.ro(ro)
                       ,.cb(cbo)
                );
        assign cb=cbo ^ 64'h00a5d7b0d65e93b5;

endmodule
