
--#########################################################################
--                                                                       --
--                                                                       --
--  Module      : DES_SBOX.vhd                                           --
--  Last Update : 08/01/2001                                             --
--                                                                       --
--  Description : Sbox description                                       --
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
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on


-- Synplify library declaration 
-- library virtex;
-- use virtex.components.all;



--*******************************************************************--
--                      S-Box SB1               		     --
--*******************************************************************--

entity SBOX1 is
port ( addr : in std_logic_vector(1 to 6);
        da   : out std_logic_vector(1 to 4);
	db   : out std_logic_vector(1 to 4));
end SBOX1;


architecture SBOX1_RTL of SBOX1 is

component MUXF5
port( O  : out std_logic;
      I0 : in std_logic;
      I1 : in std_logic;
      S	 : in std_logic );
end component;

signal d0, d1, d2, d3 : integer range 0 to 15;
signal d0_bin, d1_bin, d2_bin, d3_bin: std_logic_vector(1 to 4);

begin

process(addr(2 to 5))
begin	
	
	case addr(2 to 5) is
		when "0000" => d0 <= 14;
                               d1 <=  0;
                               d2 <=  4;
                               d3 <= 15;

                               
                when "0001" => d0 <=  4;
                               d1 <= 15; 
                               d2 <=  1;
                               d3 <= 12;

	 	when "0010" => d0 <= 13;
		               d1 <=  7;
			       d2 <= 14;
                               d3 <=  8;

		when "0011" => d0 <=  1;
		               d1 <=  4;
                               d2 <=  8;
                               d3 <=  2;

		when "0100" => d0 <=  2;
		               d1 <= 14;
                               d2 <= 13;
                               d3 <=  4;

		when "0101" => d0 <= 15;
		               d1 <=  2;
                               d2 <=  6;
                               d3 <=  9;

		when "0110" => d0 <= 11;
		               d1 <= 13;
                               d2 <=  2;
                               d3 <=  1;

		when "0111" => d0 <=  8;
		               d1 <=  1;
                               d2 <= 11;
                               d3 <=  7;

		when "1000" => d0 <=  3;
		               d1 <= 10;
                               d2 <= 15;
                               d3 <=  5;

		when "1001" => d0 <= 10;
		               d1 <=  6;
                               d2 <= 12;
                               d3 <= 11;

		when "1010" => d0 <=  6;
 		               d1 <= 12; 
                               d2 <=  9;
                               d3 <=  3;

		when "1011" => d0 <= 12;
		               d1 <= 11;
                               d2 <=  7;
                               d3 <= 14;

		when "1100" => d0 <=  5;
		               d1 <=  9;
                               d2 <=  3;
                               d3 <= 10;

		when "1101" => d0 <=  9;
		               d1 <=  5;
                               d2 <= 10;
                               d3 <=  0;

		when "1110" => d0 <=  0;
		               d1 <=  3;
                               d2 <=  5;
                               d3 <=  6;

		when "1111" => d0 <=  7;
		               d1 <=  8;
                               d2 <=  0;
                               d3 <= 13;

		when others => null;
	 end case;
end process;

d0_bin <= conv_std_logic_vector(d0, 4);
d1_bin <= conv_std_logic_vector(d1, 4);
d2_bin <= conv_std_logic_vector(d2, 4);
d3_bin <= conv_std_logic_vector(d3, 4);

-- Mux the first two together in F5 muxes
s10 : MUXF5 port map( I0 => d0_bin(1), I1 => d1_bin(1), S => addr(6), O => da(1));
s11 : MUXF5 port map( I0 => d0_bin(2), I1 => d1_bin(2), S => addr(6), O => da(2));
s12 : MUXF5 port map( I0 => d0_bin(3), I1 => d1_bin(3), S => addr(6), O => da(3));
s13 : MUXF5 port map( I0 => d0_bin(4), I1 => d1_bin(4), S => addr(6), O => da(4));


-- Mux the second two together in F5 muxes

s20 : MUXF5 port map( I0 => d2_bin(1), I1 => d3_bin(1), S => addr(6), O => db(1));
s21 : MUXF5 port map( I0 => d2_bin(2), I1 => d3_bin(2), S => addr(6), O => db(2));
s22 : MUXF5 port map( I0 => d2_bin(3), I1 => d3_bin(3), S => addr(6), O => db(3));
s23 : MUXF5 port map( I0 => d2_bin(4), I1 => d3_bin(4), S => addr(6), O => db(4));

