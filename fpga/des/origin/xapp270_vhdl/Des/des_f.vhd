
--#########################################################################
--                                                                       --
--                                                                       --
--  Module      : DES_F.vhd                                              --
--  Last Update : 01/08/2001                                             --
--                                                                       --
--  Description : One round of DES function                              --
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

entity DES_F is 
 port( 
   reset: in std_logic;
   clk  : in  std_logic;
   f_in_l: in  std_logic_vector(1 to 32); -- Left 32 bits for this round
   f_in_r: in  std_logic_vector(1 to 32); -- Right 32 bits for this round
   compressed_key : in  std_logic_vector(1 to 48); -- 48-bit key for round n
   f_out_l_reg  : out std_logic_vector(1 to 32); -- Left 32 output bits from 
                                                 -- this round
   f_out_r_reg  : out std_logic_vector(1 to 32) -- Right 32 output bits from
                                                -- this round
     );
end DES_F;


architecture DES_F_RTL of DES_F is

-- SBOX component declaration

component SBOX1 
port( addr  : in std_logic_vector( 1 to 6);
	da  :  out std_logic_vector(1 to 4);
	db  : out std_logic_vector(1 to 4));
end component;

component SBOX2 
port( addr  : in std_logic_vector( 1 to 6);
	da  : out std_logic_vector(1 to 4);
	db  : out std_logic_vector(1 to 4));
end component;

component SBOX3 
port( addr  : in std_logic_vector( 1 to 6);
	da  : out std_logic_vector(1 to 4);
	db  : out std_logic_vector(1 to 4));
end component;

component SBOX4 
port( addr  : in std_logic_vector( 1 to 6);
	da  : out std_logic_vector(1 to 4);
	db  : out std_logic_vector(1 to 4));
end component;

component SBOX5 
port( addr  : in std_logic_vector( 1 to 6);
	da  : out std_logic_vector(1 to 4);
	db  : out std_logic_vector(1 to 4));
end component;

component SBOX6 
port( addr  : in std_logic_vector( 1 to 6);
	da  : out std_logic_vector(1 to 4);
	db  : out std_logic_vector(1 to 4));
end component;

component SBOX7 
port( addr  : in std_logic_vector( 1 to 6);
	da  : out std_logic_vector(1 to 4);
	db  : out std_logic_vector(1 to 4));
end component;

component SBOX8 
port( addr  : in std_logic_vector( 1 to 6);
	da  : out std_logic_vector(1 to 4);
	db  : out std_logic_vector(1 to 4));
end component;


signal expanded_out : std_logic_vector(1 to 48);   -- Expanded F in r
signal xor_out: std_logic_vector(1 to 48);   
                                  -- xor'd expanded out and compressed key
signal xor_out_reg: std_logic_vector(1 to 48); 
                                  -- registered xor_out (PIPE)
signal sbox_out: std_logic_vector(1 to 32); -- output of sbox substitution
signal pbox_out: std_logic_vector(1 to 32); -- output of pbox substitution
signal f_out_l, f_out_r: std_logic_vector(1 to 32);	-- F results
signal f_in_l_reg, f_in_r_reg : std_logic_vector(1 to 32);
signal f_in_l_reg1, f_in_r_reg1: std_logic_vector(1 to 32);
         	                          -- Register for F results (PIPE)

-- Outputs from individual Sboxes

signal sb1outa, sb1outb, sb1outa_reg, sb1outb_reg : std_logic_vector(1 to 4);
signal sb2outa, sb2outb, sb2outa_reg, sb2outb_reg : std_logic_vector(1 to 4);
signal sb3outa, sb3outb, sb3outa_reg, sb3outb_reg : std_logic_vector(1 to 4);
signal sb4outa, sb4outb, sb4outa_reg, sb4outb_reg : std_logic_vector(1 to 4);
signal sb5outa, sb5outb, sb5outa_reg, sb5outb_reg : std_logic_vector(1 to 4);
signal sb6outa, sb6outb, sb6outa_reg, sb6outb_reg : std_logic_vector(1 to 4);
signal sb7outa, sb7outb, sb7outa_reg, sb7outb_reg : std_logic_vector(1 to 4);
signal sb8outa, sb8outb, sb8outa_reg, sb8outb_reg : std_logic_vector(1 to 4);


begin
-- Expanded right half data from 32 to 48 bits

expanded_out <=(f_in_r(32) & f_in_r(1)  & f_in_r(2)  & f_in_r(3)  &
		f_in_r(4)  &  f_in_r(5) & f_in_r(4)  & f_in_r(5)  &
		f_in_r(6)  &  f_in_r(7) & f_in_r(8)  & f_in_r(9)  &
		f_in_r(8)  &  f_in_r(9) & f_in_r(10) & f_in_r(11) &
		f_in_r(12) & f_in_r(13) & f_in_r(12) & f_in_r(13) &
		f_in_r(14) & f_in_r(15) & f_in_r(16) & f_in_r(17) &
		f_in_r(16) & f_in_r(17) & f_in_r(18) & f_in_r(19) &
		f_in_r(20) & f_in_r(21) & f_in_r(20) & f_in_r(21) &
		f_in_r(22) & f_in_r(23) & f_in_r(24) & f_in_r(25) &
		f_in_r(24) & f_in_r(25) & f_in_r(26) & f_in_r(27) &
		f_in_r(28) & f_in_r(29) & f_in_r(28) & f_in_r(29) &
		f_in_r(30) & f_in_r(31) & f_in_r(32) & f_in_r(1) );




-- xor expanded right half data with key
xor_out <= expanded_out xor compressed_key ;

