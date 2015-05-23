-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: decoder_8b10b.vhd,v $
--  /   /        Date Last Modified:  $Date: 2008-02-04 13:28:06-07 $
-- /___/   /\    Date Created: March 19, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: decoder_8b10b.vhd,v $
-- Revision 1.1  2008-02-04 13:28:06-07  hutching
-- Removed q_valid from reset block on sinit assertion.  VHDL now matches Verilog.
--
-- Revision 1.0  2007-06-07 11:44:04-06  hutching
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
-- Description:
--
-- This entity implements a 8b10b decoder according to the original patent work
-- of Widmer and Franaszek.  It is a synchronous module with registers on the
-- input and output.  It takes in a 10-bit 8b10b encoded word, and outputs an
-- 8-bit data word and a control bit to indicate if the 8-bit output data is one
-- of 12 special K-codes.
-------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.all;

use work.PKG_8b10b.all;

-------------------------------------------------------------------------------

entity decoder_8b10b is
  
  port (
    clk      : in std_logic;
    ce       : in std_logic;
    sinit    : in std_logic;
    d_10b    : in std_logic_vector (9 downto 0);
    is_kcode : out std_logic;
    code_err : out std_logic;
    disp_err : out std_logic;
    q_valid  : out std_logic;
    q_8b     : out std_logic_vector (7 downto 0));

end decoder_8b10b;

-------------------------------------------------------------------------------

architecture rtl of decoder_8b10b is

  -- Internal registers
  signal d_10b_R : std_logic_vector (9 downto 0) := "0000000000";
  signal running_disp_10b : std_logic := NEG_RD;

  -- Internal combinatorial signals
  signal decoded_5b : std_logic_vector (5 downto 0);  -- Decoded 5-bit sub-block + valid bit
  signal decoded_3b : std_logic_vector (3 downto 0);  -- Decoded 3-bit sub-block + valid bit
  signal disp_4b : std_logic_vector (1 downto 0);     -- Current 4b sub-block disparity
  signal disp_6b : std_logic_vector (1 downto 0);     -- Current 6b sub-block disparity
  signal disp_10b : std_logic_vector (1 downto 0);    -- Current 10b symbol disparity
  signal disp_err_10b : std_logic;      -- 10b symbol disparity error
  signal kchar_detected : std_logic;    -- Indicated current symbol is a K character
  signal invalid_10b : std_logic;       -- Bitwise OR of invalid_4b, invalid_6b
  signal disparity_error : std_logic;   -- Bitwise OR of all disparity error signals
  signal invalid_6b : std_logic;        -- 6b sub-block "out of table" indicator
  signal invalid_4b : std_logic;        -- 4b sub-block "out of table" indicator
                                                     
  

