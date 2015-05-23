-------------------------------------------------------------------------------- 
-- Copyright (c) 2005-2007 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Tze Yi Yeoh, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: asi_par_framer.vhd,v $
--  /   /        Date Last Modified:  $Date: 2008-04-10 08:13:15-06 $
-- /___/   /\    Date Created: Mar 22, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: asi_par_framer.vhd,v $
-- Revision 1.2  2008-04-10 08:13:15-06  hutching
-- Changed all resets to synchronous.  Original module had asyncrhonous
--
-- Revision 1.1  2008-02-06 14:23:05-07  hutching
-- Added Temp1 to next state logic sensitivity list.
--
-- Revision 1.0  2007-06-07 11:43:48-06  hutching
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

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

library unisim ;                
use unisim.vcomponents.all ;

-- synthesis translate_off
library XilinxCoreLib;
-- synthesis translate_on

ENTITY asi_par_framer IS
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
END asi_par_framer;

ARCHITECTURE asi_par_framer_arch OF asi_par_framer IS


-- Internal registers
SIGNAL in1_reg              : std_logic_vector(9 DOWNTO 0); -- input reg 
SIGNAL barrel_in            : std_logic_vector(18 DOWNTO 0);-- barrel shifter in reg

-- Internal signals
SIGNAL in_vector            : std_logic_vector(18 DOWNTO 0);-- comma detect input vector
SIGNAL barrel_out           : std_logic_vector(9 DOWNTO 0); -- output of barrel shifter
SIGNAL bs_in                : std_logic_vector(20 DOWNTO 0);-- barrel shifter input vector (2 MSBs are dummy)
SIGNAL bs_m1                : std_logic_vector(12 DOWNTO 0);-- output of first MUX level in barrel shifter
SIGNAL bs_sm                : std_logic_vector(1 DOWNTO 0); -- barrel shifter first level MUX select bits
SIGNAL bs_sl                : std_logic_vector(1 DOWNTO 0); -- barrel shifter second level MUX select bits
SIGNAL q_inv                : std_logic_vector(9 DOWNTO 0);   
SIGNAL i                    : integer;                      --  for loop variable
SIGNAL j                    : integer;                      --  for loop variable
SIGNAL comma_match          : std_logic_vector(9 DOWNTO 0);   
SIGNAL comma_detected       : std_logic;    
SIGNAL byte_counter_int     : std_logic_vector(5 DOWNTO 0);   
SIGNAL new_offset           : std_logic;   
SIGNAL offset_reg           : std_logic_vector(3 DOWNTO 0);   
SIGNAL offset_val           : std_logic_vector(3 DOWNTO 0);   
SIGNAL comma_error          : std_logic;   
-- Framing State Machine Signals
SIGNAL byte_counter_tc      : std_logic;   
SIGNAL byte_counter_rst     : std_logic;   
SIGNAL byte_counter_inc     : std_logic;   

CONSTANT FRAME_START             : std_logic_vector(1 DOWNTO 0) := "00";    
CONSTANT FRAME_SEEK_2ND_SYNC_BYTE: std_logic_vector(1 DOWNTO 0) := "01";    
CONSTANT FRAME_GO                : std_logic_vector(1 DOWNTO 0) := "10";
   
SIGNAL Frame_Current_State      :  std_logic_vector(1 DOWNTO 0) := FRAME_START;
SIGNAL Frame_Next_State         :  std_logic_vector(1 DOWNTO 0);

-- Parameter SYNC_MODE indicates whether a single sync byte is used 
-- to frame the initial data or 2 sync bytes within a 5 byte window:
-- \'0\' indicates that the receiver frames (locks) to the first sync  
-- byte it detects.  
-- \'1\' indicates that the receiver frames only when 2 sync bytes are             
-- detected on the same byte boundary within a 5 byte window. 

-- CONSTANT SYNC_MODE          : std_logic := '1';    

SIGNAL temp1                : std_logic_vector(1 DOWNTO 0);   
SIGNAL q_int                : std_logic_vector(9 DOWNTO 0);   
SIGNAL framed_int           : std_logic;   
SIGNAL nsp_int              : std_logic;  

