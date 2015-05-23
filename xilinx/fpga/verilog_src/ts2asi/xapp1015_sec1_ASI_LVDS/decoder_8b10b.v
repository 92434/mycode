//------------------------------------------------------------------------------ 
// Copyright (c) 2006 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: decoder_8b10b.v,v $
//  /   /        Date Last Modified:  $Date: 2007-08-21 09:45:52-06 $
// /___/   /\    Date Created: November 13, 2006 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: decoder_8b10b.v,v $
// Revision 1.1  2007-08-21 09:45:52-06  hutching
// q_valid removed from initialization.
//
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
 This module implements a 8b10b decoder according to the original patent work
 of Widmer and Franaszek.  It is a synchronous module with registers on the input
 and output.  It takes in a 10-bit 8b10b encoded word, and outputs and 8-bit data
 word and a control bit to indicate if the 8-bit output data is one of 12 special
 K-codes.
 */

`include "decoder_8b10b_const.v"

`timescale 1ns / 1ns

module decoder_8b10b (/*AUTOARG*/
   // Outputs
   is_kcode, code_err, disp_err, q_valid, q_8b,
   // Inputs
   clk, ce, d_10b, sinit
   ) ;

   input clk, ce;
   input [9:0] d_10b;
   input       sinit;
   

   output      is_kcode;
   output      code_err;
   output      disp_err;
   output      q_valid;     // Indicates when q output is valid.
   output [7:0] q_8b;


   // Output registers
   reg			is_kcode = 1'b0;
   reg [7:0]		q_8b = 8'b00000000;
   reg 			disp_err = 1'b0;
   reg 			code_err = 1'b0;
   reg 			q_valid = 1'b0;
   
   
   
   

   // Internal registers
   reg [9:0] 		d_10b_R = 10'b0000000000;
   reg 			running_disp_10b = `NEG_RD; // disparity of last symbol(s)

   
   
   
   

   // Internal Combinatorial "Registers"
   reg [5:0] 		decoded_5b;     // Decoded 5-bit sub-block + valid bit
   reg [3:0] 		decoded_3b;     // Decoded 3-bit sub-block + valid bit
   reg [1:0]		disp_4b;        // Current 4b sub-block disparity
   reg [1:0] 		disp_6b;        // Current 6b sub-block disparity
   reg 			disp_err_10b;   // 10b symbol disparity error indicator
   reg   		kchar_detected; // current symbol is a K character
   
   

   // Internal wires
   wire 		invalid_10b;      // OR of invalid_4b, invalid_6b
   wire 		disparity_error;  // OR of all disparity error signals
   wire 		invalid_6b;       // 6b sub-block "out of table" indicator
   wire 		invalid_4b;       // 4b sub-block "out of table" indicator
   wire [1:0] 		disp_10b;         // Current 10b symbol disparity
		
   
 		

   // Synchronous Blocks
   always @ (posedge clk) begin
      q_valid <= ce;
      
      if (sinit) begin
         running_disp_10b <= `NEG_RD;
	 code_err         <= 1'b0;
	 is_kcode         <= 1'b0;
	 q_8b             <= 8'b00000000;
	 disp_err         <= 1'b0;

      end

      // Main registers updated here.  Also, this is where disparity is tracked.
      else if (ce) begin

     	 // Unconditional register updates
	 d_10b_R    <= d_10b;
	 code_err   <= invalid_10b;
	 is_kcode   <= kchar_detected;
	 disp_err   <= disparity_error;
	 q_8b       <= {decoded_3b[2:0], decoded_5b[4:0]};  
	 
	 // Disparity tracking - running disparity of 10b symbols
	 // Note: Disparity tracking is ALWAYS updated regardless if
	 // the code was valid or there was a disparity error.
	 if (disp_10b == `POS_DISP)
           running_disp_10b <= `POS_RD;
	 else if (disp_10b == `NEG_DISP)
           running_disp_10b <= `NEG_RD;
	 
      end // if (ce)
   end // always @ (posedge clk)
   
   

   // Continous assignments
   assign invalid_6b = ~decoded_3b[3]; // bit asserted in decoded data = good code
   assign invalid_4b = ~decoded_5b[5]; // Ditto...
   assign invalid_10b = (invalid_6b | invalid_4b);  // code error occurred.
   assign disp_10b = disp_6b | disp_4b;
   assign disparity_error = disp_err_10b; 
   
   
   
   
   // Combinatorial Always processes


   // Case statement to decode 5-bit sub-block.  The decoding of bits 4:0 is ONLY 
   // dependent on encoded input bits 5:0.  This enables use of a very simple 
   // (but lengthy) case statement to decode these values.  D0_x_neg/pos denotes 
   // the D0.Y pos/neg polarity case of the input code where we "don't care" what
   // Y is.  Hence, the 'x' "assigned" to Y.  Note that not all the pos/neg 
   // polarity cases are handled.  In other words, there are only 48 items in the 
   // case statement.  This is because for 18 items, the 5-bit encoded data is the
   //  same so the duplicates are not needed.  In addition, the valid bit is also
   // set for each valid decode case.  This is used later for code checking.  If 
   // we fall through this table to the default, then we had an invalid 6b code, 
   // which means the 10b symbol was invalid.
   always @ (d_10b_R or disp_10b or disp_4b or disp_6b or running_disp_10b) begin
      case (d_10b_R[5:0])
	`D0_x_neg    : decoded_5b <= {1'b1,5'b00000};
	`D0_x_pos    : decoded_5b <= {1'b1,5'b00000};
	`D1_x_neg    : decoded_5b <= {1'b1,5'b00001};
	`D1_x_pos    : decoded_5b <= {1'b1,5'b00001};
	`D2_x_neg    : decoded_5b <= {1'b1,5'b00010};
	`D2_x_pos    : decoded_5b <= {1'b1,5'b00010};
	`D3_x_zer    : decoded_5b <= {1'b1,5'b00011};
	`D4_x_neg    : decoded_5b <= {1'b1,5'b00100};
	`D4_x_pos    : decoded_5b <= {1'b1,5'b00100};
	`D5_x_zer    : decoded_5b <= {1'b1,5'b00101};
	`D6_x_zer    : decoded_5b <= {1'b1,5'b00110};
	`D7_x_zer1   : decoded_5b <= {1'b1,5'b00111};
	`D7_x_zer2   : decoded_5b <= {1'b1,5'b00111};
	`D8_x_neg    : decoded_5b <= {1'b1,5'b01000};
	`D8_x_pos    : decoded_5b <= {1'b1,5'b01000};
	`D9_x_zer    : decoded_5b <= {1'b1,5'b01001};
	`D10_x_zer   : decoded_5b <= {1'b1,5'b01010};
	`D11_x_zer   : decoded_5b <= {1'b1,5'b01011};
	`D12_x_zer   : decoded_5b <= {1'b1,5'b01100};
	`D13_x_zer   : decoded_5b <= {1'b1,5'b01101};
	`D14_x_zer   : decoded_5b <= {1'b1,5'b01110};
	`D15_x_neg   : decoded_5b <= {1'b1,5'b01111};
	`D15_x_pos   : decoded_5b <= {1'b1,5'b01111};
	`D16_x_neg   : decoded_5b <= {1'b1,5'b10000};
	`D16_x_pos   : decoded_5b <= {1'b1,5'b10000};
	`D17_x_zer   : decoded_5b <= {1'b1,5'b10001};
	`D18_x_zer   : decoded_5b <= {1'b1,5'b10010};
	`D19_x_zer   : decoded_5b <= {1'b1,5'b10011};
	`D20_x_zer   : decoded_5b <= {1'b1,5'b10100};
	`D21_x_zer   : decoded_5b <= {1'b1,5'b10101};
	`D22_x_zer   : decoded_5b <= {1'b1,5'b10110};
	`D23K23_x_neg: decoded_5b <= {1'b1,5'b10111};
	`D23K23_x_pos: decoded_5b <= {1'b1,5'b10111};
	`D24_x_neg   : decoded_5b <= {1'b1,5'b11000};
	`D24_x_pos   : decoded_5b <= {1'b1,5'b11000};
	`D25_x_zer   : decoded_5b <= {1'b1,5'b11001};
	`D26_x_zer   : decoded_5b <= {1'b1,5'b11010};
	`D27K27_x_neg: decoded_5b <= {1'b1,5'b11011};
	`D27K27_x_pos: decoded_5b <= {1'b1,5'b11011};
	`D28_x_zer   : decoded_5b <= {1'b1,5'b11100};
	`D29K29_x_neg: decoded_5b <= {1'b1,5'b11101};
	`D29K29_x_pos: decoded_5b <= {1'b1,5'b11101};
	`D30K30_x_neg: decoded_5b <= {1'b1,5'b11110};
	`D30K30_x_pos: decoded_5b <= {1'b1,5'b11110};
	`D31_x_neg   : decoded_5b <= {1'b1,5'b11111};
	`D31_x_pos   : decoded_5b <= {1'b1,5'b11111};
	`K28_x_neg   : decoded_5b <= {1'b1,5'b11100};
	`K28_x_pos   : decoded_5b <= {1'b1,5'b11100};
	 default     : decoded_5b <= {1'b0,5'b11111};  // Invalid sub-block code
      endcase // case(d_10b[0:5])

      // Decoder to generate the 3-bit sub block  value based on all 10-bit
      // inputs.  Unlike the 5b output bits, the 3b output bits are dependent 
      // on all 10 inputs and also have some special cases we must consider.  
      // Notation in this case statement is the same as the 5b output sub block 
      // decoder.
      case (d_10b_R[9:6]) 
	`DKx_0_neg   : decoded_3b <= {1'b1,3'b000};
        `DKx_0_pos   : decoded_3b <= {1'b1,3'b000};
	`DKx_1_zer   : begin
	   if (d_10b_R[5:0] == `K28_x_neg)
	     decoded_3b <= {1'b1,3'b110};  // K28.6 special case
	   else
	     decoded_3b <= {1'b1,3'b001};  
	end
	`DKx_2_zer   :  begin
	   if ( d_10b_R[5:0] == `K28_x_neg)
	     decoded_3b <= {1'b1,3'b101};  // K28.5 positive special case
	   else
	     decoded_3b <= {1'b1,3'b010};  
	end
	`DKx_3_zer1   : decoded_3b <= {1'b1,3'b011};
	`DKx_3_zer2   : decoded_3b <= {1'b1,3'b011};
	`DKx_4_neg   : decoded_3b <= {1'b1,3'b100};
	`DKx_4_pos   : decoded_3b <= {1'b1,3'b100};
	`DKx_5_zer   :  begin
	   if (d_10b_R[5:0] == `K28_x_neg)
	     decoded_3b <= {1'b1,3'b010};  // K28.2 special case
	   else
	     decoded_3b <= {1'b1,3'b101};
	end
	`DKx_6_zer   : begin
	   if (d_10b_R[5:0] == `K28_x_neg)
	     decoded_3b <= {1'b1,3'b100};  // K28.1 special case
	   else
	     decoded_3b <= {1'b1,3'b110};  
	end
	`Dx_7_neg    : decoded_3b <= {1'b1,3'b111};
	`Dx_7_pos    : decoded_3b <= {1'b1,3'b111};
	`DKx_7_neg   : decoded_3b <= {1'b1,3'b111};
	`DKx_7_pos   : decoded_3b <= {1'b1,3'b111};
	default      : decoded_3b <= {1'b0,3'b111};  // Invalid 4b sub-block
      endcase // case(d_10b_R[6:9])

      // The following block calculates the current disparity for the symbols and 
      // sub-blocks.  All disparity must be checked at the sub-block boundaries 
      // and the symbol boundaries to insure that the incoming symbols meet the
      // disparity rules.  Note that we only do this for error checking purposes.
      // This always block calculates whether the current 4b/6b sub-blocks have 
      // positive or negative disparity.
      case (d_10b_R[9:6])
	`DKx_0_neg: disp_4b <= `NEG_DISP;
	`DKx_0_pos: disp_4b <= `POS_DISP;
	`DKx_4_neg: disp_4b <= `NEG_DISP;
	`DKx_4_pos: disp_4b <= `POS_DISP;
	`Dx_7_neg : disp_4b <= `NEG_DISP;
	`Dx_7_pos : disp_4b <= `POS_DISP;
	`DKx_7_neg: disp_4b <= `NEG_DISP;
	`DKx_7_pos: disp_4b <= `POS_DISP;
	default: disp_4b <= `ZERO_DISP;
      endcase // case(d_10b_R[9:6])

      case (d_10b_R[5:0])
	`D0_x_neg    : disp_6b <= `NEG_DISP;
	`D0_x_pos    : disp_6b <= `POS_DISP;
	`D1_x_neg    : disp_6b <= `NEG_DISP;
	`D1_x_pos    : disp_6b <= `POS_DISP;
	`D2_x_neg    : disp_6b <= `NEG_DISP;
	`D2_x_pos    : disp_6b <= `POS_DISP;
	`D4_x_neg    : disp_6b <= `NEG_DISP;
	`D4_x_pos    : disp_6b <= `POS_DISP;
	`D8_x_neg    : disp_6b <= `NEG_DISP;
	`D8_x_pos    : disp_6b <= `POS_DISP;
	`D15_x_neg   : disp_6b <= `NEG_DISP;
	`D15_x_pos   : disp_6b <= `POS_DISP;
	`D16_x_neg   : disp_6b <= `NEG_DISP;
	`D16_x_pos   : disp_6b <= `POS_DISP;
	`D23K23_x_neg: disp_6b <= `NEG_DISP;
	`D23K23_x_pos: disp_6b <= `POS_DISP;
	`D24_x_neg   : disp_6b <= `NEG_DISP;
	`D24_x_pos   : disp_6b <= `POS_DISP;
	`D27K27_x_neg: disp_6b <= `NEG_DISP;
	`D27K27_x_pos: disp_6b <= `POS_DISP;
	`D29K29_x_neg: disp_6b <= `NEG_DISP;
	`D29K29_x_pos: disp_6b <= `POS_DISP;
	`D30K30_x_neg: disp_6b <= `NEG_DISP;
	`D30K30_x_pos: disp_6b <= `POS_DISP;
	`D31_x_neg   : disp_6b <= `NEG_DISP;
	`D31_x_pos   : disp_6b <= `POS_DISP;
	`K28_x_neg   : disp_6b <= `NEG_DISP;
	`K28_x_pos   : disp_6b <= `POS_DISP;
	 default     : disp_6b <= `ZERO_DISP;
      endcase // case(d_10b[0:5])

      // For the 10b disparity compare, we must check the current disparity value
      // (POS, NEG, ZERO) against the running disparity which is POS, NEG, or 
      // ZERO.  The same basic conditions hold true:  If running disparity and 
      // current disparity are the same, it's an error.
      // 10b symbol disparity error checks - checks agains 4b, 6b errors and 
      // running disparity
      if (disp_4b == `POS_DISP && disp_6b == `POS_DISP)      // Both pos - error
	disp_err_10b <= `DISPARITY_ERROR_10B;
      else if (disp_4b == `NEG_DISP && disp_6b == `NEG_DISP) // Both neg - error
	disp_err_10b <= `DISPARITY_ERROR_10B;
      else if (disp_10b == `POS_DISP && running_disp_10b)    // Both pos - ERROR
	disp_err_10b <= `DISPARITY_ERROR_10B;
      else if (disp_10b == `NEG_DISP && !running_disp_10b)   // Both neg - ERROR
	disp_err_10b <= `DISPARITY_ERROR_10B;
      else if (disp_10b == `POS_DISP && !running_disp_10b)   // +2, running is -1
	disp_err_10b <= `DISPARITY_OK_10B;
      else
	disp_err_10b <= `DISPARITY_OK_10B;                 // -2, running is +1
                                                           // 0 running is +1
                                                           // 0 running is -1

      
      // Now we need to determine when to assert the is_kcode output.  When 
      // asserted, this output indicates that the current symbol is a K-code.  
      // The K-codes have the same decoded values as the D-codes, so the only 
      // way to differentiate them is with this output.  It is asserted based 
      // on a simple lookup table.
      if (d_10b_R[5:0] == `K28_x_neg || d_10b_R[5:0] == `K28_x_pos)
	kchar_detected <= 1'b1;
      else begin
	 case (d_10b_R) 
	    `K23_7_neg: kchar_detected <= 1'b1;
	    `K23_7_pos: kchar_detected <= 1'b1;
	    `K27_7_neg: kchar_detected <= 1'b1;
	    `K27_7_pos: kchar_detected <= 1'b1;
	    `K29_7_neg: kchar_detected <= 1'b1;
	    `K29_7_pos: kchar_detected <= 1'b1;
	    `K30_7_neg: kchar_detected <= 1'b1;
	    `K30_7_pos: kchar_detected <= 1'b1;
	    default   : kchar_detected <= 1'b0;
         endcase
      end
   end // always @ (d_10b_R)
endmodule // 8b10b_decoder
