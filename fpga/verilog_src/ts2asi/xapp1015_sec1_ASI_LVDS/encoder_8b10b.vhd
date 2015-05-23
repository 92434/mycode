-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: Jeff Hutchings, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: encoder_8b10b.vhd,v $
--  /   /        Date Last Modified:  $Date: 2007-06-07 11:45:45-06 $
-- /___/   /\    Date Created: March 19, 2007
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: encoder_8b10b.vhd,v $
-- Revision 1.0  2007-06-07 11:45:45-06  hutching
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
-- This entity implements a 8b10b encoder according to the original patent work
-- of Widmer and Franaszek.  It is a synchronous module with registers on the input
-- and output.  It takes in a 8-bit unencoded word and control inputs and outputs
-- a 10-bit code word according to the 8b10b coding rules.  The encoder properly
-- enforces the disparity and run length requirements according to the code.
-------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.all;

use work.PKG_8b10b.all;

-------------------------------------------------------------------------------

entity encoder_8b10b is
  
  port (
    din      : in  std_logic_vector (7 downto 0);
    kin      : in  std_logic;
    clk      : in  std_logic;
    ce       : in  std_logic;
    dout     : out std_logic_vector (9 downto 0);
    valid    : out std_logic;
    code_err : out std_logic);

end encoder_8b10b;

architecture rtl of encoder_8b10b is

  -- Internal Signals
  signal din_k_5b      : std_logic_vector (5 downto 0); -- 5-bit sub_block concatenated with kchar input
  signal din_k_3b      : std_logic_vector (3 downto 0); -- 3-bit sub-block concatenated with kchar input
  signal sub_block_6b : std_logic_vector (5 downto 0);  -- Encoded 6-bit sub-block
  signal sub_block_4b : std_logic_vector (3 downto 0);  -- Initial encoding of 4-bit sub-block
  signal disp_6b      : std_logic_vector (1 downto 0);  -- Disparity of current encoded block
  signal disp_4b      : std_logic_vector (1 downto 0);  -- Disparity of current encoded block
  signal disp6b_muxout : std_logic_vector (1 downto 0); -- 6-bit sub-block disparity of mux output
  signal disp4b_muxout : std_logic_vector (1 downto 0); -- 4-bit sub-block disparity of mux output
  signal disp_10b      : std_logic_vector (1 downto 0); -- Disparity of current 10-bit output code
  signal disp10b_enc   : std_logic_vector (1 downto 0); -- Disparity of initial encoding
   
  signal code6b_inv_sel : std_logic;                    -- Selects complement of 6-bit sub-block
  signal code_6b_mux    : std_logic_vector(5 downto 0); -- 6-bit sub-block mux for normal or complimented
  signal code_4b_mux    : std_logic_vector(3 downto 0); -- 4-bit sub-block mux for normal or complimented
  signal code4b_inv_sel : std_logic; 	                -- Selects complement of 4-bit sub-block
  signal code_10b       : std_logic_vector(9 downto 0); -- 6-bit and 4-bit sub-blocks concatenated for output

  -- True indicates special run-length encoding requires 4-bit sub-block modifier
  signal Dx7_special_case : std_logic;


  -- Internal registers
  signal din_8b_R     : std_logic_vector (7 downto 0) := "00000000";
  signal kin_R        : std_logic := '0';
  signal force_disp_R : std_logic := '0';
  signal disp_in_R    : std_logic := '0';
  signal rd_10b       : std_logic := NEG_RD; -- 10-bit code initial running disparity
  signal valid_R      : std_logic;

  
   
