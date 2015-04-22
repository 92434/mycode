
--#########################################################################
--                                                                       --
--                                                                       --
--  Module      : DES.vhd                                                --
--  Last Update : 08/01/2001                                             --
--                                                                       --
--  Description : DES Top level module                                   --
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
use work.des_const.all;

entity DES is
  
  port (
    reset      : in std_logic;
    clk        : in  std_logic;
    data_bus   : in  std_logic_vector(1 to 64);  -- input bus for carrying
                                                 -- en/decrypted data
    e_data_rdy : in std_logic;     -- Signal indicates data bus has valid data 
    key        : in  std_logic_vector(1 to 64);  -- Encryption/Decryption key
    decrypt    : in  std_logic;                  -- 0 = encrypt; 1 = decrypt
    data_out   : out std_logic_vector(1 to 64);  -- en/decrypted data output
    d_data_rdy : out  std_logic);       -- 1 = en/decrypted data is on data_out

end DES;



architecture DES_RTL of DES is

component DES_KEY
generic (cs : std_logic_vector(1 to 5));
  port(clk      : in std_logic;         
       key_in_l : in std_logic_vector(1 to 28);       
       key_in_r : in std_logic_vector(1 to 28);                                
       key_out_l_reg : out std_logic_vector(1 to 28);  
       key_out_r_reg : out std_logic_vector(1 to 28); 
       compressed_key: out std_logic_vector(1 to 48);
       decrypt	: in std_logic);
end component;

component DES_F
port(reset : in std_logic; 
     clk   : in  std_logic;
     f_in_l: in  std_logic_vector(1 to 32);		
     f_in_r: in  std_logic_vector(1 to 32);
     f_out_l_reg: out std_logic_vector(1 to 32);
     f_out_r_reg: out std_logic_vector(1 to 32);
     compressed_key : in  std_logic_vector(1 to 48));
end component;



signal data_bus_reg : std_logic_vector(1 to 64);
signal key_reg      : std_logic_vector(1 to 64);


-- Intermediate states in the en(de)cryption

signal perm_key_l, perm_key_r : std_logic_vector(1 to 28);  -- Permuted key
signal iperm_data_l,  iperm_data_r : std_logic_vector(1 to 32);  
                                                        -- IP encrypted data
signal f_out_rl : std_logic_vector(1 to 64);  -- Final F output
signal reverse_perm : std_logic_vector(1 to 64);   
                                       -- Output of reverse permutation setp
                                    


-- Signals between pipeline blocks - KEYS

signal key1_l, key1_r : std_logic_vector(1 to 28);
signal key2_l, key2_r : std_logic_vector(1 to 28);
signal key3_l, key3_r : std_logic_vector(1 to 28);
signal key4_l, key4_r : std_logic_vector(1 to 28);
signal key5_l, key5_r : std_logic_vector(1 to 28);
signal key6_l, key6_r : std_logic_vector(1 to 28);
signal key7_l, key7_r : std_logic_vector(1 to 28);
signal key8_l, key8_r : std_logic_vector(1 to 28);
signal key9_l, key9_r : std_logic_vector(1 to 28);
signal key10_l, key10_r : std_logic_vector(1 to 28);
signal key11_l, key11_r : std_logic_vector(1 to 28);
signal key12_l, key12_r : std_logic_vector(1 to 28);
signal key13_l, key13_r : std_logic_vector(1 to 28);
signal key14_l, key14_r : std_logic_vector(1 to 28);
signal key15_l, key15_r : std_logic_vector(1 to 28);
signal key16_l, key16_r : std_logic_vector(1 to 28);

signal key1, key2, key3, key4 : std_logic_vector(1 to 48);
signal key5, key6, key7, key8 : std_logic_vector(1 to 48);
signal key9, key10, key11, key12: std_logic_vector(1 to 48) ;
signal key13, key14, key15, key16 : std_logic_vector(1 to 48);

