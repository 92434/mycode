module r_func(in,out);
input [55:0]in;
output [47:0]out;
wire [47:0]out;

diybox diy1(in[7:0]+in[55:48],out[7:0]);
diybox diy2(in[15:8]+in[55:48],out[15:8]);
diybox diy3(in[23:16]+in[55:48],out[23:16]);
diybox diy4(in[31:24]+in[55:48],out[31:24]);
diybox diy5(in[39:32]+in[55:48],out[39:32]);
diybox diy6(in[47:40]+in[55:48],out[47:40]);
endmodule