BEGIN
   q <= q_int;
   framed <= framed_int;
   nsp <= nsp_int;
   
   --
   -- Input register
   --
   PROCESS(sclk)
   BEGIN
      
      IF sclk'event and sclk = '1' then
        IF (rst = '1') THEN
          in1_reg <= (others => '0');    
         ELSIF (ce = '1') THEN
            in1_reg <= d;    
         END IF;
      END IF;
   END PROCESS;

--
-- comma detector and offset encoder
--
-- This section detects comma characters and determines their offset in the
-- 10-bit input words. The offset_val value is encoded with offset of the
-- starting position of the command character.

   in_vector <= d(8 DOWNTO 0) & in1_reg ;

    -- Create 10 comma detectors, one at each starting bit position
   in_vector_generate:for i in 0 to 9 generate   
      PROCESS (in_vector)
      BEGIN
         comma_match(i) <=  
            (not in_vector(i + 9) and     in_vector(i + 8) and not 
                 in_vector(i + 7) and     in_vector(i + 6) and     
                 in_vector(i + 5) and     in_vector(i + 4) and     
                 in_vector(i + 3) and     in_vector(i + 2) and 
             not in_vector(i + 1) and not in_vector(i)) 
             or
            (    in_vector(i + 9) and not in_vector(i + 8) and     
                 in_vector(i + 7) and not in_vector(i + 6) and 
             not in_vector(i + 5) and not in_vector(i + 4) and 
             not in_vector(i + 3) and not in_vector(i + 2) and     
                 in_vector(i + 1) and     in_vector(i));
      END PROCESS;
   END GENERATE;

   --
   -- If any of the bits in comma_match_all are asserted, then assert 
   -- comma_detected        
   --
   comma_detected <= (comma_match(9) OR comma_match(8) OR 
                          comma_match(7) OR comma_match(6) OR 
                          comma_match(5) OR comma_match(4) OR  
                          comma_match(3) OR comma_match(2) OR 
                          comma_match(1) OR comma_match(0));

   --
   -- This code asserts comma_error if more than one bit is set in 
   -- comma_match. It also encodes the comma_match vector into a binary
   -- offset code.
   --
   
   PROCESS (comma_match)
   BEGIN
      CASE comma_match IS
         WHEN "0000000000" =>
                  comma_error <= '0';    
                  offset_val <= "0000";    
         WHEN "0000000001" =>
                  comma_error <= '0';    
                  offset_val <= "0000";    
         WHEN "0000000010" =>
                  comma_error <= '0';    
                  offset_val <= "0001";    
         WHEN "0000000100" =>
                  comma_error <= '0';    
                  offset_val <= "0010";    
         WHEN "0000001000" =>
                  comma_error <= '0';    
                  offset_val <= "0011";    
         WHEN "0000010000" =>
                  comma_error <= '0';    
                  offset_val <= "0100";    
         WHEN "0000100000" =>
                  comma_error <= '0';    
                  offset_val <= "0101";    
         WHEN "0001000000" =>
                  comma_error <= '0';    
                  offset_val <= "0110";    
         WHEN "0010000000" =>
                  comma_error <= '0';    
                  offset_val <= "0111";    
         WHEN "0100000000" =>
                  comma_error <= '0';    
                  offset_val <= "1000";    
         WHEN "1000000000" =>
                  comma_error <= '0';    
                  offset_val <= "1001";    
         WHEN OTHERS  =>
                  comma_error <= '1';    
                  offset_val <= "0000";    
         
      END CASE;
   END PROCESS;

--
-- offset_reg: barrel shifter offset register
--
-- The offset_reg loads the offset_val whenever comma_detected is
-- asserted and comma_error is not asserted and frame_en is asserted.
--
   
   PROCESS(sclk)
   BEGIN
      IF sclk'event and sclk = '1' then
        IF (rst = '1') THEN
         offset_reg <= "0000";
         ELSIF (ce = '1') THEN
            IF (((comma_detected AND NOT comma_error) AND 
                 (frame_en OR NOT framed_int)) = '1') THEN
               offset_reg <= offset_val;    
            END IF;
         END IF;
   end IF;
   END PROCESS;

