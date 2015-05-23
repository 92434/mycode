 //------------------------------------------------------------------------------ 
// Copyright (c) 2007 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: encoder_8b10b.v,v $
//  /   /        Date Last Modified:  $Date: 2007-06-07 10:13:21-06 $
// /___/   /\    Date Created: Jan 19, 2007 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: encoder_8b10b.v,v $
// Revision 1.0  2007-06-07 10:13:21-06  hutching
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

/* Module Description:  
 This module implements a complete 8b10b encoder.  It is derived from the Widmer 
 and Franaszek article in IBM Journal, Vol 27, No. 5, September 1983.  It is not
 heavily piplened, having only input and output registers.  It is not intended 
 for high-performance applications, but rather, slower word-rate ASI applications.
*/

`include "decoder_8b10b_const.v"

module encoder_8b10b (/*AUTOARG*/
   // Outputs
   dout, valid, code_err,
   // Inputs
   din, kin, clk, ce
   ) ;

   input [7:0] din;
   input       kin;
   input       clk;
   input       ce;
   //input       force_disp;
   //input       disp_in;

   output [9:0] dout;
   output 	valid;
   output 	code_err;

   // Output registers
   reg 		code_err;
   reg [9:0] 	dout;
   reg 		valid;
   
   
   
   // Internal wires
   wire [5:0] 	din_k_5b;       // 5-bit sub_block concatenated with kchar input
   wire [3:0] 	din_k_3b;       // 3-bit sub-block concatenated with kchar input
   reg  [5:0] 	sub_block_6b;   // Encoded 6-bit sub-block
   reg  [3:0] 	sub_block_4b;   // Initial encoding of 4-bit sub-block
   reg  [1:0] 	disp_6b;        // Disparity of current encoded block 
   reg  [1:0]	disp_4b;        // Disparity of current encoded block 
   wire [1:0] 	disp6b_muxout;  // 6-bit sub-block disparity of mux output
   wire [1:0] 	disp4b_muxout;  // 4-bit sub-block disparity of mux output
   wire [1:0] 	disp_10b;       // Disparity of current 10-bit output code
   wire [1:0] 	disp10b_enc;    // Disparity of initial encoding of 10-bitcode
   
   reg 	        code6b_inv_sel; // Selects complement of 6-bit sub-block
   wire [5:0] 	code_6b_mux;    // 6-bit sub-block mux for normal or compliment
   reg [3:0] 	code_4b_mux;    // 4-bit sub-block mux for normal or compliment
   reg 	        code4b_inv_sel; // Selects complement of 4-bit sub-block
   wire [9:0]	code_10b;       // 6-bit and 4-bit sub-blocks concatenated

   reg 	Dx7_special_case;       // special run-length requires 4-bit sub-block 
                                // modifier
         
   // Internal registers
   reg [7:0] 	din_8b_R      = 8'b00000000;
   reg 		kin_R         = 1'b0;
   reg 		force_disp_R  = 1'b0;
   reg 		disp_in_R     = 1'b0;
   reg 		rd_10b        = `NEG_RD; // 10-bit code initial running disparity
   
   reg 		valid_R;
   
	       

   // Synchronous process
   always @(posedge clk)
     begin
	if (ce)
	  begin

	     //10b running disparity tracking process
	     if (disp_10b == `NEG_DISP)
	       rd_10b <= `NEG_RD;
	     else if (disp_10b == `POS_DISP)
	       rd_10b <= `POS_RD;
	     

	     // Output registers
	     dout    <= code_10b;
	     valid_R <= ce;
	     valid   <= valid_R;

	     // Input Registers
	     din_8b_R <= din;
	     kin_R    <= kin;
	     
	     
	  end // if (ce)
     end // always @ (posedge clk)
   
	


   // Continuous Assignments
   assign 	din_k_5b = {kin_R, din_8b_R[4:0]};
   assign 	din_k_3b = {kin_R, din_8b_R[7:5]};

   // 10-bit output code is simple concatenation of sub_blocks
   assign 	code_10b = {code_4b_mux, code_6b_mux};

   // sub_block complentary code select mux for 6-bit sub-block
   assign 	code_6b_mux = code6b_inv_sel ? ~sub_block_6b : sub_block_6b;


   // Sub-block mux output disparity calculation.  These are used to track
   // the disparity of the 10-bit output code.
   assign 	disp6b_muxout = (code6b_inv_sel & (disp_6b != `ZERO_DISP)) ? ~disp_6b : disp_6b;
   assign 	disp4b_muxout = (code4b_inv_sel & (disp_4b != `ZERO_DISP)) ? ~disp_4b : disp_4b;

   // 10-bit pre-output code disparity.
   assign disp_10b = disp6b_muxout | disp4b_muxout;
   assign disp10b_enc = disp_6b | disp_4b;
   
   
   // Case statement to encode least significant 5 input bits and K input
   // into 6-bit sub-blocks according to coding rules.  Note that not
   // all cases are represented here.  Some input values have both a
   // positive/negative coded word result.  One is simply the full complement
   // of the other which is handled elsewhere depending on the current value
   // of the running disparity.  All encodings below are either neg. or zero.
   // A selectable inversion stage is used later to select the pos. if needed.
   always @(*)
     begin
	case (din_k_5b)
	  6'b000000: sub_block_6b <= `D0_x_neg;     // D.0
	  6'b000001: sub_block_6b <= `D1_x_neg;     // D.1
	  6'b000010: sub_block_6b <= `D2_x_neg;     // D.2
	  6'b000011: sub_block_6b <= `D3_x_zer;     // D.3
	  6'b000100: sub_block_6b <= `D4_x_neg;     // D.4
	  6'b000101: sub_block_6b <= `D5_x_zer;     // D.5
	  6'b000110: sub_block_6b <= `D6_x_zer;     // D.6
	  6'b000111: sub_block_6b <= `D7_x_zer2;    // D.7
	  6'b001000: sub_block_6b <= `D8_x_neg;     // D.8
	  6'b001001: sub_block_6b <= `D9_x_zer;     // D.9
	  6'b001010: sub_block_6b <= `D10_x_zer;    // D.10
	  6'b001011: sub_block_6b <= `D11_x_zer;    // D.11
	  6'b001100: sub_block_6b <= `D12_x_zer;    // D.12
	  6'b001101: sub_block_6b <= `D13_x_zer;    // D.13
	  6'b001110: sub_block_6b <= `D14_x_zer;    // D.14
	  6'b001111: sub_block_6b <= `D15_x_neg;    // D.15
	  6'b010000: sub_block_6b <= `D16_x_neg;    // D.16
	  6'b010001: sub_block_6b <= `D17_x_zer;    // D.17
	  6'b010010: sub_block_6b <= `D18_x_zer;    // D.18
	  6'b010011: sub_block_6b <= `D19_x_zer;    // D.19
	  6'b010100: sub_block_6b <= `D20_x_zer;    // D.20
	  6'b010101: sub_block_6b <= `D21_x_zer;    // D.21
	  6'b010110: sub_block_6b <= `D22_x_zer;    // D.22
	  6'b010111: sub_block_6b <= `D23K23_x_neg; // D.23
	  6'b110111: sub_block_6b <= `D23K23_x_neg; // K.23
	  6'b011000: sub_block_6b <= `D24_x_neg;    // D.24
	  6'b011001: sub_block_6b <= `D25_x_zer;    // D.25
	  6'b011010: sub_block_6b <= `D26_x_zer;    // D.26
	  6'b011011: sub_block_6b <= `D27K27_x_neg; // D.27
	  6'b111011: sub_block_6b <= `D27K27_x_neg; // K.27
	  6'b011100: sub_block_6b <= `D28_x_zer;    // D.28
	  6'b111100: sub_block_6b <= `K28_x_neg;    // K.28
	  6'b011101: sub_block_6b <= `D29K29_x_neg; // D.29
	  6'b111101: sub_block_6b <= `D29K29_x_neg; // K.29
	  6'b011110: sub_block_6b <= `D30K30_x_neg; // D.30
	  6'b111110: sub_block_6b <= `D30K30_x_neg; // K.30
	  6'b011111: sub_block_6b <= `D31_x_neg;    // D.31
	  default  : sub_block_6b <= 6'h3F;        // Invalid inputs
	endcase // case(din_5b)

   // Case statement to encode most significant 3 bits plus K input into
   // 4b sub-blocks according to code rules.  As in the 5b/6b case, not all
   // possibilities are represented.  We encode either zero or neg and a
   // selectable inversion stage later flips the code if running disparity
   // rules require it.  This makes the ROM small and the code simpler.

	case (din_k_3b)
	  4'b0000: sub_block_4b <= `DKx_0_pos;      // D/K.x.0
	  4'b0001: sub_block_4b <= `DKx_1_zer;      // D/K.x.1
	  4'b0010: sub_block_4b <= `DKx_2_zer;      // D/K.x.2
	  4'b0011: sub_block_4b <= `DKx_3_zer2;     // D/K.x.3
	  4'b0100: sub_block_4b <= `DKx_4_pos;      // D/K.x.4
	  4'b0101: sub_block_4b <= `DKx_5_zer;      // D/K.x.5
	  4'b0110: sub_block_4b <= `DKx_6_zer;      // D/K.x.6
	  4'b0111: begin
	     if (sub_block_6b == `D17_x_zer || sub_block_6b == `D18_x_zer || sub_block_6b == `D20_x_zer)
	       sub_block_4b <= `DKx_7_pos;
	     else
	       sub_block_4b <= `Dx_7_pos;       // D.x.7
	  end
	  4'b1111: sub_block_4b <= `DKx_7_pos;      // D/K.x.7
	  4'b1000: sub_block_4b <= `DKx_0_pos;      // K28.0
	  4'b1001: sub_block_4b <= `DKx_1_zer2;     // K28.1
	  4'b1010: sub_block_4b <= `DKx_2_zer2;     // K28.2
	  4'b1011: sub_block_4b <= `DKx_3_zer1;     // K28.3
	  4'b1100: sub_block_4b <= `DKx_4_pos;      // K28.4
	  4'b1101: sub_block_4b <= `DKx_5_zer2;     // K28.5
	  4'b1110: sub_block_4b <= `DKx_6_zer2;     // K28.6
	  default: sub_block_4b <= 4'hF;            // Illegal inputs
	endcase // case(din_k_3b)

	// Flag the 6 special cases around certain run length encodings
	if (sub_block_4b == `Dx_7_pos || sub_block_4b == `DKx_7_pos)
	  case (sub_block_6b)
	    `D11_x_zer: Dx7_special_case <= 1'b1;
	    `D13_x_zer: Dx7_special_case <= 1'b1;
	    `D14_x_zer: Dx7_special_case <= 1'b1;
	    `D17_x_zer: Dx7_special_case <= 1'b1;
	    `D18_x_zer: Dx7_special_case <= 1'b1;
	    `D20_x_zer: Dx7_special_case <= 1'b1;
	    default : Dx7_special_case  <= 1'b0;
	  endcase // case(sub_block_6b)
	else
	  Dx7_special_case <= 1'b0;
	
	// This case statement calculates the disparity of the 6b
	// sub block.  To shrink this lookup table, we take advantage
	// of the fact that we only need to check if it's NEG.  Otherwise
	// it must be ZERO based on how we encode internally.

	case (sub_block_6b)
	  `D0_x_neg     : disp_6b <= `NEG_DISP;
	  `D1_x_neg     : disp_6b <= `NEG_DISP;
	  `D2_x_neg     : disp_6b <= `NEG_DISP;
	  `D4_x_neg     : disp_6b <= `NEG_DISP;
	  `D8_x_neg     : disp_6b <= `NEG_DISP;
	  `D15_x_neg    : disp_6b <= `NEG_DISP;
	  `D16_x_neg    : disp_6b <= `NEG_DISP;
	  `D23K23_x_neg : disp_6b <= `NEG_DISP;
	  `D24_x_neg    : disp_6b <= `NEG_DISP;
	  `D27K27_x_neg : disp_6b <= `NEG_DISP;
	  `K28_x_neg    : disp_6b <= `NEG_DISP;
	  `D29K29_x_neg : disp_6b <= `NEG_DISP;
	  `D30K30_x_neg : disp_6b <= `NEG_DISP;
	  `D31_x_neg    : disp_6b <= `NEG_DISP;
	  default       : disp_6b <= `ZERO_DISP;
	endcase // case(sub_block_6b)
	
 
	// This case statement, using the same advantage determines
	// NEG or ZERO disparity for the 4b sub-block
	case (sub_block_4b)
	  `DKx_0_pos : disp_4b <= `POS_DISP;
	  `DKx_4_pos : disp_4b <= `POS_DISP;
	  `Dx_7_pos  : disp_4b <= `POS_DISP;
	  `DKx_7_pos : disp_4b <= `POS_DISP;
	  default    : disp_4b <= `ZERO_DISP;
	endcase // case(sub_block_4b)


	// This case statement determines when we complement the 6b, 4b, both
	// or niether of the sub-blocks, based on a combination of 10-bit running
	// disparity, current sub-block disparity and some special cases.
	case (disp10b_enc)
	  2'b00: begin
	     if (rd_10b == `NEG_RD)
	       begin
		  if (sub_block_4b == `DKx_3_zer2)
		    code4b_inv_sel <= 1'b1;
		  else
		    code4b_inv_sel <= 1'b0;
		  if (sub_block_6b == `D7_x_zer2)
	            code6b_inv_sel <= 1'b1;
		  else
	            code6b_inv_sel <= 1'b0;
	       end // if (rd_10b == `NEG_RD)
	     else
	       begin
		  code4b_inv_sel <= 1'b0;
		  code6b_inv_sel <= 1'b0;
	       end // else: !if(rd_10b == `NEG_RD)
	  end

	  2'b10: begin 
	     if (rd_10b == `POS_RD)
	       begin
		  code6b_inv_sel <= 1'b0;
		  code4b_inv_sel <= 1'b1;
	       end
	     else if (rd_10b == `NEG_RD && sub_block_6b == `D7_x_zer2) 
	       begin
		  code6b_inv_sel <= 1'b1;
		  code4b_inv_sel <= 1'b0;
	       end 
	     else
	       begin
		  code6b_inv_sel <= 1'b0;
		  code4b_inv_sel <= 1'b0;
	       end
	  end
	  
	  2'b01: begin
	     if (rd_10b == `NEG_RD)
	       begin
		  code6b_inv_sel <= 1'b1;
		  if (kin_R)
		    code4b_inv_sel <= 1'b1;
		  else
		    code4b_inv_sel <= 1'b0;
	       end
	     else if (rd_10b == `POS_RD && sub_block_4b == `DKx_3_zer2  && !kin_R)
	       begin
		  code6b_inv_sel <= 1'b0; 
		  code4b_inv_sel <= 1'b1;
	       end 
	     else
	       begin
		  code6b_inv_sel <= 1'b0;
		  code4b_inv_sel <= 1'b0;
	       end
	  end
	  
	  2'b11: begin
	     if (rd_10b == `POS_RD)
	       begin
		  code6b_inv_sel <= 1'b0;
		  code4b_inv_sel <= 1'b0;
	       end
	     else
	       begin
		  code6b_inv_sel <= 1'b1;
		  code4b_inv_sel <= 1'b1;
	       end
	  end

	endcase // case(disp6b | disp4b)

   // 4-bit sub-block complement mux.  Handles the DX.7 DKx.7 special cases
	if (!Dx7_special_case)   // No special case, complement or not normally.
	  begin
	     if (code4b_inv_sel)
	       code_4b_mux <= ~sub_block_4b;
	     else
	       code_4b_mux <= sub_block_4b;
	  end
	
	else     // Special case detected.  Select different 4-bit sub-block.
	  begin
	     if (!code4b_inv_sel)
	       code_4b_mux <= sub_block_4b;
	     else
	       case (sub_block_6b)
		 `D11_x_zer: code_4b_mux <= `DKx_7_neg;
		 `D13_x_zer: code_4b_mux <= `DKx_7_neg;
		 `D14_x_zer: code_4b_mux <= `DKx_7_neg;
		 `D17_x_zer: code_4b_mux <= `Dx_7_neg;
		 `D18_x_zer: code_4b_mux <= `Dx_7_neg;
		 `D20_x_zer: code_4b_mux <= `Dx_7_neg;
		 default : code_4b_mux  <= sub_block_4b;
	       endcase // case(sub_block_6b)
	  end // else: !if(!Dx7_special_case)
     end // always @ (*)
   
          
     

endmodule // encode_8b10b
