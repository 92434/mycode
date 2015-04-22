//------------------------------------------------------------------------------ 
// Copyright (c) 2006 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: link_ctl.v,v $
//  /   /        Date Last Modified:  $Date: 2008-04-11 10:27:14-06 $
// /___/   /\    Date Created: Dec 11, 2006 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: link_ctl.v,v $
// Revision 1.2  2008-04-11 10:27:14-06  hutching
// Added missing reset to link controller state machine.
//
// Revision 1.1  2007-06-21 12:24:14-06  hutching
// Changed to 4 consecutive errors for reframe.  Added clock enable
//
// Revision 1.0  2007-06-06 13:26:09-06  hutching
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
 This module implements a simple state machine to monitor the "health"
 of the DVB-ASI link.  It detects disparity and code errors from the
 decoder and after four consecutive errors have occurred, it automatically asserts
 the reframe signal.  It will loop endlessly trying to re-sync the link
 until there are no errors being reported by the decoder.
 */

`timescale 1ns / 1ns

module link_ctl (/*AUTOARG*/
   // Outputs
   reframe,
   // Inputs
   clk, reset, reframe_req, disp_err, code_err, framed, ce
   ) ;

   
   input clk;
   input reset;
   input reframe_req;
   input disp_err;
   input code_err;
   input framed;
   input       ce;
   
   

   output reframe;
   reg 	  reframe;
   

   parameter [2:0]
		LINK_MONITOR      = 3'b000,
	        LINK_ERR1         = 3'b001,
		LINK_ERR2         = 3'b011,
		LINK_ERR3         = 3'b111,
		LINK_REFRAME1     = 3'b110,
		LINK_REFRAME2     = 3'b010;
   
          


   reg [2:0] 	  linkctl_current_state = LINK_MONITOR;
   reg [2:0] 	  linkctl_next_state;


   reg 		  link_err = 1'b0;
   
   
   

   //
   // Link controller disparity/code error handler
   //
	       
   // Current state logic
   always @(posedge clk)
     begin
	if (reset) begin
	   link_err <= 1'b0;
	   linkctl_current_state <= LINK_MONITOR;
	end
	
	if (ce)
	  begin
	     link_err <= disp_err | code_err;
	     if (reframe_req)
	       linkctl_current_state  <= LINK_REFRAME1;
	     else
	       linkctl_current_state  <= linkctl_next_state;
	  end
     end // always @ (posedge clk)
   
   

   // Output decoding logic
   always @ * begin
      case (linkctl_current_state)

	LINK_MONITOR : reframe <= 1'b0;

	LINK_ERR1    : reframe <= 1'b0;

	LINK_ERR2    : reframe <= 1'b0;

	LINK_ERR3    : reframe <= 1'b0;

	LINK_REFRAME1: reframe <= 1'b1;

	LINK_REFRAME2: reframe <= 1'b1;

	default      : reframe <= 1'b0;
      endcase // case(linkctl_current_state)
      end // always @ *
   
      
   // Next state logic

   always @ * begin
     case (linkctl_current_state)

       LINK_MONITOR  : begin
	  if (link_err && framed)  // Don't start checking until framed.
	    linkctl_next_state <= LINK_ERR1;
	  else if (reframe_req)
	    linkctl_next_state <= LINK_REFRAME1;
	  else
	    linkctl_next_state <= LINK_MONITOR;
       end

       LINK_ERR1    : begin
	  if (link_err)
	    linkctl_next_state <= LINK_ERR2;
	  else if (reframe_req)
	    linkctl_next_state <= LINK_REFRAME1;
	  else
	    linkctl_next_state <= LINK_MONITOR;
       end

       LINK_ERR2    : begin
	  if (link_err)
	    linkctl_next_state <= LINK_ERR3;
	  else if (reframe_req)
	    linkctl_next_state <= LINK_REFRAME1;
	  else
	    linkctl_next_state <= LINK_ERR1;
       end

       LINK_ERR3   : begin
	  if (link_err)
	    linkctl_next_state <= LINK_REFRAME1;
	  else if (reframe_req)
	    linkctl_next_state <= LINK_REFRAME1;
	  else
	    linkctl_next_state <= LINK_ERR2;
       end

       LINK_REFRAME1 : linkctl_next_state <= LINK_REFRAME2;

       LINK_REFRAME2 : linkctl_next_state <= LINK_MONITOR;

       default       : linkctl_next_state <= LINK_REFRAME1;
     endcase // case(linkctl_current_state)
     end // always @ *

   
endmodule // link_ctl

