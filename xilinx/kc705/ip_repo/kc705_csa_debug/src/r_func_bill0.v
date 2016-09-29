module rfunc_bill(in,out);
input [55:0]in;
output [47:0] out;
reg [47:0] out;

wire[7:0] ddd1; 
assign ddd1=in[55:48]&8'h07;

always @(in)
    case (in[55:48]&8'h07)          // synthesis full_case
        8'h00:
		begin
		out[47:0]=in[47:0];
		end
        8'h01:
		begin
		out[47:0]=in[55:8]^(in[7:0]);
		end
        8'h02:
		begin
		begin
		out[47:0]=in[47:0]^(in[7:0]<<8);
		end
		end
        8'h03:
		begin
		out[47:0]=in[47:0]^(in[7:0]<<16);
		end
        8'h04:
		begin
		out[47:0]=in[47:0]^(in[7:0]<<24);
		end
        8'h05:
		begin
		out[47:0]=in[47:0]^(in[7:0]<<32);
		end
        8'h06:
		begin
		out[47:0]=in[47:0]^(in[7:0]<<40);
		end
        8'h07:
		begin
		out[47:0]=in[55:8];
		end
     
        endcase
endmodule
