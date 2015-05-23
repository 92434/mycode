-------------------------------------------------------------------------------- 
-- Copyright (c) 2006 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Catalin Baetoniu , Strategic Applications, Xilinx, Inc.
--  \   \        Filename: $RCSfile: DRU.vhd,v $
--  /   /        Date Last Modified:  $Date: 2007-09-26 09:43:19-06 $
-- /___/   /\    Date Created: Nov 11, 2006
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: DRU.vhd,v $
-- Revision 1.2  2007-09-26 09:43:19-06  hutching
-- Timing improvements to meet 290MHz for DCM jitter margin.
--
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

entity DRU is
  port(CLK270:in STD_LOGIC_VECTOR(3 downto 0);   -- 270MHz 4-phase Clocks (0, 45, 90 and 135 degrees)
       SI:in STD_LOGIC;                          -- 270Mbps Serial Data In
       DOUT:out STD_LOGIC_VECTOR(1 downto 0);      -- Serial Data Out (0, 1 or 2 bits/clock)
       DV:out UNSIGNED(1 downto 0));             -- Number of valid output bits (0, 1 or 2 bits/clock)
  
  -- JLH: Added to force XST to use VHDL coding for FSM - improves timing.
  attribute fsm_encoding : string;


    -- JLH: Force XST to use FSM encoding specified in HDL
  attribute fsm_encoding of DRU:entity is "USER";
  
end DRU;