-- Register xor_out
process (reset, clk)
begin 
        if(reset = '1') then
          xor_out_reg <= (others => '0');
	elsif(clk'event and clk = '1') then
          xor_out_reg <= xor_out;
      	end if;
end process;


-- S-box substitution

ISBOX1: SBOX1 port map(addr => xor_out(1 to 6), da => sb1outa, db => sb1outb);
ISBOX2: SBOX2 port map(addr => xor_out(7 to 12), da => sb2outa, db => sb2outb);
ISBOX3: SBOX3 port map(addr => xor_out(13 to 18), da => sb3outa, db => sb3outb);
ISBOX4: SBOX4 port map(addr => xor_out(19 to 24), da => sb4outa, db => sb4outb);
ISBOX5: SBOX5 port map(addr => xor_out(25 to 30), da => sb5outa, db => sb5outb);
ISBOX6: SBOX6 port map(addr => xor_out(31 to 36), da => sb6outa, db => sb6outb);
ISBOX7: SBOX7 port map(addr => xor_out(37 to 42), da => sb7outa, db => sb7outb);
ISBOX8: SBOX8 port map(addr => xor_out(43 to 48), da => sb8outa, db => sb8outb);


-- Register Sbox outputs

process(reset, clk) 
begin
        if (reset = '1') then
                sb1outa_reg <= "0000";
		sb1outb_reg <= "0000";

		sb2outa_reg <= "0000";
		sb2outb_reg <= "0000";

 		sb3outa_reg <= "0000";
		sb3outb_reg <= "0000";

		sb4outa_reg <= "0000";
		sb4outb_reg <= "0000";

		sb5outa_reg <= "0000";
		sb5outb_reg <= "0000";

		sb6outa_reg <= "0000";
		sb6outb_reg <= "0000";

		sb7outa_reg <= "0000";
		sb7outb_reg <= "0000";

		sb8outa_reg <= "0000";
		sb8outb_reg <= "0000";

	elsif(clk'event and clk = '1') then
		sb1outa_reg <= sb1outa;
		sb1outb_reg <= sb1outb;

		sb2outa_reg <= sb2outa;
		sb2outb_reg <= sb2outb;

 		sb3outa_reg <= sb3outa;
		sb3outb_reg <= sb3outb;

		sb4outa_reg <= sb4outa;
		sb4outb_reg <= sb4outb;

		sb5outa_reg <= sb5outa;
		sb5outb_reg <= sb5outb;

		sb6outa_reg <= sb6outa;
		sb6outb_reg <= sb6outb;

		sb7outa_reg <= sb7outa;
		sb7outb_reg <= sb7outb;

		sb8outa_reg <= sb8outa;
		sb8outb_reg <= sb8outb;

	end if;
end process;



-- Complete the SBoxes, they build two F5. 

sbox_out(1 to 4) <= sb1outb_reg when (xor_out_reg(1) = '1') else
		    sb1outa_reg;

sbox_out(5 to 8) <= sb2outb_reg when (xor_out_reg(7) = '1') else
		    sb2outa_reg;

sbox_out(9 to 12) <= sb3outb_reg when (xor_out_reg(13) = '1') else
		     sb3outa_reg;

sbox_out(13 to 16) <= sb4outb_reg when (xor_out_reg(19) = '1') else
  		      sb4outa_reg;

sbox_out(17 to 20) <= sb5outb_reg when (xor_out_reg(25) = '1') else
		      sb5outa_reg;

sbox_out(21 to 24) <= sb6outb_reg when (xor_out_reg(31) = '1') else
		      sb6outa_reg;

sbox_out(25 to 28) <= sb7outb_reg when (xor_out_reg(37) = '1') else
                      sb7outa_reg;

sbox_out(29 to 32) <= sb8outb_reg when (xor_out_reg(43) = '1') else
                      sb8outa_reg;



-- P-box permutation 
pbox_out <= (sbox_out(16) & sbox_out( 7) & sbox_out(20) & sbox_out(21) &
             sbox_out(29) & sbox_out(12) & sbox_out(28) & sbox_out(17) &
             sbox_out( 1) & sbox_out(15) & sbox_out(23) & sbox_out(26) &
             sbox_out( 5) & sbox_out(18) & sbox_out(31) & sbox_out(10) &
             sbox_out( 2) & sbox_out( 8) & sbox_out(24) & sbox_out(14) &
             sbox_out(32) & sbox_out(27) & sbox_out( 3) & sbox_out( 9) &
             sbox_out(19) & sbox_out(13) & sbox_out(30) & sbox_out( 6) &
             sbox_out(22) & sbox_out(11) & sbox_out( 4) & sbox_out(25));


-- Register F inputs

process(reset, clk)
begin
        if(reset = '1') then
          f_in_l_reg <= (others => '0');
          f_in_r_reg <= (others => '0');
 	
          f_in_l_reg1 <= (others => '0');
          f_in_r_reg1 <= (others => '0');
          
	elsif(clk'event and clk='1') then
          f_in_l_reg <= f_in_l;
          f_in_r_reg <= f_in_r;

          f_in_l_reg1 <= f_in_l_reg;
          f_in_r_reg1 <= f_in_r_reg;

	end if;
end process;


-- Generate outputs

f_out_l <= f_in_r_reg1;
f_out_r <= pbox_out xor f_in_l_reg1;

-- Register outputs of F function module

process(reset, clk)
begin
        if(reset = '1') then
          f_out_l_reg <= (others => '0');
          f_out_r_reg <= (others => '0');
	elsif(clk'event and clk='1') then
          f_out_l_reg <= f_out_l;
          f_out_r_reg <= f_out_r;
	end if;
end process;

end DES_F_RTL;












