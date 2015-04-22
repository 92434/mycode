
--##########################################################################
--                                                                       --
--                                                                       --
--  Module      : Triple_DES.vhd                                         --
--  Last Update : 08/01/2001                                             --
--                                                                       --
--  Description : Triple DES Top level module                            --
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
--###########################################################################


library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use work.des_const.all;

entity TRIPLE_DES is
  
  port (
   reset      : in std_logic;
   clk        : in  std_logic;
   data_bus   : in  std_logic_vector(1 to 64);  -- input bus for carrying 
                                                --en/decrypted data
   e_data_rdy : in std_logic;	-- Signal indicates data bus has valid data   
   key1       : in  std_logic_vector(1 to 64);  -- Encryption/Decryption key1
   key2       : in  std_logic_vector(1 to 64); -- Encryption/Decryption key2
   key3       : in  std_logic_vector(1 to 64); -- Encryption/Decryption key3
   decrypt    : in  std_logic;            -- 0 = encrypt; 1 = decrypt
   data_out   : out std_logic_vector(1 to 64);  -- en/decrypted data output
   data_rdy : out  std_logic);         -- 1 = en/decrypted data is on data_out
 
end TRIPLE_DES;



architecture TRIPLE_DES_RTL of TRIPLE_DES is

component DES
port (
    reset      : in std_logic;
    clk        : in  std_logic;
    data_bus   : in  std_logic_vector(1 to 64);  -- input bus for carrying 
                                                  -- en/decrypted data
    e_data_rdy : in std_logic;	-- Signal indicates data bus has valid data
    key        : in  std_logic_vector(1 to 64);  -- Encryption/Decryption key 
    decrypt    : in  std_logic;                  -- 0 = encrypt; 1 = decrypt
    data_out   : out std_logic_vector(1 to 64);  -- en/decrypted data output
    d_data_rdy : out  std_logic);              

end component;


-- Intermediate states in the en(de)cryption

  
signal decrypt_des1, decrypt_des2, decrypt_des3 : std_logic;
signal data_rdy_des1, data_rdy_des2, data_rdy_des3 : std_logic;    
signal data_bus_des1, data_bus_des2 : std_logic_vector(1 to 64);  
signal data_out_des1, data_out_des2 : std_logic_vector(1 to 64); 



begin  


DES1: DES  port map (reset, clk, data_bus, e_data_rdy, key1, 
		     decrypt_des1, data_bus_des1, data_rdy_des1);

DES2: DES  port map (reset, clk, data_bus_des1, data_rdy_des1, key2, 
		     decrypt_des2, data_bus_des2, data_rdy_des2);


DES3: DES  port map (reset, clk, data_bus_des2, data_rdy_des2, key3, 
                     decrypt_des3, data_out, data_rdy);



decrypt_des1 <= decrypt;
decrypt_des2 <= not decrypt_des1;
decrypt_des3 <= not decrypt_des2;


end TRIPLE_DES_RTL;




