begin  -- rtl

  -----------------------------------------------------------------------------
  -- Synchronous Process
  -----------------------------------------------------------------------------
  CLK_PROC: process (clk)
  begin  -- process CLK_PROC
    if clk'event and clk = '1' then  -- rising clock edge
      valid_R <= ce;
      valid   <= valid_R;
      
      if (ce = '1') then
        
        -- 10b running disparity tracking process
        if (disp_10b = NEG_DISP) then
          rd_10b <= NEG_RD;
        elsif (disp_10b = POS_DISP) then
          rd_10b <= POS_RD;
        end if;

        -- Output registers
        dout    <= code_10b;
        
        
        -- Input Registers
        din_8b_R <= din;
        kin_R    <= kin;

      end if;
    end if;
  end process CLK_PROC;

  -----------------------------------------------------------------------------
  -- Continuous Assignments
  -----------------------------------------------------------------------------
  din_k_5b <= kin_R & din_8b_R(4 downto 0);
  din_k_3b <= kin_R & din_8b_R(7 downto 5);

  -- 10-bit output code is simple concatenation of sub_blocks
  code_10b <= code_4b_mux & code_6b_mux;

  -- sub_block complentary code select mux for 6-bit sub-block
  -- Verilog: code_6b_mux = code6b_inv_sel ? ~sub_block_6b : sub_block_6b;
  code_6b_mux <= sub_block_6b when (code6b_inv_sel = '0') else (not sub_block_6b); 

  -- Sub-block mux output disparity calculation.  These are used to track
  -- the disparity of the 10-bit output code.
  -- Verilog: disp6b_muxout = (code6b_inv_sel & (disp_6b != `ZERO_DISP)) ? ~disp_6b : disp_6b;
  disp6b_muxout <= not disp_6b when (code6b_inv_sel = '1' and disp_6b /= ZERO_DISP) else disp_6b;
  
  -- Verilog: disp4b_muxout = (code4b_inv_sel & (disp_4b != `ZERO_DISP)) ? ~disp_4b : disp_4b;
  disp4b_muxout <= not disp_4b when (code4b_inv_sel = '1' and disp_4b /= ZERO_DISP) else disp_4b;

  -- 10-bit pre-output code disparity.
  disp_10b <= disp6b_muxout or disp4b_muxout;
  disp10b_enc <= disp_6b or disp_4b;

  -----------------------------------------------------------------------------
  -----------------------------------------------------------------------------
  -- Encode Process
  -----------------------------------------------------------------------------
  
  

  ENCODE_PROC: process (din_k_5b,
                        sub_block_6b,
                        din_k_3b,
                        sub_block_4b,
                        disp10b_enc,
                        rd_10b,
                        kin_R,
                        Dx7_special_case,
                        code4b_inv_sel)
  begin  -- process ENCODE_PROC

    ---------------------------------------------------------------------------
    -- Case statement to encode least significant 5 input bits and K input
    -- into 6-bit sub-blocks according to coding rules.  Note that not
    -- all cases are represented here.  Some input values have both a
    -- positive/negative coded word result.  One is simply the full complement
    -- of the other which is handled elsewhere depending on the current value
    -- of the running disparity.  All encodings below are either neg. or zero.
    -- A selectable inversion stage is used later to select the pos. if needed.
    case din_k_5b is
      when "000000" => sub_block_6b <= D0_x_neg;     -- D.0
      when "000001" => sub_block_6b <= D1_x_neg;     -- D.1
      when "000010" => sub_block_6b <= D2_x_neg;     -- D.2
      when "000011" => sub_block_6b <= D3_x_zer;     -- D.3
      when "000100" => sub_block_6b <= D4_x_neg;     -- D.4
      when "000101" => sub_block_6b <= D5_x_zer;     -- D.5
      when "000110" => sub_block_6b <= D6_x_zer;     -- D.6
      when "000111" => sub_block_6b <= D7_x_zer2;    -- D.7
      when "001000" => sub_block_6b <= D8_x_neg;     -- D.8
      when "001001" => sub_block_6b <= D9_x_zer;     -- D.9
      when "001010" => sub_block_6b <= D10_x_zer;    -- D.10
      when "001011" => sub_block_6b <= D11_x_zer;    -- D.11
      when "001100" => sub_block_6b <= D12_x_zer;    -- D.12
      when "001101" => sub_block_6b <= D13_x_zer;    -- D.13
      when "001110" => sub_block_6b <= D14_x_zer;    -- D.14
      when "001111" => sub_block_6b <= D15_x_neg;    -- D.15
      when "010000" => sub_block_6b <= D16_x_neg;    -- D.16
      when "010001" => sub_block_6b <= D17_x_zer;    -- D.17
      when "010010" => sub_block_6b <= D18_x_zer;    -- D.18
      when "010011" => sub_block_6b <= D19_x_zer;    -- D.19
      when "010100" => sub_block_6b <= D20_x_zer;    -- D.20
      when "010101" => sub_block_6b <= D21_x_zer;    -- D.21
      when "010110" => sub_block_6b <= D22_x_zer;    -- D.22
      when "010111" => sub_block_6b <= D23K23_x_neg; -- D.23
      when "110111" => sub_block_6b <= D23K23_x_neg; -- K.23
      when "011000" => sub_block_6b <= D24_x_neg;    -- D.24
      when "011001" => sub_block_6b <= D25_x_zer;    -- D.25
      when "011010" => sub_block_6b <= D26_x_zer;    -- D.26
      when "011011" => sub_block_6b <= D27K27_x_neg; -- D.27
      when "111011" => sub_block_6b <= D27K27_x_neg; -- K.27
      when "011100" => sub_block_6b <= D28_x_zer;    -- D.28
      when "111100" => sub_block_6b <= K28_x_neg;    -- K.28
      when "011101" => sub_block_6b <= D29K29_x_neg; -- D.29
      when "111101" => sub_block_6b <= D29K29_x_neg; -- K.29
      when "011110" => sub_block_6b <= D30K30_x_neg; -- D.30
      when "111110" => sub_block_6b <= D30K30_x_neg; -- K.30
      when "011111" => sub_block_6b <= D31_x_neg;    -- D.31
      when others   => sub_block_6b <= "111111";     -- Invalid inputs
    end case; -- case(din_5b)

    ---------------------------------------------------------------------------
    -- Case statement to encode most significant 3 bits plus K input into
    -- 4b sub-blocks according to code rules.  As in the 5b/6b case, not all
    -- possibilities are represented.  We encode either zero or pos and a
    -- selectable inversion stage later flips the code if running disparity
    -- rules require it.  This makes the ROM small and the code simpler.
    case din_k_3b is
      when "0000" => sub_block_4b <= DKx_0_pos;      -- D/K.x.0
      when "0001" => sub_block_4b <= DKx_1_zer;      -- D/K.x.1
      when "0010" => sub_block_4b <= DKx_2_zer;      -- D/K.x.2
      when "0011" => sub_block_4b <= DKx_3_zer2;     -- D/K.x.3
      when "0100" => sub_block_4b <= DKx_4_pos;      -- D/K.x.4
      when "0101" => sub_block_4b <= DKx_5_zer;      -- D/K.x.5
      when "0110" => sub_block_4b <= DKx_6_zer;      -- D/K.x.6
      when "0111" =>
        if (sub_block_6b = D17_x_zer or sub_block_6b = D18_x_zer or sub_block_6b = D20_x_zer) then
          sub_block_4b <= DKx_7_pos;
        else
          sub_block_4b <= Dx_7_pos;       -- D.x.7
        end if;
      when "1111" => sub_block_4b <= DKx_7_pos;      -- D/K.x.7
      when "1000" => sub_block_4b <= DKx_0_pos;      -- K28.0
      when "1001" => sub_block_4b <= DKx_1_zer2;     -- K28.1
      when "1010" => sub_block_4b <= DKx_2_zer2;     -- K28.2
      when "1011" => sub_block_4b <= DKx_3_zer1;     -- K28.3
      when "1100" => sub_block_4b <= DKx_4_pos;      -- K28.4
      when "1101" => sub_block_4b <= DKx_5_zer2;     -- K28.5
      when "1110" => sub_block_4b <= DKx_6_zer2;     -- K28.6
      when others => sub_block_4b <= X"F";           -- Illegal inputs
    end case; -- case(din_k_3b)
    
    ---------------------------------------------------------------------------
    -- Special case statement to flag the 6 special cases around certain run length encodings
	if (sub_block_4b = Dx_7_pos or sub_block_4b = DKx_7_pos) then
	  case sub_block_6b is
	    when D11_x_zer => Dx7_special_case <= '1';
	    when D13_x_zer => Dx7_special_case <= '1';
	    when D14_x_zer => Dx7_special_case <= '1';
	    when D17_x_zer => Dx7_special_case <= '1';
	    when D18_x_zer => Dx7_special_case <= '1';
	    when D20_x_zer => Dx7_special_case <= '1';
	    when others    => Dx7_special_case <= '0';
          end case; -- case(sub_block_6b)
	else
	  Dx7_special_case <= '0';
        end if;

    ---------------------------------------------------------------------------
    -- This case statement calculates the disparity of the 6b
    -- sub block.  To shrink this lookup table, we take advantage
    -- of the fact that we only need to check if it's NEG.  Otherwise
    -- it must be ZERO based on how we encode internally.
    case sub_block_6b is
      when D0_x_neg     => disp_6b <= NEG_DISP;
      when D1_x_neg     => disp_6b <= NEG_DISP;
      when D2_x_neg     => disp_6b <= NEG_DISP;
      when D4_x_neg     => disp_6b <= NEG_DISP;
      when D8_x_neg     => disp_6b <= NEG_DISP;
      when D15_x_neg    => disp_6b <= NEG_DISP;
      when D16_x_neg    => disp_6b <= NEG_DISP;
      when D23K23_x_neg => disp_6b <= NEG_DISP;
      when D24_x_neg    => disp_6b <= NEG_DISP;
      when D27K27_x_neg => disp_6b <= NEG_DISP;
      when K28_x_neg    => disp_6b <= NEG_DISP;
      when D29K29_x_neg => disp_6b <= NEG_DISP;
      when D30K30_x_neg => disp_6b <= NEG_DISP;
      when D31_x_neg    => disp_6b <= NEG_DISP;
      when others       => disp_6b <= ZERO_DISP;
    end case; -- case(sub_block_6b)
	

    ---------------------------------------------------------------------------
    -- This case statement, using the same advantage determines
    -- POS or ZERO disparity for the 4b sub-block
    case sub_block_4b is
      when DKx_0_pos => disp_4b <= POS_DISP;
      when DKx_4_pos => disp_4b <= POS_DISP;
      when Dx_7_pos  => disp_4b <= POS_DISP;
      when DKx_7_pos => disp_4b <= POS_DISP;
      when others    => disp_4b <= ZERO_DISP;
    end case; -- case(sub_block_4b)

    ---------------------------------------------------------------------------
    -- This case statement determines when we complement the 6b, 4b, both
    -- or niether of the sub-blocks, based on a combination of 10-bit running
    -- disparity, current sub-block disparity and some special cases.
    case disp10b_enc is
      
      when "00" =>
        if (rd_10b = NEG_RD) then
          if (sub_block_4b = DKx_3_zer2) then
            code4b_inv_sel <= '1';
          else
            code4b_inv_sel <= '0';
          end if;
          if (sub_block_6b = D7_x_zer2) then
            code6b_inv_sel <= '1';
          else
            code6b_inv_sel <= '0';
          end if;
        else
          code4b_inv_sel <= '0';
          code6b_inv_sel <= '0';
        end if;
        
        
      when "10" =>
        if (rd_10b = POS_RD) then
          code6b_inv_sel <= '0';
          code4b_inv_sel <= '1';
        elsif (rd_10b = NEG_RD and sub_block_6b = D7_x_zer2) then
          code6b_inv_sel <= '1';
          code4b_inv_sel <= '0';
        else
          code6b_inv_sel <= '0';
          code4b_inv_sel <= '0';
        end if;
        
      when "01" =>
        if (rd_10b = NEG_RD) then
          code6b_inv_sel <= '1';
          if (kin_R = '1') then
            code4b_inv_sel <= '1';
          else
            code4b_inv_sel <= '0';
          end if;
        elsif (rd_10b = POS_RD and sub_block_4b = DKx_3_zer2  and kin_R = '0') then
          code6b_inv_sel <= '0'; 
          code4b_inv_sel <= '1';
        else
          code6b_inv_sel <= '0';
          code4b_inv_sel <= '0';
        end if;
        
      when "11" =>
        if (rd_10b = POS_RD) then
          code6b_inv_sel <= '0';
          code4b_inv_sel <= '0';
        else
          code6b_inv_sel <= '1';
          code4b_inv_sel <= '1';
        end if;

      when others =>
        
    end case; -- case(disp10b_enc)


    ---------------------------------------------------------------------------
    -- 4-bit sub-block complement mux.  Handles the DX.7 DKx.7 special cases for run length
    if (Dx7_special_case = '0') then  -- No special case, complement or not normally.
      if (code4b_inv_sel = '1') then
        code_4b_mux <= not sub_block_4b;
      else
        code_4b_mux <= sub_block_4b;
      end if;
    else     -- Special case detected.  Don't complement normally.  Select different 4-bit sub-block.
      if (code4b_inv_sel = '0') then
        code_4b_mux <= sub_block_4b;
      else
        case sub_block_6b is
          when D11_x_zer => code_4b_mux <= DKx_7_neg;
          when D13_x_zer => code_4b_mux <= DKx_7_neg;
          when D14_x_zer => code_4b_mux <= DKx_7_neg;
          when D17_x_zer => code_4b_mux <= Dx_7_neg;
          when D18_x_zer => code_4b_mux <= Dx_7_neg;
          when D20_x_zer => code_4b_mux <= Dx_7_neg;
          when others    => code_4b_mux <= sub_block_4b;
        end case; -- case(sub_block_6b)
      end if;
    end if; -- else: !if(!Dx7_special_case)
    
  end process ENCODE_PROC;
  
end rtl;
