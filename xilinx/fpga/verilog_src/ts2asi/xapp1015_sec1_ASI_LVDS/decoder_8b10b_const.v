//------------------------------------------------------------------------------ 
// Copyright (c) 2007 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: decoder_8b10b_const.v,v $
//  /   /        Date Last Modified:  $Date: 2007-06-06 13:26:08-06 $
// /___/   /\    Date Created: Jan 19, 2007 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: decoder_8b10b_const.v,v $
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

/* File description:  This file contains all the `define constants used by the
 decoder_8b10b module */

`ifndef DECODER_8B10B_CONST_V

`define DECODER_8B10B_CONST_V


// `defines related to disparity functions
`define NEG_RD        1'b0           // -1 Negative running disparity
`define POS_RD        1'b1           // +1 Positive running disparity
`define POS_DISP      2'b10          // +2 Positive disparity
`define NEG_DISP      2'b01          // -2 Negative disparity
`define ZERO_DISP     2'b00          //  0 No disparity - balanced code word

// `defines for the error codes
`define DISPARITY_ERROR_4B  1'b1
`define DISPARITY_ERROR_6B  1'b1
`define DISPARITY_ERROR_10B 1'b1
`define DISPARITY_OK_4B     1'b0
`define DISPARITY_OK_6B     1'b0
`define DISPARITY_OK_10B    1'b0

// `defines related to 6b/5b sub-block decoder.  These constants define the lookup
// table values for the 6-bit DX.Y sub-blocks.  Only the X value is defined.  The
// notation reflects this with D0_x_pos/neg/zer.  The D0 means this is the D0 code
// word.  The _x_ denotes that Y is a don't care.  Y is represented in the 4b/3b 
// decoder constants.  The neg/pos/zer denotes the signal polarity of the code 
// word.  Most of the 8b/10b codes are DC balanced.  Those that are not, can have
// two possible values depending on the polarity which is determined real time by
// the encoder.  The lookup tables must account for these cases but they decode to
// the same 5b output.  These values are taken directly from the original IBM 
// Journal article by Widmer and Franaszek.  Note that these are defined MSB-LSB 
// which is the reverse order of the bits in the journal article. 
// //   Name            iedcba
//      ----------------------
`define D0_x_neg     6'b000110
`define D0_x_pos     6'b111001
`define D1_x_neg     6'b010001
`define D1_x_pos     6'b101110
`define D2_x_neg     6'b010010
`define D2_x_pos     6'b101101
`define D3_x_zer     6'b100011
`define D4_x_neg     6'b010100
`define D4_x_pos     6'b101011
`define D5_x_zer     6'b100101
`define D6_x_zer     6'b100110
`define D7_x_zer1    6'b000111
`define D7_x_zer2    6'b111000
`define D8_x_neg     6'b011000
`define D8_x_pos     6'b100111
`define D9_x_zer     6'b101001
`define D10_x_zer    6'b101010
`define D11_x_zer    6'b001011
`define D12_x_zer    6'b101100
`define D13_x_zer    6'b001101
`define D14_x_zer    6'b001110
`define D15_x_neg    6'b000101
`define D15_x_pos    6'b111010
`define D16_x_neg    6'b001001
`define D16_x_pos    6'b110110
`define D17_x_zer    6'b110001
`define D18_x_zer    6'b110010
`define D19_x_zer    6'b010011
`define D20_x_zer    6'b110100
`define D21_x_zer    6'b010101
`define D22_x_zer    6'b010110
`define D23K23_x_neg 6'b101000
`define D23K23_x_pos 6'b010111
`define D24_x_neg    6'b001100
`define D24_x_pos    6'b110011
`define D25_x_zer    6'b011001
`define D26_x_zer    6'b011010
`define D27K27_x_neg 6'b100100
`define D27K27_x_pos 6'b011011
`define D28_x_zer    6'b011100
`define D29K29_x_neg 6'b100010
`define D29K29_x_pos 6'b011101
`define D30K30_x_neg 6'b100001
`define D30K30_x_pos 6'b011110
`define D31_x_neg    6'b001010
`define D31_x_pos    6'b110101
`define K28_x_neg    6'b000011
`define K28_x_pos    6'b111100  



// `defines for the 4b/3b lookup table
//      Name           jhgf
//      -------------------
`define DKx_0_neg   4'b0010
`define DKx_0_pos   4'b1101
`define DKx_1_zer   4'b1001
`define DKx_1_zer2  4'b0110
`define DKx_2_zer   4'b1010
`define DKx_2_zer2  4'b0101
`define DKx_3_zer1  4'b0011
`define DKx_3_zer2  4'b1100
`define DKx_4_neg   4'b0100
`define DKx_4_pos   4'b1011
`define DKx_5_zer   4'b0101
`define DKx_5_zer2  4'b1010
`define DKx_6_zer   4'b0110
`define DKx_6_zer2  4'b1001
`define Dx_7_neg    4'b1000
`define Dx_7_pos    4'b0111
`define DKx_7_neg   4'b0001
`define DKx_7_pos   4'b1110


// `defines for the remaining special K-code characters.  Note that K28.x is 
// unique in that the first 6-bit patterns cannot occur with any other code 
// symbol.  The others can only be identified by examining all 10-bits.
//
//      Name          jhgfiedcba
`define K23_7_neg 10'b1110101000
`define K23_7_pos 10'b0001010111
`define K27_7_neg 10'b1110100100
`define K27_7_pos 10'b0001011011
`define K29_7_neg 10'b1110100010
`define K29_7_pos 10'b0001011101
`define K30_7_neg 10'b1110100001
`define K30_7_pos 10'b0001011110
 
`endif