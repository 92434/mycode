//------------------------------------------------------------------------------ 
// Copyright (c) 2006 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: des10.v,v $
//  /   /        Date Last Modified:  $Date: 2007-06-12 14:20:25-06 $
// /___/   /\    Date Created: Dec 11, 2006 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: des10.v,v $
// Revision 1.0  2007-06-12 14:20:25-06  hutching
// Initial revision
//
//
//------------------------------------------------------------------------------ 
//
//     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
//     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
//     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
//     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
//     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
//     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
//     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
//     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
//     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
//     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
//     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
//     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//     FOR A PARTICULAR PURPOSE.
//
//------------------------------------------------------------------------------ 

/* Verilog Port of Catalin's deserializer */

module des10 (/*AUTOARG*/
   // Outputs
   dout, vo,
   // Inputs
   clk270, di, dv, clk54, bitslip
   );


   input clk270;
   input [1:0] di;
   input [1:0] dv;
   input clk54;
   input bitslip;

   output [9:0] dout;
   output 	vo;

   reg [9:0] 	dout;
   reg 		vo;
   
   

   // Internal Signals
   reg [10:0] sh = 11'b00000000000;
   reg [9:0]  d  = 10'b0000000000;
   wire [9:0]  d1;
   reg 	      v  = 1'b0;
   wire	      v1;
   reg	      vd;
   reg 	      bitslip1d = 1'b0;
   reg 	      bs = 1'b0;
   reg 	      qb = 1'b0;
   reg e = 1'b0;
   reg [4:0]  qh = 5'b00000;


   always @(posedge clk270) begin
	
      if (dv[0]== 1'b1 && bs == 1'b0)
	qb <= ~qb;
      
      if ((qb == 1'b1 && dv[0] == 1'b1 && bs == 1'b0) || dv[1] == 1'b1) begin
         if (qh[3:0] == 4'b0000)
           qh[0] <= 1'b1;
         else
           qh[0] <= 1'b0;
         if (qh[4] == 1'B0)
           qh[4:1] <= qh[3:0];
         else
           qh[4:1] <= 4'b0000;
      end
      
      if ((dv[0] == 1'b1 && bs == 1'b0) || dv[1] == 1'b1) begin
        if (dv[1] == 1'b0)
          sh <= {sh[9:0],di[0]};  // Shift by one bit
        else
          sh <= {sh[8:0],di[1:0]}; // Shift by two bits
      end
      
      if (qh[4] == 1'b1 && ((qb == 1'b1 && dv[0] == 1'b1 && bs == 1'b0) || dv[1] == 1'b1))
        e <= 1'b1;
      else
        e <= 1'b0;
   
      if (e == 1'b1)
        v <= ~v;

      if (e == 1'b1) begin
        if (qb == 1'b0)
          d <= sh[9:0];
        else
          d <= sh[10:1];
      end 

      bitslip1d <= bitslip;

      if (bitslip != bitslip1d) 
        bs <= 1'b1;
      else if (dv == 2'b01)
        bs <= 1'b0;
      
   end // always @ (posedge clk270)
   

   assign d1 = d;
   assign v1 = v;

   always @(posedge clk54)  begin
      vd   <=v1;
      dout <=d1;
      vo   <=v1 ^ vd;
   end

endmodule // des10
