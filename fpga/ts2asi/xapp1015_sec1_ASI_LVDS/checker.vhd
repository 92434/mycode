-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: checker.vhd,v $
--  /   /        Date Last Modified:  $Date: 2007-06-07 11:44:31-06 $
-- /___/   /\    Date Created: Mar 26, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: checker.vhd,v $
-- Revision 1.0  2007-06-07 11:44:31-06  hutching
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

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use ieee.numeric_std.all;


--   Module Description:  
--   This module implements a DVB-ASI transport stream checker.  The checker is
--   only compatible with the stimulus generator in asi_stimulus.vhd/.v
-- 
--

entity checker is
  
  port (
    user_clk    : in  std_logic;
    ce          : in  std_logic;
    fifo_empty  : in  std_logic;
    fifo_data   : in  std_logic_vector (7 downto 0);
    fifo_kchar  : in  std_logic;
    clear_err   : in  std_logic;
    asi_rx_err  : in  std_logic;
    checker_err : out std_logic);

end checker;

architecture rtl of checker is

  -- Internal registers
  signal fifo_data_R1  : unsigned (7 downto 0)  := X"00";  -- Pipeline stage 
  signal fifo_chk_data : unsigned (7 downto 0) := X"00";  -- Data counter 
  signal sop_wdt       : unsigned (9 downto 0); -- Start of packet watchdog timer
  signal cnt_en        : std_logic  := '0';     -- Enable for chk data counter.
                                                        
  -- Internal signals
  signal sop : std_logic;
  
begin  -- rtl

  -- Start of packet detector 
  sop <= '1' when (fifo_data = X"47") else '0';
             

  -- Synchronous process
  CHECK_PROC: process (user_clk)
  begin  -- process CHECK_PROC
    if user_clk'event and user_clk = '1' then  -- rising clock edge
      
      if (clear_err = '1') then
        cnt_en        <= '0';
        checker_err   <= '0';
        fifo_chk_data <= X"00";
        sop_wdt       <= "0000000000";
        
      elsif (ce = '1') then
        
        -- Watchdog timer process
        if (sop = '1') then
          sop_wdt <= "0000000000";
        else
          sop_wdt <= sop_wdt + 1;
        end if;

        -- Here's the actual check against the data
        -- fifo_chk_value_u <= (fifo_chk_data + X"47");
        if (((fifo_data_R1 /= unsigned(fifo_chk_data + X"47")) and cnt_en = '1') or sop_wdt = "1000000000") then
          checker_err <= '1';
        else
          checker_err <= '0';
        end if;
        
        -- Simple one-bit state machine for counter enable to synchronize
        -- counter to input packet
        if (sop = '1') then -- Wait for start of packet
          cnt_en   <= '1';
        elsif (fifo_chk_data = X"BB") then  -- If end of packet, stop counting,
                                            -- wait for new sop
          cnt_en <= '0';
          fifo_chk_data <= X"00";
        end if;
	     
        -- Counter process.  
        if (fifo_kchar = '0' and cnt_en = '1') then
          if (fifo_chk_data < X"BB") then
            fifo_chk_data <= fifo_chk_data + 1;
          else
            fifo_chk_data <= X"00";
          end if;
        end if;
        
        -- Register load process.  Only load non-comma data for comparison
        if (fifo_kchar = '0') then
          fifo_data_R1 <= unsigned(fifo_data);
        end if; -- else: !if(clear_err)
          
      end if;
    end if;
  end process CHECK_PROC;
      
end rtl;
