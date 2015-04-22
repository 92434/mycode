-------------------------------------------------------------------------------- 
-- Copyright (c) 2006 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: pkg_8b10b.vhd,v $
--  /   /        Date Last Modified:  $Date: 2007-06-07 11:44:04-06 $
-- /___/   /\    Date Created: March 19, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: pkg_8b10b.vhd,v $
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

-- Description:
--
--      This package contains all the numeric constants for the 8b10b decoder
--      and encoder.


library IEEE;
use IEEE.STD_LOGIC_1164.all;


package PKG_8b10b is

  -----------------------------------------------------------------------------
  -- Disparity Constants
  -----------------------------------------------------------------------------
  constant NEG_RD : std_logic := '0';   -- -1 Negative running disparity
  constant POS_RD : std_logic := '1';   -- +1 Positive running disparity
  constant POS_DISP : std_logic_vector (1 downto 0) := "10";  -- +2 Positive disparity
  constant NEG_DISP : std_logic_vector (1 downto 0) := "01";  -- -2 Negative disparity
  constant ZERO_DISP : std_logic_vector (1 downto 0) := "00"; -- No disparity 

  -----------------------------------------------------------------------------
  -- Error Code Constants
  -----------------------------------------------------------------------------
  constant DISPARITY_ERROR_4B : std_logic := '1';  -- 4B sub-block disparity error
  constant DISPARITY_ERROR_6B : std_logic := '1';  -- 6B sub-block disparity error
  constant DISPARITY_ERROR_10B : std_logic := '1'; -- 10B block disparity error
  constant DISPARITY_OK_4B : std_logic := '0';
  constant DISPARITY_OK_6B : std_logic := '0';
  constant DISPARITY_OK_10B : std_logic := '0';

  -----------------------------------------------------------------------------
  -- 6B/5B sub-block code constants.
  --
  -- These constants define the lookup table values for the 6-bit DX.Y sub-blocks.
  -- Only the X value is defined.  The notation reflects this with D0_x_pos/neg/zer.
  -- The D0 means this is the D0 code word.  The _x_ denotes that Y is a don't care.
  -- Y is represented in the 4b/3b decoder constants.  The neg/pos/zer
  -- denotes the signal polarity of the code word.  Most of the 8b/10b codes are DC
  -- balanced.
  -- Those that are not, can have two possible values depending on the polarity which is
  -- determined real time by the encoder.  The lookup tables must account for these cases
  -- but they decode to the same 5b output.  These values are taken directly from the
  -- original
  -- IBM Journal article by Widmer and Franaszek.  Note that these are defined MSB-LSB
  -- which is the reverse order of the bits in the journal article. 
  -----------------------------------------------------------------------------

  --       Name                                             iedcba
  -----------------------------------------------------------------------------
  constant D0_x_neg     : std_logic_vector (5 downto 0) := "000110";
  constant D0_x_pos     : std_logic_vector (5 downto 0) := "111001";
  constant D1_x_neg     : std_logic_vector (5 downto 0) := "010001";
  constant D1_x_pos     : std_logic_vector (5 downto 0) := "101110";
  constant D2_x_neg     : std_logic_vector (5 downto 0) := "010010";
  constant D2_x_pos     : std_logic_vector (5 downto 0) := "101101";
  constant D3_x_zer     : std_logic_vector (5 downto 0) := "100011";
  constant D4_x_neg     : std_logic_vector (5 downto 0) := "010100";
  constant D4_x_pos     : std_logic_vector (5 downto 0) := "101011";
  constant D5_x_zer     : std_logic_vector (5 downto 0) := "100101";
  constant D6_x_zer     : std_logic_vector (5 downto 0) := "100110";
  constant D7_x_zer1    : std_logic_vector (5 downto 0) := "000111";
  constant D7_x_zer2    : std_logic_vector (5 downto 0) := "111000";
  constant D8_x_neg     : std_logic_vector (5 downto 0) := "011000";
  constant D8_x_pos     : std_logic_vector (5 downto 0) := "100111";
  constant D9_x_zer     : std_logic_vector (5 downto 0) := "101001";
  constant D10_x_zer    : std_logic_vector (5 downto 0) := "101010";
  constant D11_x_zer    : std_logic_vector (5 downto 0) := "001011";
  constant D12_x_zer    : std_logic_vector (5 downto 0) := "101100";
  constant D13_x_zer    : std_logic_vector (5 downto 0) := "001101";
  constant D14_x_zer    : std_logic_vector (5 downto 0) := "001110";
  constant D15_x_neg    : std_logic_vector (5 downto 0) := "000101";
  constant D15_x_pos    : std_logic_vector (5 downto 0) := "111010";
  constant D16_x_neg    : std_logic_vector (5 downto 0) := "001001";
  constant D16_x_pos    : std_logic_vector (5 downto 0) := "110110";
  constant D17_x_zer    : std_logic_vector (5 downto 0) := "110001";
  constant D18_x_zer    : std_logic_vector (5 downto 0) := "110010";
  constant D19_x_zer    : std_logic_vector (5 downto 0) := "010011";
  constant D20_x_zer    : std_logic_vector (5 downto 0) := "110100";
  constant D21_x_zer    : std_logic_vector (5 downto 0) := "010101";
  constant D22_x_zer    : std_logic_vector (5 downto 0) := "010110";
  constant D23K23_x_neg : std_logic_vector (5 downto 0) := "101000";
  constant D23K23_x_pos : std_logic_vector (5 downto 0) := "010111";
  constant D24_x_neg    : std_logic_vector (5 downto 0) := "001100";
  constant D24_x_pos    : std_logic_vector (5 downto 0) := "110011";
  constant D25_x_zer    : std_logic_vector (5 downto 0) := "011001";
  constant D26_x_zer    : std_logic_vector (5 downto 0) := "011010";
  constant D27K27_x_neg : std_logic_vector (5 downto 0) := "100100";
  constant D27K27_x_pos : std_logic_vector (5 downto 0) := "011011";
  constant D28_x_zer    : std_logic_vector (5 downto 0) := "011100";
  constant D29K29_x_neg : std_logic_vector (5 downto 0) := "100010";
  constant D29K29_x_pos : std_logic_vector (5 downto 0) := "011101";
  constant D30K30_x_neg : std_logic_vector (5 downto 0) := "100001";
  constant D30K30_x_pos : std_logic_vector (5 downto 0) := "011110";
  constant D31_x_neg    : std_logic_vector (5 downto 0) := "001010";
  constant D31_x_pos    : std_logic_vector (5 downto 0) := "110101";
  constant K28_x_neg    : std_logic_vector (5 downto 0) := "000011";
  constant K28_x_pos    : std_logic_vector (5 downto 0) := "111100";


  -----------------------------------------------------------------------------
  -- 4b/3b sub-block constants
  -----------------------------------------------------------------------------
  
  --     Name                                           jhgf
  --     ----------------------------------------------------------------------