begin  -- rtl

  -----------------------------------------------------------------------------
  -- Synchronous process block for clk
  -----------------------------------------------------------------------------
  -- purpose: Register updates for signal "clk"
  -- type   : sequential
  -- inputs : clk, sinit
  -- outputs: 
  CLK_PROC: process (clk)
  begin  -- process CLK
    if clk'event and clk = '1' then  -- rising clock edge

      -- Unconditional register assignments
      q_valid <= ce;
      
      -- Reset statements
      if sinit = '1' then
        running_disp_10b <= NEG_RD;
	code_err         <= '0';
	is_kcode         <= '0';
	q_8b             <= "00000000";
	disp_err         <= '0';

        -- clock enable statements
      elsif ce = '1' then

        -- Register updates
        d_10b_R    <= d_10b;
	code_err   <= invalid_10b;
	is_kcode   <= kchar_detected;
	disp_err   <= disparity_error;
	q_8b       <= decoded_3b(2 downto 0)& decoded_5b(4 downto 0);

        -- Disparity tracking - running disparity of 10b symbols
	-- Note: Disparity tracking is ALWAYS updated, regardless of whether or not
	-- the code was valid or there was a disparity error.
        if (disp_10b = POS_DISP)then
           running_disp_10b <= POS_RD;
	elsif (disp_10b = NEG_DISP) then
          running_disp_10b <= NEG_RD;
        end if;
      end if;
    end if;
  end process CLK_PROC;

  -----------------------------------------------------------------------------
  -- Concurrent assignments
  -----------------------------------------------------------------------------
  invalid_6b      <= not decoded_3b(3);         -- Tag bit asserted in decoded data =  good code
  invalid_4b      <= not decoded_5b(5);         -- Ditto...
  invalid_10b     <= invalid_6b or invalid_4b;  -- Indicates a code error occurred.
  disp_10b        <= disp_6b or disp_4b;
  disparity_error <= disp_err_10b;


  -----------------------------------------------------------------------------
  -- ROM lookup tables
  -- These are written as combinatorial processes
  -----------------------------------------------------------------------------

  -----------------------------------------------------------------------------
  -- purpose: 5b sub-block decoder, 4b sub-block decoder, disparity and error
  -- code
  -- type   : combinational
  -- inputs : d_10b_R or disp_10b or disp_4b or disp_6b or running_disp_10b
  -- outputs: 
  DECODE: process (d_10b_R, disp_10b, disp_4b, disp_6b, running_disp_10b)
  begin  -- process DECODE

    ---------------------------------------------------------------------------
    -- 5b sub-block decoder
    ---------------------------------------------------------------------------
    -- Case statement to decode 5-bit sub-block.  The decoding of bits 4:0 is
    -- ONLY dependent on encoded input bits 5:0.  This enables use of a very
    -- simple (but lengthy) case statement to decode these values.  D0_x_neg/pos
    -- denotes the D0.Y pos/neg polarity case of the input code where we "don't
    -- care" what Y is.  Hence, the 'x' "assigned" to Y.  Note that not all the
    -- pos/neg polarity cases are handled.  In other words, there are only 48
    -- items in the case statement.  This is because for 18 items, the 5-bit
    -- encoded data is the same so the duplicates are not needed.  In addition,
    -- the valid bit is also set for each valid decode case.  This is used later
    -- for code checking.  If we fall through this table to the default, then we
    -- had an invalid 6b code, which means the 10b symbol was invalid.
    ---------------------------------------------------------------------------
    case d_10b_R(5 downto 0) is
      when D0_x_neg     => decoded_5b <= '1' & "00000";
      when D0_x_pos     => decoded_5b <= '1' & "00000";
      when D1_x_neg     => decoded_5b <= '1' & "00001";
      when D1_x_pos     => decoded_5b <= '1' & "00001";
      when D2_x_neg     => decoded_5b <= '1' & "00010";
      when D2_x_pos     => decoded_5b <= '1' & "00010";
      when D3_x_zer     => decoded_5b <= '1' & "00011";
      when D4_x_neg     => decoded_5b <= '1' & "00100";
      when D4_x_pos     => decoded_5b <= '1' & "00100";
      when D5_x_zer     => decoded_5b <= '1' & "00101";
      when D6_x_zer     => decoded_5b <= '1' & "00110";
      when D7_x_zer1    => decoded_5b <= '1' & "00111";
      when D7_x_zer2    => decoded_5b <= '1' & "00111";
      when D8_x_neg     => decoded_5b <= '1' & "01000";
      when D8_x_pos     => decoded_5b <= '1' & "01000";
      when D9_x_zer     => decoded_5b <= '1' & "01001";
      when D10_x_zer    => decoded_5b <= '1' & "01010";
      when D11_x_zer    => decoded_5b <= '1' & "01011";
      when D12_x_zer    => decoded_5b <= '1' & "01100";
      when D13_x_zer    => decoded_5b <= '1' & "01101";
      when D14_x_zer    => decoded_5b <= '1' & "01110";
      when D15_x_neg    => decoded_5b <= '1' & "01111";
      when D15_x_pos    => decoded_5b <= '1' & "01111";
      when D16_x_neg    => decoded_5b <= '1' & "10000";
      when D16_x_pos    => decoded_5b <= '1' & "10000";
      when D17_x_zer    => decoded_5b <= '1' & "10001";
      when D18_x_zer    => decoded_5b <= '1' & "10010";
      when D19_x_zer    => decoded_5b <= '1' & "10011";
      when D20_x_zer    => decoded_5b <= '1' & "10100";
      when D21_x_zer    => decoded_5b <= '1' & "10101";
      when D22_x_zer    => decoded_5b <= '1' & "10110";
      when D23K23_x_neg => decoded_5b <= '1' & "10111";
      when D23K23_x_pos => decoded_5b <= '1' & "10111";
      when D24_x_neg    => decoded_5b <= '1' & "11000";
      when D24_x_pos    => decoded_5b <= '1' & "11000";
      when D25_x_zer    => decoded_5b <= '1' & "11001";
      when D26_x_zer    => decoded_5b <= '1' & "11010";
      when D27K27_x_neg => decoded_5b <= '1' & "11011";
      when D27K27_x_pos => decoded_5b <= '1' & "11011";
      when D28_x_zer    => decoded_5b <= '1' & "11100";
      when D29K29_x_neg => decoded_5b <= '1' & "11101";
      when D29K29_x_pos => decoded_5b <= '1' & "11101";
      when D30K30_x_neg => decoded_5b <= '1' & "11110";
      when D30K30_x_pos => decoded_5b <= '1' & "11110";
      when D31_x_neg    => decoded_5b <= '1' & "11111";
      when D31_x_pos    => decoded_5b <= '1' & "11111";
      when K28_x_neg    => decoded_5b <= '1' & "11100";
      when K28_x_pos    => decoded_5b <= '1' & "11100";
      when others       => decoded_5b <= '0' & "11111";  --  Invalid sub-block
    end case;

    ---------------------------------------------------------------------------
    -- 3b sub-block decoder
    ---------------------------------------------------------------------------
    -- Decoder to generate the 3-bit sub block  value based on all 10-bit
    -- inputs.  Unlike the 5b output bits, the 3b output bits are dependent on
    -- all 10 inputs and also have some special cases we must consider.
    -- Notation in this case statement is the same as the 5b output sub block
    -- decoder.
      case d_10b_R(9 downto 6) is  
	when DKx_0_neg  => decoded_3b <= '1' & "000";
        when DKx_0_pos  => decoded_3b <= '1' & "000";
	when DKx_1_zer  => 
	   if (d_10b_R(5 downto 0) = K28_x_neg) then
	     decoded_3b <= '1' & "110";  -- K28.6 special case
	   else
	     decoded_3b <= '1' & "001";  
           end if;
	when DKx_2_zer  =>  
	   if ( d_10b_R(5 downto 0) = K28_x_neg) then
	     decoded_3b <= '1' & "101";  -- K28.5 positive special case
	   else
	     decoded_3b <= '1' & "010";  
           end if;
	when DKx_3_zer1  => decoded_3b <= '1' & "011";
	when DKx_3_zer2  => decoded_3b <= '1' & "011";
	when DKx_4_neg  => decoded_3b <= '1' & "100";
	when DKx_4_pos  => decoded_3b <= '1' & "100";
	when DKx_5_zer  =>  
	   if (d_10b_R(5 downto 0) = K28_x_neg) then
	     decoded_3b <= '1' & "010";  -- K28.2 special case
	   else
	     decoded_3b <= '1' & "101";
           end if;
	when DKx_6_zer  => 
	   if (d_10b_R(5 downto 0) = K28_x_neg) then
	     decoded_3b <= '1' & "100";  -- K28.1 special case
	   else
	     decoded_3b <= '1' & "110";  
           end if;
	when Dx_7_neg   => decoded_3b <= '1' & "111";
	when Dx_7_pos   => decoded_3b <= '1' & "111";
	when DKx_7_neg  => decoded_3b <= '1' & "111";
	when DKx_7_pos  => decoded_3b <= '1' & "111";
	when others => decoded_3b <= '0' & "111";  -- Invalid 4b sub-block
      end case;

    ---------------------------------------------------------------------------

    ---------------------------------------------------------------------------
    -- Disparity calculations for 4b and 6b sub-blocks
    ---------------------------------------------------------------------------
    case d_10b_R(9 downto 6)is
	when DKx_0_neg => disp_4b <= NEG_DISP;
	when DKx_0_pos => disp_4b <= POS_DISP;
	when DKx_4_neg => disp_4b <= NEG_DISP;
	when DKx_4_pos => disp_4b <= POS_DISP;
	when Dx_7_neg  => disp_4b <= NEG_DISP;
	when Dx_7_pos  => disp_4b <= POS_DISP;
	when DKx_7_neg => disp_4b <= NEG_DISP;
	when DKx_7_pos => disp_4b <= POS_DISP;
	when others    => disp_4b <= ZERO_DISP;
    end case;

      case d_10b_R(5 downto 0) is
	when D0_x_neg     => disp_6b <= NEG_DISP;
	when D0_x_pos     => disp_6b <= POS_DISP;
	when D1_x_neg     => disp_6b <= NEG_DISP;
	when D1_x_pos     => disp_6b <= POS_DISP;
	when D2_x_neg     => disp_6b <= NEG_DISP;
	when D2_x_pos     => disp_6b <= POS_DISP;
	when D4_x_neg     => disp_6b <= NEG_DISP;
	when D4_x_pos     => disp_6b <= POS_DISP;
	when D8_x_neg     => disp_6b <= NEG_DISP;
	when D8_x_pos     => disp_6b <= POS_DISP;
	when D15_x_neg    => disp_6b <= NEG_DISP;
	when D15_x_pos    => disp_6b <= POS_DISP;
	when D16_x_neg    => disp_6b <= NEG_DISP;
	when D16_x_pos    => disp_6b <= POS_DISP;
	when D23K23_x_neg => disp_6b <= NEG_DISP;
	when D23K23_x_pos => disp_6b <= POS_DISP;
	when D24_x_neg    => disp_6b <= NEG_DISP;
	when D24_x_pos    => disp_6b <= POS_DISP;
	when D27K27_x_neg => disp_6b <= NEG_DISP;
	when D27K27_x_pos => disp_6b <= POS_DISP;
	when D29K29_x_neg => disp_6b <= NEG_DISP;
	when D29K29_x_pos => disp_6b <= POS_DISP;
	when D30K30_x_neg => disp_6b <= NEG_DISP;
	when D30K30_x_pos => disp_6b <= POS_DISP;
	when D31_x_neg    => disp_6b <= NEG_DISP;
	when D31_x_pos    => disp_6b <= POS_DISP;
	when K28_x_neg    => disp_6b <= NEG_DISP;
	when K28_x_pos    => disp_6b <= POS_DISP;
	when others       => disp_6b <= ZERO_DISP;
      end case;

    ---------------------------------------------------------------------------

    ---------------------------------------------------------------------------
    -- Disparity checking
    ---------------------------------------------------------------------------
    -- For the 10b disparity compare, we must check the current disparity value
    -- (POS, NEG, ZERO) against the running disparity which is POS, NEG, or
    -- ZERO.  The same basic conditions hold true:  If running disparity and
    -- current disparity are the same, it's an error.

      if (disp_4b = POS_DISP and disp_6b = POS_DISP) then -- Both positive - error
	disp_err_10b <= DISPARITY_ERROR_10B;
      elsif (disp_4b = NEG_DISP and disp_6b = NEG_DISP) then -- Both negative - error
	disp_err_10b <= DISPARITY_ERROR_10B;
      elsif (disp_10b = POS_DISP and running_disp_10b = POS_RD) then -- Both are positive - ERROR
	disp_err_10b <= DISPARITY_ERROR_10B;
      elsif (disp_10b = NEG_DISP and running_disp_10b = NEG_RD) then -- Both are negative - ERROR
	disp_err_10b <= DISPARITY_ERROR_10B;
      elsif (disp_10b = POS_DISP and running_disp_10b = NEG_RD) then -- Current is +2, running is -1
	disp_err_10b <= DISPARITY_OK_10B;
      else
	disp_err_10b <= DISPARITY_OK_10B;     -- Current -2, running is +1
      end if;                                 -- or current is 0 running is +1
                                              -- or current is 0 running is -1
                                                    	
  

    ---------------------------------------------------------------------------
    -- K-code detection
    ---------------------------------------------------------------------------
    -- Now we need to determine when to assert the is_kcode output.  When asserted, this
    -- output indicates that the current symbol is a K-code.  The K-codes have the same
    -- decoded values as the D-codes, so the only way to differentiate them is with this
    -- output.  It is asserted based on a simple lookup table.
      if (d_10b_R(5 downto 0) = K28_x_neg or d_10b_R(5 downto 0) = K28_x_pos) then
	kchar_detected <= '1';
      else 
	 case d_10b_R is 
	    when K23_7_neg => kchar_detected <= '1';
	    when K23_7_pos => kchar_detected <= '1';
	    when K27_7_neg => kchar_detected <= '1';
	    when K27_7_pos => kchar_detected <= '1';
	    when K29_7_neg => kchar_detected <= '1';
	    when K29_7_pos => kchar_detected <= '1';
	    when K30_7_neg => kchar_detected <= '1';
	    when K30_7_pos => kchar_detected <= '1';
	    when others    => kchar_detected <= '0';
         end case;
      end if;
      
  end process DECODE;
  
end rtl;
