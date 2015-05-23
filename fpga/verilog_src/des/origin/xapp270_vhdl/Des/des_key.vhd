
--#########################################################################
--                                                                       --
--                                                                       --
--  Module      : DES_KEY.vhd                                            --
--  Last Update : 08/01/2001                                             --
--                                                                       --
--  Description : One round of Key scheduler                             --
--                                                                       --
--                                                                       --
--  Designer    : Vikram Pasham                                          --
--  Company     : Xilinx & Inc.                                          --
--                                                                       --
--                                                                       --
--  Disclaimer  : THESE DESIGNS ARE PROVIDED "AS IS" WITH NO WARRANTY    --
--                WHATSOEVER AND XILINX SPECIFICALLY DISCLAIMS ANY       --
--                IMPLIED WARRANTIES OF MERCHANTABILITY & FITNESS FOR    --
--                A PARTICULAR PURPOSE, OR AGAINST INFRINGEMENT.         --
--                THEY ARE ONLY INTENDED TO BE USED BY XILINX            --
--                CUSTOMERS, AND WITHIN XILINX DEVICES.                  --
--                                                                       --
--                Copyright (c) 2000 Xilinx, Inc.                        --
--                All rights reserved                                    --
--                                                                       --
--                                                                       --
--                                                                       --
--#########################################################################



library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use Work.des_const.all;

-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on

entity DES_KEY is
generic (cs : std_logic_vector(1 to 5));
  port(clk      : in std_logic;         
       key_in_l : in std_logic_vector(1 to 28);       -- left 28 bit key
       key_in_r : in std_logic_vector(1 to 28);       -- right 28 bit key
       key_out_l_reg : out std_logic_vector(1 to 28); -- circularly shifted 
                                                      -- key_in_l;
       key_out_r_reg : out std_logic_vector(1 to 28); -- circularly shifted 
                                                      -- key_in_r
       compressed_key: out std_logic_vector(1 to 48); -- shifted key after 
                                                     --compression permutation
       decrypt  : in std_logic          -- shift right if decrypt=1, else left
       );            
            
end DES_KEY;


architecture DES_KEY_RTL of DES_KEY is

component SRL16 
port(CLK : in std_logic;
     D : in std_logic;
     A0 : in std_logic;
     A1 : in std_logic;
     A2 : in std_logic;
     A3 : in std_logic;
     Q  : out std_logic);
end component;


signal key_out_lr: std_logic_vector(1 to 56);
                                     -- Concatenated key_out_l + key_out_r
signal key_out_lr_reg : std_logic_vector(1 to 56);  
                                  -- Concatenated key_out_l + key_out_r reg
signal shift_1bit  : std_logic;
signal no_shift : std_logic;
signal key_out_l, key_out_r : std_logic_vector(1 to 28);

signal key_out_l_reg1, key_out_r_reg1 : std_logic_vector(1 to 28);  
                                  -- Delay to accommodate deeper pipe in F
signal key_out_l_reg2, key_out_r_reg2 : std_logic_vector(1 to 28);  
                                  -- Delay to accommodate deeper pipe in F
signal key_out_l_reg3, key_out_r_reg3 : std_logic_vector(1 to 28);

signal key_shift_select: std_logic_vector(1 to 2);

signal key_l_1, key_l_2, key_l_3, key_l_4 : std_logic_vector(1 to 16); 
signal key_l_5, key_l_6, key_l_7, key_l_8 : std_logic_vector(1 to 16);
signal key_l_9, key_l_10, key_l_11, key_l_12 : std_logic_vector(1 to 16);
signal key_l_13, key_l_14, key_l_15 : std_logic_vector(1 to 16);
signal key_l_16, key_l_17, key_l_18 : std_logic_vector(1 to 16);  
signal key_l_19, key_l_20, key_l_21 : std_logic_vector(1 to 16);
signal key_l_22, key_l_23, key_l_24 : std_logic_vector(1 to 16);
signal key_l_25, key_l_26, key_l_27, key_l_28 : std_logic_vector(1 to 16) ;


signal key_r_1, key_r_2, key_r_3, key_r_4 : std_logic_vector(1 to 16); 
signal key_r_5, key_r_6, key_r_7, key_r_8 : std_logic_vector(1 to 16);
signal key_r_9, key_r_10, key_r_11, key_r_12 : std_logic_vector(1 to 16);
signal key_r_13, key_r_14, key_r_15 : std_logic_vector(1 to 16);
signal key_r_16, key_r_17, key_r_18 : std_logic_vector(1 to 16);
signal key_r_19, key_r_20, key_r_21 : std_logic_vector(1 to 16);
signal key_r_22, key_r_23, key_r_24 : std_logic_vector(1 to 16);
signal key_r_25, key_r_26, key_r_27, key_r_28 : std_logic_vector(1 to 16);