--
-- New start position detector
-- 
-- A comparison between offset_val and offset_reg determines if
-- the new offset is different than the current one. If there is
-- a mismatch and frame_en is not asserted, then the nsp output
-- will be asserted.
--

   new_offset <= '0' when offset_val = offset_reg else '1';

   PROCESS (sclk)
   BEGIN
      
      IF sclk'event and sclk = '1' then
        IF (rst = '1') THEN
           nsp_int <= '1';
        ELSIF (ce = '1') THEN
            IF ((comma_detected AND NOT comma_error) = '1') THEN
               nsp_int <= NOT frame_en AND new_offset;    
            END IF;
         END IF;
end IF;
   END PROCESS;

--
-- barrel_in: barrel shifter input register
--
-- This register implements a pipeline delay stage so that the
-- barrel shifter's data input matches the delay on the offset
-- input caused by the offset_reg.
--

   PROCESS (sclk)
   BEGIN

      IF sclk'event and sclk = '1' then
         IF (rst = '1') THEN
           barrel_in <= "0000000000000000000";
         ELSIF (ce = '1') THEN
            barrel_in <= d(8 DOWNTO 0) & in1_reg;    
         END IF;
      end IF;
   END PROCESS;

--
-- barrel shifter
--
-- The barrel shifter extracts a 10-bit field from the 19-bit barrel_in vector.
-- The bits extracted depend on the value of the offset_reg. If offset_reg is
-- an offset_reg value of 9 causes the barrel shifter to output barrel_in[18:9].
--
-- The barrel shifter can be described in many ways. One way is a simple
-- shift assignment as shown below:
--
-- assign barrel_out = barrel_in >> offset_reg;
--
-- This often produces less than optimal results.
--
-- Another method is to explicitly code the barrel shifter as two levels of
-- MUXes. The first level consists of thirteen 3-input MUXes which provide
-- the course shifting in increments of four bits. The second level consists of
-- ten 4-input MUXes which provide fine shifting (1-bit increments). This method
-- produces a much better result with XST.
--
   bs_in <= "00" & barrel_in ;
   bs_sm <= offset_reg(3 DOWNTO 2) ;
   bs_sl <= offset_reg(1 DOWNTO 0) ;

    process(bs_in, bs_sm)                   -- implement first level of MUXes
    begin
        for i in 0 to 12 loop
            case bs_sm is
                when "00"   => bs_m1(i) <= bs_in(i);
                when "01"   => bs_m1(i) <= bs_in(i + 4);
                when "10"   => bs_m1(i) <= bs_in(i + 8);
                when others => bs_m1(i) <= '0';
            end case;
        end loop;
    end process;

    process(bs_m1, bs_sl)                   -- implement second level of MUXes
    begin
        for i in 0 to 9 loop
            case bs_sl is
                when "00"   => barrel_out(i) <= bs_m1(i);
                when "01"   => barrel_out(i) <= bs_m1(i + 1);
                when "10"   => barrel_out(i) <= bs_m1(i + 2);
                when others => barrel_out(i) <= bs_m1(i + 3);
            end case;
        end loop;
    end process;

--
-- The output of the module is the barrel shifter output
--
   q_inv <= barrel_out ;
   q_int <= (q_inv(0) & q_inv(1) & q_inv(2) & q_inv(3) & q_inv(4) & q_inv(5) & 
             q_inv(6) & q_inv(7) & q_inv(8) & q_inv(9));

--
-- Byte counter
--
   
   PROCESS (sclk)
   BEGIN
   IF sclk'event and sclk = '1' then
     if (rst = '1') then
       byte_counter_int <= "000000";
      ELSIF (ce = '1') THEN
         IF (byte_counter_rst = '1') THEN
            byte_counter_int <= "000000";    
         ELSIF (byte_counter_inc = '1') THEN
               byte_counter_int <= byte_counter_int(4 DOWNTO 0) & '1';    
         END IF;
   END IF;
   end IF;
   END PROCESS;
  
   byte_counter_tc <= byte_counter_int(5) ;

