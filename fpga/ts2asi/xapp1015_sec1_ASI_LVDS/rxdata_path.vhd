-------------------------------------------------------------------------------- 
-- Copyright (c) 2007 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: rxdata_path.vhd,v $
--  /   /        Date Last Modified:  $Date: 2008-04-11 10:24:26-06 $
-- /___/   /\    Date Created: Mar 29, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: rxdata_path.vhd,v $
-- Revision 1.2  2008-04-11 10:24:26-06  hutching
-- Added resets to framer, link controller, FIFO
--
-- Revision 1.1  2007-06-21 12:25:28-06  hutching
-- Modifications for new link controller functionality
--
-- Revision 1.0  2007-06-15 10:46:14-06  hutching
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
-- This module implements the complete DVB-ASI receiver.  It is mostly
-- instantiations of lower level modules with some glue.
--

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

-------------------------------------------------------------------------------

entity rxdata_path is
  
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

end rxdata_path;

architecture rtl of rxdata_path is

  -- Component declarations
  component DRU 
  port(CLK270:in STD_LOGIC_VECTOR(3 downto 0); 
       SI:in STD_LOGIC;                        
       DOUT:out STD_LOGIC_VECTOR(1 downto 0);  
       DV:out UNSIGNED(1 downto 0));           
  end component;

  -- Deserializer
  component DES10
  port(CLK270:in STD_LOGIC;                   
       DI:in STD_LOGIC_VECTOR(1 downto 0);    
       DV:in UNSIGNED(1 downto 0);             
       CLK54:in STD_LOGIC;                     
       BITSLIP:in STD_LOGIC;                   
       DOUT:out STD_LOGIC_VECTOR(9 downto 0);  
       VO:out STD_LOGIC);                      
  end component;

  -- ASI parallel framer component
  component asi_par_framer 
   PORT (
      sclk                    : IN std_logic;   
      rst                     : IN std_logic;   
      ce                      : IN std_logic;   
      d                       : IN std_logic_vector(9 DOWNTO 0);   
      frame_en                : IN std_logic;   
      reframe                 : IN std_logic;
      sync_mode               : in std_logic;
      q                       : OUT std_logic_vector(9 DOWNTO 0);   
      framed                  : OUT std_logic;   
      nsp                     : OUT std_logic);   
  end component;

  -- 8b10b decoder component
  component decoder_8b10b 
    port (
      clk      : in std_logic;
      ce       : in std_logic;
      sinit    : in std_logic;
      d_10b    : in std_logic_vector (9 downto 0);
      is_kcode : out std_logic;
      code_err : out std_logic;
      disp_err : out std_logic;
      q_valid  : out std_logic;
      q_8b     : out std_logic_vector (7 downto 0));
  end component;

  -- DVB Comma correction FIFO component
  component dvb_fifo 
    generic (
      FIFO_LEVEL_HIGH : std_logic_vector (10 downto 0) := "11000000000";
      FIFO_LEVEL_LOW  : std_logic_vector (10 downto 0):= "00100010001"); 
                                                                   
    port (
      clk            : in  std_logic;     -- 2X word-rate clock
      user_clk       : in  std_logic;     -- FIFO read domain clock <= clk
      data8b_in      : in  std_logic_vector (7 downto 0);  -- 8-bit decoded input
      data8b_valid   : in  std_logic;     -- New data flag from decoder;
      framed         : in  std_logic;     -- ASI framing sequence detect
      reset          : in  std_logic;     -- Synchronous reset
      is_kchar       : in  std_logic;     -- Indicates if 8-bit data is comma
      fifo_rden      : in  std_logic;     -- FIFO read enable (unused)
      fifo_dout_8b   : out std_logic_vector (7 downto 0);  -- 8-bit output data
      fifo_kcode_out : out std_logic;     -- Current output is comma
      rd_count       : out std_logic_vector (10 downto 0);  -- FIFO read count 
      fifo_empty     : out std_logic;     -- FIFO empty flag 
      fifo_full      : out std_logic);    -- FIFO full flag 
  end component;

  -- Link controller component
  component link_ctl 
    port (
      clk         : in  std_logic;
      ce          : in  std_logic;
      reset       : in  std_logic;
      reframe_req : in  std_logic;
      disp_err    : in  std_logic;
      code_err    : in  std_logic;
      framed      : in  std_logic;
      reframe     : out std_logic);
  end component;

  -- Internal Signals
  signal data8b : std_logic_vector (7 downto 0);
  signal is_kchar : std_logic;
  signal framed_asi_int : std_logic;
  signal pdata10b : std_logic_vector (9 downto 0);
  signal pdata10b_inv : std_logic_vector (9 downto 0);    
  signal pdata10b_framed : std_logic_vector (9 downto 0); 
  signal pdata10b_framed_inv : std_logic_vector (9 downto 0);
   
   
  signal pdata10b_en : std_logic;
  signal decoder_code_err : std_logic; 
  signal decoder_disp_err : std_logic; 
  signal decoder_init : std_logic; 
  signal error_condition : std_logic; 
  signal newdata : std_logic;

  signal dru_dout : std_logic_vector (1 downto 0); 
  signal dru_dv_out : unsigned (1 downto 0);
  signal reframe : std_logic;

   

