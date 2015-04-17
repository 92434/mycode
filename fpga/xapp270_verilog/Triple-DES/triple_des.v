
/*##########################################################################
--
--                                                                       --
--  Module      : Triple_DES.v                                           --
--  Last Update : 08/01/2001
--                                                                       --
--  Description : DES Top level module                                   --
--                                                                       --
--                                                                       --
--  Designers   : Steve Trimberger & Vikram Pasham                       --
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
--#########################################################################*/

///////////////////////////////////////////////////////////////////////////////
// Top-level module: Triple DES Pipelined encryptor/decryptor
//                   144 cycle latency on encryption/decryption
///////////////////////////////////////////////////////////////////////////////







module triple_des(reset, clk, data_bus, e_data_rdy, key1, key2, key3, 
decrypt, data_out, d_data_rdy);

input reset;
input [1:64] data_bus;      // input bus for carrying en/decrypted data.
input e_data_rdy;           // Signal indicates data bus is valid data
input [1:64] key1;           // Encryption/decryption key1
input [1:64] key2;           // Encryption/decryption key2
input [1:64] key3;           // Encryption/decryption key3
input decrypt;              // 0 = encrypt; 1 = decrypt
input clk;                  // Clock
output [1:64] data_out;     // output bus carrying en/decrypted data.
output d_data_rdy;          // 1=valid data is on data bus (==e_data delayed)

//reg d_data_rdy;             // 1=en/decrypted data is on data_out

// Intermediate states in the en(de)cryption

wire decrypt_des1, decrypt_des2, decrypt_des3;
wire data_rdy_des1, data_rdy_des2, data_rdy_des3;    
wire [1:64] data_bus_des1, data_bus_des2 ;  
wire [1:64] data_out_des1, data_out_des2 ;

des DES1 (reset, clk, data_bus, e_data_rdy, key1, 
          decrypt_des1, data_bus_des1, data_rdy_des1);

des DES2 (reset, clk, data_bus_des1, data_rdy_des1, key2, 
          decrypt_des2, data_bus_des2, data_rdy_des2);

des DES3 (reset, clk, data_bus_des2, data_rdy_des2, key3, 
         decrypt_des3, data_out, data_rdy);


assign decrypt_des1 = decrypt;
assign decrypt_des2 = ~ decrypt_des1;
assign decrypt_des3 = ~ decrypt_des2;

endmodule //triple des //















