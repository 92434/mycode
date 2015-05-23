-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: asi_stimulus.vhd,v $
--  /   /        Date Last Modified:  $Date: 2007-06-07 11:45:14-06 $
-- /___/   /\    Date Created: Mar 26, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: asi_stimulus.vhd,v $
-- Revision 1.0  2007-06-07 11:45:14-06  hutching
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
--   This module implements a simple stimulus generator for testing purposes.  It
--   generates simple synthetic MPEG-2 packets that consist of 188 bytes of data
--   followed by two comma characters.  The pattern then repeats.  This can be used
--   to generate stimulus data compatible with Matt Klein's ML403 design, or the 
--   checker implemented for the ASI rx.
-------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use ieee.numeric_std.all;

entity asi_stimulus is
  
  port (
    clk       : in  std_logic;
    ce        : in  std_logic;
    rst       : in  std_logic;
    asi_dout  : out std_logic_vector (7 downto 0);
    comma_out : out std_logic);

end asi_stimulus;


architecture rtl of asi_stimulus is

  -- Internal registers
  signal byte_cntr : unsigned (7 downto 0) := X"00";
  
begin  -- rtl

  STIM_PROC: process (clk)
  begin  -- process STIM_PROC

    if rst = '1' then                   -- synchronous reset (active high)
      asi_dout  <= X"00";
      byte_cntr <= X"00";
      comma_out <= '0';
      
    elsif clk'event and clk = '1' then  -- rising clock edge
      if (ce = '1') then
        
        -------------------------------------------------------------------------
        -- asi_dout process:
        -- asi_dout gets 47h, 48h, 49h, ... 02h, BCh, BCh, 47h...
        if (byte_cntr = X"BC" or byte_cntr = X"BD") then
          asi_dout <= X"BC";
          comma_out <= '1';
        else
          asi_dout <= std_logic_vector(byte_cntr + X"47");
          comma_out <= '0';
        end if;
        
        ---------------------------------------------------------------------
        -- byte_cntr process  - 00, 01, 02, ... BC, 00, 01, 02...
        if (byte_cntr <= X"BC") then
          byte_cntr <= byte_cntr + 1;
        else
          byte_cntr <= X"00";
        end if;
      end if;
    end if;
  end process STIM_PROC;
end rtl;