end SBOX1_RTL;

--*******************************************************************--
-- 			S-Box SB2                                    --
--*******************************************************************--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;		   
use IEEE.std_logic_unsigned.all;

-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on

-- Synplify library declaration 
-- library virtex;
-- use virtex.components.all;


entity SBOX2 is
port ( addr : in std_logic_vector(1 to 6);
       da   : out std_logic_vector(1 to 4);
       db   : out std_logic_vector(1 to 4));
end SBOX2;


architecture SBOX2_RTL of SBOX2 is

component MUXF5
port( O  : out std_logic;
      I0 : in std_logic;
      I1 : in std_logic;
      S	 : in std_logic );
end component;


signal d0, d1, d2, d3 : integer range 0 to 15;
signal d0_bin, d1_bin, d2_bin, d3_bin: std_logic_vector(1 to 4);

begin

process(addr(2 to 5))
begin	
	
	case addr(2 to 5) is
		when "0000" => d0 <= 15;
                               d1 <=  3;
                               d2 <=  0;
                               d3 <= 13;

                when "0001" => d0 <=  1;
		               d1 <= 13;
                               d2 <= 14;
                               d3 <=  8;

	 	when "0010" => d0 <=  8;
                               d1 <=  4;
                               d2 <=  7;
                               d3 <= 10;

		when "0011" => d0 <= 14;
		               d1 <=  7;
                               d2 <= 11;
                               d3 <=  1;

		when "0100" => d0 <=  6;
		               d1 <= 15;
                               d2 <= 10;
                               d3 <=  3;

		when "0101" => d0 <= 11;
		               d1 <=  2;
                               d2 <=  4;
                               d3 <= 15;

		when "0110" => d0 <=  3;
		               d1 <=  8;
                               d2 <= 13;
                               d3 <=  4;

		when "0111" => d0 <=  4;
		               d1 <= 14;
                               d2 <=  1;
                               d3 <=  2;

		when "1000" => d0 <=  9;
		               d1 <= 12;
                               d2 <=  5;
                               d3 <= 11;

		when "1001" => d0 <=  7;
		               d1 <=  0;
                               d2 <=  8;
                               d3 <=  6;

		when "1010" => d0 <=  2;
 		               d1 <=  1; 
                               d2 <= 12;
                               d3 <=  7;

		when "1011" => d0 <= 13;
		               d1 <= 10;
                               d2 <=  6;
                               d3 <= 12;

		when "1100" => d0 <= 12;
		               d1 <=  6;
                               d2 <=  9;
                               d3 <=  0;

		when "1101" => d0 <=  0;
		               d1 <=  9;
                               d2 <=  3;
                               d3 <=  5;

		when "1110" => d0 <=  5;
		               d1 <= 11;
                               d2 <=  2;
                               d3 <= 14;

		when "1111" => d0 <= 10;
		               d1 <=  5;
                               d2 <= 15;
                               d3 <=  9;

		when others => null;
	 end case;
end process;

d0_bin <= conv_std_logic_vector(d0, 4);
d1_bin <= conv_std_logic_vector(d1, 4);
d2_bin <= conv_std_logic_vector(d2, 4);
d3_bin <= conv_std_logic_vector(d3, 4);

-- Mux the first two together in F5 muxes
s10 : MUXF5 port map( I0 => d0_bin(1), I1 => d1_bin(1), S => addr(6), O => da(1));
s11 : MUXF5 port map( I0 => d0_bin(2), I1 => d1_bin(2), S => addr(6), O => da(2));
s12 : MUXF5 port map( I0 => d0_bin(3), I1 => d1_bin(3), S => addr(6), O => da(3));
s13 : MUXF5 port map( I0 => d0_bin(4), I1 => d1_bin(4), S => addr(6), O => da(4));


-- Mux the second two together in F5 muxes

s20 : MUXF5 port map( I0 => d2_bin(1), I1 => d3_bin(1), S => addr(6), O => db(1));
s21 : MUXF5 port map( I0 => d2_bin(2), I1 => d3_bin(2), S => addr(6), O => db(2));
s22 : MUXF5 port map( I0 => d2_bin(3), I1 => d3_bin(3), S => addr(6), O => db(3));
s23 : MUXF5 port map( I0 => d2_bin(4), I1 => d3_bin(4), S => addr(6), O => db(4));