constant DKx_0_neg  : std_logic_vector (3 downto 0) := "0010";
constant DKx_0_pos  : std_logic_vector (3 downto 0) := "1101";
constant DKx_1_zer  : std_logic_vector (3 downto 0) := "1001";
constant DKx_1_zer2 : std_logic_vector (3 downto 0) := "0110";
constant DKx_2_zer  : std_logic_vector (3 downto 0) := "1010";
constant DKx_2_zer2 : std_logic_vector (3 downto 0) := "0101";
constant DKx_3_zer1 : std_logic_vector (3 downto 0) := "0011";
constant DKx_3_zer2 : std_logic_vector (3 downto 0) := "1100";
constant DKx_4_neg  : std_logic_vector (3 downto 0) := "0100";
constant DKx_4_pos  : std_logic_vector (3 downto 0) := "1011";
constant DKx_5_zer  : std_logic_vector (3 downto 0) := "0101";
constant DKx_5_zer2 : std_logic_vector (3 downto 0) := "1010";
constant DKx_6_zer  : std_logic_vector (3 downto 0) := "0110";
constant DKx_6_zer2 : std_logic_vector (3 downto 0) := "1001";
constant Dx_7_neg   : std_logic_vector (3 downto 0) := "1000";
constant Dx_7_pos   : std_logic_vector (3 downto 0) := "0111";
constant DKx_7_neg  : std_logic_vector (3 downto 0) := "0001";
constant DKx_7_pos  : std_logic_vector (3 downto 0) := "1110";


  -- constants for the remaining special K-code characters.  Note that K28.x is unique in
  -- that the first 6-bit patterns cannot occur with any other code symbol.  The others
  -- can only be identified by examining all 10-bits.
  --
  --     Name                                          jhgfiedcba
  -----------------------------------------------------------------------------
constant K23_7_neg : std_logic_vector (9 downto 0) := "1110101000";
constant K23_7_pos : std_logic_vector (9 downto 0) := "0001010111";
constant K27_7_neg : std_logic_vector (9 downto 0) := "1110100100";
constant K27_7_pos : std_logic_vector (9 downto 0) := "0001011011";
constant K29_7_neg : std_logic_vector (9 downto 0) := "1110100010";
constant K29_7_pos : std_logic_vector (9 downto 0) := "0001011101";
constant K30_7_neg : std_logic_vector (9 downto 0) := "1110100001";
constant K30_7_pos : std_logic_vector (9 downto 0) := "0001011110";


end PKG_8b10b;
