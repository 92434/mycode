//------------------------------------------------------------------------------ 
// Copyright (c) 2007 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: txdata_path.v,v $
//  /   /        Date Last Modified:  $Date: 2008-04-11 10:22:55-06 $
// /___/   /\    Date Created: Jan 12, 2007 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: txdata_path.v,v $
// Revision 1.1  2008-04-11 10:22:55-06  hutching
// Removed word-rate clock
//
// Revision 1.0  2007-06-15 11:02:31-06  hutching
// Initial revision
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

/* Module Description:
  This module implements the complete data path for the DVB ASI transmitter for SP3E
 */

module txdata_path (/*AUTOARG*/
   // Outputs
   asi_out,
   // Inputs
   clk, ce, rst, din_8b, kchar_in, force_disp
   ) ;

   input clk;                 // half-bit rate  clock, 
   input ce;                  // Clock enable for parallel domain
   input rst;                 // Synchronous reset

   input [7:0] din_8b;         // 8-bit input data to be encoded, serialized, transmitted
   input       kchar_in;       // Flag asserted means 8-bit input word is k28.5 comma char
   input       force_disp;     // unused
   

   output      asi_out;        // 1-bit serialized output data.  8b10b encoded.
   

   
   // Internal signals
   wire [9:0]  data_enc10b;

   // Internal registers
   reg [7:0]   din_8b_R   = 8'h00;
   reg 	       kchar_in_R = 1'b0;
   reg 	       force_disp_R = 1'b0;
   

   // Synchronous process
   always @(posedge clk)
     begin
	if (ce)
	  begin
	     din_8b_R     <= din_8b;
	     kchar_in_R   <= kchar_in;
	     force_disp_R <= force_disp;
	  end
     end
   

   // 8b10b Encoder
   encoder_8b10b encoder_inst(.din(din_8b_R),
			      .kin(kchar_in_R),
			      .clk(clk),
			      .ce(ce),
			      .dout(data_enc10b),
			      .valid(),
			      .code_err());
   
   
   // Instantiate the 10:1 serializer
   serializer_10b1b asi_serializer(.sclk_0(clk),
				   .sclk_180(~clk),
				   .ce(ce),
				   .reset(rst),
				   .din_10b(data_enc10b),
				   .sdout(asi_out));
   
endmodule // txdata_path