end SBOX2_RTL;



--*******************************************************************--
-- 			S-Box SB3                                    --
--*******************************************************************--


library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on


-- Synplify library declaration 
-- library virtex;
-- use virtex.components.all;


entity SBOX3 is
port ( addr : in std_logic_vector(1 to 6);
	 da	: out std_logic_vector(1 to 4);
	 db	: out std_logic_vector(1 to 4));
end SBOX3;


architecture SBOX3_RTL of SBOX3 is

component MUXF5
port( O  : out std_logic;
      I0 : in std_logic;
      I1 : in std_logic;
      S	 : in std_logic );
end component;

signal d0, d1, d2, d3 : integer range 0 to 15;
signal d0_bin, d1_bin, d2_bin, d3_bin: std_logic_vector(1 to 4);

begin

process(addr(2 to 5))
begin	
	
	case addr(2 to 5) is
		when "0000" => d0 <= 10;
                               d1 <= 13;
                               d2 <= 13;
                               d3 <=  1;

                when "0001" => d0 <=  0;
		               d1 <=  7; 
                               d2 <=  6;
                               d3 <= 10;

	 	when "0010" => d0 <=  9;
		               d1 <=  0;
                               d2 <=  4;
                               d3 <= 13;

		when "0011" => d0 <= 14;
		               d1 <=  9;
                               d2 <=  9;
                               d3 <=  0;

		when "0100" => d0 <=  6;
		               d1 <=  3;
                               d2 <=  8;
                               d3 <=  6;

		when "0101" => d0 <=  3;
		               d1 <=  4;
                               d2 <= 15;
                               d3 <=  9;

		when "0110" => d0 <= 15;
		               d1 <=  6;
                               d2 <=  3;
                               d3 <=  8;

		when "0111" => d0 <=  5;
		               d1 <= 10;
                               d2 <=  0;
                               d3 <=  7;

		when "1000" => d0 <=  1;
		               d1 <=  2;
                               d2 <= 11;
                               d3 <=  4;

		when "1001" => d0 <= 13;
		               d1 <=  8;
                               d2 <=  1;
                               d3 <= 15;

		when "1010" => d0 <= 12;
 		               d1 <=  5; 
                               d2 <=  2;
                               d3 <= 14;

		when "1011" => d0 <=  7;
		               d1 <= 14;
                               d2 <= 12;
                               d3 <=  3;

		when "1100" => d0 <= 11;
		               d1 <= 12;
                               d2 <=  5;
                               d3 <= 11;

		when "1101" => d0 <=  4;
		               d1 <=  11;
                               d2 <=  10;
                               d3 <=   5;

		when "1110" => d0 <=  2;
		               d1 <= 15;
                               d2 <= 14;
                               d3 <=  2;

		when "1111" => d0 <=  8;
		               d1 <=  1;
                               d2 <=  7;
                               d3 <= 12;

		when others => null;
	 end case;
end process;

d0_bin <= conv_std_logic_vector(d0, 4);
d1_bin <= conv_std_logic_vector(d1, 4);
d2_bin <= conv_std_logic_vector(d2, 4);
d3_bin <= conv_std_logic_vector(d3, 4);

-- Mux the first two together in F5 muxes
s10 : MUXF5 port map( I0 => d0_bin(1), I1 => d1_bin(1), S => addr(6), O => da(1));
s11 : MUXF5 port map( I0 => d0_bin(2), I1 => d1_bin(2), S => addr(6), O => da(2));
s12 : MUXF5 port map( I0 => d0_bin(3), I1 => d1_bin(3), S => addr(6), O => da(3));
s13 : MUXF5 port map( I0 => d0_bin(4), I1 => d1_bin(4), S => addr(6), O => da(4));


-- Mux the second two together in F5 muxes

s20 : MUXF5 port map( I0 => d2_bin(1), I1 => d3_bin(1), S => addr(6), O => db(1));
s21 : MUXF5 port map( I0 => d2_bin(2), I1 => d3_bin(2), S => addr(6), O => db(2));
s22 : MUXF5 port map( I0 => d2_bin(3), I1 => d3_bin(3), S => addr(6), O => db(3));
s23 : MUXF5 port map( I0 => d2_bin(4), I1 => d3_bin(4), S => addr(6), O => db(4));

