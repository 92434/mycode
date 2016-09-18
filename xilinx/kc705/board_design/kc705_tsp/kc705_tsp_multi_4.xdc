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

#FMC_HPC_HA15_P
set_property PACKAGE_PIN H15 [get_ports {mpeg_clk_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_clk_1}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {mpeg_clk_1}]

#FMC_HPC_HA16_P
set_property PACKAGE_PIN L15 [get_ports {mpeg_sync_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_sync_1}]

#FMC_HPC_HA20_N
set_property PACKAGE_PIN J13 [get_ports {mpeg_valid_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_valid_1}]

#FMC_HPC_HA15_N
set_property PACKAGE_PIN G15 [get_ports {mpeg_data_1[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[0]}]

#FMC_HPC_HA16_N
set_property PACKAGE_PIN K15 [get_ports {mpeg_data_1[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[1]}]

#FMC_HPC_HA17_CC_P
set_property PACKAGE_PIN G13 [get_ports {mpeg_data_1[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[2]}]

#FMC_HPC_HA19_P
set_property PACKAGE_PIN H11 [get_ports {mpeg_data_1[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[3]}]

#FMC_HPC_HA18_N
set_property PACKAGE_PIN J14 [get_ports {mpeg_data_1[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[4]}]

#FMC_HPC_HA19_N
set_property PACKAGE_PIN H12 [get_ports {mpeg_data_1[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[5]}]

#FMC_HPC_HA20_P
set_property PACKAGE_PIN K13 [get_ports {mpeg_data_1[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[6]}]

#FMC_HPC_HA21_P
set_property PACKAGE_PIN J11 [get_ports {mpeg_data_1[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_1[7]}]

#FMC_HPC_LA29_P
set_property PACKAGE_PIN C17 [get_ports {asi_out_p_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p_1}]

#FMC_HPC_LA29_N
set_property PACKAGE_PIN B17 [get_ports {asi_out_n_1}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n_1}]

#FMC_HPC_CLK1_M2C_P
set_property PACKAGE_PIN D17 [get_ports {mpeg_clk_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_clk_2}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {mpeg_clk_2}]

#FMC_HPC_CLK0_M2C_P
set_property PACKAGE_PIN D27 [get_ports {mpeg_sync_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_sync_2}]

#FMC_HPC_LA03_N
set_property PACKAGE_PIN H27 [get_ports {mpeg_valid_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_valid_2}]

#FMC_HPC_CLK1_M2C_N
set_property PACKAGE_PIN D18 [get_ports {mpeg_data_2[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[0]}]

#FMC_HPC_CLK0_M2C_N
set_property PACKAGE_PIN C27 [get_ports {mpeg_data_2[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[1]}]

#FMC_HPC_LA00_CC_P
set_property PACKAGE_PIN C25 [get_ports {mpeg_data_2[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[2]}]

#FMC_HPC_LA02_P
set_property PACKAGE_PIN H24 [get_ports {mpeg_data_2[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[3]}]

#FMC_HPC_LA01_CC_N
set_property PACKAGE_PIN C26 [get_ports {mpeg_data_2[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[4]}]

#FMC_HPC_LA02_N
set_property PACKAGE_PIN H25 [get_ports {mpeg_data_2[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[5]}]

#FMC_HPC_LA03_P
set_property PACKAGE_PIN H26 [get_ports {mpeg_data_2[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[6]}]

#FMC_HPC_LA04_P
set_property PACKAGE_PIN G28 [get_ports {mpeg_data_2[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_2[7]}]

#FMC_HPC_LA08_P
set_property PACKAGE_PIN E29 [get_ports {asi_out_p_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p_2}]

#FMC_HPC_LA08_N
set_property PACKAGE_PIN E30 [get_ports {asi_out_n_2}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n_2}]

#FMC_HPC_LA13_P
set_property PACKAGE_PIN A25 [get_ports {mpeg_clk_3}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_clk_3}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {mpeg_clk_3}]

#FMC_HPC_LA14_P
set_property PACKAGE_PIN B28 [get_ports {mpeg_sync_3}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_sync_3}]

#FMC_HPC_LA18_CC_N
set_property PACKAGE_PIN E21 [get_ports {mpeg_valid_3}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_valid_3}]

#FMC_HPC_LA13_N
set_property PACKAGE_PIN A26 [get_ports {mpeg_data_3[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_3[0]}]

#FMC_HPC_LA14_N
set_property PACKAGE_PIN A28 [get_ports {mpeg_data_3[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_3[1]}]

#FMC_HPC_LA15_P
set_property PACKAGE_PIN C24 [get_ports {mpeg_data_3[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_3[2]}]

#FMC_HPC_LA17_CC_P
set_property PACKAGE_PIN F20 [get_ports {mpeg_data_3[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_3[3]}]

#FMC_HPC_LA16_N
set_property PACKAGE_PIN A27 [get_ports {mpeg_data_3[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_3[4]}]

#FMC_HPC_LA17_CC_N
set_property PACKAGE_PIN E20 [get_ports {mpeg_data_3[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_3[5]}]

#FMC_HPC_LA18_CC_P
set_property PACKAGE_PIN F21 [get_ports {mpeg_data_3[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_3[6]}]

#FMC_HPC_LA19_P
set_property PACKAGE_PIN G18 [get_ports {mpeg_data_3[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {mpeg_data_3[7]}]

#FMC_HPC_LA23_P
set_property PACKAGE_PIN B22 [get_ports {asi_out_p_3}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_p_3}]

#FMC_HPC_LA23_N
set_property PACKAGE_PIN A22 [get_ports {asi_out_n_3}]
set_property IOSTANDARD LVCMOS25 [get_ports {asi_out_n_3}]
# ----------------------------------------------------------------------------------------------------
# gpio constrain
# ----------------------------------------------------------------------------------------------------

#FMC_LPC_LA33_P, i2c_s0, R60.1, 252
set_property PACKAGE_PIN AC29 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[0]}]

#FMC_LPC_LA33_N, i2c_s1, R61.1, 253
set_property PACKAGE_PIN AC30 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[1]}]

#FMC_LPC_LA30_P, i2c_sda, R83.1, 254
set_property PACKAGE_PIN AB29 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[2]}]

#FMC_LPC_LA30_N, i2c_sck, R84.1, 255
set_property PACKAGE_PIN AB30 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[3]}]
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
