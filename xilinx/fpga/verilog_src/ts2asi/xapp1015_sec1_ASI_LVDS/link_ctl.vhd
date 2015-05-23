-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: link_ctl.vhd,v $
--  /   /        Date Last Modified:  $Date: 2008-04-10 08:13:29-06 $
-- /___/   /\    Date Created: Mar 26. 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: link_ctl.vhd,v $
-- Revision 1.3  2008-04-10 08:13:29-06  hutching
-- New release for 10.1i
--
-- Revision 1.2  2008-02-06 14:23:40-07  hutching
-- Fixed missing reset on state machine synchronous logic.
--
-- Revision 1.1  2007-06-21 12:24:31-06  hutching
-- Changed to 4 consecutive errors for reframe.  Added clock enable
--
-- Revision 1.0  2007-06-07 11:44:24-06  hutching
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
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.all;

-------------------------------------------------------------------------------
-- Module Description:
-- This module implements a simple state machine to monitor the "health"
-- of the DVB-ASI link.  It detects disparity and code errors from the
-- decoder and after four consecutive errors have occurred, it automatically
-- asserts the reframe signal.  It will loop endlessly trying to re-sync the link
-- until there are no errors being reported by the decoder.
-------------------------------------------------------------------------------

entity link_ctl is
  
  port (
    clk         : in  std_logic;
    ce          : in  std_logic;
    reset       : in  std_logic;
    reframe_req : in  std_logic;
    disp_err    : in  std_logic;
    code_err    : in  std_logic;
    framed      : in  std_logic;
    reframe     : out std_logic);

end link_ctl;

architecture RTL of link_ctl is

  -- Enumerated states
  type STATE_TYPE is (LINK_MONITOR,
                 LINK_ERR1,
                 LINK_ERR2,
                 LINK_ERR3,
                 LINK_REFRAME1,
                 LINK_REFRAME2);

  attribute STATE_ENUM : string;
  attribute STATE_ENUM of STATE_TYPE : type is "000 001 011 111 110 010";

  -- Internal Registers
  signal linkctl_current_state : STATE_TYPE := LINK_MONITOR;
  signal linkctl_next_state : STATE_TYPE;
  signal link_err : std_logic := '0';

begin  -- RTL

  -- Current state logic, and register updates
  CLK_PROC: process (clk)
  begin  -- process CLK_PROC
    if clk'event and clk = '1' then  -- rising clock edge
      if (reset = '1') then
        linkctl_current_state <= LINK_MONITOR;
        link_err <= '0';
      elsif (ce = '1') then
        link_err <= disp_err or code_err;
        if reframe_req = '1' then
          linkctl_current_state <= LINK_REFRAME1;
        else
          linkctl_current_state <= linkctl_next_state;
        end if;
      end if;
    end if;
  end process CLK_PROC;
   
  -- Output decoding logic
   OUTPUT_DECODE: process (linkctl_current_state)
   begin  -- process OUTPUT_DECODE
     case linkctl_current_state is
       
	when LINK_MONITOR  => reframe <= '0';

	when LINK_ERR1     => reframe <= '0';

	when LINK_ERR2     => reframe <= '0';

	when LINK_ERR3     => reframe <= '0';

	when LINK_REFRAME1 => reframe <= '1';

	when LINK_REFRAME2 => reframe <= '1';

	when others       => reframe  <= '0';
     end case; -- case(linkctl_current_state)
   end process OUTPUT_DECODE;


  -- Next state decoding logic
  NS_DECODE: process (linkctl_current_state, reframe_req, link_err)
  begin  -- process NS_DECODE
    case linkctl_current_state is

       when LINK_MONITOR  => 
	  if (link_err = '1' and framed = '1') then
	    linkctl_next_state <= LINK_ERR1;
	  elsif (reframe_req = '1') then
	    linkctl_next_state <= LINK_REFRAME1;
	  else
	    linkctl_next_state <= LINK_MONITOR;
          end if;

       when LINK_ERR1    => 
	  if (link_err = '1') then
	    linkctl_next_state <= LINK_ERR2;
	  elsif (reframe_req = '1')then
	    linkctl_next_state <= LINK_REFRAME1;
	  else
	    linkctl_next_state <= LINK_MONITOR;
          end if;

       when LINK_ERR2    => 
	  if (link_err = '1') then
	    linkctl_next_state <= LINK_ERR3;
	  elsif (reframe_req = '1') then
	    linkctl_next_state <= LINK_REFRAME1;
	  else
	    linkctl_next_state <= LINK_ERR1;
          end if;

       when LINK_ERR3   => 
	  if (link_err = '1') then
	    linkctl_next_state <= LINK_REFRAME1;
	  elsif (reframe_req = '1') then
	    linkctl_next_state <= LINK_REFRAME1;
	  else
	    linkctl_next_state <= LINK_ERR2;
          end if;

       when LINK_REFRAME1 => linkctl_next_state <= LINK_REFRAME2;

       when LINK_REFRAME2 => linkctl_next_state <= LINK_MONITOR;

       when others       => linkctl_next_state <= LINK_REFRAME1;
    end case; -- case(linkctl_current_state)
  end process NS_DECODE;

end RTL;