end SBOX3_RTL;



--*******************************************************************--
-- 			S-Box SB4				     --
--*******************************************************************--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on


-- Synplify library declaration 
-- library virtex;
-- use virtex.components.all;



entity SBOX4 is
port ( addr : in std_logic_vector(1 to 6);
	 da	: out std_logic_vector(1 to 4);
	 db	: out std_logic_vector(1 to 4));
end SBOX4;


architecture SBOX4_RTL of SBOX4 is

component MUXF5
port( O  : out std_logic;
      I0 : in std_logic;
      I1 : in std_logic;
      S	 : in std_logic );
end component;

signal d0, d1, d2, d3 : integer range 0 to 16;
signal d0_bin, d1_bin, d2_bin, d3_bin: std_logic_vector(1 to 4);

begin

process(addr(2 to 5))
begin	
	
	case addr(2 to 5) is
		when "0000" => d0 <=  7;
                               d1 <= 13;
                               d2 <= 10;
                               d3 <=  3;

        	when "0001" => d0 <= 13;
		               d1 <=  8; 
                               d2 <=  6;
                               d3 <= 15;

	 	when "0010" => d0 <= 14;
		               d1 <= 11;
                               d2 <=  9;
                               d3 <=  0;

		when "0011" => d0 <=  3;
		               d1 <=  5;
                               d2 <=  0;
                               d3 <=  6;

		when "0100" => d0 <=  0;
		               d1 <=  6;
                               d2 <= 12;
                               d3 <= 10;

		when "0101" => d0 <=  6;
		               d1 <= 15;
                               d2 <= 11;
                               d3 <=  1;

		when "0110" => d0 <=  9;
		               d1 <=  0;
                               d2 <=  7;
                               d3 <= 13;

		when "0111" => d0 <= 10;
		               d1 <=  3;
                               d2 <= 13;
                               d3 <=  8;

		when "1000" => d0 <=  1;
		               d1 <=  4;
                               d2 <= 15;
                               d3 <=  9;

		when "1001" => d0 <=  2;
		               d1 <=  7;
                               d2 <=  1;
                               d3 <=  4;

		when "1010" => d0 <=  8;
 		               d1 <=  2; 
                               d2 <=  3;
                               d3 <=  5;

		when "1011" => d0 <=  5;
		               d1 <= 12;
                               d2 <= 14;
                               d3 <= 11;

		when "1100" => d0 <= 11;
		               d1 <=  1;
                               d2 <=  5;
                               d3 <= 12;

		when "1101" => d0 <=  12;
		               d1 <=  10;
                               d2 <=   2;
                               d3 <=   7;

		when "1110" => d0 <=  4;
		               d1 <= 14;
                               d2 <=  8;
                               d3 <=  2;

		when "1111" => d0 <= 15;
		               d1 <=  9;
                               d2 <=  4;
                               d3 <= 14;

		when others => null;
	 end case;
end process;

d0_bin <= conv_std_logic_vector(d0, 4);
d1_bin <= conv_std_logic_vector(d1, 4);
d2_bin <= conv_std_logic_vector(d2, 4);
d3_bin <= conv_std_logic_vector(d3, 4);

-- Mux the first two together in F5 muxes
s10 : MUXF5 port map( I0 => d0_bin(1), I1 => d1_bin(1), S => addr(6), O => da(1));
s11 : MUXF5 port map( I0 => d0_bin(2), I1 => d1_bin(2), S => addr(6), O => da(2));
s12 : MUXF5 port map( I0 => d0_bin(3), I1 => d1_bin(3), S => addr(6), O => da(3));
s13 : MUXF5 port map( I0 => d0_bin(4), I1 => d1_bin(4), S => addr(6), O => da(4));


-- Mux the second two together in F5 muxes

s20 : MUXF5 port map( I0 => d2_bin(1), I1 => d3_bin(1), S => addr(6), O => db(1));
s21 : MUXF5 port map( I0 => d2_bin(2), I1 => d3_bin(2), S => addr(6), O => db(2));
s22 : MUXF5 port map( I0 => d2_bin(3), I1 => d3_bin(3), S => addr(6), O => db(3));
s23 : MUXF5 port map( I0 => d2_bin(4), I1 => d3_bin(4), S => addr(6), O => db(4));