-- Signals between pipeline blocks - DATA
signal f1_l, f2_l, f3_l, f4_l : std_logic_vector(1 to 32);
signal f5_l, f6_l, f7_l, f8_l : std_logic_vector(1 to 32);
signal f9_l, f10_l, f11_l, f12_l  : std_logic_vector(1 to 32);
signal f13_l, f14_l, f15_l, f16_l : std_logic_vector(1 to 32);

signal f1_r, f2_r, f3_r, f4_r : std_logic_vector(1 to 32) ;
signal f5_r, f6_r, f7_r, f8_r : std_logic_vector(1 to 32) ;
signal f9_r, f10_r, f11_r, f12_r  : std_logic_vector (1 to 32);
signal f13_r, f14_r, f15_r, f16_r : std_logic_vector(1 to 32) ;




-- encrypt/decrypt and data_valid flags


signal decrypt1, decrypt2, decrypt3, decrypt4 :std_logic ;
signal decrypt5, decrypt6, decrypt7, decrypt8 :std_logic;
signal decrypt9, decrypt10, decrypt11, decrypt12 :std_logic;
signal decrypt13, decrypt14, decrypt15 :std_logic;
signal e_data_rdy_reg, decrypt_reg : std_logic;

signal decrypt1_reg, decrypt2_reg :std_logic_vector(1 to 3);
signal decrypt3_reg, decrypt4_reg :std_logic_vector(1 to 3);
signal decrypt5_reg, decrypt6_reg :std_logic_vector(1 to 3); 
signal decrypt7_reg, decrypt8_reg :std_logic_vector(1 to 3);
signal decrypt9_reg, decrypt10_reg  :std_logic_vector(1 to 3); 
signal decrypt11_reg, decrypt12_reg :std_logic_vector(1 to 3); 
signal decrypt13_reg, decrypt14_reg :std_logic_vector(1 to 3);
signal decrypt15_reg :std_logic_vector(1 to 3);

signal count : integer range 0 to 50;

begin  -- DES_RTL

