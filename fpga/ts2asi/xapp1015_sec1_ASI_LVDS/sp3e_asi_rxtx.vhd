-------------------------------------------------------------------------------- 
-- Copyright (c) 2007 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: sp3e_asi_rxtx.vhd,v $
--  /   /        Date Last Modified:  $Date: 2008-04-11 10:21:32-06 $
-- /___/   /\    Date Created: Mar 29, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: sp3e_asi_rxtx.vhd,v $
-- Revision 1.1  2008-04-11 10:21:32-06  hutching
-- Converted to clock enables, added sync reset generators.
--
-- Revision 1.0  2007-06-15 10:50:43-06  hutching
-- Initial revision
--
--
-------------------------------------------------------------------------------- 
--   
--   XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" 
--   AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND 
--   SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, 
--   OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, 
--   APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION 
--   THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT, 
--   AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE 
--   FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY 
--   WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE 
--   IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR 
--   REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF 
--   INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
--   FOR A PARTICULAR PURPOSE. 
--
-------------------------------------------------------------------------------- 

-------------------------------------------------------------------------------
-- Module Description:
-- Top level module for SP3E ASI design.  Includes Rx, Tx with pass-thru mode
-- and stimulus generation mode.  Design targeted at CTXIL206 SDV board.
-- 

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

library UNISIM;
use UNISIM.vcomponents.all;

-------------------------------------------------------------------------------

entity sp3e_asi_rxtx is
  
  port (
    clk_135p_P        : in  std_logic;   -- 135MHz LVDS input clock p-side
    clk_135n_P        : in  std_logic;   -- 135MHz LVDS input clock n-side
    
    pb0_rst_P         : in  std_logic;   -- Reset push button
    pb1_force_err_P   : in  std_logic;   -- unused
    pb2_clear_err_P   : in  std_logic;   -- Clear error LEDs
    pb3_reframe_P     : in  std_logic;   -- Force ASI framer to reframe
    sync_mode_P       : in  std_logic;   -- single/double comma framing pattern
    stimgen_en        : in  std_logic;   -- Select stimulus/pass-thru mode
    
    asi_rxdatap_P     : in  std_logic;   -- ASI Rx input p-side
    asi_rxdatan_P     : in  std_logic;   -- ASI Rx input n-side
    
    dcm_locked_grn_P  : out std_logic;   -- DCM locked LED
    dcm_locked_red_P  : out std_logic;   -- DCM unlocked LED
    framed_P          : out std_logic;   -- ASI framed LED
    framed_not_P      : out std_logic;   -- ASI Rx not framed LED
    disp_err_grn_P    : out std_logic;   -- No disparity error LED
    code_err_grn_P    : out std_logic;   -- No code error LED
    disp_err_red_P    : out std_logic;   -- disparity error LED
    code_err_red_P    : out std_logic;   -- code err LED
    checker_err_red_P : out std_logic;   -- pattern check error LED
    checker_err_grn_P : out std_logic;   -- pattern check OK LED
    asi_tx_active_P   : out std_logic;   -- ASI Tx mode/active
    
    xo_sdi_fsel_P     : out std_logic;   -- VCXO Freq. select
    
    asi_txdatap_P     : out std_logic;   -- ASI LVDS output p-side
    asi_txdatan_P     : out std_logic);  -- ASI LVDS output n-side

end sp3e_asi_rxtx;