end SBOX4_RTL;




--*******************************************************************--
-- 			S-Box SB5                                    --
--*******************************************************************--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on


-- Synplify library declaration 
-- library virtex;
-- use virtex.components.all;



entity SBOX5 is
port ( addr : in std_logic_vector(1 to 6);
	 da	: out std_logic_vector(1 to 4);
	 db	: out std_logic_vector(1 to 4));
end SBOX5;


architecture SBOX5_RTL of SBOX5 is

component MUXF5
port( O  : out std_logic;
      I0 : in std_logic;
      I1	: in std_logic;
      S	 : in std_logic );
end component;

signal d0, d1, d2, d3 : integer range 0 to 16;
signal d0_bin, d1_bin, d2_bin, d3_bin: std_logic_vector(1 to 4);

begin

process(addr(2 to 5))
begin	
	
	case addr(2 to 5) is
		when "0000" => d0 <=  2;
                               d1 <= 14;
			       d2 <=  4;
			       d3 <= 11;
                when "0001" => d0 <= 12;
		               d1 <= 11; 
                               d2 <=  2;
			       d3 <=  8;

	 	when "0010" => d0 <=  4;
		               d1 <=  2;
			       d2 <=  1;
			       d3 <= 12;

		when "0011" => d0 <=  1;
		               d1 <= 12;
			       d2 <= 11;
			       d3 <=  7;

		when "0100" => d0 <=  7;
		               d1 <=  4;
			       d2 <= 10;
                               d3 <=  1;

		when "0101" => d0 <= 10;
		               d1 <=  7;
			       d2 <= 13;
			       d3 <= 14;

		when "0110" => d0 <= 11;
		               d1 <= 13;
			       d2 <=  7;
			       d3 <=  2;

		when "0111" => d0 <=  6;
		               d1 <=  1;
			       d2 <=  8;
			       d3 <= 13;

		when "1000" => d0 <=  8;
		               d1 <=  5;
			       d2 <= 15;
			       d3 <=  6;

		when "1001" => d0 <=  5;
		               d1 <=  0;
			       d2 <=  9;
			       d3 <= 15;

		when "1010" => d0 <=  3;
 		               d1 <= 15; 
 			       d2 <= 12;
                               d3 <=  0;

		when "1011" => d0 <= 15;
		               d1 <= 10;
			       d2 <=  5;
			       d3 <=  9;

		when "1100" => d0 <= 13;
		               d1 <=  3;
			       d2 <=  6;
                               d3 <= 10;

		when "1101" => d0 <=  0;
		               d1 <=  9;
			       d2 <=  3;
			       d3 <=  4;

		when "1110" => d0 <= 14;
		               d1 <=  8;
			       d2 <=  0;
			       d3 <=  5;

		when "1111" => d0 <=  9;
		               d1 <=  6;
			       d2 <= 14;
			       d3 <=  3;

		when others => null;
	 end case;
end process;

d0_bin <= conv_std_logic_vector(d0, 4);
d1_bin <= conv_std_logic_vector(d1, 4);
d2_bin <= conv_std_logic_vector(d2, 4);
d3_bin <= conv_std_logic_vector(d3, 4);

-- Mux the first two together in F5 muxes
s10 : MUXF5 port map( I0 => d0_bin(1), I1 => d1_bin(1), S => addr(6), O => da(1));
s11 : MUXF5 port map( I0 => d0_bin(2), I1 => d1_bin(2), S => addr(6), O => da(2));
s12 : MUXF5 port map( I0 => d0_bin(3), I1 => d1_bin(3), S => addr(6), O => da(3));
s13 : MUXF5 port map( I0 => d0_bin(4), I1 => d1_bin(4), S => addr(6), O => da(4));


-- Mux the second two together in F5 muxes

s20 : MUXF5 port map( I0 => d2_bin(1), I1 => d3_bin(1), S => addr(6), O => db(1));
s21 : MUXF5 port map( I0 => d2_bin(2), I1 => d3_bin(2), S => addr(6), O => db(2));
s22 : MUXF5 port map( I0 => d2_bin(3), I1 => d3_bin(3), S => addr(6), O => db(3));
s23 : MUXF5 port map( I0 => d2_bin(4), I1 => d3_bin(4), S => addr(6), O => db(4));

end SBOX5_RTL;