process(reset, clk, e_data_rdy)
begin
   if(reset = '1') then
     data_bus_reg  <= (others => '0');
     key_reg <= (others => '0');
     e_data_rdy_reg <= '0';
     decrypt_reg <= '0';
   elsif(clk'event and clk = '1') then
     data_bus_reg <= data_bus;
     key_reg <= key;
     e_data_rdy_reg <= e_data_rdy;
     decrypt_reg <=  decrypt;

   end if;
end process;

--////////////////////////////////////////////// --
-- Counter to count upto 48 and assert d_data_rdy
-- indicating valid data on the output.
--//////////////////////////////////////////////--


process(reset, clk)
begin
  if(reset = '1') then
      count <= 0;
  elsif( clk'event and clk = '1') then
    if(count < 48 and e_data_rdy_reg = '1') then
      count <= count + 1;
    end if;
  end if;


end process;
d_data_rdy <= '1' when (count = 48) else
	      '0';

process(clk)
begin
  if(clk'event and clk = '1') then
    decrypt1_reg  <= decrypt_reg & decrypt1_reg(1 to 2);
    decrypt2_reg  <= decrypt1_reg(3) & decrypt2_reg(1 to 2);
    decrypt3_reg  <= decrypt2_reg(3) & decrypt3_reg(1 to 2);
    decrypt4_reg  <= decrypt3_reg(3) & decrypt4_reg(1 to 2);
    decrypt5_reg  <= decrypt4_reg(3) & decrypt5_reg(1 to 2);
    decrypt6_reg  <= decrypt5_reg(3) & decrypt6_reg(1 to 2);
    decrypt7_reg  <= decrypt6_reg(3) & decrypt7_reg(1 to 2);
    decrypt8_reg  <= decrypt7_reg(3) & decrypt8_reg(1 to 2);
    decrypt9_reg  <= decrypt8_reg(3) & decrypt9_reg(1 to 2);
    decrypt10_reg  <= decrypt9_reg(3) & decrypt10_reg(1 to 2);
    decrypt11_reg  <= decrypt10_reg(3) & decrypt11_reg(1 to 2);
    decrypt12_reg  <= decrypt11_reg(3) & decrypt12_reg(1 to 2);
    decrypt13_reg  <= decrypt12_reg(3) & decrypt13_reg(1 to 2);
    decrypt14_reg  <= decrypt13_reg(3) & decrypt14_reg(1 to 2);
    decrypt15_reg  <= decrypt14_reg(3) & decrypt15_reg(1 to 2);
  end if;

end process;

decrypt1 <= decrypt1_reg(3);
decrypt2 <= decrypt2_reg(3);
decrypt3 <= decrypt3_reg(3);
decrypt4 <= decrypt4_reg(3);
decrypt5 <= decrypt5_reg(3);
decrypt6 <= decrypt6_reg(3);
decrypt7 <= decrypt7_reg(3);
decrypt8 <= decrypt8_reg(3);
decrypt9 <= decrypt9_reg(3);
decrypt10 <= decrypt10_reg(3);
decrypt11 <= decrypt11_reg(3);
decrypt12 <= decrypt12_reg(3);
decrypt13 <= decrypt13_reg(3);
decrypt14 <= decrypt14_reg(3);
decrypt15 <= decrypt15_reg(3);





perm_key_l <= ( key_reg(57) & key_reg(49) & key_reg(41) & key_reg(33) &
                key_reg(25) & key_reg(17) & key_reg(9)  & key_reg(1)  &
                key_reg(58) & key_reg(50) & key_reg(42) & key_reg(34) &
                key_reg(26) & key_reg(18) & key_reg(10) & key_reg(2)  &
                key_reg(59) & key_reg(51) & key_reg(43) & key_reg(35) &
                key_reg(27) & key_reg(19) & key_reg(11) & key_reg(3)  &
                key_reg(60) & key_reg(52) & key_reg(44) & key_reg(36));


perm_key_r <= ( key_reg(63) & key_reg(55) & key_reg(47) & key_reg(39) &
                key_reg(31) & key_reg(23) & key_reg(15) & key_reg(7)  &
                key_reg(62) & key_reg(54) & key_reg(46) & key_reg(38) &
                key_reg(30) & key_reg(22) & key_reg(14) & key_reg(6)  &
                key_reg(61) & key_reg(53) & key_reg(45) & key_reg(37) &
                key_reg(29) & key_reg(21) & key_reg(13) & key_reg(5)  &
                key_reg(28) & key_reg(20) & key_reg(12) & key_reg(4));



--/////////////////////////////////////////////////--
--		Instantiate key shift modules      --
--/////////////////////////////////////////////////--

IDES_KEY01:  DES_KEY generic map(cs => S1) port map(clk, perm_key_l, 
                     perm_key_r, key1_l, key1_r, key1, decrypt_reg);
IDES_KEY02:  DES_KEY generic map(cs => S2) port map(clk, key1_l, 
                     key1_r, key2_l, key2_r, key2, decrypt1);
IDES_KEY03:  DES_KEY generic map(cs => S3) port map(clk, key2_l, 
                     key2_r, key3_l, key3_r, key3, decrypt2);
IDES_KEY04:  DES_KEY generic map(cs => S4) port map(clk, key3_l, 
                     key3_r, key4_l, key4_r, key4, decrypt3);
IDES_KEY05:  DES_KEY generic map(cs => S5) port map(clk, key4_l, 
                     key4_r, key5_l, key5_r, key5, decrypt4);
IDES_KEY06:  DES_KEY generic map(cs => S6) port map(clk, key5_l, 
                     key5_r, key6_l, key6_r, key6, decrypt5);
IDES_KEY07:  DES_KEY generic map(cs => S7) port map (clk, key6_l, 
                     key6_r, key7_l, key7_r, key7, decrypt6);
IDES_KEY08:  DES_KEY generic map(cs => S8) port map(clk, key7_l, 
                     key7_r, key8_l, key8_r, key8, decrypt7);
IDES_KEY09:  DES_KEY generic map(cs => S9) port map(clk, key8_l, 
                     key8_r, key9_l, key9_r, key9, decrypt8);
IDES_KEY10:  DES_KEY generic map(cs => S10) port map(clk, key9_l, 
                     key9_r, key10_l, key10_r, key10, decrypt9);
IDES_KEY11:  DES_KEY generic map(cs => S11) port map(clk, key10_l, 
                     key10_r, key11_l, key11_r, key11, decrypt10);
IDES_KEY12:  DES_KEY generic map(cs => S12) port map(clk, key11_l, 
                     key11_r, key12_l, key12_r, key12, decrypt11);
IDES_KEY13:  DES_KEY generic map(cs => S13) port map(clk, key12_l, 
                     key12_r, key13_l, key13_r, key13, decrypt12);
IDES_KEY14:  DES_KEY generic map(cs => S14) port map(clk, key13_l, 
                     key13_r, key14_l, key14_r, key14, decrypt13);
IDES_KEY15:  DES_KEY generic map(cs => S15) port map(clk, key14_l, 
                     key14_r, key15_l, key15_r, key15, decrypt14);
IDES_KEY16:  DES_KEY generic map(cs => S16) port map(clk, key15_l, 
                     key15_r, key16_l, key16_r, key16, decrypt15);

--/////////////////////////////////////////////////////////////////--
-- Perform initial permutation on encrypted data		   --
--/////////////////////////////////////////////////////////////////--
			
							
iperm_data_l <= (data_bus_reg(58) & data_bus_reg(50) & data_bus_reg(42) & 
                 data_bus_reg(34) & data_bus_reg(26) & data_bus_reg(18) & 
                 data_bus_reg(10) & data_bus_reg(2)  & data_bus_reg(60) & 
                 data_bus_reg(52) & data_bus_reg(44) & data_bus_reg(36) &
                 data_bus_reg(28) & data_bus_reg(20) & data_bus_reg(12) & 
                 data_bus_reg(4)  & data_bus_reg(62) & data_bus_reg(54) & 
                 data_bus_reg(46) & data_bus_reg(38) & data_bus_reg(30) & 
                 data_bus_reg(22) & data_bus_reg(14) & data_bus_reg(6)  &
                 data_bus_reg(64) & data_bus_reg(56) & data_bus_reg(48) & 
                 data_bus_reg(40) & data_bus_reg(32) & data_bus_reg(24) & 
                 data_bus_reg(16) & data_bus_reg(8));


iperm_data_r <= (data_bus_reg(57) & data_bus_reg(49) & data_bus_reg(41) &
                 data_bus_reg(33) & data_bus_reg(25) & data_bus_reg(17) & 
                 data_bus_reg(9)  & data_bus_reg(1)  & data_bus_reg(59) & 
                 data_bus_reg(51) & data_bus_reg(43) & data_bus_reg(35) &
                 data_bus_reg(27) & data_bus_reg(19) & data_bus_reg(11) & 
                 data_bus_reg(3)  & data_bus_reg(61) & data_bus_reg(53) & 
                 data_bus_reg(45) & data_bus_reg(37) & data_bus_reg(29) & 
                 data_bus_reg(21) & data_bus_reg(13) & data_bus_reg(5)  &
                 data_bus_reg(63) & data_bus_reg(55) & data_bus_reg(47) & 
                 data_bus_reg(39) & data_bus_reg(31) & data_bus_reg(23) & 
                 data_bus_reg(15) & data_bus_reg(7));



								
--////////////////////////////////////////////////////////////
--		 Instantiate f function modules ////
--////////////////////////////////////////////////////////////

IDES_F01:  DES_F port map(reset, clk, iperm_data_l, iperm_data_r, 
                         f1_l, f1_r, key1);
IDES_F02:  DES_F port map(reset, clk, f1_l, f1_r, f2_l, f2_r, key2);
IDES_F03:  DES_F port map(reset, clk, f2_l, f2_r, f3_l, f3_r, key3);
IDES_F04:  DES_F port map(reset, clk, f3_l, f3_r, f4_l, f4_r, key4);
IDES_F05:  DES_F port map(reset, clk, f4_l, f4_r, f5_l, f5_r, key5);
IDES_F06:  DES_F port map(reset, clk, f5_l, f5_r, f6_l, f6_r, key6);
IDES_F07:  DES_F port map(reset, clk, f6_l, f6_r, f7_l, f7_r, key7);
IDES_F08:  DES_F port map(reset, clk, f7_l, f7_r, f8_l, f8_r, key8);
IDES_F09:  DES_F port map(reset, clk, f8_l, f8_r, f9_l, f9_r, key9);
IDES_F10:  DES_F port map(reset, clk, f9_l, f9_r, f10_l, f10_r, key10);
IDES_F11:  DES_F port map(reset, clk, f10_l, f10_r, f11_l, f11_r, key11);
IDES_F12:  DES_F port map(reset, clk, f11_l, f11_r, f12_l, f12_r, key12);
IDES_F13:  DES_F port map(reset, clk, f12_l, f12_r, f13_l, f13_r, key13);
IDES_F14:  DES_F port map(reset, clk, f13_l, f13_r, f14_l, f14_r, key14);
IDES_F15:  DES_F port map(reset, clk, f14_l, f14_r, f15_l, f15_r, key15); 
IDES_F16:  DES_F port map(reset, clk, f15_l, f15_r, f16_l, f16_r, key16);
		



--////////////////////////////////////////////////////////////--
-- Swap final left and right results for reverse permutation 
--////////////////////////////////////////////////////////////--

f_out_rl <= ( f16_r & f16_l);

--////////////////////////////////////////////////////////////
-- Reverse permutaion 					   --		
--////////////////////////////////////////////////////////////

reverse_perm <= (f_out_rl(40) & f_out_rl( 8) & f_out_rl(48) & f_out_rl(16) &
                 f_out_rl(56) & f_out_rl(24) & f_out_rl(64) & f_out_rl(32) &
                 f_out_rl(39) & f_out_rl( 7) & f_out_rl(47) & f_out_rl(15) &
                 f_out_rl(55) & f_out_rl(23) & f_out_rl(63) & f_out_rl(31) &
                 f_out_rl(38) & f_out_rl( 6) & f_out_rl(46) & f_out_rl(14) &
                 f_out_rl(54) & f_out_rl(22) & f_out_rl(62) & f_out_rl(30) &
                 f_out_rl(37) & f_out_rl( 5) & f_out_rl(45) & f_out_rl(13) &
                 f_out_rl(53) & f_out_rl(21) & f_out_rl(61) & f_out_rl(29) &
                 f_out_rl(36) & f_out_rl( 4) & f_out_rl(44) & f_out_rl(12) &
                 f_out_rl(52) & f_out_rl(20) & f_out_rl(60) & f_out_rl(28) &
                 f_out_rl(35) & f_out_rl( 3) & f_out_rl(43) & f_out_rl(11) &
                 f_out_rl(51) & f_out_rl(19) & f_out_rl(59) & f_out_rl(27) &
                 f_out_rl(34) & f_out_rl( 2) & f_out_rl(42) & f_out_rl(10) &
                 f_out_rl(50) & f_out_rl(18) & f_out_rl(58) & f_out_rl(26) &
                 f_out_rl(33) & f_out_rl( 1) & f_out_rl(41) & f_out_rl( 9) &
                 f_out_rl(49) & f_out_rl(17) & f_out_rl(57) & f_out_rl(25));


--////////////////////////////////////////////////////////////
-- Place decrypted data on data bus  
--////////////////////////////////////////////////////////////


data_out <= reverse_perm;

									
end DES_RTL;






















