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
set_property PACKAGE_PIN AB8 [get_ports {EXT_LEDS[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[0]}]

set_property PACKAGE_PIN AA8 [get_ports {EXT_LEDS[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[1]}]

set_property PACKAGE_PIN AC9 [get_ports {EXT_LEDS[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[2]}]

set_property PACKAGE_PIN AB9 [get_ports {EXT_LEDS[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[3]}]

set_property PACKAGE_PIN AE26 [get_ports {EXT_LEDS[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[4]}]

set_property PACKAGE_PIN G19 [get_ports {EXT_LEDS[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[5]}]

set_property PACKAGE_PIN E18 [get_ports {EXT_LEDS[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[6]}]

set_property PACKAGE_PIN F16 [get_ports {EXT_LEDS[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[7]}]
	
# ----------------------------------------------------------------------------------------------------
# ip constrain
# ----------------------------------------------------------------------------------------------------

#FMC_HPC_HA00_CC_P
set_property PACKAGE_PIN D12 [get_ports {mpeg_clk}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_clk}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {mpeg_clk}]

#FMC_HPC_HA01_CC_P
set_property PACKAGE_PIN H14 [get_ports {mpeg_sync}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_sync}]

#FMC_HPC_HA05_N
set_property PACKAGE_PIN E16 [get_ports {mpeg_valid}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_valid}]

#FMC_HPC_HA00_CC_N
set_property PACKAGE_PIN D13 [get_ports {mpeg_data[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_data[0]}]

#FMC_HPC_HA01_CC_N
set_property PACKAGE_PIN G14 [get_ports {mpeg_data[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_data[1]}]

#FMC_HPC_HA02_P
set_property PACKAGE_PIN D11 [get_ports {mpeg_data[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_data[2]}]

#FMC_HPC_HA04_P
set_property PACKAGE_PIN F11 [get_ports {mpeg_data[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_data[3]}]

#FMC_HPC_HA03_N
set_property PACKAGE_PIN B12 [get_ports {mpeg_data[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_data[4]}]

#FMC_HPC_HA04_N
set_property PACKAGE_PIN E11 [get_ports {mpeg_data[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_data[5]}]

#FMC_HPC_HA05_P
set_property PACKAGE_PIN F15 [get_ports {mpeg_data[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_data[6]}]

#FMC_HPC_HA06_P
set_property PACKAGE_PIN D14 [get_ports {mpeg_data[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {mpeg_data[7]}]

#FMC_HPC_HA10_P
set_property PACKAGE_PIN A11 [get_ports {asi_out_p}]
set_property IOSTANDARD LVCMOS15 [get_ports {asi_out_p}]

#FMC_HPC_HA10_N
set_property PACKAGE_PIN A12 [get_ports {asi_out_n}]
set_property IOSTANDARD LVCMOS15 [get_ports {asi_out_n}]

#FMC_LPC_CLK0_M2C_P
set_property PACKAGE_PIN AF22 [get_ports {fs_0p5_en}]
set_property IOSTANDARD LVCMOS15 [get_ports {fs_0p5_en}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {fs_0p5_en}]

#FMC_LPC_LA10_P
set_property PACKAGE_PIN AJ24 [get_ports {symbol_2x_oe}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_oe}]

#FMC_LPC_LA00_CC_P
set_property PACKAGE_PIN AD23 [get_ports {symbol_2x_re_out[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[0]}]

#FMC_LPC_LA02_P
set_property PACKAGE_PIN AF20 [get_ports {symbol_2x_re_out[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[1]}]

#FMC_LPC_LA00_CC_N
set_property PACKAGE_PIN AE24 [get_ports {symbol_2x_re_out[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[2]}]

#FMC_LPC_LA02_N
set_property PACKAGE_PIN AF21 [get_ports {symbol_2x_re_out[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[3]}]

#FMC_LPC_LA03_P
set_property PACKAGE_PIN AG20 [get_ports {symbol_2x_re_out[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[4]}]

#FMC_LPC_LA04_P
set_property PACKAGE_PIN AH21 [get_ports {symbol_2x_re_out[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[5]}]

#FMC_LPC_LA03_N
set_property PACKAGE_PIN AH20 [get_ports {symbol_2x_re_out[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[6]}]

#FMC_LPC_LA04_N
set_property PACKAGE_PIN AJ21 [get_ports {symbol_2x_re_out[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[7]}]

#FMC_LPC_LA07_P
set_property PACKAGE_PIN AG25 [get_ports {symbol_2x_re_out[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[8]}]

#FMC_LPC_LA08_P
set_property PACKAGE_PIN AJ22 [get_ports {symbol_2x_re_out[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[9]}]

#FMC_LPC_LA01_CC_P
set_property PACKAGE_PIN AE23 [get_ports {symbol_2x_re_out[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[10]}]

#FMC_LPC_LA01_CC_N
set_property PACKAGE_PIN AF23 [get_ports {symbol_2x_re_out[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[11]}]

#FMC_LPC_LA06_P
set_property PACKAGE_PIN AK20 [get_ports {symbol_2x_re_out[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[12]}]

#FMC_LPC_LA06_N
set_property PACKAGE_PIN AK21 [get_ports {symbol_2x_re_out[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[13]}]

#FMC_LPC_LA05_P
set_property PACKAGE_PIN AG22 [get_ports {symbol_2x_re_out[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[14]}]

#FMC_LPC_LA05_N
set_property PACKAGE_PIN AH22 [get_ports {symbol_2x_re_out[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_re_out[15]}]

#FMC_LPC_LA10_N
set_property PACKAGE_PIN AK25 [get_ports {symbol_2x_im_out[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[0]}]

#FMC_LPC_LA09_P
set_property PACKAGE_PIN AK23 [get_ports {symbol_2x_im_out[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[1]}]

#FMC_LPC_LA09_N
set_property PACKAGE_PIN AK24 [get_ports {symbol_2x_im_out[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[2]}]

#FMC_LPC_LA13_P
set_property PACKAGE_PIN AB24 [get_ports {symbol_2x_im_out[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[3]}]

#FMC_LPC_LA14_P
set_property PACKAGE_PIN AD21 [get_ports {symbol_2x_im_out[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[4]}]

#FMC_LPC_LA13_N
set_property PACKAGE_PIN AC25 [get_ports {symbol_2x_im_out[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[5]}]

#FMC_LPC_LA14_N
set_property PACKAGE_PIN AE21 [get_ports {symbol_2x_im_out[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[6]}]

#FMC_LPC_LA07_N
set_property PACKAGE_PIN AH25 [get_ports {symbol_2x_im_out[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[7]}]

#FMC_LPC_LA08_N
set_property PACKAGE_PIN AJ23 [get_ports {symbol_2x_im_out[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[8]}]

#FMC_LPC_LA12_P
set_property PACKAGE_PIN AA20 [get_ports {symbol_2x_im_out[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[9]}]

#FMC_LPC_LA11_P
set_property PACKAGE_PIN AE25 [get_ports {symbol_2x_im_out[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[10]}]

#FMC_LPC_LA12_N
set_property PACKAGE_PIN AB20 [get_ports {symbol_2x_im_out[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[11]}]

#FMC_LPC_LA11_N
set_property PACKAGE_PIN AF25 [get_ports {symbol_2x_im_out[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[12]}]

#FMC_LPC_LA16_P
set_property PACKAGE_PIN AC22 [get_ports {symbol_2x_im_out[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[13]}]

#FMC_LPC_LA16_N
set_property PACKAGE_PIN AD22 [get_ports {symbol_2x_im_out[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[14]}]

#FMC_LPC_LA15_P
set_property PACKAGE_PIN AC24 [get_ports {symbol_2x_im_out[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {symbol_2x_im_out[15]}]
# ----------------------------------------------------------------------------------------------------
# gpio constrain
# ----------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------
# gpio constrain for HPC
# ----------------------------------------------------------------------------------------------------

#FMC_HPC_HA03_P, i2c_sck, R126.1, 239
set_property PACKAGE_PIN C12 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[0]}]

#FMC_HPC_HA02_N, i2c_sda, R139.1, 240
set_property PACKAGE_PIN C11 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[1]}]

#FMC_HPC_HA06_N, lnb1_on_off, R141.1, 241
set_property PACKAGE_PIN C14 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[2]}]

#FMC_HPC_HA08_P, TUNB_3.3V_ON, R118.1, 242
set_property PACKAGE_PIN E14 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[3]}]

#FMC_HPC_HA08_N, undefined, R119.1, 243
set_property PACKAGE_PIN E15 [get_ports {gpio_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[4]}]

#FMC_HPC_HA12_P, undefined, R120.1, 244
set_property PACKAGE_PIN C15 [get_ports {gpio_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[5]}]

#FMC_HPC_HA12_N, undefined, R121.1, 245
set_property PACKAGE_PIN B15 [get_ports {gpio_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[6]}]

#FMC_HPC_HA14_P, undefined, R132.1, 246
set_property PACKAGE_PIN J16 [get_ports {gpio_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[7]}]

#FMC_HPC_HA07_P, undefined, R128.1, 247
set_property PACKAGE_PIN B14 [get_ports {gpio_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[8]}]

#FMC_HPC_HA07_N, undefined, R129.1, 248
set_property PACKAGE_PIN A15 [get_ports {gpio_tri_io[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[9]}]

#FMC_HPC_HA09_P, undefined, R105.1, 249
set_property PACKAGE_PIN F12 [get_ports {gpio_tri_io[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[10]}]

#FMC_HPC_HA09_N, undefined, R106.1, 250
set_property PACKAGE_PIN E13 [get_ports {gpio_tri_io[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[11]}]

#FMC_HPC_HA11_P, undefined, R130.1, 251
set_property PACKAGE_PIN B13 [get_ports {gpio_tri_io[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[12]}]

#FMC_HPC_HA11_N, undefined, R131.1, 252
set_property PACKAGE_PIN A13 [get_ports {gpio_tri_io[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[13]}]

#FMC_HPC_HA13_P, undefined, R107.1, 253
set_property PACKAGE_PIN L16 [get_ports {gpio_tri_io[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[14]}]

#FMC_HPC_HA13_N, undefined, R108.1, 254
set_property PACKAGE_PIN K16 [get_ports {gpio_tri_io[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[15]}]

#FMC_HPC_HA14_N, undefined, R133.1, 255
set_property PACKAGE_PIN H16 [get_ports {gpio_tri_io[16]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[16]}]
# ----------------------------------------------------------------------------------------------------
# gpio constrain for LPC
# ----------------------------------------------------------------------------------------------------

#FMC_LPC_LA21_N, spi_clk, R78.1, 230
set_property PACKAGE_PIN AG28 [get_ports {gpio2_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[0]}]

#FMC_LPC_LA19_P, spi_mosi, R75.1, 231
set_property PACKAGE_PIN AJ26 [get_ports {gpio2_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[1]}]

#FMC_LPC_LA19_N, spi_miso, R76.1, 232
set_property PACKAGE_PIN AK26 [get_ports {gpio2_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[2]}]

#FMC_LPC_LA22_P, 74138G2A, R52.1, 233
set_property PACKAGE_PIN AJ27 [get_ports {gpio2_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[3]}]

#FMC_LPC_LA15_N, spi_s0, R74.1, 234
set_property PACKAGE_PIN AD24 [get_ports {gpio2_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[4]}]

#FMC_LPC_CLK0_M2C_N, spi_s1, R64.1, 235
set_property PACKAGE_PIN AG23 [get_ports {gpio2_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[5]}]

#FMC_LPC_PRSNT_M2C_B_LS, spi_s2, R62.1, 236
set_property PACKAGE_PIN J22 [get_ports {gpio2_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[6]}]

#FMC_LPC_LA20_N, AD9125_INTB, R51.1, 237
set_property PACKAGE_PIN AF27 [get_ports {gpio2_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[7]}]

#FMC_LPC_CLK1_M2C_N, AD5375_DSOP, R39.1, 238
set_property PACKAGE_PIN AH29 [get_ports {gpio2_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[8]}]
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
