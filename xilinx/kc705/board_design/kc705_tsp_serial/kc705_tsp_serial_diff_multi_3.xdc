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

#FMC_HPC_HA01_CC_P, J94(6)
set_property PACKAGE_PIN H14 [get_ports {mpeg_clk_P}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_clk_P}]

#FMC_HPC_HA01_CC_N, J94(8)
set_property PACKAGE_PIN G14 [get_ports {mpeg_clk_N}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_clk_N}]

#FMC_HPC_HA02_P, J94(9)
set_property PACKAGE_PIN D11 [get_ports {mpeg_valid_P}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_valid_P}]

#FMC_HPC_HA02_N, J94(11)
set_property PACKAGE_PIN C11 [get_ports {mpeg_valid_N}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_valid_N}]

#FMC_HPC_HA03_P, J94(10)
set_property PACKAGE_PIN C12 [get_ports {mpeg_sync_P}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_sync_P}]

#FMC_HPC_HA03_N, J94(12)
set_property PACKAGE_PIN B12 [get_ports {mpeg_sync_N}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_sync_N}]

#FMC_HPC_HA00_CC_P, J94(5)
set_property PACKAGE_PIN D12 [get_ports {mpeg_data_P}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_data_P}]

#FMC_HPC_HA00_CC_N, J94(7)
set_property PACKAGE_PIN D13 [get_ports {mpeg_data_N}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_data_N}]

#FMC_HPC_HA11_P, J94(30)
set_property PACKAGE_PIN B13 [get_ports {asi_out_p}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p}]

#FMC_HPC_HA11_N, J94(32)
set_property PACKAGE_PIN A13 [get_ports {asi_out_n}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n}]

#FMC_HPC_HA16_P, J95(6)
set_property PACKAGE_PIN L15 [get_ports {mpeg_clk_P_1}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_clk_P_1}]

#FMC_HPC_HA16_N, J95(8)
set_property PACKAGE_PIN K15 [get_ports {mpeg_clk_N_1}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_clk_N_1}]

#FMC_HPC_HA17_CC_P, J95(9)
set_property PACKAGE_PIN G13 [get_ports {mpeg_valid_P_1}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_valid_P_1}]

#FMC_HPC_HA17_CC_N, J95(11)
set_property PACKAGE_PIN F13 [get_ports {mpeg_valid_N_1}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_valid_N_1}]

#FMC_HPC_HA18_P, J95(10)
set_property PACKAGE_PIN K14 [get_ports {mpeg_sync_P_1}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_sync_P_1}]

#FMC_HPC_HA18_N, J95(12)
set_property PACKAGE_PIN J14 [get_ports {mpeg_sync_N_1}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_sync_N_1}]

#FMC_HPC_HA15_P, J95(5)
set_property PACKAGE_PIN H15 [get_ports {mpeg_data_P_1}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_data_P_1}]

#FMC_HPC_HA15_N, J95(7)
set_property PACKAGE_PIN G15 [get_ports {mpeg_data_N_1}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_data_N_1}]

#FMC_HPC_LA30_P, J95(30)
set_property PACKAGE_PIN D22 [get_ports {asi_out_p_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p_1}]

#FMC_HPC_LA30_N, J95(32)
set_property PACKAGE_PIN C22 [get_ports {asi_out_n_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n_1}]

#FMC_HPC_CLK0_M2C_P, J96(6)
set_property PACKAGE_PIN D27 [get_ports {mpeg_clk_P_2}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_clk_P_2}]

#FMC_HPC_CLK0_M2C_N, J96(8)
set_property PACKAGE_PIN C27 [get_ports {mpeg_clk_N_2}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_clk_N_2}]

#FMC_HPC_LA00_CC_P, J96(9)
set_property PACKAGE_PIN C25 [get_ports {mpeg_valid_P_2}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_valid_P_2}]

#FMC_HPC_LA00_CC_N, J96(11)
set_property PACKAGE_PIN B25 [get_ports {mpeg_valid_N_2}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_valid_N_2}]

