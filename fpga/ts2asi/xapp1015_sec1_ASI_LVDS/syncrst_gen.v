//------------------------------------------------------------------------------ 
// Copyright (c) 2006 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: syncrst_gen.v,v $
//  /   /        Date Last Modified:  $Date: 2007-06-07 10:18:32-06 $
// /___/   /\    Date Created: January 30, 2007 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: syncrst_gen.v,v $
// Revision 1.0  2007-06-07 10:18:32-06  hutching
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

/* Module Description
  This simple module takes as input a clock and the locked signal from a DCM.
  It generates a synchronous reset that is held asserted for 16 clock cycles 
  after the DCM asserts lock.  It also takes another synchronous reset input
  and ORs this with the internally generated one.
*/

module syncrst_gen (/*AUTOARG*/
   // Outputs
   rst,
   // Inputs
   clk, rst_in, dcm_locked
   ) ;

   input clk;
   input rst_in;
   input dcm_locked;

   output rst;

   // Internal registers
   reg 	  reset_delayed = 1'b1;
   reg [14:0] 	  locked_dly_line;
   
   // Continuous assignments
   assign 	  rst = ~reset_delayed | rst_in;
   

   // Simply use an SRL16 plus one register to provide the delay on locked
   always @(posedge clk)
      begin
	 locked_dly_line    <= locked_dly_line << 1;
	 locked_dly_line[0] <= dcm_locked;
	 reset_delayed <= locked_dly_line[14];
      end
   
   
endmodule // syncrst_gen
