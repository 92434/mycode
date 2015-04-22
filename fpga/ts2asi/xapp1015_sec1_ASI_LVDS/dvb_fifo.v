//------------------------------------------------------------------------------ 
// Copyright (c) 2006 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: dvb_fifo.v,v $
//  /   /        Date Last Modified:  $Date: 2008-04-11 10:26:42-06 $
// /___/   /\    Date Created: November 22, 2006 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: dvb_fifo.v,v $
// Revision 1.1  2008-04-11 10:26:42-06  hutching
// Added reset to coregen FIFO.
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
 This module instantiates the CoreGen FIFO and has the FIFO control state machine.
 This  module is the last piece of the rx data path for the DVB ASI receiver.  The
 user interfaces with  this module to extract the MPEG-2 packet data from the 
 receiver and forward to the next section.
 */

`timescale 1ns / 1ns

`define FIFO_LEVEL_HIGH 11'h600    // FIFO High water mark
`define FIFO_LEVEL_LOW  11'h111    // FIFO Low water mark


module dvb_fifo (/*AUTOARG*/
   // Outputs
   fifo_dout_8b, fifo_kcode_out, rd_count, fifo_empty, fifo_full,
   // Inputs
   clk, user_clk, data8b_in, data8b_valid, framed, reset, is_kchar,
   fifo_rden
   ) ;

   
   input clk;                 // 54MHz clock (receiver parallel clock domain)
   input user_clk;            // User read domain clock 
   input [7:0]  data8b_in;    // 8-bit input data from 8b10b decoder
   input 	data8b_valid; // New data output from 8b10b decoder
   input 	framed;       // ASI framing sequence detected and locked
   input 	reset;        // Synchronous reset for state machine logic
   input 	is_kchar;     // Input indicates current 8-bit word is a comma
   input 	fifo_rden;    // FIFO read enable input synchronous to user_clk
   

   output [7:0] 	fifo_dout_8b;    // 8-bit output data
   output 		fifo_kcode_out;  // indicates current data is comma 
   output [10:0] 	rd_count;    // FIFO read count - synchronous to user_clk
   output 		fifo_empty;  // FIFO empty flag - synchronous to user_clk
   output 		fifo_full;   // FIFO full flag, synchronous to clk_54
   


   // Internal registers
   reg [7:0] 		data8b_in_R;
   reg 			is_kchar_R;
   reg 			data8b_valid_R;
   reg 			framed_R;

   // Registers to equalize data path delay with control path
   reg [7:0] 		data8b_in_R2;
   reg 			data8b_valid_R2;
   reg 			is_kchar_R2;
   
   
   

   // Internal data path Signals
   wire [8:0] 		din_fifo;   // FIFO input data 
   wire [8:0] 		fifo_dout;  // Internal FIFO data out
   wire [10:0] 		wr_count;   // FIFO write count
   
   
   

   // Internal control path signals from state machine.
   reg 		fifo_din_sel;   // Selects between real data or comma data.
   reg 		fifo_wr_comma;
   reg 		fifo_wren_gate;
   

   // State Machine signals, parameters
   parameter [1:0]                         
		FIFO_RESET      = 2'b00,
		FIFO_MONITOR    = 2'b10,
       		FIFO_INS_COMMA  = 2'b11,
		FIFO_DEL_COMMA  = 2'b01;
   

   reg [1:0]   fifoctlr_current_state = FIFO_RESET;
   reg [1:0]   fifoctlr_next_state;


   // Input register process
   always @(posedge clk)
     begin
	if (reset)
	  begin
             data8b_valid_R <= 1'b0;
   	     framed_R <= 1'b0;
	  end
	else
	  begin
	     data8b_in_R <= data8b_in;
	     data8b_in_R2 <= data8b_in_R;
	     is_kchar_R <= is_kchar;
	     is_kchar_R2 <= is_kchar_R;
	     data8b_valid_R <= data8b_valid;
	     data8b_valid_R2 <= data8b_valid_R;
	     framed_R <= framed;
	  end // else: !if(reset)
     end // always @ (posedge clk)
   

	
   
   
   // FIFO Data path.  The FIFO data path needs some muxing to handle the comma 
   // insertion case.  When we need to insert a comma, the muxes switch the input 
   // data of the FIFO to a constant 0xBC value which is the 8-bit comma value.  
   // Since 0xBC is also a valid data value, we also mux in a value of 0x1 for the
   // kchar input bit to tag the FIFO data.  Data path mux muxes 8-bit input data 
   // and kchar flag between actual values from decoder, and constant values for 
   // comma insertion.  "fifo_din_sel" is asserted from the state machine to
   // select the correct data path.  FIFO tagged output data is separated.
   assign din_fifo = fifo_din_sel ? {1'b1, 8'hBC} : {is_kchar_R2, data8b_in_R2};
   assign fifo_dout_8b   = fifo_dout[7:0];
   assign fifo_kcode_out = fifo_dout[8];
   


   
   // FIFO write enable control logic.  In the comma insertion case, we assert the
   // write enable only when the incoming data is not valid - which indicates a 
   // empty cycle.  This module runs at 2X the word rate, so on average, the 
   // "data_in_valid" input will only be asserted every other cycle.  The state 
   // machine looks for those cycles, and asserts the "fifo_wr_comma" signal, and
   // the "fifo_wren_gate" signal.  In the comma deletion case, the state machine
   // deasserts the "fifo_wren_gate" signal which disables all writes to the FIFO.
   // Otherwise, the "fifo_wr_comma" signal is deasserted, and the 
   // "fifo_wren_gate" signal is asserted, allowing writes to occurr normally as
   // valid data comes through.
   assign fifo_wren = ((data8b_valid_R2 | fifo_wr_comma) & fifo_wren_gate);
   

   // FIFO Controller.  This state machine manages the "write" side of the FIFO 
   // and does the comma insertion and deletion; thereby providing basic comma 
   // correction capability to match small frequency differences between the 
   // receiver clock domain and the user clock domain.  A block RAM based CoreGen
   // FIFO is used as the asynchronous FIFO storage block.  

   // Current state logic
   always @(posedge clk)
     if (reset)
       fifoctlr_current_state <= FIFO_RESET;
     else
       fifoctlr_current_state <= fifoctlr_next_state;
   // Output decoding logic
   always @ * begin
      case (fifoctlr_current_state)
	
	FIFO_RESET:
	  begin
	     fifo_din_sel   <= 1'b1;       // Select constant comma data path
	     fifo_wren_gate <= 1'b1;       // comma stuffing if link lost.
	     fifo_wr_comma  <= 1'b0;       // Don't force, writes occurr normally.
	  end
	
	FIFO_MONITOR:
	  begin
	     fifo_wren_gate <= 1'b1;       // Enable writes to FIFO.
	     fifo_din_sel   <= 1'b0;       // Select input data to FIFO
	     fifo_wr_comma  <= 1'b0;
	  end
	
	FIFO_INS_COMMA:
	  begin
	     fifo_din_sel   <= 1'b1;      // Select constant comma data to FIFO
	     fifo_wren_gate <= 1'b1;      // Enable writes to FIFO
	     fifo_wr_comma  <= 1'b1;      // Force write to FIFO
	  end
	
	FIFO_DEL_COMMA:
	  begin
	     fifo_din_sel   <= 1'b0;     // Select input data path to FIFO
	     fifo_wren_gate <= 1'b0;     // Disable writes to FIFO
	     fifo_wr_comma  <= 1'b0;
	  end
	
      endcase // case(fifoctlr_current_state)
   end // always @ *
   
   

   // Next state decode logic
   always @ * begin
      case (fifoctlr_current_state)
	
	FIFO_RESET:
	  if (framed_R)                            
	    fifoctlr_next_state <= FIFO_MONITOR;
	  else if (wr_count >= `FIFO_LEVEL_HIGH)   
	    fifoctlr_next_state <= FIFO_DEL_COMMA; 
	  else if (wr_count <= `FIFO_LEVEL_LOW)    
	    fifoctlr_next_state <= FIFO_INS_COMMA; 
	  else
	    fifoctlr_next_state <= FIFO_RESET;     
	
	FIFO_MONITOR:
	  if (!framed_R)   
	    fifoctlr_next_state <= FIFO_RESET;
	  else if ((wr_count >= `FIFO_LEVEL_HIGH) && data8b_valid_R && is_kchar_R)
	    fifoctlr_next_state <= FIFO_DEL_COMMA;                             
	  else if ((wr_count <= `FIFO_LEVEL_LOW) && !data8b_valid_R) 
	    fifoctlr_next_state <= FIFO_INS_COMMA;                   
	  else
	    fifoctlr_next_state <= FIFO_MONITOR;                     
	
	FIFO_DEL_COMMA:
	  if (!framed_R)   
	    fifoctlr_next_state <= FIFO_RESET;
	  else 
	    fifoctlr_next_state <= FIFO_MONITOR;
					 
        FIFO_INS_COMMA:
	  if (!framed_R)  
	    fifoctlr_next_state <= FIFO_RESET;
	  else 
	  fifoctlr_next_state <= FIFO_MONITOR;
	
      endcase // case(fifoctrl_current_state)
   end // always @ *
   
   // Packet FIFO
   //
   // Asynchronous FIFO that transfers the Rx data packets 
   // into the system clock domain.  The "write" side of this FIFO
   // is completely under the control the comma insert/delete state machine
   // in this module.
   //
   // The FIFO is 9-bits wide to support data tagging.  The lower 8-bits are the 
   // decoded data from the 8b10b decoder.  The 9th bit, is the kchar indicator. 
   // These bits are separated in the output ports section of the module.
   //
   dcfifo_2kx9  rx_fifo (.din(din_fifo), 
		         .wr_en(fifo_wren), 
			 .wr_clk(clk), 
			 .rd_en(fifo_rden), 
			 .rd_clk(user_clk), 
			 .rst(reset), 
			 .dout(fifo_dout), 
			 .full(fifo_full), 
			 .empty(fifo_empty), 
			 .wr_data_count(wr_count), 
			 .rd_data_count(rd_count));
 


endmodule // dvb_fifo