--*******************************************************************--
-- 			S-Box SB6				     --
--*******************************************************************--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on


-- Synplify library declaration 
-- library virtex;					   
-- use virtex.components.all;



entity SBOX6 is
port ( addr : in std_logic_vector(1 to 6);
	 da	: out std_logic_vector(1 to 4);
	 db	: out std_logic_vector(1 to 4));
end SBOX6;


architecture SBOX6_RTL of SBOX6 is

component MUXF5
port( O  : out std_logic;
      I0 : in std_logic;
      I1 : in std_logic;
      S	 : in std_logic );
end component;

signal d0, d1, d2, d3 : integer range 0 to 16;
signal d0_bin, d1_bin, d2_bin, d3_bin: std_logic_vector(1 to 4);

begin

process(addr(2 to 5))
begin	
	
	case addr(2 to 5) is
		when "0000" => d0 <= 12;
			       d1 <= 10;
			       d2 <=  9;
			       d3 <=  4;
                               

                when "0001" => d0 <=  1;
		               d1 <= 15; 
                               d2 <= 14;
			       d3 <=  3;

	 	when "0010" => d0 <= 10;
		               d1 <=  4;
			       d2 <= 15;
			       d3 <=  2;

		when "0011" => d0 <= 15;
		               d1 <=  2;
			       d2 <=  5;
			       d3 <= 12;
		when "0100" => d0 <=  9;
		               d1 <=  7;
			       d2 <=  2;
			       d3 <=  9;

		when "0101" => d0 <=  2;
		               d1 <= 12;
			       d2 <=  8;
			       d3 <=  5;

		when "0110" => d0 <=  6;
		               d1 <=  9;
			       d2 <= 12;
			       d3 <= 15;

		when "0111" => d0 <=  8;
		               d1 <=  5;
			       d2 <=  3;
			       d3 <= 10;

		when "1000" => d0 <=  0;
		               d1 <=  6;
			       d2 <=  7;
			       d3 <= 11;

		when "1001" => d0 <= 13;
		               d1 <=  1;
			       d2 <=  0;
			       d3 <= 14;

		when "1010" => d0 <=  3;
 		               d1 <= 13; 
 			       d2 <=  4;
			       d3 <=  1;

		when "1011" => d0 <=  4;
		               d1 <= 14;
			       d2 <= 10;
			       d3 <=  7;

		when "1100" => d0 <= 14;
		               d1 <=  0;
			       d2 <=  1;
			       d3 <=  6;

		when "1101" => d0 <=  7;
		               d1 <= 11;
			       d2 <= 13;
			       d3 <=  0;

		when "1110" => d0 <=  5;
		               d1 <=  3;
			       d2 <= 11;
			       d3 <=  8;

		when "1111" => d0 <= 11;
		               d1 <=  8;
			       d2 <=  6;
			       d3 <= 13;

		when others => null;
	 end case;
end process;

d0_bin <= conv_std_logic_vector(d0, 4);
d1_bin <= conv_std_logic_vector(d1, 4);
d2_bin <= conv_std_logic_vector(d2, 4);
d3_bin <= conv_std_logic_vector(d3, 4);

-- Mux the first two together in F5 muxes
s10 : MUXF5 port map( I0 => d0_bin(1), I1 => d1_bin(1), S => addr(6), O => da(1));
s11 : MUXF5 port map( I0 => d0_bin(2), I1 => d1_bin(2), S => addr(6), O => da(2));
s12 : MUXF5 port map( I0 => d0_bin(3), I1 => d1_bin(3), S => addr(6), O => da(3));
s13 : MUXF5 port map( I0 => d0_bin(4), I1 => d1_bin(4), S => addr(6), O => da(4));


-- Mux the second two together in F5 muxes

s20 : MUXF5 port map( I0 => d2_bin(1), I1 => d3_bin(1), S => addr(6), O => db(1));
s21 : MUXF5 port map( I0 => d2_bin(2), I1 => d3_bin(2), S => addr(6), O => db(2));
s22 : MUXF5 port map( I0 => d2_bin(3), I1 => d3_bin(3), S => addr(6), O => db(3));
s23 : MUXF5 port map( I0 => d2_bin(4), I1 => d3_bin(4), S => addr(6), O => db(4));

end SBOX6_RTL;




