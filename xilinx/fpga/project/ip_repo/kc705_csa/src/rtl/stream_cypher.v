`include "../bench/timescale.v"


// this file implement the stream cypher module

module stream_cypher(sb,ck,cb);
//input                 clk;
//input                 init;   // hi enable
//input                 en;      // hi enable
input  [8 *8-1:0]     ck;
input  [8 *8-1:0]     sb;
output [8 *8-1:0]     cb;



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
        ck[7*4-1:6*4],ck[8*4-1:7*4], 
        ck[5*4-1:4*4],ck[6*4-1:5*4], 
        ck[3*4-1:2*4],ck[4*4-1:3*4], 
        ck[1*4-1:0*4],ck[2*4-1:1*4] 
};

assign Binit = { 
                   4'b0,           4'b0,
        ck[15*4-1:14*4],ck[16*4-1:15*4], 
        ck[13*4-1:12*4],ck[14*4-1:13*4], 
        ck[11*4-1:10*4],ck[12*4-1:11*4], 
        ck[ 9*4-1: 8*4],ck[10*4-1: 9*4]
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
                        .init(1)
                       ,.sb(sb)
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
                        .init(0)
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
        assign cb=cbo;

endmodule
