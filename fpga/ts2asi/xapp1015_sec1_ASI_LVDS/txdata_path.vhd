-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: txdata_path.vhd,v $
--  /   /        Date Last Modified:  $Date: 2008-04-11 10:22:31-06 $
-- /___/   /\    Date Created: Mar 26, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: txdata_path.vhd,v $
-- Revision 1.1  2008-04-11 10:22:31-06  hutching
-- Removed word-rate clock.
--
-- Revision 1.0  2007-06-15 10:46:52-06  hutching
-- Initial revision
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
--  This module implements the complete data path for the DVB ASI transmitter for Spartan-3E.  The
--  8b10b encoder (encoder_8b10b) and serializer (serializer_10b1b_v5) are instantiated by this 
--  module.  Required clocking is simple with a word-rate (27MHz), and a 135MHz half bit-rate clock
--  required.  These clocks must be synchronous to one another.  Alternatively, the 135MHz clock can
--  be used for both if the user generates clock enables on the "ce" input for the word-rate logic.
--
--  The circuitry takes incoming 8-bit data, 8b10b encodes the data, serializes it and sends it out
--  the "asi_out" port.
--
--

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity txdata_path is
  
  port (
    clk        : in  std_logic;         -- word-rate clock or bit-rate clock (when using ce)
    ce         : in  std_logic;         -- clock enable for ce
    rst        : in  std_logic;         -- synchronous reset
    din_8b     : in  std_logic_vector (7 downto 0);  -- 8-bit unencoded input data
    kchar_in   : in  std_logic;         -- control input indicates current byte is comma
    force_disp : in  std_logic;         -- Force encoder disparity - unused
    asi_out    : out std_logic);        -- Encoded/Serialized ASI output

end txdata_path;

architecture rtl of txdata_path is

 ------------------------------------------------------------------------------
 -- Component declarations

  component encoder_8b10b
    port (
      din      : in  std_logic_vector (7 downto 0);
      kin      : in  std_logic;
      clk      : in  std_logic;
      ce       : in  std_logic;
      dout     : out std_logic_vector (9 downto 0);
      valid    : out std_logic;
      code_err : out std_logic);
  end component;

  component serializer_10b1b
    port (
      sclk_0    : in  std_logic;
      sclk_180  : in std_logic;
      ce      : in  std_logic;
      reset   : in  std_logic;
      din_10b : in  std_logic_vector (9 downto 0);
      sdout   : out std_logic);
  end component;

  -- Internal registers
  signal din_8b_R : std_logic_vector (7 downto 0) := X"00";
  signal kchar_in_R : std_logic := '0';
  signal force_disp_R : std_logic := '0';

  -- Internal signals
  signal data_enc10b : std_logic_vector (9 downto 0);
  

begin  -- rtl

 ------------------------------------------------------------------------------
 -- Syncrhonous process for clk

  CLK_PROC: process (clk)
  begin  -- process CLK_PROC
    if clk'event and clk = '1' then  -- rising clock edge
      if ce = '1' then
	     din_8b_R     <= din_8b;
	     kchar_in_R   <= kchar_in;
	     force_disp_R <= force_disp;
      end if;
    end if;
  end process CLK_PROC;

  -----------------------------------------------------------------------------
  -- Component instantiations

  -- Instantiate 8b10b encoder
  encoder_inst : encoder_8b10b port map (din      => din_8b_R,
                                         kin      => kchar_in_R,
                                         clk      => clk,
                                         ce       => ce,
                                         dout     => data_enc10b,
                                         valid    => open,
                                         code_err => open);

  -- Instantiate serializer
  asi_serializer : serializer_10b1b port map (sclk_0    => clk,
                                              sclk_180  => not clk,
                                              ce        => ce,
                                              reset     => rst,
                                              din_10b   => data_enc10b,
                                              sdout     => asi_out);
  
end rtl;

