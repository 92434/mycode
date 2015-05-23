//------------------------------------------------------------------------------ 
// Copyright (c) 2006 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: checker.v,v $
//  /   /        Date Last Modified:  $Date: 2007-06-06 13:26:08-06 $
// /___/   /\    Date Created: Dec 14, 2006 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: checker.v,v $
// Revision 1.0  2007-06-06 13:26:08-06  hutching
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

/* Module Description:  
   This module implements a DVB-ASI transport stream checker.  
*/

`timescale 1ns / 1ns

module checker (/*AUTOARG*/
   // Outputs
   checker_err,
   // Inputs
   user_clk, ce, fifo_empty, fifo_data, fifo_kchar, clear_err,
   asi_rx_err
   );


   input user_clk;
   input ce;
      input fifo_empty;     // Only used initially after a reset.
   input [7:0] fifo_data;   // 8-bit DVB-ASI data
   input       fifo_kchar;  // Bit indicating current word is comma char.
   input       clear_err;   // Clears error bit and re-initializes checker
   input       asi_rx_err;  // Indicates a code or disparity error occurred.

   output      checker_err; // Indicates packet data error or count error

   // Output registers
   reg 	       checker_err;

   // Internal registers
   reg [7:0]   fifo_data_R1 = 8'h00;   // Pipeline stage 
   reg [7:0]   fifo_chk_data = 8'h00;  // compare data synchronized to input
   reg 	       cnt_en = 1'b0;          // Enable for chk data counter.

   reg [9:0]   sop_wdt;        // Start of packet watchdog timer
   
   
   

   // Wire
   wire        sop; // Pulses high when input data byte equals packet header 8'h47
   
   
   // Continous assignments
   assign      sop = (fifo_data == 8'h47);  // Pulse for start of packet
   


   always @(posedge user_clk)
     begin
	if (clear_err)
	  begin
	     cnt_en        <= 1'b0;
	     checker_err   <= 1'b0;
	     fifo_chk_data <= 8'h00;
	     sop_wdt       <= 8'h00;
	  end
	else if (ce)
	  begin

	     // Watchdog timer process
	     if (sop)
	       sop_wdt <= 8'h00;   // Reset watchdog timer every sop
	     else
	       sop_wdt <= sop_wdt + 1;
	     
	     

	     // Here's the actual check against the data
	     checker_err <= ((fifo_data_R1 != (fifo_chk_data + 8'h47)) & cnt_en) || sop_wdt == 10'h200;  
	     	     
	     // counter enable to synchronize counter to input packet
	     if (sop)  // Wait for start of packet
	       cnt_en   <= 1'b1;

		  
	     else if (fifo_chk_data == 8'hBB)  // stop counting, wait for new sop
	       begin
		  cnt_en <= 1'b0;
		  fifo_chk_data <= 8'h00;
	       end
	     
             // Counter process.  
	     if (!fifo_kchar && cnt_en)
               fifo_chk_data <= (fifo_chk_data < 8'hBB) ? fifo_chk_data + 1'b1 : 8'h00;

	     // Register load process.  Only load non-comma data for comparison
	     if (!fifo_kchar)
	       fifo_data_R1 <= fifo_data;
	     
	  end // else: !if(clear_err)
     end // always @ (posedge user_clk)
   
   

endmodule // checker
