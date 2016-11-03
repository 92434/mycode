# ----------------------------------------------------------------------------------------------------
# kc705 default constrain
# ----------------------------------------------------------------------------------------------------

# Sys Clock Pins
#set_property PACKAGE_PIN AD11 [get_ports MIG_SYS_CLK_clk_n]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports MIG_SYS_CLK_clk_n]

#set_property PACKAGE_PIN AD12 [get_ports MIG_SYS_CLK_clk_p]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports MIG_SYS_CLK_clk_p]

# Sys Reset Pins
set_property PACKAGE_PIN AB7 [get_ports reset]
set_property IOSTANDARD LVCMOS15 [get_ports reset]

# PCIe Refclk Pins
set_property PACKAGE_PIN U8 [get_ports EXT_PCIE_REFCLK_P]
set_property PACKAGE_PIN U7 [get_ports EXT_PCIE_REFCLK_N]

# PCIe TX RX Pins
#set_property PACKAGE_PIN M6 [get_ports {EXT_PCIE_rxp[0]}]
#set_property PACKAGE_PIN M5 [get_ports {EXT_PCIE_rxn[0]}]
#set_property PACKAGE_PIN P6 [get_ports {EXT_PCIE_rxp[1]}]
#set_property PACKAGE_PIN P5 [get_ports {EXT_PCIE_rxn[1]}]
#set_property PACKAGE_PIN R4 [get_ports {EXT_PCIE_rxp[2]}]
#set_property PACKAGE_PIN R3 [get_ports {EXT_PCIE_rxn[2]}]
#set_property PACKAGE_PIN T6 [get_ports {EXT_PCIE_rxp[3]}]
#set_property PACKAGE_PIN T5 [get_ports {EXT_PCIE_rxn[3]}]
#set_property PACKAGE_PIN L4 [get_ports {EXT_PCIE_txp[0]}]
#set_property PACKAGE_PIN L3 [get_ports {EXT_PCIE_txn[0]}]
#set_property PACKAGE_PIN M2 [get_ports {EXT_PCIE_txp[1]}]
#set_property PACKAGE_PIN M1 [get_ports {EXT_PCIE_txn[1]}]
#set_property PACKAGE_PIN N4 [get_ports {EXT_PCIE_txp[2]}]
#set_property PACKAGE_PIN N3 [get_ports {EXT_PCIE_txn[2]}]
#set_property PACKAGE_PIN P2 [get_ports {EXT_PCIE_txp[3]}]
#set_property PACKAGE_PIN P1 [get_ports {EXT_PCIE_txn[3]}]

# LED Pins
#set_property PACKAGE_PIN AB8 [get_ports {EXT_LEDS[0]}]
#set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[0]}]
#
#set_property PACKAGE_PIN AA8 [get_ports {EXT_LEDS[1]}]
#set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[1]}]
#
#set_property PACKAGE_PIN AC9 [get_ports {EXT_LEDS[2]}]
#set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[2]}]
#
#set_property PACKAGE_PIN AB9 [get_ports {EXT_LEDS[3]}]
#set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[3]}]
#
#set_property PACKAGE_PIN AE26 [get_ports {EXT_LEDS[4]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[4]}]
#
#set_property PACKAGE_PIN G19 [get_ports {EXT_LEDS[5]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[5]}]
#
#set_property PACKAGE_PIN E18 [get_ports {EXT_LEDS[6]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[6]}]
#
#set_property PACKAGE_PIN F16 [get_ports {EXT_LEDS[7]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[7]}]
	
# ----------------------------------------------------------------------------------------------------
# ip constrain
# ----------------------------------------------------------------------------------------------------

#FMC_HPC_HA00_CC_P, J94(5)
set_property PACKAGE_PIN D12 [get_ports {mpeg_clk}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_clk}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {mpeg_clk}]

#FMC_HPC_HA01_CC_P, J94(6)
set_property PACKAGE_PIN H14 [get_ports {mpeg_sync}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_sync}]

#FMC_HPC_HA05_N, J94(18)
set_property PACKAGE_PIN E16 [get_ports {mpeg_valid}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_valid}]