architecture rtl of sp3e_asi_rxtx is

  -- Attribute declarations
  attribute route:STRING;
  attribute syn_keep:BOOLEAN;

  -- Clock generation
  component CLKGEN 
  port(CLK135_IN:in STD_LOGIC;                 -- 135MHz Input Clock
       RST:in STD_LOGIC;                         -- async reset, active high
       CLK270:out STD_LOGIC_VECTOR(3 downto 0);  -- 270MHz 4-phase Clocks
       CLK135:out STD_LOGIC;            -- 135MHz 0-degree phase clock output.
       CLK54:out STD_LOGIC;                      -- 54MHz Parallel Clock
       LOCKED:out STD_LOGIC_VECTOR(3 downto 0)); -- high when DCMs are locked
  end component;

  -- TX clock enable generation
  component ce_gen 
  port (
    clk : in  std_logic;
    rst : in  std_logic;
    ce  : out std_logic);
  end component;

  -- ASI Rx
  component  rxdata_path 
  port (
    clk_270_4phase : in  std_logic_vector (3 downto 0);
    clk_54         : in  std_logic;
    user_clk       : in  std_logic;
    reset          : in  std_logic;
    sdata_in       : in  std_logic;     -- serial input data
    rx_fifo_rden   : in  std_logic;
    reframe_req    : in  std_logic;     -- Force ASI reframe
    sync_mode      : in  std_logic;     -- ASI sync pattern select
    dout_8b        : out std_logic_vector (7 downto 0);  -- 8-bit output
    kchar_out      : out std_logic;     -- Comma char out flag
    fifo_empty     : out std_logic;     -- testing only
    fifo_full      : out std_logic;     -- testing only
    code_err       : out std_logic;     -- 8b10b code error
    disp_err       : out std_logic;     -- 8b10b disparity error
    framed_asi     : out std_logic);    -- Indicates Rx is framed

  end component;

  -- Stream Checker
  component checker 
  port (
    user_clk    : in  std_logic;
    ce          : in  std_logic;
    fifo_empty  : in  std_logic;
    fifo_data   : in  std_logic_vector (7 downto 0);
    fifo_kchar  : in  std_logic;
    clear_err   : in  std_logic;
    asi_rx_err  : in  std_logic;
    checker_err : out std_logic);
  end component;

  -- ASI Tx
  component txdata_path 
  port (
    clk        : in  std_logic;   -- word-rate clock or bit-rate clock 
    ce         : in  std_logic;   -- clock enable for ce
    rst        : in  std_logic;   -- synchronous reset
    din_8b     : in  std_logic_vector (7 downto 0);  -- 8-bit un-encoded input data
    kchar_in   : in  std_logic;         -- indicates current byte is comma
    force_disp : in  std_logic;         -- Force encoder disparity - unused
    asi_out    : out std_logic);        -- Encoded/Serialized ASI output
  end component;

  -- Stream generator
  component asi_stimulus
  port (
    clk       : in  std_logic;
    ce        : in  std_logic;
    rst       : in  std_logic;
    asi_dout  : out std_logic_vector (7 downto 0);
    comma_out : out std_logic);
  end component;

  component syncrst_gen 
  port (
    clk        : in  std_logic;
    rst_in     : in  std_logic;    -- External synchronous reset
    dcm_locked : in  std_logic;    -- DCM Lock signal
    rst        : out std_logic);   -- Synchronous reset, held until DCM locked
  end component;

  -- Internal Signals
  signal      reframe : std_logic;
  signal      framed : std_logic;
  signal      clk_270_4phase : std_logic_vector (3 downto 0);
  signal      dcms_locked : std_logic_vector (3 downto 0);
  signal      clk_135_ibufg : std_logic;
  signal      clk_54 : std_logic;
  signal      code_err : std_logic;
  signal      disp_err : std_logic;
  signal      rd_fifo_empty : std_logic;
  signal      fifo_dout : std_logic_vector (7 downto 0);
  signal      fifo_kchar_out : std_logic;
  signal      asi_rx_err : std_logic;
  signal      checker_err : std_logic;
  signal      rxrst : std_logic;
  signal clk_135 : std_logic;
  signal syncrst_135 : std_logic;
  signal syncrst_54 : std_logic;
   

   -- ASI Transmitter signals
   signal      force_err : std_logic;         
   signal      asi_txdata : std_logic;
   signal      txrst : std_logic;
   signal      txce : std_logic;
   signal      tx_ce_sr : std_logic_vector (4 downto 0) := "00001";
   signal      txclk_135 : std_logic;
   signal      stimulus_data : std_logic_vector (7 downto 0);
   signal      asi_txdata_out : std_logic_vector (7 downto 0);
   signal      asi_txcomma : std_logic;
   signal      stimulus_comma : std_logic;


   -- LED blink counter
   signal tx3_led_blinkcnt : unsigned (23 downto 0) := (others => '0');
   signal tx3_led_blink : std_logic := '0';
   signal tx3_led_passthru : std_logic := '0';

  -- ASI serial input data.  Directed routing string attribute important.  Net
  -- must reach all 8 sample f/fs with no skew.
  signal asi_datain : std_logic;        -- Serial input data
  attribute syn_keep of asi_datain : signal is TRUE;
  attribute route of asi_datain : signal is "{3;1;3s1600efg400;691f401c!-1;94560;130400;S!0;-159;0!1;1680;"&
                                        "-1400!1;-49;-1263!1;-1724;-1416!2;1800;-1248!2;1800;-1592!3;73;-2081!3;"&
                                        "73;-2425!4;1748;-1240!4;-1708;-1240!4;1748;-1584!4;-1708;-1584!5;327;0;L!"&
                                        "6;327;0;L!7;167;0;L!8;167;0;L!9;327;0;L!10;327;0;L!11;327;0;L!12;327;0;L!}";


  -- Registers
  -- Output Registers for error LEDs
   signal code_err_R : std_logic;
   signal disp_err_R : std_logic;
   signal checker_err_R : std_logic;

   -- Double Synchronizing registers for input push buttons.  Not debounced...
   signal pb2_clear_err_rxR1 : std_logic;
   signal pb2_clear_err_rxR2 : std_logic;
   signal pb2_clear_err_usrR1 : std_logic;
   signal pb2_clear_err_usrR2 : std_logic;
   signal pb3_reframe_R1 : std_logic;
   signal pb3_reframe_R2 : std_logic;
   signal pb1_force_err_R : std_logic_vector (1 downto 0);
   signal pb0_rst_R : std_logic_vector (1 downto 0);
   signal txrst_usr_R : std_logic_vector (1 downto 0);
   

