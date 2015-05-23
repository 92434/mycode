//------------------------------------------------------------------------------ 
// Copyright (c) 2006 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: rxdata_path.v,v $
//  /   /        Date Last Modified:  $Date: 2008-04-11 10:24:57-06 $
// /___/   /\    Date Created: Dec 11, 2006 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: rxdata_path.v,v $
// Revision 1.2  2008-04-11 10:24:57-06  hutching
// Added resets to framer, link controller and FIFO
//
// Revision 1.1  2007-06-21 12:25:20-06  hutching
// Modifications for new link controller functionality
//
// Revision 1.0  2007-06-12 14:22:37-06  hutching
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

/* Module Description
 
  This module implements a complete ASI receiver for Spartan3E.  It is mostly
  instantiations of lower level modules with some glue.  Use this module to
  instantiate a complete ASI receiver including the rate matching FIFO.
 
 */

`timescale 1ns / 1ns

module rxdata_path (/*AUTOARG*/
   // Outputs
   dout_8b, kchar_out, fifo_empty, fifo_full, code_err, disp_err,
   framed_asi,
   // Inputs
   clk_270_4phase, clk_54, user_clk, reset, sdata_in, rx_fifo_rden,
   reframe_req, sync_mode
   );

   // Clock inputs
   input [3:0] clk_270_4phase;     // 270MHz 0, 45, 90, 135-degree phase clocks for DRU
   input clk_54;                   // 54MHz parallel domain clock
   input user_clk;                 // User clock for read side of packet FIFO
   
   


   // Other Inputs
   input reset;        // Active high reset  
   input sdata_in;     // Serial input data
   input rx_fifo_rden; // User read enable for rx packet FIFO
   input reframe_req;      // Reframe signal
   input sync_mode;
   
   
   // Outputs
   output [7:0] dout_8b; // Decoded 8-bit word output  
   output kchar_out;     // Status signal indicating if dout_8b is comma character
   output fifo_empty;    // Empty flag from rx packet FIFO - testing only
   output fifo_full;     // Full flag from FIFO - testing only
   output code_err;      // Indicates code error in 8b/10b decoder  
   output disp_err;      // Indicates disparity error in 8b/10b decoder
   output framed_asi;    // Indicates DVB-ASI framing sequence detected

   //
   // Internal Signals
   //
   wire [7:0] data8b;
   wire is_kchar;
   wire code_err;
   wire disp_err;
   wire framed_asi;
   wire [9:0] pdata10b;         // 10-bit parallel output of data recovery unit.
   wire [9:0] pdata10b_inv;     // Inverted 10-bit parallel output of data recovery unit. 
   wire [9:0] pdata10b_framed;  // ASI framed parallel data.  Input to 8b10b decoder.
   wire [9:0] pdata10b_framed_inv;
   
   
   wire pdata10b_en;      // from deserializer -  valid 10-bit parallel word available
   wire decoder_code_err; // Indicates code error in 8b/10b decoder   
   wire decoder_disp_err; // Indicates disparity error in 8b/10b decoder 
   wire decoder_init; 
   wire error_condition;   // Indicates disparity error in 8b/10b decoder 
   wire newdata;           // Indicates new data is available on output of 8b/10b decoder
   wire reframe;
   

   wire [1:0] dru_dout;    // Data from DRU to deserializer
   wire [1:0] dru_dv_out;  // Data valid signal from DRU to deserializer
   

   // Continous assignments
   assign     decoder_init = reframe;
   assign code_err = decoder_code_err ;
   assign disp_err = decoder_disp_err ;
   assign error_condition = decoder_code_err | decoder_disp_err ;

     // Invert the bit ordering of the 10-bit word output of the deserializer for input 
     // to the 8b/10b decoder.
     // JLH: Note this may not be necessary depending on Catalin's deserializer.
     //
   assign pdata10b_inv = {pdata10b[0], 
                          pdata10b[1], 
                          pdata10b[2], 
                          pdata10b[3], 
                          pdata10b[4], 
                          pdata10b[5], 
                          pdata10b[6], 
                          pdata10b[7], 
                          pdata10b[8], 
                          pdata10b[9]} ;

   // The framer frames things inverted for my 8b10b decoder 
   assign pdata10b_framed_inv = {pdata10b_framed[0],
				 pdata10b_framed[1],
				 pdata10b_framed[2],
				 pdata10b_framed[3],
				 pdata10b_framed[4],
				 pdata10b_framed[5],
				 pdata10b_framed[6],
				 pdata10b_framed[7],
				 pdata10b_framed[8],
				 pdata10b_framed[9]};
   

   //DRU (Data Recovery Unit) DRU written in VHDL only at this time.
   dru dru_8phase (.dout(dru_dout), 
		   .dv(dru_dv_out),
		   .clk270(clk_270_4phase),
                   .si(sdata_in));

   // Deserializer - written in VHDL presently
   des10 deserializer(.dout(pdata10b),
		      .vo(pdata10b_en),
		      .clk270(clk_270_4phase[0]),
		      .di(dru_dout),
		      .dv(dru_dv_out),
		      .clk54(clk_54),
		      .bitslip(1'b0));

   // The framer handles syncing to DVB ASI comma sequences and aligning data
   asi_par_framer asi_framer(.sclk(clk_54),
			     .rst(reset),
			     .ce(pdata10b_en),
			     .d(pdata10b_inv),
			     .frame_en(1'b1),
			     .reframe(reframe),
			     .sync_mode(sync_mode),
			     .q(pdata10b_framed),
			     .framed(framed_asi),
			     .nsp());
			     
   // 8b10b decoder
   decoder_8b10b decoder10b8bunit (
         .clk(clk_54), 
	 .d_10b(pdata10b_framed_inv), 
	 .q_8b(data8b), 
	 .is_kcode(is_kchar), 
	 .ce(pdata10b_en), 
	 .sinit(decoder_init), 
	 .code_err(decoder_code_err), 
	 .disp_err(decoder_disp_err), 
	 .q_valid(newdata));

   // Rate Matching FIFO
   dvb_fifo rx_packet_fifo(.fifo_dout_8b(dout_8b),
			   .fifo_kcode_out(kchar_out),
			   .rd_count(),
			   .fifo_empty(fifo_empty),
			   .fifo_full(fifo_full),
			   .fifo_rden(rx_fifo_rden),
			   .clk(clk_54),
			   .user_clk(user_clk),
			   .data8b_in(data8b),
			   .data8b_valid(newdata),
			   .framed(framed_asi),
			   .reset(reset),
			   .is_kchar(is_kchar));


   // Instantiate the link monitor state machine.
   link_ctl link_mon(.clk(clk_54),
		     .ce(newdata),
		     .reset(reset),
		     .reframe_req(reframe_req),
		     .disp_err(decoder_disp_err),
		     .code_err(decoder_code_err),
		     .framed(framed_asi),
		     .reframe(reframe));
   
   

endmodule
