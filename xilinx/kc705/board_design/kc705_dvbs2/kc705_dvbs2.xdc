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
set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[4]}]

set_property PACKAGE_PIN G19 [get_ports {EXT_LEDS[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[5]}]

set_property PACKAGE_PIN E18 [get_ports {EXT_LEDS[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[6]}]

set_property PACKAGE_PIN F16 [get_ports {EXT_LEDS[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[7]}]
	
# ----------------------------------------------------------------------------------------------------
# ip constrain
# ----------------------------------------------------------------------------------------------------

#FMC_HPC_HA00_CC_P
set_property PACKAGE_PIN D12 [get_ports {mpeg_clk}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_clk}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {mpeg_clk}]

#FMC_HPC_HA01_CC_P
set_property PACKAGE_PIN H14 [get_ports {mpeg_sync}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_sync}]

#FMC_HPC_HA05_N
set_property PACKAGE_PIN E16 [get_ports {mpeg_valid}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_valid}]

#FMC_HPC_HA00_CC_N
set_property PACKAGE_PIN D13 [get_ports {mpeg_data[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[0]}]

#FMC_HPC_HA01_CC_N
set_property PACKAGE_PIN G14 [get_ports {mpeg_data[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[1]}]

#FMC_HPC_HA02_P
set_property PACKAGE_PIN D11 [get_ports {mpeg_data[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[2]}]

#FMC_HPC_HA04_P
set_property PACKAGE_PIN F11 [get_ports {mpeg_data[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[3]}]

#FMC_HPC_HA03_N
set_property PACKAGE_PIN B12 [get_ports {mpeg_data[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[4]}]

#FMC_HPC_HA04_N
set_property PACKAGE_PIN E11 [get_ports {mpeg_data[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[5]}]

#FMC_HPC_HA05_P
set_property PACKAGE_PIN F15 [get_ports {mpeg_data[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[6]}]

#FMC_HPC_HA06_P
set_property PACKAGE_PIN D14 [get_ports {mpeg_data[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data[7]}]

#FMC_HPC_HA10_P
set_property PACKAGE_PIN A11 [get_ports {asi_out_p}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p}]

#FMC_HPC_HA10_N
set_property PACKAGE_PIN A12 [get_ports {asi_out_n}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n}]

#HDMI_R_D10
set_property PACKAGE_PIN G23 [get_ports {fs_0p5_en}]
set_property IOSTANDARD LVCMOS25 [get_ports {fs_0p5_en}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {fs_0p5_en}]

#FMC_HPC_LA02_N
set_property PACKAGE_PIN H25 [get_ports {symbol_2x_oe}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_oe}]

#FMC_HPC_LA02_P
set_property PACKAGE_PIN H24 [get_ports {symbol_2x_re_out[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[0]}]

#FMC_HPC_LA03_P
set_property PACKAGE_PIN H26 [get_ports {symbol_2x_re_out[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[1]}]

#FMC_HPC_LA00_CC_N
set_property PACKAGE_PIN B25 [get_ports {symbol_2x_re_out[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[2]}]

#FMC_HPC_LA01_CC_N
set_property PACKAGE_PIN C26 [get_ports {symbol_2x_re_out[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[3]}]

#FMC_HPC_LA00_CC_P
set_property PACKAGE_PIN C25 [get_ports {symbol_2x_re_out[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[4]}]

#FMC_HPC_LA01_CC_P
set_property PACKAGE_PIN D26 [get_ports {symbol_2x_re_out[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[5]}]

#FMC_HPC_CLK1_M2C_N
set_property PACKAGE_PIN D18 [get_ports {symbol_2x_re_out[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[6]}]

#FMC_HPC_CLK0_M2C_N
set_property PACKAGE_PIN C27 [get_ports {symbol_2x_re_out[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[7]}]

#FMC_HPC_CLK1_M2C_P
set_property PACKAGE_PIN D17 [get_ports {symbol_2x_re_out[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[8]}]

#FMC_HPC_CLK0_M2C_P
set_property PACKAGE_PIN D27 [get_ports {symbol_2x_re_out[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[9]}]

#HDMI_R_D16
set_property PACKAGE_PIN K19 [get_ports {symbol_2x_re_out[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[10]}]

#HDMI_R_D17
set_property PACKAGE_PIN K20 [get_ports {symbol_2x_re_out[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[11]}]

#HDMI_R_D14
set_property PACKAGE_PIN L17 [get_ports {symbol_2x_re_out[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[12]}]

#HDMI_R_D15
set_property PACKAGE_PIN L18 [get_ports {symbol_2x_re_out[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[13]}]

#HDMI_R_D12
set_property PACKAGE_PIN J19 [get_ports {symbol_2x_re_out[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[14]}]

#HDMI_R_D13
set_property PACKAGE_PIN H19 [get_ports {symbol_2x_re_out[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_re_out[15]}]

#FMC_HPC_LA10_N
set_property PACKAGE_PIN C30 [get_ports {symbol_2x_im_out[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[0]}]

#FMC_HPC_LA11_N
set_property PACKAGE_PIN F27 [get_ports {symbol_2x_im_out[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[1]}]

#FMC_HPC_LA10_P
set_property PACKAGE_PIN D29 [get_ports {symbol_2x_im_out[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[2]}]

#FMC_HPC_LA11_P
set_property PACKAGE_PIN G27 [get_ports {symbol_2x_im_out[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[3]}]

#FMC_HPC_LA08_N
set_property PACKAGE_PIN E30 [get_ports {symbol_2x_im_out[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[4]}]

#FMC_HPC_LA09_N
set_property PACKAGE_PIN A30 [get_ports {symbol_2x_im_out[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[5]}]

#FMC_HPC_LA08_P
set_property PACKAGE_PIN E29 [get_ports {symbol_2x_im_out[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[6]}]

#FMC_HPC_LA09_P
set_property PACKAGE_PIN B30 [get_ports {symbol_2x_im_out[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[7]}]

#FMC_HPC_LA06_N
set_property PACKAGE_PIN G30 [get_ports {symbol_2x_im_out[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[8]}]

#FMC_HPC_LA07_N
set_property PACKAGE_PIN D28 [get_ports {symbol_2x_im_out[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[9]}]

#FMC_HPC_LA06_P
set_property PACKAGE_PIN H30 [get_ports {symbol_2x_im_out[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[10]}]

#FMC_HPC_LA07_P
set_property PACKAGE_PIN E28 [get_ports {symbol_2x_im_out[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[11]}]

#FMC_HPC_LA04_N
set_property PACKAGE_PIN F28 [get_ports {symbol_2x_im_out[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[12]}]

#FMC_HPC_LA05_N
set_property PACKAGE_PIN F30 [get_ports {symbol_2x_im_out[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[13]}]

#FMC_HPC_LA04_P
set_property PACKAGE_PIN G28 [get_ports {symbol_2x_im_out[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[14]}]

#FMC_HPC_LA05_P
set_property PACKAGE_PIN G29 [get_ports {symbol_2x_im_out[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {symbol_2x_im_out[15]}]
# ----------------------------------------------------------------------------------------------------
# gpio constrain
# ----------------------------------------------------------------------------------------------------

#FMC_HPC_HA03_P, i2c_sck, R126.1, 230
set_property PACKAGE_PIN C12 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[0]}]

#FMC_HPC_HA02_N, i2c_sda, R139.1, 231
set_property PACKAGE_PIN C11 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[1]}]

#FMC_HPC_HA06_N, lnb1_on_off, R141.1, 232
set_property PACKAGE_PIN C14 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[2]}]

#FMC_HPC_HA08_P, TUNB_3.3V_ON, R118.1, 233
set_property PACKAGE_PIN E14 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[3]}]

#FMC_HPC_HA08_N, undefined, R119.1, 234
set_property PACKAGE_PIN E15 [get_ports {gpio_tri_io[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[4]}]

#FMC_HPC_HA12_P, undefined, R120.1, 235
set_property PACKAGE_PIN C15 [get_ports {gpio_tri_io[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[5]}]

#FMC_HPC_HA12_N, undefined, R121.1, 236
set_property PACKAGE_PIN B15 [get_ports {gpio_tri_io[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[6]}]

#FMC_HPC_HA14_P, undefined, R132.1, 237
set_property PACKAGE_PIN J16 [get_ports {gpio_tri_io[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[7]}]

#FMC_HPC_HA07_P, undefined, R128.1, 238
set_property PACKAGE_PIN B14 [get_ports {gpio_tri_io[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[8]}]

#FMC_HPC_HA07_N, undefined, R129.1, 239
set_property PACKAGE_PIN A15 [get_ports {gpio_tri_io[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[9]}]

#FMC_HPC_HA09_P, undefined, R105.1, 240
set_property PACKAGE_PIN F12 [get_ports {gpio_tri_io[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[10]}]

#FMC_HPC_HA09_N, undefined, R106.1, 241
set_property PACKAGE_PIN E13 [get_ports {gpio_tri_io[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[11]}]

#FMC_HPC_HA11_P, undefined, R130.1, 242
set_property PACKAGE_PIN B13 [get_ports {gpio_tri_io[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[12]}]

#FMC_HPC_HA11_N, undefined, R131.1, 243
set_property PACKAGE_PIN A13 [get_ports {gpio_tri_io[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[13]}]

#FMC_HPC_HA13_P, undefined, R107.1, 244
set_property PACKAGE_PIN L16 [get_ports {gpio_tri_io[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[14]}]

#FMC_HPC_HA13_N, undefined, R108.1, 245
set_property PACKAGE_PIN K16 [get_ports {gpio_tri_io[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[15]}]

#FMC_HPC_HA14_N, undefined, R133.1, 246
set_property PACKAGE_PIN H16 [get_ports {gpio_tri_io[16]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[16]}]

#HDMI_R_D1, spi_clk, undefined, 247
set_property PACKAGE_PIN A23 [get_ports {gpio_tri_io[17]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[17]}]

#HDMI_R_D3, spi_mosi, undefined, 248
set_property PACKAGE_PIN D23 [get_ports {gpio_tri_io[18]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[18]}]

#HDMI_R_D5, spi_miso, undefined, 249
set_property PACKAGE_PIN E25 [get_ports {gpio_tri_io[19]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[19]}]

#HDMI_R_D8, 74138G2A, undefined, 250
set_property PACKAGE_PIN F26 [get_ports {gpio_tri_io[20]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[20]}]

#HDMI_R_D2, spi_s0, undefined, 251
set_property PACKAGE_PIN E23 [get_ports {gpio_tri_io[21]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[21]}]

#HDMI_R_D4, spi_s1, undefined, 252
set_property PACKAGE_PIN F25 [get_ports {gpio_tri_io[22]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[22]}]

#HDMI_R_D6, spi_s2, undefined, 253
set_property PACKAGE_PIN E24 [get_ports {gpio_tri_io[23]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[23]}]

#FMC_LPC_LA20_N, AD9125_INTB, R51.1, 254
set_property PACKAGE_PIN AF27 [get_ports {gpio_tri_io[24]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[24]}]

#FMC_LPC_CLK1_M2C_N, AD5375_DSOP, R39.1, 255
set_property PACKAGE_PIN AH29 [get_ports {gpio_tri_io[25]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[25]}]
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
