//------------------------------------------------------------------------------ 
// Copyright (c) 2007 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: serializer_10b1b.v,v $
//  /   /        Date Last Modified:  $Date: 2008-04-11 10:31:15-06 $
// /___/   /\    Date Created: Jan 24, 2007 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: serializer_10b1b.v,v $
// Revision 1.3  2008-04-11 10:31:15-06  hutching
// Removed word-rate clock input.
//
// Revision 1.2  2008-04-11 10:15:46-06  hutching
// Added FSM encoding attribute to force gray coding.
//
// Revision 1.1  2007-10-29 10:00:13-06  hutching
// Startup sync logic added.
//
// Revision 1.0  2007-06-12 14:26:09-06  hutching
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
 This module implements a simple 10bit to 1-bit serializer function.  It is targeted
 towards Spartan3E and uses the Spartan3E DDR output flip flops.  Clocks are provided
 at 135MHz with clock enable and the data is output at DDR 270Mbps.
 */

module serializer_10b1b(/*AUTOARG*/
   // Outputs
   sdout_p,
   sdout_n,
   start,
   sout_data,
   // Inputs
   sclk_0, sclk_180, ce, reset, din_10b
   ) ;
   
   input sclk_0, sclk_180;  // Half bit-rate clock input
   input ce;                // Clock enable
   input reset;         // Synchronous reset for clk domain
   input [9:0] din_10b;     // 10-bit parallel data
   output      sdout_p;       // Serial data at sclk*2 rate (uses DDR)
   output      sdout_n;       // Serial data at sclk*2 rate (uses DDR)
   output wire start;
   output wire [9:0] sout_data;
   // Parameters for gray code counter
   parameter [2:0]
		CNT_RESET   = 3'b000,
		CNT_STATE1  = 3'b001,
		CNT_STATE2  = 3'b011,
		CNT_STATE3  = 3'b010,
		CNT_STATE4  = 3'b110;
   

   // Internal clk registers
   reg [9:0]   din_10b_R;
   reg 	       start_bit = 1'b0;

   // Internal sclk registers
   (* fsm_encoding = "GRAY" *) reg [2:0]   cnt_curr_state = 0;   // Note that this is a gray coded counter.
   reg [2:0]   cnt_next_state = 0;
   reg [1:0]   sdata_slice_R = 0;    // Pipeline state to ease DDR timing
   
   
   

   // Internal wires
   reg [1:0]   sdata_slice;
   
   // Synchronous process for word rate registers
  always @(posedge sclk_0)
     if (reset)
	  start_bit <= 1'b0;
     else if (ce)
       begin
	  din_10b_R <= din_10b;
	  start_bit <= 1'b1;
      end 

  assign start = start_bit;

  assign sout_data = din_10b;

  // Synchronous process for serial half bit-rate clock
   // gray code counter.
   always @(posedge sclk_0)
     begin
	sdata_slice_R <= sdata_slice;
	if (reset) 
	   cnt_curr_state <= CNT_RESET;
	else
	  cnt_curr_state <= cnt_next_state;
     end // always @ (posedge sclk)
   

   // Gray code counter next state logic
   always @ (*)
     begin
	case (cnt_curr_state)
	  CNT_RESET  : begin
	     if (start_bit)  // Align counter with word boundaries
	       cnt_next_state <= CNT_STATE1;
	     else
	       cnt_next_state <= CNT_RESET;
	  end
	  
	  CNT_STATE1 : cnt_next_state <= CNT_STATE2;
	  CNT_STATE2 : cnt_next_state <= CNT_STATE3;
	  CNT_STATE3 : cnt_next_state <= CNT_STATE4;
	  CNT_STATE4 : cnt_next_state <= CNT_RESET;
	  default    : cnt_next_state <= CNT_RESET;
	endcase // case(cnt_curr_state)
     end   
	      

   // Simple 2-bit wide mux for slicing 10-bit word into 2-bit chunks
   // select is output of gray coded counter
   always @ (*)
     begin
	case (cnt_curr_state)
	  CNT_RESET  : sdata_slice <= din_10b_R[1:0];
	  CNT_STATE1 : sdata_slice <= din_10b_R[3:2];
	  CNT_STATE2 : sdata_slice <= din_10b_R[5:4]; 
	  CNT_STATE3 : sdata_slice <= din_10b_R[7:6];
	  CNT_STATE4 : sdata_slice <= din_10b_R[9:8];
	  default    : sdata_slice <= 2'b00;
	endcase // case(cnt_curr_state)
     end


   // DDR primitive instantiation
   ODDR2 #(
   //oddr_2 #(
      .DDR_ALIGNMENT("NONE"), // Sets output alignment to "NONE", "C0" or "C1
      .INIT(1'b0),            // Sets initial state of the Q output to 1'b0 or 1'b1
      .SRTYPE("SYNC")         // Specifies "SYNC" or "ASYNC" set/reset
   ) ser_oddr2_inst_0 (
      .Q(sdout_p),           // 1-bit DDR output data
      .C0(sclk_0),         // 1-bit clock input
      .C1(sclk_180),       // 1-bit clock input
      .CE(1'b1),           // 1-bit clock enable input
      .D0(sdata_slice_R[1]), // 1-bit data input (associated with C0)
      .D1(sdata_slice_R[0]), // 1-bit data input (associated with C1)
      .R(1'b0),              // 1-bit reset input
      .S(1'b0)                 // 1-bit set input
   ); 

   wire [1:0] sdata_slice_R_n;    // Pipeline state to ease DDR timing

   assign sdata_slice_R_n = ~sdata_slice_R;

   // DDR primitive instantiation
   ODDR2 #(
   //oddr_2 #(
      .DDR_ALIGNMENT("NONE"), // Sets output alignment to "NONE", "C0" or "C1
      .INIT(1'b0),            // Sets initial state of the Q output to 1'b0 or 1'b1
      .SRTYPE("SYNC")         // Specifies "SYNC" or "ASYNC" set/reset
   ) ser_oddr2_inst_1 (
      .Q(sdout_n),           // 1-bit DDR output data
      .C0(sclk_0),         // 1-bit clock input
      .C1(sclk_180),       // 1-bit clock input
      .CE(1'b1),           // 1-bit clock enable input
      .D0(sdata_slice_R_n[1]), // 1-bit data input (associated with C0)
      .D1(sdata_slice_R_n[0]), // 1-bit data input (associated with C1)
      .R(1'b0),              // 1-bit reset input
      .S(1'b0)                 // 1-bit set input
   ); 

endmodule // serializer_10b1b