begin  -- rtl

  -- Synchronous process for error LEDs - write clock domain
   CLK54W: process (clk_54)
   begin  -- process CLK54
     if clk_54'event and clk_54 = '1' then  -- rising clock edge
       pb0_rst_R(0)       <= pb0_rst_P;
       pb0_rst_R(1)       <= pb0_rst_R(0);
       pb2_clear_err_rxR1 <= pb2_clear_err_P;
       pb2_clear_err_rxR2 <= pb2_clear_err_rxR1;
       pb3_reframe_R1     <= pb3_reframe_P;
       pb3_reframe_R2     <= pb3_reframe_R1;
      if (pb2_clear_err_rxR2 = '0') then    -- Clear error LEDs
        code_err_R <= '0';   
        disp_err_R <= '0';
      elsif (code_err = '1') then
        code_err_R <= '1';
      elsif (disp_err = '1') then
        disp_err_R <= '1';
      end if;
     end if;
   end process CLK54W;
	
   -- Synchronous process for error LEDs - read clock domain
   CLK135R: process (txclk_135)
   begin  -- process CLK135R
     if txclk_135'event and txclk_135 = '1' then  -- rising clock edge
       pb2_clear_err_usrR1 <= pb2_clear_err_P;
       pb2_clear_err_usrR2 <= pb2_clear_err_usrR1;
       pb1_force_err_R(0)  <= pb1_force_err_P;
       pb1_force_err_R(1)  <= pb1_force_err_R(0);
       txrst_usr_R(0)      <= pb0_rst_P;
       txrst_usr_R(1)      <= txrst_usr_R(0);
       if (pb2_clear_err_usrR2 = '0') then
         checker_err_R <= '0';
       elsif (checker_err = '1') then
         checker_err_R <= '1';
       end if;
     end if;
   end process CLK135R;

   -- Continuous Assignments
   rxrst <=  not pb0_rst_R(1);
   reframe <=  not pb3_reframe_R2;
   dcm_locked_grn_P <= not (dcms_locked(3) and dcms_locked(2) and dcms_locked(1) and dcms_locked(0));
   dcm_locked_red_P <= (dcms_locked(3) and dcms_locked(2) and dcms_locked(1) and dcms_locked(0));
   xo_sdi_fsel_P <= '1';      -- Set input clock for 135MHz,  0 = 270MHz.
   framed_P <= not framed;    -- Provides positive framed indicator (Green LED)
   framed_not_P <= framed;     -- Provides positive NOT framed indicator (Red LED)
   disp_err_red_P <= not disp_err_R;   -- Provides positive disparity error indicator
   disp_err_grn_P <= disp_err_R;       -- Provides positive NOT disparity error
   code_err_red_P <= not code_err_R;   -- Ditto
   code_err_grn_P <= code_err_R;       -- Ditto
   checker_err_grn_P <= checker_err_R;
   checker_err_red_P <= not checker_err_R;
   asi_rx_err <= disp_err or code_err;
   force_err <= not pb1_force_err_R(1);
   txrst <= not txrst_usr_R(1);


   -- 2:1 mux to select transmitter mode (pass-thru or stimulus generator)
   asi_txdata_out  <= fifo_dout when stimgen_en = '1' else stimulus_data;
   asi_txcomma     <= fifo_kchar_out when stimgen_en = '1' else stimulus_comma;
   asi_tx_active_P <= tx3_led_passthru when stimgen_en = '1' else tx3_led_blink;

   --
   -- Instantiate input/output buffers
   --
   DATAIN_IBUFDS : IBUFDS port map (O  => asi_datain,
                                    I  => asi_rxdatap_P,
                                    IB => asi_rxdatan_P);

   asi_data_obuf : OBUFDS port map (I => asi_txdata,
                                    O => asi_txdatap_P,
                                    OB => asi_txdatan_P);

   --
   -- Instantiate global clock input buffers
   --
   CLKIN_IBUFGDS_135 : IBUFGDS port map (O  => clk_135_ibufg,
                                         I  => clk_135p_P,
                                         IB => clk_135n_P);

   BUFG_135 : BUFG port map (I => clk_135_ibufg,
                             O => clk_135);

   -- Instantiate clock generation module - VHDL only
   asi_clkgen : clkgen port map (clk270    => clk_270_4phase,
                                 clk54     => clk_54,
                                 clk135    => txclk_135,
                                 locked    => dcms_locked,
                                 clk135_in => clk_135,
                                 rst       => not pb0_rst_P);

   -- Synchronous reset generators
   rx_rstgen : syncrst_gen port map (
    clk        => clk_54,
    rst_in     => rxrst,
    dcm_locked => dcms_locked(0),
    rst        => syncrst_54);

   tx_rstgen : syncrst_gen port map (
    clk        => txclk_135,
    rst_in     => txrst,
    dcm_locked => dcms_locked(0),
    rst        => syncrst_135);
   

   -- Tx clock enable generator
   TXCE_GEN: process (txclk_135)
  begin  -- process TXCE_GEN
    if txclk_135'event and txclk_135 = '1' then  -- rising clock edge
      if syncrst_135 = '1' then
        txce <= '0';
        tx_ce_sr <= "00001";
      else
        tx_ce_sr <= tx_ce_sr(3 downto 0) & tx_ce_sr(0);
        tx_ce_sr(0) <= tx_ce_sr(4);
        txce <= tx_ce_sr(4);
      end if;
    end if;
  end process TXCE_GEN;


   --
   -- ASI Rx
   --
   asi_receiver : rxdata_path port map (clk_270_4phase => clk_270_4phase,
                                        clk_54         => clk_54,
                                        user_clk       => txclk_135,
                                        reset          => syncrst_54,
                                        sdata_in       => asi_datain,
                                        rx_fifo_rden   => txce,
                                        reframe_req    => reframe,
                                        sync_mode      => sync_mode_P,
                                        dout_8b        => fifo_dout,
                                        kchar_out      => fifo_kchar_out,
                                        fifo_empty     => rd_fifo_empty,
                                        fifo_full      => open,
                                        code_err       => code_err,
                                        disp_err       => disp_err,
                                        framed_asi     => framed);

   -- Stream Checker
   pkt_checker : checker port map (user_clk    => txclk_135,
                                   ce          => txce,
                                   fifo_empty  => rd_fifo_empty,
                                   fifo_data   => fifo_dout,
                                   fifo_kchar  => fifo_kchar_out,
                                   clear_err   => not pb2_clear_err_P,
                                   asi_rx_err  => asi_rx_err,
                                   checker_err => checker_err);

   
   --
   -- ASI Tx Section
   --

   -- Tx 
   asi_tx : txdata_path port map (clk        => txclk_135,
                                  ce         => txce,
                                  rst        => syncrst_135,
                                  din_8b     => asi_txdata_out,
                                  kchar_in   => asi_txcomma,
                                  force_disp => force_err,
                                  asi_out    => asi_txdata);

   

   -- Stream Generator
   pattgen : asi_stimulus port map (asi_dout  => stimulus_data,
                                    comma_out => stimulus_comma,
                                    clk       => txclk_135,
                                    ce        => txce,
                                    rst       => syncrst_135);

   -- Quick LED Blinker
   BLINK: process (clk_54)
   begin  -- process BLINK
     if clk_54'event and clk_54 = '1' then  -- rising clock edge
       tx3_led_blinkcnt <= tx3_led_blinkcnt + 1;
       if (tx3_led_blinkcnt = "111111111111111111111111") then
         tx3_led_blink <=  not tx3_led_blink;
       end if;
     end if;
   end process BLINK;

   
end rtl;
