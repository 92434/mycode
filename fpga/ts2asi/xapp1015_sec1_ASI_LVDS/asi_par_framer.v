//------------------------------------------------------------------------------ 
// Copyright (c) 2005 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: asi_par_framer.v,v $
//  /   /        Date Last Modified:  $Date: 2008-04-11 10:25:42-06 $
// /___/   /\    Date Created: Dec 11, 2006 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History: 
// $Log: asi_par_framer.v,v $
// Revision 1.1  2008-04-11 10:25:42-06  hutching
// Converted existing resets to sync, added reset to byte counter.
//
// Revision 1.0  2007-06-06 13:26:07-06  hutching
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

`timescale 1ns / 1ns

module asi_par_framer (/*AUTOARG*/
   // Outputs
   q, framed, nsp,
   // Inputs
   sclk, rst, ce, d, frame_en, reframe, sync_mode
   );


// IO definitions   
input               sclk;
input               rst;
input               ce;
input   [9:0]       d;
input               frame_en;
input               reframe;
input 	    sync_mode;  // Changed from parameter to input.  JLH.
output  [9:0]       q;
output              framed;
output              nsp;

// Internal registers
reg     [9:0]       in1_reg;        // input register 1 
reg     [3:0]       offset_reg;     // offset register
reg     [18:0]      barrel_in;      // input register for the barrel shifter
reg                 nsp;            // nsp output register

// Internal signals
wire    [18:0]      in_vector;      // concatenation of the four input registers
reg     [9:0]       comma_match;    // offsets in in_vector[18:0] match comma
wire                comma_detected; // asserted when comma symbol is detected
reg                 comma_error;    // > one offset matched the comma symbol
reg     [3:0]       offset_val;     // calculated offset value 
reg     [9:0]       barrel_out;     // output of barrel shifter
wire                new_offset;     // mismatch between offset_val and offset_reg
wire    [20:0]      bs_in;          // barrel shifter input (2 MSBs are dummy)
reg     [12:0]      bs_m1;          // output of first MUX level in barrel shifter
wire    [1:0]       bs_sm;          // barrel shifter first level MUX select bits
wire    [1:0]       bs_sl;          // barrel shifter second level MUX select bits
wire    [9:0]       q_inv;
integer             i, j;           // for loop variables

// Framing State Machine Signals
reg     [5:0]       byte_counter;
wire                byte_counter_tc;
reg                 byte_counter_rst; 
reg                 byte_counter_inc; 
reg                 framed;

parameter [1:0]
    FRAME_START                 = 2'b00,
    FRAME_SEEK_2ND_SYNC_BYTE    = 2'b01,
    FRAME_GO                    = 2'b10;

reg     [1:0]       Frame_Current_State = FRAME_START;
reg     [1:0]       Frame_Next_State;



//
// Input register
//
always @ (posedge sclk)
    if (rst)
        in1_reg <= 0;
    else if (ce)
        in1_reg <= d;
    
//
// comma detector and offset encoder
//
// This code detects comma characters at any starting bit position. Ten
// comparators are produced, one for each starting bit position.
// 
assign in_vector = {d[8:0],in1_reg};

always @ *
    for (i = 0; i < 10; i = i + 1)
        comma_match[i] <=  (~in_vector[i + 9] &  in_vector[i + 8] & 
                            ~in_vector[i + 7] &  in_vector[i + 6] & 
                             in_vector[i + 5] &  in_vector[i + 4] & 
                             in_vector[i + 3] &  in_vector[i + 2] & 
                            ~in_vector[i + 1] & ~in_vector[i]) |
                           ( in_vector[i + 9] & ~in_vector[i + 8] & 
                             in_vector[i + 7] & ~in_vector[i + 6] & 
                            ~in_vector[i + 5] & ~in_vector[i + 4] & 
                            ~in_vector[i + 3] & ~in_vector[i + 2] & 
                             in_vector[i + 1] &  in_vector[i]);

//
// If any of the bits in comma_match_all are asserted, the assert comma_detected        
//
assign comma_detected = |comma_match;

//
// This code asserts comma_error if more than one bit is set in comma_match.
// It also also encodes the comma_match vector into a binary offset code.
//
always @ *
    case (comma_match)
        10'b00_0000_0000: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h0;
            end

        10'b00_0000_0001: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h0;
            end

        10'b00_0000_0010: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h1;
            end

        10'b00_0000_0100: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h2;
            end

        10'b00_0000_1000: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h3;
            end

        10'b00_0001_0000: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h4;
            end

        10'b00_0010_0000: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h5;
            end

        10'b00_0100_0000: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h6;
            end

        10'b00_1000_0000: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h7;
            end

        10'b01_0000_0000: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h8;
            end

        10'b10_0000_0000: 
            begin
                comma_error  <= 0;
                offset_val <= 4'h9;
            end

        default:          
            begin
                comma_error  <= 1;
                offset_val <= 4'h0;
            end
    endcase

//
// offset_reg: barrel shifter offset register
//
// The offset_reg loads the offset_val whenever comma_detected is
// asserted and comma_error is not asserted and frame_en is asserted.
//
always @ (posedge sclk)
    if (rst)
        offset_reg <= 0;
    else if (ce) 
        if (comma_detected & ~comma_error & (frame_en | ~framed))
            offset_reg <= offset_val;

//
// New start position detector
// 
// A comparison between offset_val and offset_reg determines if
// the new offset is different than the current one. If there is
// a mismatch and frame_en is not asserted, then the nsp output
// will be asserted.
//
assign new_offset = offset_val != offset_reg;

always @ (posedge sclk)
    if (rst)
        nsp <= 1;
    else if (ce)        
        if (comma_detected & ~comma_error)
            nsp <= ~frame_en & new_offset;

//
// barrel_in: barrel shifter input register
//
// This register implements a pipeline delay stage so that the
// barrel shifter's data input matches the delay on the offset
// input caused by the offset_reg.
//
always @ (posedge sclk)
    if (rst)
        barrel_in <= 0;
    else if (ce)
        barrel_in <= {d[8:0], in1_reg};

//
// barrel shifter
//
// The barrel shifter extracts a 10-bit field from the 19-bit barrel_in vector.
// The bits extracted depend on the value of the offset_reg. If offset_reg is
// zero, barrel_in[9:0] are output by the barrel shifter. At the other extreme,
// an offset_reg value of 9 causes the barrel shifter to output barrel_in[18:9].
//
// The barrel shifter can be described in many ways. One way is a simple
// shift assignment as shown below:
//
// assign barrel_out = barrel_in >> offset_reg;
//
// This produces excellent results with FPGA Express, but doesn't produce such
// good results with the current version of XST.
//
// Another method is to explicitly code the barrel shifter as two levels of
// MUXes. The first level consists of thirteen 3-input MUXes which provide
// the course shifting in increments of four bits. The second level consists of
// ten 4-input MUXes which provide fine shifting (1-bit increments). This method
// produces a much better result with XST, but produces a very small (4 LUTs)
// larger implementation with FPGA Express.
//
assign bs_in = {2'b00, barrel_in};
assign bs_sm = offset_reg[3:2];
assign bs_sl = offset_reg[1:0];

always @ *
    for (i = 0; i < 13; i = i + 1)
        case (bs_sm)
            2'b00: bs_m1[i] <= bs_in[i];
            2'b01: bs_m1[i] <= bs_in[i + 4];
            2'b10: bs_m1[i] <= bs_in[i + 8]; 
            2'b11: bs_m1[i] <= 1'b0;
        endcase

always @ *
    for (j = 0; j < 10; j = j + 1)
        case (bs_sl)
            2'b00: barrel_out[j] <= bs_m1[j];
            2'b01: barrel_out[j] <= bs_m1[j + 1];
            2'b10: barrel_out[j] <= bs_m1[j + 2];
            2'b11: barrel_out[j] <= bs_m1[j + 3];
        endcase

//
// The output of the module is the barrel shifter output
//
assign q_inv = barrel_out;
assign q = {q_inv[0], q_inv[1], q_inv[2], q_inv[3], q_inv[4], q_inv[5], 
            q_inv[6], q_inv[7], q_inv[8], q_inv[9]};

//
// Byte counter
//
always @ (posedge sclk)
    if (rst)
      byte_counter <= 0;
    else if (ce)
    begin
        if (byte_counter_rst)
            byte_counter <= 0;
        else if (byte_counter_inc)
            byte_counter <= {byte_counter[4:0], 1'b1};
    end

assign byte_counter_tc = byte_counter[5];

//
// Idle Character Detection
//
// Current State Logic
always @(posedge sclk)
    if (rst)
        Frame_Current_State <= FRAME_START ; 
    else if (ce)
        Frame_Current_State <= Frame_Next_State ; 

// Output Forming Logic
always @ *
begin
    byte_counter_rst = 1'b0;
    byte_counter_inc = 1'b0;
    framed = 1'b0;
            
    case (Frame_Current_State)
     //
     // Look for the first sync byte
     //
        FRAME_START :
            byte_counter_rst = 1'b1;
    
     //
     // Case where sync_mode = '1' (Lock declared only when 2 sync bytes are 
     // found on the same byte boundary within a 5 byte window). If first sync 
     // byte found, look for the 2nd sync byte. Start byte counter. If byte 
     // counter reaches 4 and 2nd sync byte not found, go back to FRAME_START.
     //
        FRAME_SEEK_2ND_SYNC_BYTE :
            if (byte_counter_tc/*[2]*/ | (comma_detected & ~new_offset))
                byte_counter_rst = 1'b1;
            else
                byte_counter_inc = 1'b1;

     // 
     // Link is framed (locked). Real data is being received.
     // If 'reframe' signal is asserted on a decode error, go back to 
     // FRAME_START and start looking for the 1st sync byte. 
     //                                                                
        FRAME_GO :
            framed = ~reframe;
    endcase 
end
  
// Next State Logic
always @ *
    case (Frame_Current_State)
         //
         // Look for the first sync byte
         //
         FRAME_START :
            if (comma_detected)
                Frame_Next_State <= sync_mode ? FRAME_SEEK_2ND_SYNC_BYTE : FRAME_GO;
            else
                Frame_Next_State <= FRAME_START ;
                 
         //
         // Case where sync_mode = '1' (Lock declared only when 2 sync bytes are
         // found on the same byte boundary within a 5 byte window). If first 
         // sync byte found, look for the 2nd sync byte. Start byte counter. If 
         // byte counter reaches 4 and 2nd sync byte not found, go back to 
         // FRAME_START.
         //
         FRAME_SEEK_2ND_SYNC_BYTE :
            // If byte_counter = 4 and 2nd sync byte not found (2nd sync byte 
            // not found within a 5 byte window per ASI spec) , go back to 
            // FRAME_START state
            if (byte_counter_tc/*[2]*/)
                Frame_Next_State <= FRAME_START ; 
            else if (comma_detected & ~new_offset)
                Frame_Next_State <= FRAME_GO ; 
            else
                Frame_Next_State <= FRAME_SEEK_2ND_SYNC_BYTE ; 
         
         // 
         // Link is framed (locked). Real data is being received.
         // If 'reframe' signal is asserted on a decode error, go back to 
         // FRAME_START and start looking for the 1st sync byte. 
         //                                                                
         FRAME_GO :
            if (reframe)
                Frame_Next_State <= FRAME_START ; 
            else
                Frame_Next_State <= FRAME_GO ; 
         
         default :
                Frame_Next_State <= FRAME_START ; 
      endcase 

endmodule
