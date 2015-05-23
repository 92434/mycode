
--#########################################################################
--                                                                       --
--                                                                       --
--  Module      : DES_CONST.vhd                                          --
--  Last Update : 08/01/2001                                             --
--                                                                       --
--  Description : Key schedule constants for 16 rounds                   --
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
use work.all;

package des_const is

constant S1  : std_logic_vector(1 to 5) := "00001";
constant S2  : std_logic_vector(1 to 5) := "00010";
constant S3  : std_logic_vector(1 to 5) := "00011";
constant S4  : std_logic_vector(1 to 5) := "00100";
constant S5  : std_logic_vector(1 to 5) := "00101";
constant S6  : std_logic_vector(1 to 5) := "00110";
constant S7  : std_logic_vector(1 to 5) := "00111";
constant S8  : std_logic_vector(1 to 5) := "01000";
constant S9  : std_logic_vector(1 to 5) := "01001";
constant S10 : std_logic_vector(1 to 5) := "01010";
constant S11 : std_logic_vector(1 to 5) := "01011";
constant S12 : std_logic_vector(1 to 5) := "01100";
constant S13 : std_logic_vector(1 to 5) := "01101";
constant S14 : std_logic_vector(1 to 5) := "01110";
constant S15 : std_logic_vector(1 to 5) := "01111";
constant S16 : std_logic_vector(1 to 5) := "10000";


end des_const;