--
-- Idle Character Detection
--
-- Current State Logic
   
   PROCESS (sclk)
   BEGIN
      IF sclk'event and sclk = '1' then
         IF (rst = '1') THEN
           Frame_Current_State <= FRAME_START;
         ELSIF (ce = '1') THEN
            Frame_Current_State <= Frame_Next_State;    
         END IF;
      END IF;
   END PROCESS;

   -- Output Forming Logic
   
   PROCESS (Frame_Current_State, byte_counter_tc, comma_detected, new_offset, reframe)
   BEGIN
      byte_counter_rst <= '0';    
      byte_counter_inc <= '0';    
      framed_int <= '0';    
     
      CASE Frame_Current_State IS
         --
         -- Look for the first sync byte
         --
         
         WHEN (FRAME_START) =>
                  byte_counter_rst <= '1';    
         --
         -- Case where SYNC_MODE = '1' (Lock declared only when 2 sync bytes 
         -- are found on the same byte boundary within a 5 byte window). If 
         -- first sync byte found, look for the 2nd sync byte. Start byte 
         -- counter. If byte counter reaches 4 and 2nd sync byte not found, go 
         -- back to FRAME_START.
         --
         
         WHEN (FRAME_SEEK_2ND_SYNC_BYTE) =>
                  IF ((byte_counter_tc OR (comma_detected AND 
                       NOT new_offset)) = '1') THEN
                     byte_counter_rst <= '1';    
                  ELSE
                     byte_counter_inc <= '1';    
                  END IF;
         -- 
         -- Link is framed (locked). Real data is being received.
         -- If reframe signal is asserted on a decode error, go back to 
         -- FRAME_START and start looking for the 1st sync byte. 
         --                                                                         
         WHEN (FRAME_GO) =>
                  framed_int <= NOT reframe; 

         WHEN OTHERS  =>
                  null;
      END CASE;
   END PROCESS;

   -- Next State Logic
   temp1 <= FRAME_SEEK_2ND_SYNC_BYTE WHEN sync_mode = '1' ELSE FRAME_GO;

   PROCESS (Frame_Current_State, comma_detected, byte_counter_tc, 
            new_offset, reframe, temp1)
   BEGIN
      CASE Frame_Current_State IS
         WHEN (FRAME_START) =>
                  IF (comma_detected = '1') THEN
                     Frame_Next_State <= temp1;    
                  ELSE
                     Frame_Next_State <= FRAME_START;    
                  END IF;
         --
         -- Case where SYNC_MODE = '1' (Lock declared only when 2 sync bytes are
         -- found on the same byte boundary within a 5 byte window). If first 
         -- sync byte found, look for the 2nd sync byte. Start byte counter. If 
         -- byte counter reaches 4 and 2nd sync byte not found, go back to 
         -- FRAME_START.
         --
         
         WHEN (FRAME_SEEK_2ND_SYNC_BYTE) =>
         -- If byte_counter = 4 and 2nd sync byte not found (2nd sync byte not 
         -- found within a 5 byte window per ASI spec) , go back to FRAME_START.
                  
                  IF (byte_counter_tc = '1') THEN
                     Frame_Next_State <= FRAME_START;    
                  ELSIF ((comma_detected AND NOT new_offset) = '1') THEN
                     Frame_Next_State <= FRAME_GO;    
                  ELSE
                     Frame_Next_State <= FRAME_SEEK_2ND_SYNC_BYTE;    
                  END IF;
         -- 
         -- Link is framed (locked). Real data is being received.
         -- If 'reframe' signal is asserted on a decode error, go back to 
         -- FRAME_START and start looking for the 1st sync byte. 
         --                                                                
         
         WHEN (FRAME_GO) =>
                  IF (reframe = '1') THEN
                     Frame_Next_State <= FRAME_START;    
                  ELSE
                     Frame_Next_State <= FRAME_GO;    
                  END IF;
         --
         -- Look for the first sync byte
         --
         
         WHEN OTHERS  =>
                  Frame_Next_State <= FRAME_START;    
         
      END CASE;
   END PROCESS;

END asi_par_framer_arch;
