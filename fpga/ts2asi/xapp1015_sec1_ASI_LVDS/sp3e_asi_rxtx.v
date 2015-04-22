//------------------------------------------------------------------------------ 
// Copyright (c) 2007 Xilinx, Inc. 
// All Rights Reserved 
//------------------------------------------------------------------------------ 
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /   Vendor: Xilinx 
// \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
//  \   \        Filename: $RCSfile: sp3e_asi_rxtx.v,v $
//  /   /        Date Last Modified:  $Date: 2008-04-11 10:22:07-06 $
// /___/   /\    Date Created: Jan 12, 2007 
// \   \  /  \ 
//  \___\/\___\ 
// 
//
// Revision History:
//
//
// $Log: sp3e_asi_rxtx.v,v $
// Revision 1.1  2008-04-11 10:22:07-06  hutching
// Converted for clock enables, added sync reset generators.
//
// Revision 1.0  2007-06-12 14:14:33-06  hutching
// Initial revision
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

/* Top level module for DVB ASI receiver, transmitter single channel
   design targeted at CTXIL206  */

`timescale 1ns / 1ns

module sp3e_asi_rxtx (/*AUTOARG*/
   // Outputs
   dcm_locked_grn_P, dcm_locked_red_P, framed_P, framed_not_P,
   disp_err_grn_P, code_err_grn_P, disp_err_red_P, code_err_red_P,
   checker_err_red_P, checker_err_grn_P, asi_tx_active_P,
   xo_sdi_fsel_P, asi_txdatap_P, asi_txdatan_P,
   // Inputs
   clk_135p_P, clk_135n_P, pb0_rst_P, pb1_force_err_P,
   pb2_clear_err_P, pb3_reframe_P, sync_mode_P, stimgen_en,
   asi_rxdatap_P, asi_rxdatan_P
   );

   // Clock Inputs
   input clk_135p_P;         // 135MHz p-side LVDS input receiver clock
   input clk_135n_P;         // 135MHz n-side LVDS input receiver clock

   // External Pushbuttons on Spartan3E SDV Board
   input pb0_rst_P;          // Reset pushbutton
   input pb1_force_err_P;    // unused
   input pb2_clear_err_P;    // Clear error LED
   input pb3_reframe_P;      // Force ASI receiver to reframe

   // Dipswitch inputs
   input sync_mode_P;       // Selects single or double comma sync patters.  '1' = double
   input stimgen_en;        // Enables pass-through mode 
   
   

   // Other Inputs
   input asi_rxdatap_P;      // ASI Rx input p-side
   input asi_rxdatan_P;      // ASI Rx input n-side

   // Outputs
   output dcm_locked_grn_P;  // DCM Locked status signal
   output dcm_locked_red_P;

   
   

   // LED Outputs
   output framed_P;         // Indicated ASI rx has successfully framed
   output framed_not_P;     // Indicates link not framed. 
   output disp_err_grn_P;   // Indicates disparity no error occurred in the 8b10b decoder
   output code_err_grn_P;   // Indicates no code error in 8b10b decoder
   output disp_err_red_P;
   output code_err_red_P;
   output checker_err_red_P;
   output checker_err_grn_P;
   output asi_tx_active_P;
   
   

   // Crystal Oscillator frequency select output
   output xo_sdi_fsel_P;  // Set to 1 for 135MHz input clock.


   // ASI Tx output
   output asi_txdatap_P;         // LVDS ASI outputs
   output asi_txdatan_P;
   
   
   // Internal Wires
   wire      reframe;
   wire      framed;
   wire [3:0]clk_270_4phase;
   wire [3:0] dcms_locked;
   wire       clk_135_ibufg;
   wire       clk_54;
   wire       code_err;
   wire       disp_err;
   wire       rd_fifo_empty;
   wire [7:0] fifo_dout;
   wire       fifo_kchar_out;
   wire       asi_rx_err;
   wire       checker_err;
   wire       rxrst;
   wire       syncrst_54;   // Rx synchronous reset + DCM lock
   wire       syncrst_135;   // Tx synchronous reset + DCM lock
   
   
   

   // ASI Transmitter signals
   wire       force_err;         
   wire       asi_txdata;
   wire       txrst;
   reg        txce;
   reg [4:0]  tx_ce_sr  = 5'b00001;
   wire       txclk_135;
   wire [7:0] stimulus_data;
   wire [7:0] asi_txdata_out;
   wire       asi_txcomma;
   wire       stimulus_comma;


   // LED blinnk counter
   reg [23:0]  tx3_led_blinkcnt = 24'h000000;
   reg 	       tx3_led_blink = 1'b0;
   reg 	       tx3_led_passthru = 1'b0;
   
   
   
   
   
   
   
 
   
   
   // Wires with associated DIRT.  These are low skew nets necessary for proper
   // timing and sampling in the DRU.
   (* syn_keep = "1" *) 
   (* route = {"{3;1;3s1600efg400;691f401c!-1;94560;130400;S!0;-159;0!1;1680;",
              "-1400!1;-49;-1263!1;-1724;-1416!2;1800;-1248!2;1800;-1592!3;73;-2081!3;",
              "73;-2425!4;1748;-1240!4;-1708;-1240!4;1748;-1584!4;-1708;-1584!5;327;0;L!",
              "6;327;0;L!7;167;0;L!8;167;0;L!9;327;0;L!10;327;0;L!11;327;0;L!12;327;0;L!}"} *)
   wire       asi_datain;


   // Output Registers for error LEDs
   reg 	      code_err_R;
   reg 	      disp_err_R;
   reg 	      checker_err_R;

   // Double Synchronizing registers for input push buttons.  Not debounced...
   reg 	      pb2_clear_err_rxR1, pb2_clear_err_rxR2;
   reg 	      pb2_clear_err_usrR1, pb2_clear_err_usrR2;
   reg 	      pb3_reframe_R1, pb3_reframe_R2;
   reg [1:0]  pb1_force_err_R;
   reg [1:0]  pb0_rst_R;
   reg [1:0]  txrst_usr_R;
   
   
   
   
   


   // Synchronous process for error LEDs - write clock domain
   always @(posedge clk_54)
     begin
	pb0_rst_R[0]       <= pb0_rst_P;
	pb0_rst_R[1]       <= pb0_rst_R[0];
	pb2_clear_err_rxR1 <= pb2_clear_err_P;
	pb2_clear_err_rxR2 <= pb2_clear_err_rxR1;
	pb3_reframe_R1     <= pb3_reframe_P;
	pb3_reframe_R2     <= pb3_reframe_R1;
      if (!pb2_clear_err_rxR2)    // Clear error LEDs
	begin
	   code_err_R <= 1'b0;   
	   disp_err_R <= 1'b0;
	end
      else if (code_err)
        code_err_R <= 1'b1;
      else if (disp_err)
        disp_err_R <= 1'b1;
     end // always @ (posedge clk54)

   // Synchronous process for error LEDs - read clock domain
   always @(posedge txclk_135)
     begin
	pb2_clear_err_usrR1 <= pb2_clear_err_P;
	pb2_clear_err_usrR2 <= pb2_clear_err_usrR1;
	pb1_force_err_R[0]  <= pb1_force_err_P;
	pb1_force_err_R[1]  <= pb1_force_err_R[0];
	txrst_usr_R[0]      <= pb0_rst_P;
	txrst_usr_R[1]      <= txrst_usr_R[0];
	
	if (!pb2_clear_err_usrR2)
	  begin
	     checker_err_R <= 1'b0;
	  end
	else if (checker_err)
	  begin
	     checker_err_R <= 1'b1;
	  end
     end // always @ (posedge txclk_27)
   
   

   
   // Continuous Assignments
   assign    rxrst = ~pb0_rst_R[1];
   assign    reframe = ~pb3_reframe_R2;
   assign    dcm_locked_grn_P = ~(dcms_locked[3] & dcms_locked[2] & dcms_locked[1] & dcms_locked[0]);
   assign    dcm_locked_red_P = (dcms_locked[3] & dcms_locked[2] & dcms_locked[1] & dcms_locked[0]);
   assign    xo_sdi_fsel_P = 1'b1;  // Set input clock for 135MHz,  0 = 270MHz.
   assign    framed_P = ~framed;    // Provides positive framed indicator (Green LED)
   assign    framed_not_P = framed; // Provides positive NOT framed indicator (Red LED)
   assign    disp_err_red_P = ~disp_err_R;   // Provides positive disparity error indicator
   assign    disp_err_grn_P = disp_err_R;     // Provides positive NOT disparity error
   assign    code_err_red_P = ~code_err_R;    // Ditto
   assign    code_err_grn_P = code_err_R;     // Ditto
   assign    checker_err_grn_P = checker_err_R;
   assign    checker_err_red_P = ~checker_err_R;
   assign    asi_rx_err = disp_err | code_err;
   assign    force_err = ~pb1_force_err_R;

   assign    txrst = ~txrst_usr_R[1];


   // 2:1 mux to select transmitter mode (pass-thru or stimulus generator
   assign    asi_txdata_out = ~stimgen_en ? stimulus_data : fifo_dout;
   assign    asi_txcomma = ~stimgen_en ? stimulus_comma : fifo_kchar_out;
   assign    asi_tx_active_P = ~stimgen_en ? tx3_led_blink : tx3_led_passthru;
   
   


   //
   // Instantiate input buffer for asi input data
   //
   IBUFDS DATAIN_IBUFDS (
      .O(asi_datain), 
	 .I(asi_rxdatap_P), 
	 .IB(asi_rxdatan_P)
	 );


   // LVDS Output buffer for transmitter
   OBUFDS asi_data_obuf (.I(asi_txdata),
			 .O(asi_txdatap_P),
			 .OB(asi_txdatan_P));
   
   //	
   // Instantiate input buffer for input reference clock
   //				   
   IBUFGDS CLKIN_IBUFGDS_135 (
      .O(clk_135_ibufg), 
	 .I(clk_135p_P), 
	 .IB(clk_135n_P)
	 );

   BUFG BUFG_135 (.I(clk_135_ibufg), .O(clk_135));
   


   // Instantiate the clock generation module.  Currently only written in VHDL.
   clkgen asi_clkgen (.clk270(clk_270_4phase),
		      .clk54(clk_54),
		      .clk135(txclk_135),
		      .locked(dcms_locked),
		      .clk135_in(clk_135),
		      .rst(~pb0_rst_P));


   // Synchronous reset generator for Rx
   syncrst_gen rx_rstgen (.clk(clk_54),
			  .rst_in(rxrst),
			  .dcm_locked(dcms_locked[0]),
			  .rst(syncrst_54));

   syncrst_gen tx_rstgen (.clk(txclk_135),
			  .rst_in(txrst),
			  .dcm_locked(dcms_locked[0]),
			  .rst(syncrst_135));
   
   

   //
   // DVB-ASI Receiver
   //
   // The rxdata_path module contains the data recovery unit, the deserializer, asi framer
   // 8b/10b decoder, and rate matching FIFO
   //  		
   rxdata_path asi_receiver (.clk_270_4phase(clk_270_4phase),
			     .clk_54(clk_54),
			     .user_clk(txclk_135),
			     .reset(syncrst_54), 
			     .sdata_in(asi_datain),
			     .rx_fifo_rden(txce), 
			     .reframe_req(reframe),
			     .sync_mode(sync_mode_P),
			     .dout_8b(fifo_dout), 
			     .kchar_out(fifo_kchar_out),
			     .fifo_empty(rd_fifo_empty),
			     .fifo_full(),
			     .code_err(code_err), 
			     .disp_err(disp_err), 
			     .framed_asi(framed));

   // Packet checker
   checker pkt_checker(.user_clk(txclk_135),
		       .ce(txce),
		       .fifo_empty(rd_fifo_empty),
		       .fifo_data(fifo_dout),
		       .fifo_kchar(fifo_kchar_out),
		       .clear_err(!pb2_clear_err_P),
		       .asi_rx_err(asi_rx_err),
		       .checker_err(checker_err));


   //
   //  ASI Transmitter section including stimulus stream generator
   //

   // Tx clock enable generation
   always @(posedge txclk_135)
     begin
	if (syncrst_135)
	  begin
	     txce <= 1'b0;
	     tx_ce_sr <= 5'b00001;
	  end
	else
	  begin
	     tx_ce_sr <= {tx_ce_sr[3:0], tx_ce_sr[0]};
	     tx_ce_sr[0] <= tx_ce_sr[4];
	     txce <= tx_ce_sr[4];
	  end
     end 

   
   // Instantiate ASI Transmitter
   txdata_path asi_tx(.clk(txclk_135),
		      .ce(txce),
		      .rst(syncrst_135),
		      .din_8b(asi_txdata_out),
		      .kchar_in(asi_txcomma),
		      .force_disp(force_err),
		      .asi_out(asi_txdata));

   // Instantiate the stimulus generator
   asi_stimulus pattgen (.asi_dout(stimulus_data),
			 .comma_out(stimulus_comma),
			 .clk(txclk_135),
			 .ce(txce),
			 .rst(syncrst_135));

   // Quick LED blinker.  Indicates transmitter in stimulus mode, not pass-thru mode
  
   
   always @(posedge clk_54)
     begin
	tx3_led_blinkcnt <= tx3_led_blinkcnt + 1'b1;
	if (tx3_led_blinkcnt == 24'hffffff)
	  tx3_led_blink <= ~tx3_led_blink;
     end
   

endmodule