--*******************************************************************--
-- 				S-Box SB7			     --
--*******************************************************************--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_arith.all;



-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on


-- Synplify library declaration 
-- library virtex;
-- use virtex.components.all;



entity SBOX7 is
port ( addr : in std_logic_vector(1 to 6);
       da	: out std_logic_vector(1 to 4);
       db	: out std_logic_vector(1 to 4));
end SBOX7;


architecture SBOX7_RTL of SBOX7 is

component MUXF5
port( O  : out std_logic;
      I0 : in std_logic;
      I1 : in std_logic;
      S	 : in std_logic );
end component;

signal d0, d1, d2, d3 : integer range 0 to 16;
signal d0_bin, d1_bin, d2_bin, d3_bin: std_logic_vector(1 to 4);

begin

process(addr(2 to 5))
begin	
	
	case addr(2 to 5) is
		when "0000" => d0 <=  4;
			       d1 <= 13;
			       d2 <=  1;
			       d3 <=  6;

                when "0001" => d0 <= 11;
                               d1 <=  0; 
                               d2 <=  4;
                               d3 <= 11;

	 	when "0010" => d0 <=  2;
		               d1 <= 11;
			       d2 <= 11;
			       d3 <= 13;

		when "0011" => d0 <= 14;
		               d1 <=  7;
			       d2 <= 13;
			       d3 <=  8;

		when "0100" => d0 <= 15;
		               d1 <=  4;
			       d2 <= 12;
			       d3 <=  1;

		when "0101" => d0 <=  0;
		               d1 <=  9;
			       d2 <=  3;
			       d3 <=  4;

		when "0110" => d0 <=  8;
		               d1 <=  1;
			       d2 <=  7;
			       d3 <= 10;

		when "0111" => d0 <= 13;
		               d1 <= 10;
			       d2 <= 14;
			       d3 <=  7;

		when "1000" => d0 <=  3;
		               d1 <= 14;
			       d2 <= 10;
			       d3 <=  9;

		when "1001" => d0 <= 12;
		               d1 <=  3;
			       d2 <= 15;
			       d3 <=  5;

		when "1010" => d0 <=  9;
 		               d1 <=  5; 
 			       d2 <=  6;
			       d3 <=  0;

		when "1011" => d0 <=  7;
		               d1 <= 12;
			       d2 <=  8;
			       d3 <= 15;

		when "1100" => d0 <=  5;
		               d1 <=  2;
			       d2 <=  0;
			       d3 <= 14;

		when "1101" => d0 <= 10;
		               d1 <= 15;
			       d2 <=  5;
			       d3 <=  2;

		when "1110" => d0 <=  6;
		               d1 <=  8;
			       d2 <=  9;
			       d3 <=  3;

		when "1111" => d0 <=  1;
		               d1 <=  6;
			       d2 <=  2;
			       d3 <= 12;

		when others => null;
	 end case;
end process;

d0_bin <= conv_std_logic_vector(d0, 4);
d1_bin <= conv_std_logic_vector(d1, 4);
d2_bin <= conv_std_logic_vector(d2, 4);
d3_bin <= conv_std_logic_vector(d3, 4);

-- Mux the first two together in F5 muxes
s10 : MUXF5 port map( I0 => d0_bin(1), I1 => d1_bin(1), S => addr(6), O => da(1));
s11 : MUXF5 port map( I0 => d0_bin(2), I1 => d1_bin(2), S => addr(6), O => da(2));
s12 : MUXF5 port map( I0 => d0_bin(3), I1 => d1_bin(3), S => addr(6), O => da(3));
s13 : MUXF5 port map( I0 => d0_bin(4), I1 => d1_bin(4), S => addr(6), O => da(4));


-- Mux the second two together in F5 muxes

s20 : MUXF5 port map( I0 => d2_bin(1), I1 => d3_bin(1), S => addr(6), O => db(1));
s21 : MUXF5 port map( I0 => d2_bin(2), I1 => d3_bin(2), S => addr(6), O => db(2));
s22 : MUXF5 port map( I0 => d2_bin(3), I1 => d3_bin(3), S => addr(6), O => db(3));
s23 : MUXF5 port map( I0 => d2_bin(4), I1 => d3_bin(4), S => addr(6), O => db(4));

end SBOX7_RTL;




