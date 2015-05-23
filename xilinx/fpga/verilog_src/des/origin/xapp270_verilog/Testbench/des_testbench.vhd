--#########################################################################
--                                                                       --
--                                                                       --
--  Module      : DES Testbench                                          --
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

--**********************************NIST Test***************************--
--   Known Answer Test

--  For a given Key and Plain text data (DIN) 
--  Key :        x"00 00 00 00 00 00 00 00"
--  Input (DIN): x"80 00 00 00 00 00 00 00"

--  The expected encrypted result should be
--  Encrypted output (DOUT): x"95 f8 a5 e5 dd 31 d9 00"

--  This encrypted output is decryted  with the same Key to verify
--  decryption

--  This test bench verifies DES Encryption and Decrytion for the above DIN 
--  and Key values

--  This testbench does NOT cover all NIST test vectors.
--  Xilinx recommends verification of all NIST test vectors for
--  DES complaince
--  For complete test suite, refer NIST Testing requirements at
--  http://csrc.nist.gov/cryptval/des.htm

--*********************************************************************--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library work;




entity des_testbench1 is

end des_testbench1;


architecture arch_des_testbench1 of des_testbench1 is

component DES 
  port (clk  : in  std_logic;
   reset     : in std_logic;
   key       : in  std_logic_vector(1 to 64);  -- Encryption/Decryption key
   data_bus  : in  std_logic_vector(1 to 64);  -- input bus for carrying 
                                               -- en/decrypted data
   e_data_rdy: in std_logic;	-- Signal indicates data bus has valid data   
   decrypt    : in  std_logic;                  -- 0 = encrypt; 1 = decrypt
   data_out   : out std_logic_vector(1 to 64);  -- output bus carrying 
                                                -- en/decrypted data
   d_data_rdy : out  std_logic);                -- 1 = en/decrypted data is on data_out
end component;

  signal clk		:std_logic := '1';
  signal reset		:std_logic := '1';
  signal key		:std_logic_vector (1 to 64) := (others => '0');
  signal din    	:std_logic_vector (1 to 64) := (others => '0');
  signal din_valid	:std_logic := '0'; 
  signal dout           :std_logic_vector (1 to 64) := (others => '0');
  signal dout_valid     :std_logic := '0';
  signal decrypt_flag	:std_logic := '0';
 
begin
  process (clk)    -- Generate 100 Mhz clock
  begin
    if clk='1' then
      clk <= '0' after 5 ns, '1' after 10 ns;
    end if;
  end process;

  reset <= '1' after 0 ns, '0' after 50 ns, '1' after 540 ns, 
            '0' after 600 ns, '1' after  1100 ns  ;
  din_valid <= '1' after 0 ns, '0' after 540 ns, '1' after 610 ns ;
  decrypt_flag <= '0' after 0 ns , '1' after 600 ns, '0' after 1100 ns;
  
  


  process(clk, decrypt_flag)
  begin
   if(decrypt_flag = '0') then             -- Encryption    

    din <=  "1000000000000000000000000000000000000000000000000000000000000000";
                                           -- DIN : x"80 00 00 00 00 00 00 00"
    key <= (others => '0');                -- Key : x"00 00 00 00 00 00 00 00"
    
   else                                    -- Decryption

     din <= "1001010111111000101001011110010111011101001100011101100100000000";
                                           -- DIN : x"95 f8 a5 e5 dd 31 d9 00"
    key <= (others => '0');                -- Key : x"00 00 00 00 00 00 00 00"
  
    end if ;  
   end process;

 
  DES0: des port map(clk, reset, key, din, din_valid, decrypt_flag, dout, 
                    dout_valid);
 

 ------------------------------------------------------------------------
 -- Verify the ENCRYPTED output 
 -- Expected DOUT : x"95 f8 a5 e5 dd 31 d9 00"
 ------------------------------------------------------------------------

  process(clk, dout_valid, decrypt_flag) 
   begin
     if(clk'event and clk='0') then
      if (dout_valid ='1' and decrypt_flag = '0') then  
       if(dout /= "1001010111111000101001011110010111011101001100011101100100000000") then
          assert false
          report "Simulation Ended, DES Encryption Failed !!!!!!!!!!!!!!!!!!"
         severity failure;
       else
        assert false
        report " DES Encryption Successful !!!!!!!!!!!!!!!!!!!!!"
        severity note;
        end if;
       end if;
      end if;
   end process;



-----------------------------------------------------------------------
-- Verify the  DECRYPTED output
-- Expected DOUT : x"80 00 00 00 00 00 00 00" 
-----------------------------------------------------------------------

  process(clk, dout_valid, decrypt_flag) 
   begin
     if(clk'event and clk='0') then
      if (dout_valid ='1' and decrypt_flag = '1') then  
       if(dout /= "1000000000000000000000000000000000000000000000000000000000000000") then
          assert false
          report "Simulation Ended, DES Decryption Failed !!!!!!!!!!!!!!!!!!"
         severity failure;
       else
        assert false
        report " DES Decryption Successful !!!!!!!!!!!!!!!!!!!!!"
        severity note;
       end if;  
      end if;
     end if;
    
  end process;
  
 

end arch_des_testbench1;













