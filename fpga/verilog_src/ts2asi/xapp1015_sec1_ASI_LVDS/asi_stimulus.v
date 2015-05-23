//------------------------------------------------------------------------------ 
// Copyright (c) 2006 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: asi_stimulus.v,v $
//  /   /        Date Last Modified:  $Date: 2007-06-07 10:14:59-06 $
// /___/   /\    Date Created: Jan 24, 2007 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: asi_stimulus.v,v $
// Revision 1.0  2007-06-07 10:14:59-06  hutching
// Initial revision
//
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

/* This module implements a simple stimulus generator for testing purposes.  It
   generates simple synthetic MPEG-2 packets that consist of 188 bytes of data
   followed by two comma characters.  The pattern then repeats. 
*/

module asi_stimulus (/*AUTOARG*/
   // Outputs
   asi_dout, comma_out,
   // Inputs
   clk, ce, rst
   ) ;

   input clk;
   input ce;
   input rst;

   output [7:0] asi_dout;
   output 	comma_out;
   

   // Output registers
   reg [7:0] 	asi_dout = 8'h00;
   reg 		comma_out = 1'b0;
   
   

   // Internal registers
   reg [7:0] 	byte_cntr = 8'h00;
   

   always @(posedge clk)
     if (rst)
       begin
	  asi_dout  <= 8'h00;
	  byte_cntr <= 8'h00;
	  comma_out <= 1'b0;
       end
   
     else if (ce)
       begin
	  
          // asi_dout process:
	  // asi_dout gets 47h, 48h, 49h, ... 02h, BCh, BCh, 47h...
	  if (byte_cntr == 8'hBC || byte_cntr == 8'hBD)
	    begin
	       asi_dout <= 8'hBC;
	       comma_out <= 1'b1;
	    end
	  else
	    begin
	       asi_dout <= byte_cntr + 8'h47;
	       comma_out <= 1'b0;
	    end

	  // byte_cntr process  - 00, 01, 02, ... BC, 00, 01, 02...
	  if (byte_cntr <= 8'hBC)
	    byte_cntr <= byte_cntr + 1'b1;
	  else
	    byte_cntr <= 8'h00;
       end // else: !if(rst)
       
endmodule // asi_stimulus
