-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: syncrst_gen.vhd,v $
--  /   /        Date Last Modified:  $Date: 2007-06-07 11:42:28-06 $
-- /___/   /\    Date Created: Mar 26, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: syncrst_gen.vhd,v $
-- Revision 1.0  2007-06-07 11:42:28-06  hutching
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
-- Module Description 
--  This simple module takes as input a clock and the locked signal from a DCM.
--  It generates a synchronous reset that is held asserted for 16 clock cycles 
--  after the DCM asserts lock.  It also takes another synchronous reset input
--  and ORs this with the internally generated one.


library IEEE;
use IEEE.STD_LOGIC_1164.all;
use ieee.numeric_std.all;


entity syncrst_gen is
  
  port (
    clk        : in  std_logic;
    rst_in     : in  std_logic;    -- External synchronous reset
    dcm_locked : in  std_logic;    -- DCM Lock signal
    rst        : out std_logic);   -- Synchronous reset output, held until locked
end syncrst_gen;

architecture rtl of syncrst_gen is

  -- Internal Registers
  signal reset_delayed : std_logic := '1';
  signal locked_dly_line : unsigned (14 downto 0);

  begin  -- rtl
    
  -- Reset output is OR'd with input
  rst <= not reset_delayed or rst_in;

  -- SRL16 shift register delays DCM locked to hold rst asserted
  DCMDLY_PROC: process (clk)
  begin  -- process DCMDLY_PROC
    if clk'event and clk = '1' then  -- rising clock edge
      locked_dly_line    <= locked_dly_line sll 1;
      locked_dly_line(0) <= dcm_locked;
      reset_delayed <= locked_dly_line(14);
    end if;
  end process DCMDLY_PROC;
  


  

end rtl;