--*******************************************************************--
-- 			S-Box SB8				     --
--*******************************************************************--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- pragma translate_off
-- Libraries for Function simulation
library unisim;
use unisim.vcomponents.all;
-- pragma translate_on

-- Synplify library declaration 
-- library virtex;
-- use virtex.components.all;


entity SBOX8 is
port ( addr : in std_logic_vector(1 to 6);
	 da	: out std_logic_vector(1 to 4);
	 db	: out std_logic_vector(1 to 4));
end SBOX8;


architecture SBOX8_RTL of SBOX8 is

component MUXF5
port( O  : out std_logic;
      I0 : in std_logic;
      I1 : in std_logic;
      S	 : in std_logic );
end component;

signal d0, d1, d2, d3 : integer range 0 to 16;
signal d0_bin, d1_bin, d2_bin, d3_bin: std_logic_vector(1 to 4);

begin

process(addr(2 to 5))
begin	
	
	case addr(2 to 5) is
		when "0000" => d0 <= 13;
                               d1 <=  1;
			       d2 <=  7;
                               d3 <=  2;

    	        when "0001" => d0 <=  2;
		               d1 <= 15; 
                               d2 <= 11;
			       d3 <=  1;

	 	when "0010" => d0 <=  8;
		               d1 <= 13;
			       d2 <=  4;
			       d3 <= 14;

		when "0011" => d0 <=  4;
		               d1 <=  8;
			       d2 <=  1;
			       d3 <=  7;

		when "0100" => d0 <=  6;
		               d1 <= 10;
			       d2 <=  9;
			       d3 <=  4;

		when "0101" => d0 <= 15;
		               d1 <=  3;
			       d2 <= 12;
			       d3 <= 10;

		when "0110" => d0 <= 11;
		               d1 <=  7;
			       d2 <= 14;
			       d3 <=  8;

		when "0111" => d0 <=  1;
		               d1 <=  4;
			       d2 <=  2;
			       d3 <= 13;

		when "1000" => d0 <= 10;
		               d1 <= 12;
			       d2 <=  0;
			       d3 <= 15;

		when "1001" => d0 <=  9;
		               d1 <=  5;
			       d2 <=  6;
			       d3 <= 12;

		when "1010" => d0 <=  3;
 		               d1 <=  6; 
 			       d2 <= 10;
			       d3 <=  9;

		when "1011" => d0 <= 14;
		               d1 <= 11;
			       d2 <= 13;
			       d3 <=  0;

		when "1100" => d0 <=  5;
		               d1 <=  0;
			       d2 <= 15;
			       d3 <=  3;

		when "1101" => d0 <=  0;
		               d1 <= 14;
			       d2 <=  3;
			       d3 <=  5;

		when "1110" => d0 <= 12;
		               d1 <=  9;
			       d2 <=  5;
			       d3 <=  6;

		when "1111" => d0 <=  7;
		               d1 <=  2;
			       d2 <=  8;
			       d3 <= 11;
		when others => null;
	 end case;
end process;

d0_bin <= conv_std_logic_vector(d0, 4);
d1_bin <= conv_std_logic_vector(d1, 4);
d2_bin <= conv_std_logic_vector(d2, 4);
d3_bin <= conv_std_logic_vector(d3, 4);

-- Mux the first two together in F5 muxes
s10 : MUXF5 port map( I0 => d0_bin(1), I1 => d1_bin(1), S => addr(6), O => da(1));
s11 : MUXF5 port map( I0 => d0_bin(2), I1 => d1_bin(2), S => addr(6), O => da(2));
s12 : MUXF5 port map( I0 => d0_bin(3), I1 => d1_bin(3), S => addr(6), O => da(3));
s13 : MUXF5 port map( I0 => d0_bin(4), I1 => d1_bin(4), S => addr(6), O => da(4));


-- Mux the second two together in F5 muxes
s20 : MUXF5 port map( I0 => d2_bin(1), I1 => d3_bin(1), S => addr(6), O => db(1));
s21 : MUXF5 port map( I0 => d2_bin(2), I1 => d3_bin(2), S => addr(6), O => db(2));
s22 : MUXF5 port map( I0 => d2_bin(3), I1 => d3_bin(3), S => addr(6), O => db(3));
s23 : MUXF5 port map( I0 => d2_bin(4), I1 => d3_bin(4), S => addr(6), O => db(4));

end SBOX8_RTL;