#FMC_HPC_HA00_CC_N, J94(7)
set_property PACKAGE_PIN D13 [get_ports {mpeg_data[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[0]}]

#FMC_HPC_HA01_CC_N, J94(8)
set_property PACKAGE_PIN G14 [get_ports {mpeg_data[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[1]}]

#FMC_HPC_HA02_P, J94(9)
set_property PACKAGE_PIN D11 [get_ports {mpeg_data[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[2]}]

#FMC_HPC_HA04_P, J94(13)
set_property PACKAGE_PIN F11 [get_ports {mpeg_data[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[3]}]

#FMC_HPC_HA03_N, J94(12)
set_property PACKAGE_PIN B12 [get_ports {mpeg_data[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[4]}]

#FMC_HPC_HA04_N, J94(17)
set_property PACKAGE_PIN E11 [get_ports {mpeg_data[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[5]}]

#FMC_HPC_HA05_P, J94(14)
set_property PACKAGE_PIN F15 [get_ports {mpeg_data[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[6]}]

#FMC_HPC_HA06_P, J94(19)
set_property PACKAGE_PIN D14 [get_ports {mpeg_data[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[7]}]

#FMC_HPC_HA10_P, J94(29)
set_property PACKAGE_PIN A11 [get_ports {asi_out_p}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p}]

#FMC_HPC_HA10_N, J94(31)
set_property PACKAGE_PIN A12 [get_ports {asi_out_n}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n}]

#FMC_LPC_LA25_P, J101(5)
set_property PACKAGE_PIN AC26 [get_ports {mpeg_clk_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_clk_1}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {mpeg_clk_1}]

#FMC_LPC_LA25_N, J101(6)
set_property PACKAGE_PIN AD26 [get_ports {mpeg_sync_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_sync_1}]

#FMC_LPC_LA05_N, J101(18)
set_property PACKAGE_PIN AH22 [get_ports {mpeg_valid_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_valid_1}]

#FMC_LPC_LA11_P, J101(7)
set_property PACKAGE_PIN AE25 [get_ports {mpeg_data_1[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[0]}]

#FMC_LPC_LA11_N, J101(8)
set_property PACKAGE_PIN AF25 [get_ports {mpeg_data_1[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[1]}]

#FMC_LPC_LA15_P, J101(9)
set_property PACKAGE_PIN AC24 [get_ports {mpeg_data_1[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[2]}]

#FMC_LPC_LA08_P, J101(13)
set_property PACKAGE_PIN AJ22 [get_ports {mpeg_data_1[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[3]}]

#FMC_LPC_LA10_N, J101(12)
set_property PACKAGE_PIN AK25 [get_ports {mpeg_data_1[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[4]}]

#FMC_LPC_LA05_P, J101(17)
set_property PACKAGE_PIN AG22 [get_ports {mpeg_data_1[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[5]}]

#FMC_LPC_LA08_N, J101(14)
set_property PACKAGE_PIN AJ23 [get_ports {mpeg_data_1[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[6]}]

#FMC_LPC_LA00_CC_P, J101(19)
set_property PACKAGE_PIN AD23 [get_ports {mpeg_data_1[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[7]}]

#FMC_LPC_LA06_P, J101(29)
set_property PACKAGE_PIN AK20 [get_ports {asi_out_p_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p_1}]

#FMC_LPC_LA01_CC_P, J101(31)
set_property PACKAGE_PIN AE23 [get_ports {asi_out_n_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n_1}]

#FMC_HPC_CLK1_M2C_P, J96(5)
set_property PACKAGE_PIN D17 [get_ports {mpeg_clk_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_clk_2}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {mpeg_clk_2}]

#FMC_HPC_CLK0_M2C_P, J96(6)
set_property PACKAGE_PIN D27 [get_ports {mpeg_sync_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_sync_2}]

#FMC_HPC_LA03_N, J96(18)
set_property PACKAGE_PIN H27 [get_ports {mpeg_valid_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_valid_2}]

#FMC_HPC_CLK1_M2C_N, J96(7)
set_property PACKAGE_PIN D18 [get_ports {mpeg_data_2[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[0]}]

#FMC_HPC_CLK0_M2C_N, J96(8)
set_property PACKAGE_PIN C27 [get_ports {mpeg_data_2[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[1]}]

#FMC_HPC_LA00_CC_P, J96(9)
set_property PACKAGE_PIN C25 [get_ports {mpeg_data_2[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[2]}]

#FMC_HPC_LA02_P, J96(13)
set_property PACKAGE_PIN H24 [get_ports {mpeg_data_2[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[3]}]

#FMC_HPC_LA01_CC_N, J96(12)
set_property PACKAGE_PIN C26 [get_ports {mpeg_data_2[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[4]}]

#FMC_HPC_LA02_N, J96(17)
set_property PACKAGE_PIN H25 [get_ports {mpeg_data_2[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[5]}]

#FMC_HPC_LA03_P, J96(14)
set_property PACKAGE_PIN H26 [get_ports {mpeg_data_2[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[6]}]

#FMC_HPC_LA04_P, J96(19)
set_property PACKAGE_PIN G28 [get_ports {mpeg_data_2[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[7]}]

#FMC_HPC_LA08_P, J96(29)
set_property PACKAGE_PIN E29 [get_ports {asi_out_p_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p_2}]

#FMC_HPC_LA08_N, J96(31)
set_property PACKAGE_PIN E30 [get_ports {asi_out_n_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n_2}]
# ----------------------------------------------------------------------------------------------------
# gpio constrain
# ----------------------------------------------------------------------------------------------------

#FMC_HPC_HA03_P, i2c_sck, R126.1, J94(10), 241
set_property PACKAGE_PIN C12 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[0]}]

#FMC_HPC_HA02_N, i2c_sda, R139.1, J94(11), 242
set_property PACKAGE_PIN C11 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[1]}]

#FMC_HPC_HA06_N, lnb1_on_off, R141.1, J94(21), 243
set_property PACKAGE_PIN C14 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[2]}]

#FMC_HPC_HA07_N, nim_reset, R129.1, J94(22), 244
set_property PACKAGE_PIN A15 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[3]}]

#FMC_HPC_HA08_P, tunb_33_on_off, R118.1, J94(23), 245
set_property PACKAGE_PIN E14 [get_ports {gpio_tri_io[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[4]}]

#FMC_LPC_LA15_N, i2c_sck_1, R74.1, J101(10), 246
set_property PACKAGE_PIN AD24 [get_ports {gpio_tri_io[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[5]}]

#FMC_LPC_LA10_P, i2c_sda_1, R7.1, J101(11), 247
set_property PACKAGE_PIN AJ24 [get_ports {gpio_tri_io[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[6]}]

#FMC_LPC_LA16_P, lnb1_on_off_1, R48.1, J101(21), 248
set_property PACKAGE_PIN AC22 [get_ports {gpio_tri_io[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[7]}]

#FMC_LPC_LA16_N, nim_reset_1, R49.1, J101(22), 249
set_property PACKAGE_PIN AD22 [get_ports {gpio_tri_io[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[8]}]

#FMC_LPC_LA02_P, tunb_33_on_off_1, R65.1, J101(23), 250
set_property PACKAGE_PIN AF20 [get_ports {gpio_tri_io[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[9]}]

#FMC_HPC_LA01_CC_P, i2c_sck_2, R20.1, J96(10), 251
set_property PACKAGE_PIN D26 [get_ports {gpio_tri_io[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[10]}]

#FMC_HPC_LA00_CC_N, i2c_sda_2, R41.1, J96(11), 252
set_property PACKAGE_PIN B25 [get_ports {gpio_tri_io[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[11]}]

#FMC_HPC_LA04_N, lnb1_on_off_2, R68.1, J96(21), 253
set_property PACKAGE_PIN F28 [get_ports {gpio_tri_io[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[12]}]

#FMC_HPC_LA05_N, nim_reset_2, R23.1, J96(22), 254
set_property PACKAGE_PIN F30 [get_ports {gpio_tri_io[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[13]}]

#FMC_HPC_LA06_P, tunb_33_on_off_2, R5.1, J96(23), 255
set_property PACKAGE_PIN H30 [get_ports {gpio_tri_io[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[14]}]
# ----------------------------------------------------------------------------------------------------
# bitstream constrain
# ----------------------------------------------------------------------------------------------------

set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 40 [current_design]
set_property BITSTREAM.CONFIG.SPI_FALL_EDGE YES [current_design]

##Encryption Settings
#set_property BITSTREAM.ENCRYPTION.ENCRYPT YES [current_design]
#set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT BBRAM [current_design]
##set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT eFUSE [current_design]
#set_property BITSTREAM.ENCRYPTION.KEY0 256'h12345678ABCDDCBA1234578ABCDDCBA1234578ABCDDCBA1234578ABCDDCBA [current_design]