begin  -- rtl

  -- Continous assignments
  decoder_init <= error_condition or reframe_req ;
  code_err <= decoder_code_err ;
  disp_err <= decoder_disp_err ;
  error_condition <= decoder_code_err or decoder_disp_err ;
  framed_asi <= framed_asi_int;

  -- Invert the bit ordering of the 10-bit word output of the deserializer
  -- to the 8b/10b decoder.
  pdata10b_inv <= (pdata10b(0) & 
                   pdata10b(1) & 
                   pdata10b(2) & 
                   pdata10b(3) & 
                   pdata10b(4) & 
                   pdata10b(5) & 
                   pdata10b(6) & 
                   pdata10b(7) & 
                   pdata10b(8) & 
                   pdata10b(9));

   -- The framer frames things inverted for my 8b10b decoder 
   pdata10b_framed_inv <= (pdata10b_framed(0) &
                           pdata10b_framed(1) &
                           pdata10b_framed(2) &
                           pdata10b_framed(3) &
                           pdata10b_framed(4) &
                           pdata10b_framed(5) &
                           pdata10b_framed(6) &
                           pdata10b_framed(7) &
                           pdata10b_framed(8) &
                           pdata10b_framed(9));

  -- Instantiate DRU
  dru_8phase : dru port map (dout   => dru_dout,
                             dv     => dru_dv_out,
                             clk270 => clk_270_4phase,
                             si     => sdata_in);

  -- Instantiate deserializer
  deserializer : des10 port map (dout    => pdata10b,
                                 vo      => pdata10b_en,
                                 clk270  => clk_270_4phase(0),
                                 di      => dru_dout,
                                 dv      => dru_dv_out,
                                 clk54   => clk_54,
                                 bitslip => '0');

  -- Instantiate framer
  asi_framer : asi_par_framer port map (sclk      => clk_54,
                                        rst       => reset,
                                        ce        => pdata10b_en,
                                        d         => pdata10b_inv,
                                        frame_en  => '1',
                                        reframe   => reframe,
                                        sync_mode => sync_mode,
                                        q         => pdata10b_framed,
                                        framed    => framed_asi_int,
                                        nsp       => open);

  -- Instantiate 8b10b decoder
  decoder10b8b_unit : decoder_8b10b port map (clk      => clk_54,
                                              d_10b    => pdata10b_framed_inv,
                                              q_8b     => data8b,
                                              is_kcode => is_kchar,
                                              ce       => pdata10b_en,
                                              sinit    => decoder_init,
                                              code_err => decoder_code_err,
                                              disp_err => decoder_disp_err,
                                              q_valid  => newdata);

  -- Instantiate Rate matching FIFO
  rx_packet_fifo : dvb_fifo port map (fifo_dout_8b   => dout_8b,
                                      fifo_kcode_out => kchar_out,
                                      rd_count       => open,
                                      fifo_empty     => fifo_empty,
                                      fifo_full      => fifo_full,
                                      fifo_rden      => rx_fifo_rden,
                                      clk            => clk_54,
                                      user_clk       => user_clk,
                                      data8b_in      => data8b,
                                      data8b_valid   => newdata,
                                      framed         => framed_asi_int,
                                      reset          => reset,
                                      is_kchar       => is_kchar);

  -- Instantiate the link controller/monitor
  link_mon : link_ctl port map (clk         => clk_54,
                                ce          => newdata,
                                reset       => reset,
                                reframe_req => reframe_req,
                                disp_err    => decoder_disp_err,
                                code_err    => decoder_code_err,
                                framed      => framed_asi_int,
                                reframe     => reframe);
   


end rtl;