#FMC_HPC_LA01_CC_P, J96(10)
set_property PACKAGE_PIN D26 [get_ports {mpeg_sync_P_2}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_sync_P_2}]

#FMC_HPC_LA01_CC_N, J96(12)
set_property PACKAGE_PIN C26 [get_ports {mpeg_sync_N_2}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_sync_N_2}]

#FMC_HPC_CLK1_M2C_P, J96(5)
set_property PACKAGE_PIN D17 [get_ports {mpeg_data_P_2}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_data_P_2}]

#FMC_HPC_CLK1_M2C_N, J96(7)
set_property PACKAGE_PIN D18 [get_ports {mpeg_data_N_2}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {mpeg_data_N_2}]

#FMC_HPC_LA09_P, J96(30)
set_property PACKAGE_PIN B30 [get_ports {asi_out_p_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p_2}]

#FMC_HPC_LA09_N, J96(32)
set_property PACKAGE_PIN A30 [get_ports {asi_out_n_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n_2}]

set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets design_1_i/util_ds_buf_0/U0/IBUF_OUT[0]]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets design_1_i/util_ds_buf_4/U0/IBUF_OUT[0]]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets design_1_i/util_ds_buf_8/U0/IBUF_OUT[0]]
#set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets design_1_i/util_ds_buf_12/U0/IBUF_OUT[0]]
# ----------------------------------------------------------------------------------------------------
# gpio constrain
# ----------------------------------------------------------------------------------------------------

#FMC_HPC_HA04_N, i2c_sck, R117.1, J94(17), 241
set_property PACKAGE_PIN E11 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[0]}]

#FMC_HPC_HA05_N, i2c_sda, R104.1, J94(18), 242
set_property PACKAGE_PIN E16 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[1]}]

#FMC_HPC_HA06_N, lnb1_on_off, R141.1, J94(21), 243
set_property PACKAGE_PIN C14 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[2]}]

#FMC_HPC_HA06_P, nim_reset, R140.1, J94(19), 244
set_property PACKAGE_PIN D14 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[3]}]

#FMC_HPC_HA07_P, tunb_33_on_off, R128.1, J94(20), 245
set_property PACKAGE_PIN B14 [get_ports {gpio_tri_io[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[4]}]

#FMC_HPC_HA19_N, i2c_sck_1, R125.1, J95(17), 246
set_property PACKAGE_PIN H12 [get_ports {gpio_tri_io[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[5]}]

#FMC_HPC_HA20_N, i2c_sda_1, R112.1, J95(18), 247
set_property PACKAGE_PIN J13 [get_ports {gpio_tri_io[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[6]}]

#FMC_HPC_HA21_N, lnb1_on_off_1, R147.1, J95(21), 248
set_property PACKAGE_PIN J12 [get_ports {gpio_tri_io[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[7]}]

#FMC_HPC_HA21_P, nim_reset_1, R146.1, J95(19), 249
set_property PACKAGE_PIN J11 [get_ports {gpio_tri_io[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[8]}]

#FMC_HPC_HA22_P, tunb_33_on_off_1, R136.1, J95(20), 250
set_property PACKAGE_PIN L11 [get_ports {gpio_tri_io[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[9]}]

#FMC_HPC_LA02_N, i2c_sck_2, R66.1, J96(17), 251
set_property PACKAGE_PIN H25 [get_ports {gpio_tri_io[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[10]}]

#FMC_HPC_LA03_N, i2c_sda_2, R43.1, J96(18), 252
set_property PACKAGE_PIN H27 [get_ports {gpio_tri_io[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[11]}]

#FMC_HPC_LA04_N, lnb1_on_off_2, R68.1, J96(21), 253
set_property PACKAGE_PIN F28 [get_ports {gpio_tri_io[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[12]}]

#FMC_HPC_LA04_P, nim_reset_2, R67.1, J96(19), 254
set_property PACKAGE_PIN G28 [get_ports {gpio_tri_io[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[13]}]

#FMC_HPC_LA05_P, tunb_33_on_off_2, R22.1, J96(20), 255
set_property PACKAGE_PIN G29 [get_ports {gpio_tri_io[14]}]
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
