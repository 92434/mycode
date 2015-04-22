-------------------------------------------------------------------------------- 
-- Copyright (c) 2006 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Catalin Baetoniu , Strategic Applications, Xilinx, Inc.
--  \   \        Filename: $RCSfile: DES10.vhd,v $
--  /   /        Date Last Modified:  $Date: 2007-06-12 14:28:47-06 $
-- /___/   /\    Date Created: Nov 11, 2006
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: DES10.vhd,v $
-- Revision 1.1  2007-06-12 14:28:47-06  hutching
-- Initial Release
--
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

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

library UNISIM;
use UNISIM.VCOMPONENTS.all;

entity DES10 is
-- serial clock domain
  port(CLK270:in STD_LOGIC;                      -- 270MHz serial clock 
       DI:in STD_LOGIC_VECTOR(1 downto 0);       -- serial data in, 0, 1 or 2 bits/clock
       DV:in UNSIGNED(1 downto 0);               -- number of valid bits
-- parallel clock domain
       CLK54:in STD_LOGIC;                       -- 54MHz parallel clock (phase aligned with CLK270)
       BITSLIP:in STD_LOGIC;                     -- every change in BITSLIP slip one bit
       DOUT:out STD_LOGIC_VECTOR(9 downto 0);      -- parallel data out
       VO:out STD_LOGIC);                        -- high when DO valid
end DES10;

architecture TEST of DES10 is
  attribute rloc:STRING;
  attribute bel:STRING;
  attribute route:STRING;
  attribute syn_keep:BOOLEAN;
  attribute syn_useenables:BOOLEAN;
  
  signal SH:STD_LOGIC_VECTOR(10 downto 0):=(others=>'0');
  attribute syn_useenables of SH:signal is FALSE;
  signal D,D1:STD_LOGIC_VECTOR(9 downto 0):=(others=>'0');
  attribute syn_useenables of D:signal is FALSE;
  signal V,V1,VD:STD_LOGIC:='0';
  attribute syn_useenables of V:signal is FALSE;
  signal BITSLIP1D,BS:STD_LOGIC:='0';
  
  signal QB,E:STD_LOGIC:='0';
  signal QH:STD_LOGIC_VECTOR(4 downto 0):="00001";
begin
  process(CLK270)
  begin
    if rising_edge(CLK270) then
      if DV(0)='1' and BS='0' then
        QB<=not QB;
      end if;
      if (QB='1' and DV(0)='1' and BS='0') or DV(1)='1' then
        if QH(3 downto 0)="0000" then
          QH(0)<='1';
        else
          QH(0)<='0';
        end if;
        if QH(4)='0' then
          QH(4 downto 1)<=QH(3 downto 0);
        else
          QH(4 downto 1)<="0000";
        end if;
      end if;
      if (DV(0)='1' and BS='0') or DV(1)='1' then
        if DV(1)='0' then
          SH<=SH(SH'high-1 downto SH'low)&DI(0);
        else
          SH<=SH(SH'high-2 downto SH'low)&DI(1 downto 0);
        end if;
      end if;
      if QH(4)='1'and ((QB='1' and DV(0)='1' and BS='0') or DV(1)='1') then
        E<='1';
      else
        E<='0';
      end if;
      if E='1'then
        V<=not V;
      end if;
      if E='1' then
        if QB='0' then
          D<=SH(SH'high-1 downto SH'low);
        else
          D<=SH(SH'high downto SH'low+1);
        end if;
      end if;
      BITSLIP1D<=BITSLIP;
      if BITSLIP/=BITSLIP1D then
        BS<='1';
      elsif DV="01" then
        BS<='0';
      end if;
    end if;
  end process;

  D1<=transport D after 0.1 ns;
  V1<=transport V after 0.1 ns;
  process(CLK54)
  begin
    if rising_edge(CLK54) then
      VD<=V1;
      DOUT<=D1;
      VO<=V1 xor VD;
    end if;
  end process;
end TEST;
