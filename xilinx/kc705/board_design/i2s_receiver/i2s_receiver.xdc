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

#FMC_LPC_LA28_P
set_property PACKAGE_PIN AE30 [get_ports {bclk[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[0]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[0]}]

#FMC_LPC_LA28_N
set_property PACKAGE_PIN AF30 [get_ports {lrclk[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[0]}]

#FMC_LPC_LA24_P
set_property PACKAGE_PIN AG30 [get_ports {sdata[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[0]}]

#FMC_LPC_LA24_N
set_property PACKAGE_PIN AH30 [get_ports {bclk[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[1]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[1]}]

#FMC_LPC_LA26_P
set_property PACKAGE_PIN AK29 [get_ports {lrclk[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[1]}]

#FMC_LPC_LA26_N
set_property PACKAGE_PIN AK30 [get_ports {sdata[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[1]}]

#FMC_LPC_LA27_P
set_property PACKAGE_PIN AJ28 [get_ports {bclk[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[2]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[2]}]

#FMC_LPC_LA27_N
set_property PACKAGE_PIN AJ29 [get_ports {lrclk[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[2]}]

#FMC_LPC_LA21_P
set_property PACKAGE_PIN AG27 [get_ports {sdata[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[2]}]

#FMC_LPC_LA21_N
set_property PACKAGE_PIN AG28 [get_ports {bclk[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[3]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[3]}]

#FMC_LPC_LA23_P
set_property PACKAGE_PIN AH26 [get_ports {lrclk[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[3]}]

#FMC_LPC_LA23_N
set_property PACKAGE_PIN AH27 [get_ports {sdata[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[3]}]

#FMC_LPC_LA22_P
set_property PACKAGE_PIN AJ27 [get_ports {bclk[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[4]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[4]}]

#FMC_LPC_LA22_N
set_property PACKAGE_PIN AK28 [get_ports {lrclk[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[4]}]

#FMC_LPC_LA19_P
set_property PACKAGE_PIN AJ26 [get_ports {sdata[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[4]}]

#FMC_LPC_LA19_N
set_property PACKAGE_PIN AK26 [get_ports {bclk[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[5]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[5]}]

#FMC_LPC_LA20_P
set_property PACKAGE_PIN AF26 [get_ports {lrclk[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[5]}]

#FMC_LPC_LA20_N
set_property PACKAGE_PIN AF27 [get_ports {sdata[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[5]}]

#FMC_LPC_LA25_P
set_property PACKAGE_PIN AC26 [get_ports {bclk[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[6]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[6]}]

#FMC_LPC_LA25_N
set_property PACKAGE_PIN AD26 [get_ports {lrclk[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[6]}]

#FMC_LPC_LA11_P
set_property PACKAGE_PIN AE25 [get_ports {sdata[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[6]}]

#FMC_LPC_LA11_N
set_property PACKAGE_PIN AF25 [get_ports {bclk[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[7]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[7]}]

#FMC_LPC_LA15_P
set_property PACKAGE_PIN AC24 [get_ports {lrclk[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[7]}]

#FMC_LPC_LA15_N
set_property PACKAGE_PIN AD24 [get_ports {sdata[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[7]}]

#FMC_LPC_LA10_P
set_property PACKAGE_PIN AJ24 [get_ports {bclk[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[8]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[8]}]

#FMC_LPC_LA10_N
set_property PACKAGE_PIN AK25 [get_ports {lrclk[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[8]}]

#FMC_LPC_LA08_P
set_property PACKAGE_PIN AJ22 [get_ports {sdata[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[8]}]

#FMC_LPC_LA08_N
set_property PACKAGE_PIN AJ23 [get_ports {bclk[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[9]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[9]}]

#FMC_LPC_LA05_P
set_property PACKAGE_PIN AG22 [get_ports {lrclk[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[9]}]

#FMC_LPC_LA05_N
set_property PACKAGE_PIN AH22 [get_ports {sdata[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[9]}]

#FMC_LPC_LA00_CC_P
set_property PACKAGE_PIN AD23 [get_ports {bclk[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[10]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[10]}]

#FMC_LPC_LA00_CC_N
set_property PACKAGE_PIN AE24 [get_ports {lrclk[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[10]}]

#FMC_LPC_LA16_P
set_property PACKAGE_PIN AC22 [get_ports {sdata[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[10]}]

#FMC_LPC_LA16_N
set_property PACKAGE_PIN AD22 [get_ports {bclk[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[11]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[11]}]

#FMC_LPC_LA02_P
set_property PACKAGE_PIN AF20 [get_ports {lrclk[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[11]}]

#FMC_LPC_LA02_N
set_property PACKAGE_PIN AF21 [get_ports {sdata[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[11]}]

#FMC_LPC_LA03_P
set_property PACKAGE_PIN AG20 [get_ports {bclk[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[12]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[12]}]

#FMC_LPC_LA03_N
set_property PACKAGE_PIN AH20 [get_ports {lrclk[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[12]}]

#FMC_LPC_LA06_P
set_property PACKAGE_PIN AK20 [get_ports {sdata[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[12]}]

#FMC_LPC_LA06_N
set_property PACKAGE_PIN AK21 [get_ports {bclk[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[13]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[13]}]

#FMC_LPC_LA01_CC_P
set_property PACKAGE_PIN AE23 [get_ports {lrclk[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[13]}]

#FMC_LPC_LA01_CC_N
set_property PACKAGE_PIN AF23 [get_ports {sdata[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[13]}]

#FMC_LPC_LA13_P
set_property PACKAGE_PIN AB24 [get_ports {bclk[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[14]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[14]}]

#FMC_LPC_LA13_N
set_property PACKAGE_PIN AC25 [get_ports {lrclk[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[14]}]

#FMC_LPC_LA09_P
set_property PACKAGE_PIN AK23 [get_ports {sdata[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[14]}]

#FMC_LPC_LA09_N
set_property PACKAGE_PIN AK24 [get_ports {bclk[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {bclk[15]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[15]}]

#FMC_LPC_LA14_P
set_property PACKAGE_PIN AD21 [get_ports {lrclk[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {lrclk[15]}]

#FMC_LPC_LA14_N
set_property PACKAGE_PIN AE21 [get_ports {sdata[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {sdata[15]}]
# ----------------------------------------------------------------------------------------------------
# gpio constrain
# ----------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------
# gpio constrain for LPC
# ----------------------------------------------------------------------------------------------------

#FMC_LPC_LA32_P, SOMI, R85.1, 170
set_property PACKAGE_PIN Y30 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[0]}]

#FMC_LPC_LA32_N, MOSI, R86.1, 171
set_property PACKAGE_PIN AA30 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[1]}]

#FMC_LPC_LA30_P, SCLK, R83.1, 172
set_property PACKAGE_PIN AB29 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[2]}]

#FMC_LPC_LA30_N, 74138GA(CS), R84.1, 173
set_property PACKAGE_PIN AB30 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[3]}]

#FMC_LPC_LA33_P, 74138GB, R60.1, 174
set_property PACKAGE_PIN AC29 [get_ports {gpio_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[4]}]

#FMC_LPC_LA33_N, 74138GC, R61.1, 175
set_property PACKAGE_PIN AC30 [get_ports {gpio_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[5]}]

#FMC_LPC_LA17_CC_P, SPI_S0, R28.1, 176
set_property PACKAGE_PIN AB27 [get_ports {gpio_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[6]}]

#FMC_LPC_LA17_CC_N, SPI_S1, R29.1, 177
set_property PACKAGE_PIN AC27 [get_ports {gpio_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[7]}]

#FMC_LPC_LA31_P, SPI_S2, R58.1, 178
set_property PACKAGE_PIN AD29 [get_ports {gpio_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[8]}]

#FMC_LPC_LA31_N, SPI_S3, R59.1, 179
set_property PACKAGE_PIN AE29 [get_ports {gpio_tri_io[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[9]}]
# ----------------------------------------------------------------------------------------------------
# bitstream constrain
# ----------------------------------------------------------------------------------------------------

set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 33 [current_design]

##Encryption Settings
#set_property BITSTREAM.ENCRYPTION.ENCRYPT YES [current_design]
#set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT BBRAM [current_design]
##set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT eFUSE [current_design]
#set_property BITSTREAM.ENCRYPTION.KEY0 256'h12345678ABCDDCBA1234578ABCDDCBA1234578ABCDDCBA1234578ABCDDCBA [current_design]
