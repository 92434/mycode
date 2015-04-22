-------------------------------------------------------------------------------- 
-- Copyright (c) 2006 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Catalin Baetoniu , Strategic Applications, Xilinx, Inc.
--  \   \        Filename: $RCSfile: CLKGEN.vhd,v $
--  /   /        Date Last Modified:  $Date: 2007-06-12 14:28:47-06 $
-- /___/   /\    Date Created: Nov 11, 2006
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: CLKGEN.vhd,v $
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

library UNISIM;
use UNISIM.VCOMPONENTS.all;

entity CLKGEN is
  port(CLK135_IN:in STD_LOGIC;                      -- 135MHz Input Clock
       RST:in STD_LOGIC;                         -- async reset, active high
       CLK270:out STD_LOGIC_VECTOR(3 downto 0);  -- 270MHz 4-phase Clocks (0, 45, 90 and 135 degrees)
       CLK135:out STD_LOGIC;            -- 135MHz 0-degree phase clock output.
       CLK54:out STD_LOGIC;                      -- 54MHz Parallel Clock
       LOCKED:out STD_LOGIC_VECTOR(3 downto 0)); -- high when DCMs are locked
end CLKGEN;

architecture TEST of CLKGEN is
  attribute loc:STRING;
  attribute loc of bd:label is "BUFGMUX_X2Y1";

  signal CLK2X,CLK0,CLKDV,iCLK270:STD_LOGIC_VECTOR(3 downto 0);
begin
  lk:for K in CLK270'range generate
       attribute loc of dc:label is "DCM_X"&INTEGER'image(K mod 2+1)&"Y"&INTEGER'image(K/2*3);
       attribute loc of bg:label is "BUFGMUX_X"&INTEGER'image(K mod 2+1)&"Y"&INTEGER'image(K/2*10);
     begin 
       dc:DCM_SP generic map(CLK_FEEDBACK=>"2X",
                             CLKDV_DIVIDE=>2.5,
                             CLKFX_DIVIDE=>1,
                             CLKFX_MULTIPLY=>4,
                             CLKIN_DIVIDE_BY_2=>FALSE,
                             CLKIN_PERIOD=>7.40741,
                             CLKOUT_PHASE_SHIFT=>"FIXED",
                             DESKEW_ADJUST=>"SYSTEM_SYNCHRONOUS",
                             DFS_FREQUENCY_MODE=>"LOW",
                             DLL_FREQUENCY_MODE=>"LOW",
                             DUTY_CYCLE_CORRECTION=>TRUE,
                             FACTORY_JF=>x"C080",
                             PHASE_SHIFT=>16*K,
                             STARTUP_WAIT=>FALSE)
                 port map(CLKFB=>iCLK270(K),
                          CLKIN=>CLK135_IN,
                          DSSEN=>'0',
                          PSCLK=>'0',
                          PSEN=>'0',
                          PSINCDEC=>'0',
                          RST=>RST,
                          CLKDV=>CLKDV(K),
                          CLKFX=>open,
                          CLKFX180=>open,
                          CLK0=>CLK0(K),
                          CLK2X=>CLK2X(K),
                          CLK2X180=>open,
                          CLK90=>open,
                          CLK180=>open,
                          CLK270=>open,
                          LOCKED=>LOCKED(K),
                          PSDONE=>open,
                          STATUS=>open);
       bg:BUFG port map(I=>CLK2X(K),O=>iCLK270(K));
     end generate;
  bd:BUFG port map(I=>CLKDV(0),O=>CLK54);
  b0:BUFG port map (I=>CLK0(0), O=>CLK135);
  CLK270<=iCLK270;
end TEST;
