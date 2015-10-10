module sim_ts_to_asi(clk,rst,din,dout);
//parameter PACK_BYTE_SIZE = 8'hBC;
output[7:0] dout;
input [7:0] din;
input clk,rst;

//internal reg

reg[7:0] ts_8b;
reg[7:0] dout_8b;

initial
  begin
    ts_8b<=0;
    dout_8b<=0;
  end
  
  assign dout = dout_8b;
  
  always @(posedge clk)
    begin
      dout_8b=din;
      $display("ts_to_asi get %h", dout_8b);
    end
endmodule

