-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: serializer_10b1b.vhd,v $
--  /   /        Date Last Modified:  $Date: 2008-04-11 10:32:07-06 $
-- /___/   /\    Date Created: Mar 26, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: serializer_10b1b.vhd,v $
-- Revision 1.3  2008-04-11 10:32:07-06  hutching
-- Removed word-rate clock input.
--
-- Revision 1.2  2008-04-10 11:09:57-06  hutching
-- Added fsn encoding attribute to force GRAY encoding of counter.
--
-- Revision 1.1  2007-10-29 10:00:28-06  hutching
-- Start up sync logic added.
--
-- Revision 1.0  2007-06-15 10:50:10-06  hutching
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
-- This module implements a simple 10bit to 1-bit DDR serializer function.  It is
-- targeted towards Spartan-3E and uses the DDR output flip flops.  Clocks are
-- provided at 135MHz with clock enables and the data is output at DDR 270Mbps.  
--


library IEEE;
use IEEE.STD_LOGIC_1164.all;

library UNISIM;
use UNISIM.vcomponents.all;



entity serializer_10b1b is
  
  port (
    sclk_0    : in  std_logic;                    -- Half bit-rate clock -
    sclk_180  : in std_logic;
    ce      : in  std_logic;                      -- Clock enable for clk
    reset   : in  std_logic;                      -- Synchronous reset
    din_10b : in  std_logic_vector (9 downto 0);  -- 10-bit parallel input data
    sdout   : out std_logic);                     -- Serial data output at sclk*2 (uses DDR)
  attribute fsm_encoding : string;
  attribute fsm_encoding of serializer_10b1b:entity is "GRAY";
  
end serializer_10b1b;

architecture STRUC of serializer_10b1b is

  -- Enumerated types for state machine
  type STATE_TYPE is (CNT_RESET,
                      CNT_STATE1,
                      CNT_STATE2,
                      CNT_STATE3,
                      CNT_STATE4);
  attribute STATE_ENUM : string;
  attribute STATE_ENUM of STATE_TYPE : type is "000 001 011 010 110";

  -- Internal clk registers
  signal din_10b_R : std_logic_vector (9 downto 0);
  signal start_bit : std_logic := '0';

  -- Internal sclk registers
  signal cnt_curr_state : STATE_TYPE := CNT_RESET;
  signal cnt_next_state : STATE_TYPE := CNT_RESET;
  signal sdata_slice_R : std_logic_vector (1 downto 0) := "00";

  -- Internal signals
  signal sdata_slice : std_logic_vector (1 downto 0);
  

begin  -- STRUC

  -----------------------------------------------------------------------------
  -- Synchronous process for clk

  CLK_PROC: process (sclk_0)
  begin  -- process CLK_PROC
    if sclk_0'event and sclk_0 = '1' then  -- rising clock edge
      if reset = '1' then            -- synchronous reset (active high)
        start_bit <= '0';
      elsif (ce = '1') then
        din_10b_R <= din_10b;
	start_bit <= '1';
      end if;
    end if;
  end process CLK_PROC;

  -----------------------------------------------------------------------------
  -- Synchronous process for sclk

  SCLK_PROC: process (sclk_0)
  begin  -- process SCLK_PROC
    
    if sclk_0'event and sclk_0 = '1' then  -- rising clock edge
      sdata_slice_R <= sdata_slice;
      
      if reset = '1' then              -- synchronous reset (active high)
        cnt_curr_state <= CNT_RESET;
      else
        cnt_curr_state <= cnt_next_state;
      end if;
    end if;
  end process SCLK_PROC;
  
    ---------------------------------------------------------------------------
    -- Gray code counter next state logic

    GCNS_PROC: process (cnt_curr_state, start_bit)
    begin  -- process GCNS_PROC
      case cnt_curr_state is
        
	  when CNT_RESET  =>
	     if (start_bit = '1') then  -- Align counter on word boundaries
	       cnt_next_state <= CNT_STATE1;
	     else
	       cnt_next_state <= CNT_RESET;
             end if;
	  
	  WHEN CNT_STATE1 => cnt_next_state <= CNT_STATE2;
	  WHEN CNT_STATE2 => cnt_next_state <= CNT_STATE3;
	  WHEN CNT_STATE3 => cnt_next_state <= CNT_STATE4;
	  WHEN CNT_STATE4 => cnt_next_state <= CNT_RESET;
	  when others     => cnt_next_state <= CNT_RESET;
      end case; -- case(cnt_curr_state)
    end process GCNS_PROC;

  -----------------------------------------------------------------------------
  -- 2-bit wide mux for slicing 10-bit word into 2-bit chunks using gray code
  -- counter.

  MUX_PROC: process (cnt_curr_state)
  begin  -- process MUX_PROC
    case cnt_curr_state is
	  WHEN CNT_RESET  => sdata_slice <= din_10b_R(1 downto 0);
	  WHEN CNT_STATE1 => sdata_slice <= din_10b_R(3 downto 2);
	  WHEN CNT_STATE2 => sdata_slice <= din_10b_R(5 downto 4); 
	  WHEN CNT_STATE3 => sdata_slice <= din_10b_R(7 downto 6);
	  WHEN CNT_STATE4 => sdata_slice <= din_10b_R(9 downto 8);
	  when others     => sdata_slice <= "00";
    end case; -- case(cnt_curr_state)
  end process MUX_PROC;

  -----------------------------------------------------------------------------
  -- Spartan-3E DDR register instantiation

  ODDR_inst : ODDR2
   generic map(
      DDR_ALIGNMENT => "NONE", -- "OPPOSITE_EDGE"
      INIT => '0',                     -- Initial value of 0
      SRTYPE => "SYNC")                -- Synchronous reset
   port map (
      Q => sdout,                      -- 1-bit DDR output
      C0 => sclk_0,                    -- 1-bit clock input
      C1 => sclk_180,
      CE => '1',                       -- 1-bit clock enable input
      D0 => sdata_slice_R(1), -- 1-bit data input (positive edge)
      D1 => sdata_slice_R(0), -- 1-bit data input (negative edge)
      R => '0',                      -- 1-bit reset input
      S => '0'                         -- 1-bit set input
   );

end STRUC;