signal srl_addr : std_logic_vector(1 to 4) := "0000";

begin  -- DES_KEY_RTL

shift_1bit <= '1' when (cs = S1 or cs = S2 or cs = S9 or cs = S16) else
             '0';
no_shift <= '1' when (decrypt = '1' and cs ="00001") else
	    '0';

key_shift_select <= decrypt & shift_1bit;

process(no_shift, key_shift_select, key_in_l, key_in_r)
begin
	if(no_shift = '1') then
	  key_out_l <= key_in_l;
	  key_out_r <= key_in_r;
	else
	  case key_shift_select is
            when "00" => 
			key_out_l <= key_in_l(3 to 28) & key_in_l(1 to 2);
			key_out_r <= key_in_r(3 to 28) & key_in_r(1 to 2);
            when "01" => 
			key_out_l <= key_in_l(2 to 28) & key_in_l(1);
			key_out_r <= key_in_r(2 to 28) & key_in_r(1);
            when "10" => 
			key_out_l <= key_in_l(27 to 28) & key_in_l(1 to 26);
			key_out_r <= key_in_r(27 to 28) & key_in_r(1 to 26);
            when "11" => 
			key_out_l <= key_in_l(28) & key_in_l(1 to 27);
			key_out_r <= key_in_r(28) & key_in_r(1 to 27);
            when others => null;
          end case;
    end if;
end process;

key_out_lr <= key_out_l & key_out_r; -- Compress shifted left and right keys

srl_addr <= "0001";

-- Register output of key scheduler
-- Shift Register primitive SRL16 is used to for 2-stage shift of LEFT/Right Keys

SRLeft: for i in 1 to 28 generate

SHIFT_LEFT : SRL16 port map(clk, key_out_l(i), srl_addr(4), 
             srl_addr(3), srl_addr(2), srl_addr(1), key_out_l_reg1(i));

SHIFT_RIGHT : SRL16 port map(clk, key_out_r(i), srl_addr(4), 
              srl_addr(3), srl_addr(2), srl_addr(1), key_out_r_reg1(i));

end generate;


        
process(clk)
begin
  if( clk'event and clk = '1') then
    key_out_l_reg <= key_out_l_reg1;
    key_out_r_reg <= key_out_r_reg1;
  end if;
end process;

	
key_out_lr_reg <= key_out_l & key_out_r;


compressed_key <= (
          key_out_lr_reg(14) & key_out_lr_reg(17) & key_out_lr_reg(11) &
          key_out_lr_reg(24) & key_out_lr_reg( 1) & key_out_lr_reg( 5) &
          key_out_lr_reg( 3) & key_out_lr_reg(28) & key_out_lr_reg(15) & 
          key_out_lr_reg( 6) & key_out_lr_reg(21) & key_out_lr_reg(10) &
          key_out_lr_reg(23) & key_out_lr_reg(19) & key_out_lr_reg(12) &
          key_out_lr_reg( 4) & key_out_lr_reg(26) & key_out_lr_reg( 8) & 
          key_out_lr_reg(16) & key_out_lr_reg( 7) & key_out_lr_reg(27) & 
          key_out_lr_reg(20) & key_out_lr_reg(13) & key_out_lr_reg( 2) &
          key_out_lr_reg(41) & key_out_lr_reg(52) & key_out_lr_reg(31) & 
          key_out_lr_reg(37) & key_out_lr_reg(47) & key_out_lr_reg(55) & 
          key_out_lr_reg(30) & key_out_lr_reg(40) & key_out_lr_reg(51) & 
          key_out_lr_reg(45) & key_out_lr_reg(33) & key_out_lr_reg(48) &
          key_out_lr_reg(44) & key_out_lr_reg(49) & key_out_lr_reg(39) & 
          key_out_lr_reg(56) & key_out_lr_reg(34) & key_out_lr_reg(53) & 
          key_out_lr_reg(46) & key_out_lr_reg(42) & key_out_lr_reg(50) & 
          key_out_lr_reg(36) & key_out_lr_reg(29) & key_out_lr_reg(32));



end DES_KEY_RTL;




