architecture TEST of DRU is
  attribute rloc:STRING;
  attribute bel:STRING;
  attribute route:STRING;
  attribute syn_keep:BOOLEAN;

  -- JLH: Added to test for timing improvement of E signals
  attribute max_fanout : string;

  
  signal DATA,DATA1D,DATA2D,DATA3D,DATA4D,cEDGE,EDGE,E:STD_LOGIC_VECTOR(7 downto 0):=(others=>'0');
  signal Q:UNSIGNED(2 downto 0):=(others=>'0');
  
  signal DVE:UNSIGNED(DV'range):=(others=>'0');
  attribute syn_keep of DVE:signal is TRUE; -- no SRL16 please!

  -- JLH: Added to improve timing on E signals
  attribute max_fanout of E:signal is "1";

  
begin
  lk:for K in CLK270'range generate
       signal QR,QF,QFD,RQ,FQ:STD_LOGIC:='0';
       attribute syn_keep of QR:signal is TRUE;
       attribute syn_keep of QF:signal is TRUE;
       attribute syn_keep of QFD:signal is TRUE;
       attribute rloc of QR:signal is "X"&INTEGER'image(K+K/3+1)&"Y0";
       attribute rloc of QF:signal is "X"&INTEGER'image(K+K/3+1)&"Y1";
       attribute rloc of QFD:signal is "X"&INTEGER'image(K+K/3+1)&"Y0";
       attribute rloc of RQ:signal is "X"&INTEGER'image(K/2*4)&"Y"&INTEGER'image(K mod 2);
       attribute rloc of FQ:signal is "X"&INTEGER'image(K/2*4)&"Y"&INTEGER'image(K mod 2);
       function FFXY(K:INTEGER) return STRING is
       begin
         if K mod 2=0 then
           return "FFX";
         else
           return "FFY";
         end if;
       end;
       attribute bel of QR:signal is FFXY(K/3);
       attribute bel of QF:signal is FFXY(K/3);
       attribute bel of QFD:signal is FFXY(K/3+1);
       attribute bel of RQ:signal is FFXY(0);
       attribute bel of FQ:signal is FFXY(1);
       function DIRT_QF(K:INTEGER) return STRING is
       begin
         case K is
           when 0=>return "{3;1;3s1600efg400;2632ae48!-1;91296;127952;S!0;-2235;456!1;1908;-1000!2;327;0;L!}";
           when 1=>return "{3;1;3s1600efg400;7d868db8!-1;94592;127264;S!0;-2075;-848!1;1908;304!2;167;0;L!}";
           when 2=>return "{3;1;3s1600efg400;2533fe31!-1;94752;127952;S!0;-2235;456!1;1908;-1000!2;327;0;L!}";
           when 3=>return "{3;1;3s1600efg400;f54b7645!-1;98208;127984;S!0;1261;408!1;-1588;-992!2;327;0;L!}";
           when others=>return "";
         end case;
       end;
       attribute route of QF:signal is DIRT_QF(K);
       function DIRT_QR(K:INTEGER) return STRING is
       begin
         case K is
           when 0=>return "{3;1;3s1600efg400;d6bed311!-1;91296;127608;S!0;1261;-1176!1;-1588;280!2;167;0;L!}";
           when 1=>return "{3;1;3s1600efg400;3e028464!-1;94592;126920;S!0;-2099;-512!1;-1524;648!2;167;0;L!}";
           when 2=>return "{3;1;3s1600efg400;2e7ffe9b!-1;94752;127608;S!0;1261;-1176!1;1868;280!2;167;0;L!}";
           when 3=>return "{3;1;3s1600efg400;ecde71c1!-1;98208;127640;S!0;1261;-1208!1;-1588;624!2;167;0;L!}";
           when others=>return "";
         end case;
       end;
       attribute route of QR:signal is DIRT_QR(K);
       function DIRT_QFD(K:INTEGER) return STRING is
       begin
         case K is
           when 0=>return "{3;1;3s1600efg400;854cd698!-1;91296;127640;S!0;-2235;-1224!1;1908;304!2;167;0;L!}";
           when 1=>return "{3;1;3s1600efg400;b65469f4!-1;94592;126952;S!0;-251;-376!1;-4397;-163!2;1025;651!3;167;0;L!}";
           when 2=>return "{3;1;3s1600efg400;ab1f6584!-1;94752;127640;S!0;1241;-1216!1;1888;296!2;167;0;L!}";
           when 3=>return "{3;1;3s1600efg400;48d64f3a!-1;98208;127608;S!0;-2235;-1192!1;1908;648!2;167;0;L!}";
           when others=>return "";
         end case;
       end;
       attribute route of QFD:signal is DIRT_QFD(K);
     begin 
       process(CLK270(K))
       begin
         if falling_edge(CLK270(K)) then
           QF<=SI;
         end if;
       end process;
       process(CLK270(K))
       begin
         if rising_edge(CLK270(K)) then
           QR<=SI;
           QFD<=QF;
         end if;
       end process;
       process(CLK270(0))
       begin
         if rising_edge(CLK270(0)) then
           RQ<=QR;
           FQ<=QFD;
         end if;
       end process;
       DATA(K)<=FQ;
       DATA(K+4)<=RQ;
     end generate;

--  cEDGE<=DATA xor DATA(6 downto 0)&DATA1D(7);
  process(CLK270(0))
  begin
    if rising_edge(CLK270(0)) then
--      EDGE<=cEDGE;
--      E<=cEDGE or cEDGE(6 downto 0)&cEDGE(7) or cEDGE(0)&cEDGE(7 downto 1);
      EDGE<=DATA xor DATA(6 downto 0)&DATA1D(7);
      E<=EDGE or EDGE(6 downto 0)&EDGE(7) or EDGE(0)&EDGE(7 downto 1);
      case Q is
        when "000"=>if E(2)='1' then
                      Q<="111";DVE<="10";
                    elsif E(6)='1' then
                      Q<="001";DVE<="01";
                    else
                      DVE<="01";
                    end if;
        when "001"=>if E(3)='1' then
                      Q<="000";DVE<="01";
                    elsif E(7)='1' then
                      Q<="010";DVE<="01";
                    else
                      DVE<="01";
                    end if;
        when "010"=>if E(4)='1' then
                      Q<="001";DVE<="01";
                    elsif E(0)='1' then
                      Q<="011";DVE<="01";
                    else
                      DVE<="01";
                    end if;
        when "011"=>if E(5)='1' then
                      Q<="010";DVE<="01";
                    elsif E(1)='1' then
                      Q<="100";DVE<="01";
                    else
                      DVE<="01";
                    end if;
        when "100"=>if E(6)='1' then
                      Q<="011";DVE<="01";
                    elsif E(2)='1' then
                      Q<="101";DVE<="01";
                    else
                      DVE<="01";
                    end if;
        when "101"=>if E(7)='1' then
                      Q<="100";DVE<="01";
                    elsif E(3)='1' then
                      Q<="110";DVE<="01";
                    else
                      DVE<="01";
                    end if;
        when "110"=>if E(0)='1' then
                      Q<="101";DVE<="01";
                    elsif E(4)='1' then
                      Q<="111";DVE<="01";
                    else
                      DVE<="01";
                    end if;
        when "111"=>if E(1)='1' then
                      Q<="110";DVE<="01";
                    elsif E(5)='1' then
                      Q<="000";DVE<="00";
                    else
                      DVE<="01";
                    end if;
        when others=>null;
      end case;
      DATA1D<=DATA;
      DATA2D<=DATA1D;
      DATA3D<=DATA2D;
      DATA4D<=DATA3D;
      DOUT<=DATA4D(7)&DATA3D(TO_INTEGER(Q));
      DV<=DVE;
    end if;
  end process;
-- This is just for test, remove from the final version of the design
--  ILA_TEST<=STD_LOGIC_VECTOR(Q)&E&EDGE&DATA;
end TEST;
