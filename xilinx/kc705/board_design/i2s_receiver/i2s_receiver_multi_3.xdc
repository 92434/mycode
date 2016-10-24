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

#FMC_LPC_LA05_P, J101(17)
set_property PACKAGE_PIN AG22 [get_ports {bclk[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[0]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[0]}]

#FMC_LPC_LA05_N, J101(18)
set_property PACKAGE_PIN AH22 [get_ports {lrclk[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[0]}]

#FMC_LPC_LA00_CC_P, J101(19)
set_property PACKAGE_PIN AD23 [get_ports {sdata[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[0]}]

#FMC_LPC_LA00_CC_N, J101(20)
set_property PACKAGE_PIN AE24 [get_ports {bclk[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[1]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[1]}]

#FMC_LPC_LA16_P, J101(21)
set_property PACKAGE_PIN AC22 [get_ports {lrclk[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[1]}]

#FMC_LPC_LA16_N, J101(22)
set_property PACKAGE_PIN AD22 [get_ports {sdata[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[1]}]

#FMC_LPC_LA02_P, J101(23)
set_property PACKAGE_PIN AF20 [get_ports {bclk[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[2]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[2]}]

#FMC_LPC_LA02_N, J101(24)
set_property PACKAGE_PIN AF21 [get_ports {lrclk[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[2]}]

#FMC_LPC_LA03_P, J101(25)
set_property PACKAGE_PIN AG20 [get_ports {sdata[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[2]}]

#FMC_LPC_LA03_N, J101(26)
set_property PACKAGE_PIN AH20 [get_ports {bclk[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[3]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[3]}]

#FMC_LPC_LA06_P, J101(29)
set_property PACKAGE_PIN AK20 [get_ports {lrclk[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[3]}]

#FMC_LPC_LA06_N, J101(30)
set_property PACKAGE_PIN AK21 [get_ports {sdata[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[3]}]

#FMC_LPC_LA01_CC_P, J101(31)
set_property PACKAGE_PIN AE23 [get_ports {bclk[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[4]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[4]}]

#FMC_LPC_LA01_CC_N, J101(32)
set_property PACKAGE_PIN AF23 [get_ports {lrclk[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[4]}]

#FMC_LPC_LA13_P, J101(33)
set_property PACKAGE_PIN AB24 [get_ports {sdata[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[4]}]

#FMC_LPC_LA13_N, J101(34)
set_property PACKAGE_PIN AC25 [get_ports {bclk[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[5]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[5]}]

#FMC_LPC_LA09_P, J101(35)
set_property PACKAGE_PIN AK23 [get_ports {lrclk[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[5]}]

#FMC_LPC_LA09_N, J101(36)
set_property PACKAGE_PIN AK24 [get_ports {sdata[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[5]}]

#FMC_LPC_LA32_P, J102(5)
set_property PACKAGE_PIN Y30 [get_ports {bclk[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[6]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[6]}]

#FMC_LPC_LA32_N, J102(6)
set_property PACKAGE_PIN AA30 [get_ports {lrclk[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[6]}]

#FMC_LPC_LA30_P, J102(7)
set_property PACKAGE_PIN AB29 [get_ports {sdata[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[6]}]

#FMC_LPC_LA30_N, J102(8)
set_property PACKAGE_PIN AB30 [get_ports {bclk[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[7]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[7]}]

#FMC_LPC_LA33_P, J102(9)
set_property PACKAGE_PIN AC29 [get_ports {lrclk[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[7]}]

#FMC_LPC_LA33_N, J102(10)
set_property PACKAGE_PIN AC30 [get_ports {sdata[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[7]}]

#FMC_LPC_LA17_CC_P, J102(11)
set_property PACKAGE_PIN AB27 [get_ports {bclk[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[8]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[8]}]

#FMC_LPC_LA17_CC_N, J102(12)
set_property PACKAGE_PIN AC27 [get_ports {lrclk[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[8]}]

#FMC_LPC_LA31_P, J102(13)
set_property PACKAGE_PIN AD29 [get_ports {sdata[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[8]}]

#FMC_LPC_LA31_N, J102(14)
set_property PACKAGE_PIN AE29 [get_ports {bclk[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[9]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[9]}]

#FMC_LPC_LA28_P, J102(17)
set_property PACKAGE_PIN AE30 [get_ports {lrclk[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[9]}]

#FMC_LPC_LA28_N, J102(18)
set_property PACKAGE_PIN AF30 [get_ports {sdata[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[9]}]

#FMC_LPC_LA29_P, J102(19)
set_property PACKAGE_PIN AE28 [get_ports {bclk[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[10]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[10]}]

#FMC_LPC_LA29_N, J102(20)
set_property PACKAGE_PIN AF28 [get_ports {lrclk[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[10]}]

#FMC_LPC_LA24_P, J102(21)
set_property PACKAGE_PIN AG30 [get_ports {sdata[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[10]}]

#FMC_LPC_LA24_N, J102(22)
set_property PACKAGE_PIN AH30 [get_ports {bclk[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[11]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[11]}]

#FMC_LPC_LA26_P, J102(23)
set_property PACKAGE_PIN AK29 [get_ports {lrclk[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[11]}]

#FMC_LPC_LA26_N, J102(24)
set_property PACKAGE_PIN AK30 [get_ports {sdata[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[11]}]

#FMC_LPC_LA27_P, J102(25)
set_property PACKAGE_PIN AJ28 [get_ports {bclk[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[12]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[12]}]

#FMC_LPC_LA27_N, J102(26)
set_property PACKAGE_PIN AJ29 [get_ports {lrclk[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[12]}]

#FMC_LPC_LA21_P, J102(29)
set_property PACKAGE_PIN AG27 [get_ports {sdata[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[12]}]

#FMC_LPC_LA21_N, J102(30)
set_property PACKAGE_PIN AG28 [get_ports {bclk[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[13]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[13]}]

#FMC_LPC_LA23_P, J102(31)
set_property PACKAGE_PIN AH26 [get_ports {lrclk[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[13]}]

#FMC_LPC_LA23_N, J102(32)
set_property PACKAGE_PIN AH27 [get_ports {sdata[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[13]}]

#FMC_LPC_LA22_P, J102(33)
set_property PACKAGE_PIN AJ27 [get_ports {bclk[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[14]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[14]}]

#FMC_LPC_LA22_N, J102(34)
set_property PACKAGE_PIN AK28 [get_ports {lrclk[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[14]}]

#FMC_LPC_LA19_P, J102(35)
set_property PACKAGE_PIN AJ26 [get_ports {sdata[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[14]}]

#FMC_LPC_LA19_N, J102(36)
set_property PACKAGE_PIN AK26 [get_ports {bclk[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk[15]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk[15]}]

#FMC_LPC_LA20_P, J102(37)
set_property PACKAGE_PIN AF26 [get_ports {lrclk[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk[15]}]

#FMC_LPC_LA20_N, J102(38)
set_property PACKAGE_PIN AF27 [get_ports {sdata[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata[15]}]

#HDMI_R_SPDIF, J86(17)
set_property PACKAGE_PIN J17 [get_ports {bclk_1[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[0]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[0]}]

#HDMI_R_DE, J86(18)
set_property PACKAGE_PIN H17 [get_ports {lrclk_1[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[0]}]

#HDMI_R_D0, J86(19)
set_property PACKAGE_PIN B23 [get_ports {sdata_1[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[0]}]

#HDMI_R_D1, J86(20)
set_property PACKAGE_PIN A23 [get_ports {bclk_1[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[1]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[1]}]

#HDMI_R_D2, J86(21)
set_property PACKAGE_PIN E23 [get_ports {lrclk_1[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[1]}]

#HDMI_R_D3, J86(22)
set_property PACKAGE_PIN D23 [get_ports {sdata_1[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[1]}]

#HDMI_R_D4, J86(23)
set_property PACKAGE_PIN F25 [get_ports {bclk_1[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[2]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[2]}]

#HDMI_R_D5, J86(24)
set_property PACKAGE_PIN E25 [get_ports {lrclk_1[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[2]}]

#HDMI_R_D6, J86(25)
set_property PACKAGE_PIN E24 [get_ports {sdata_1[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[2]}]

#HDMI_R_D7, J86(26)
set_property PACKAGE_PIN D24 [get_ports {bclk_1[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[3]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[3]}]

#HDMI_R_D8, J86(29)
set_property PACKAGE_PIN F26 [get_ports {lrclk_1[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[3]}]

#HDMI_R_D9, J86(30)
set_property PACKAGE_PIN E26 [get_ports {sdata_1[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[3]}]

#HDMI_R_D10, J86(31)
set_property PACKAGE_PIN G23 [get_ports {bclk_1[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[4]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[4]}]

#HDMI_R_D11, J86(32)
set_property PACKAGE_PIN G24 [get_ports {lrclk_1[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[4]}]

#HDMI_R_D12, J86(33)
set_property PACKAGE_PIN J19 [get_ports {sdata_1[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[4]}]

#HDMI_R_D13, J86(34)
set_property PACKAGE_PIN H19 [get_ports {bclk_1[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[5]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[5]}]

#HDMI_R_D14, J86(35)
set_property PACKAGE_PIN L17 [get_ports {lrclk_1[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[5]}]

#HDMI_R_D15, J86(36)
set_property PACKAGE_PIN L18 [get_ports {sdata_1[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[5]}]

#FMC_HPC_CLK1_M2C_P, J96(5)
set_property PACKAGE_PIN D17 [get_ports {bclk_1[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[6]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[6]}]

#FMC_HPC_CLK0_M2C_P, J96(6)
set_property PACKAGE_PIN D27 [get_ports {lrclk_1[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[6]}]

#FMC_HPC_CLK1_M2C_N, J96(7)
set_property PACKAGE_PIN D18 [get_ports {sdata_1[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[6]}]

#FMC_HPC_CLK0_M2C_N, J96(8)
set_property PACKAGE_PIN C27 [get_ports {bclk_1[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[7]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[7]}]

#FMC_HPC_LA00_CC_P, J96(9)
set_property PACKAGE_PIN C25 [get_ports {lrclk_1[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[7]}]

#FMC_HPC_LA01_CC_P, J96(10)
set_property PACKAGE_PIN D26 [get_ports {sdata_1[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[7]}]

#FMC_HPC_LA00_CC_N, J96(11)
set_property PACKAGE_PIN B25 [get_ports {bclk_1[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[8]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[8]}]

#FMC_HPC_LA01_CC_N, J96(12)
set_property PACKAGE_PIN C26 [get_ports {lrclk_1[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[8]}]

#FMC_HPC_LA02_P, J96(13)
set_property PACKAGE_PIN H24 [get_ports {sdata_1[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[8]}]

#FMC_HPC_LA03_P, J96(14)
set_property PACKAGE_PIN H26 [get_ports {bclk_1[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[9]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[9]}]

#FMC_HPC_LA02_N, J96(17)
set_property PACKAGE_PIN H25 [get_ports {lrclk_1[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[9]}]

#FMC_HPC_LA03_N, J96(18)
set_property PACKAGE_PIN H27 [get_ports {sdata_1[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[9]}]

#FMC_HPC_LA04_P, J96(19)
set_property PACKAGE_PIN G28 [get_ports {bclk_1[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[10]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[10]}]

#FMC_HPC_LA05_P, J96(20)
set_property PACKAGE_PIN G29 [get_ports {lrclk_1[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[10]}]

#FMC_HPC_LA04_N, J96(21)
set_property PACKAGE_PIN F28 [get_ports {sdata_1[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[10]}]

#FMC_HPC_LA05_N, J96(22)
set_property PACKAGE_PIN F30 [get_ports {bclk_1[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[11]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[11]}]

#FMC_HPC_LA06_P, J96(23)
set_property PACKAGE_PIN H30 [get_ports {lrclk_1[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[11]}]

#FMC_HPC_LA07_P, J96(24)
set_property PACKAGE_PIN E28 [get_ports {sdata_1[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[11]}]

#FMC_HPC_LA06_N, J96(25)
set_property PACKAGE_PIN G30 [get_ports {bclk_1[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[12]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[12]}]

#FMC_HPC_LA07_N, J96(26)
set_property PACKAGE_PIN D28 [get_ports {lrclk_1[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[12]}]

#FMC_HPC_LA08_P, J96(29)
set_property PACKAGE_PIN E29 [get_ports {sdata_1[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[12]}]

#FMC_HPC_LA09_P, J96(30)
set_property PACKAGE_PIN B30 [get_ports {bclk_1[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[13]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[13]}]

#FMC_HPC_LA08_N, J96(31)
set_property PACKAGE_PIN E30 [get_ports {lrclk_1[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[13]}]

#FMC_HPC_LA09_N, J96(32)
set_property PACKAGE_PIN A30 [get_ports {sdata_1[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[13]}]

#FMC_HPC_LA10_P, J96(33)
set_property PACKAGE_PIN D29 [get_ports {bclk_1[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[14]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[14]}]

#FMC_HPC_LA11_P, J96(34)
set_property PACKAGE_PIN G27 [get_ports {lrclk_1[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[14]}]

#FMC_HPC_LA10_N, J96(35)
set_property PACKAGE_PIN C30 [get_ports {sdata_1[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[14]}]

#FMC_HPC_LA11_N, J96(36)
set_property PACKAGE_PIN F27 [get_ports {bclk_1[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_1[15]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_1[15]}]

#FMC_HPC_LA12_P, J96(37)
set_property PACKAGE_PIN C29 [get_ports {lrclk_1[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_1[15]}]

#FMC_HPC_LA12_N, J96(38)
set_property PACKAGE_PIN B29 [get_ports {sdata_1[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_1[15]}]

#FMC_HPC_HA04_N, J94(17)
set_property PACKAGE_PIN E11 [get_ports {bclk_2[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[0]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[0]}]

#FMC_HPC_HA05_N, J94(18)
set_property PACKAGE_PIN E16 [get_ports {lrclk_2[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[0]}]

#FMC_HPC_HA06_P, J94(19)
set_property PACKAGE_PIN D14 [get_ports {sdata_2[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[0]}]

#FMC_HPC_HA07_P, J94(20)
set_property PACKAGE_PIN B14 [get_ports {bclk_2[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[1]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[1]}]

#FMC_HPC_HA06_N, J94(21)
set_property PACKAGE_PIN C14 [get_ports {lrclk_2[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[1]}]

#FMC_HPC_HA07_N, J94(22)
set_property PACKAGE_PIN A15 [get_ports {sdata_2[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[1]}]

#FMC_HPC_HA08_P, J94(23)
set_property PACKAGE_PIN E14 [get_ports {bclk_2[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[2]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[2]}]

#FMC_HPC_HA09_P, J94(24)
set_property PACKAGE_PIN F12 [get_ports {lrclk_2[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[2]}]

#FMC_HPC_HA08_N, J94(25)
set_property PACKAGE_PIN E15 [get_ports {sdata_2[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[2]}]

#FMC_HPC_HA09_N, J94(26)
set_property PACKAGE_PIN E13 [get_ports {bclk_2[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[3]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[3]}]

#FMC_HPC_HA10_P, J94(29)
set_property PACKAGE_PIN A11 [get_ports {lrclk_2[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[3]}]

#FMC_HPC_HA11_P, J94(30)
set_property PACKAGE_PIN B13 [get_ports {sdata_2[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[3]}]

#FMC_HPC_HA10_N, J94(31)
set_property PACKAGE_PIN A12 [get_ports {bclk_2[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[4]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[4]}]

#FMC_HPC_HA11_N, J94(32)
set_property PACKAGE_PIN A13 [get_ports {lrclk_2[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[4]}]

#FMC_HPC_HA12_P, J94(33)
set_property PACKAGE_PIN C15 [get_ports {sdata_2[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[4]}]

#FMC_HPC_HA13_P, J94(34)
set_property PACKAGE_PIN L16 [get_ports {bclk_2[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[5]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[5]}]

#FMC_HPC_HA12_N, J94(35)
set_property PACKAGE_PIN B15 [get_ports {lrclk_2[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[5]}]

#FMC_HPC_HA13_N, J94(36)
set_property PACKAGE_PIN K16 [get_ports {sdata_2[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[5]}]

#FMC_HPC_LA13_P, J97(5)
set_property PACKAGE_PIN A25 [get_ports {bclk_2[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[6]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[6]}]

#FMC_HPC_LA14_P, J97(6)
set_property PACKAGE_PIN B28 [get_ports {lrclk_2[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[6]}]

#FMC_HPC_LA13_N, J97(7)
set_property PACKAGE_PIN A26 [get_ports {sdata_2[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[6]}]

#FMC_HPC_LA14_N, J97(8)
set_property PACKAGE_PIN A28 [get_ports {bclk_2[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[7]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[7]}]

#FMC_HPC_LA15_P, J97(9)
set_property PACKAGE_PIN C24 [get_ports {lrclk_2[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[7]}]

#FMC_HPC_LA16_P, J97(10)
set_property PACKAGE_PIN B27 [get_ports {sdata_2[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[7]}]

#FMC_HPC_LA15_N, J97(11)
set_property PACKAGE_PIN B24 [get_ports {bclk_2[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[8]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[8]}]

#FMC_HPC_LA16_N, J97(12)
set_property PACKAGE_PIN A27 [get_ports {lrclk_2[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[8]}]

#FMC_HPC_LA17_CC_P, J97(13)
set_property PACKAGE_PIN F20 [get_ports {sdata_2[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[8]}]

#FMC_HPC_LA18_CC_P, J97(14)
set_property PACKAGE_PIN F21 [get_ports {bclk_2[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[9]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[9]}]

#FMC_HPC_LA17_CC_N, J97(17)
set_property PACKAGE_PIN E20 [get_ports {lrclk_2[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[9]}]

#FMC_HPC_LA18_CC_N, J97(18)
set_property PACKAGE_PIN E21 [get_ports {sdata_2[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[9]}]

#FMC_HPC_LA19_P, J97(19)
set_property PACKAGE_PIN G18 [get_ports {bclk_2[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[10]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[10]}]

#FMC_HPC_LA20_P, J97(20)
set_property PACKAGE_PIN E19 [get_ports {lrclk_2[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[10]}]

#FMC_HPC_LA19_N, J97(21)
set_property PACKAGE_PIN F18 [get_ports {sdata_2[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[10]}]

#FMC_HPC_LA20_N, J97(22)
set_property PACKAGE_PIN D19 [get_ports {bclk_2[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[11]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[11]}]

#FMC_HPC_LA21_P, J97(23)
set_property PACKAGE_PIN A20 [get_ports {lrclk_2[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[11]}]

#FMC_HPC_LA22_P, J97(24)
set_property PACKAGE_PIN C20 [get_ports {sdata_2[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[11]}]

#FMC_HPC_LA21_N, J97(25)
set_property PACKAGE_PIN A21 [get_ports {bclk_2[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[12]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[12]}]

#FMC_HPC_LA22_N, J97(26)
set_property PACKAGE_PIN B20 [get_ports {lrclk_2[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[12]}]

#FMC_HPC_LA23_P, J97(29)
set_property PACKAGE_PIN B22 [get_ports {sdata_2[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[12]}]

#FMC_HPC_LA24_P, J97(30)
set_property PACKAGE_PIN A16 [get_ports {bclk_2[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[13]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[13]}]

#FMC_HPC_LA23_N, J97(31)
set_property PACKAGE_PIN A22 [get_ports {lrclk_2[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[13]}]

#FMC_HPC_LA24_N, J97(32)
set_property PACKAGE_PIN A17 [get_ports {sdata_2[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[13]}]

#FMC_HPC_LA25_P, J97(33)
set_property PACKAGE_PIN G17 [get_ports {bclk_2[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[14]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[14]}]

#FMC_HPC_LA26_P, J97(34)
set_property PACKAGE_PIN B18 [get_ports {lrclk_2[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[14]}]

#FMC_HPC_LA25_N, J97(35)
set_property PACKAGE_PIN F17 [get_ports {sdata_2[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[14]}]

#FMC_HPC_LA26_N, J97(36)
set_property PACKAGE_PIN A18 [get_ports {bclk_2[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bclk_2[15]}]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {bclk_2[15]}]

#FMC_HPC_LA27_P, J97(37)
set_property PACKAGE_PIN C19 [get_ports {lrclk_2[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {lrclk_2[15]}]

#FMC_HPC_LA27_N, J97(38)
set_property PACKAGE_PIN B19 [get_ports {sdata_2[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {sdata_2[15]}]
# ----------------------------------------------------------------------------------------------------
# gpio constrain
# ----------------------------------------------------------------------------------------------------

#FMC_LPC_LA25_N, SOMI, R55.1, J101(6), 226
set_property PACKAGE_PIN AD26 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[0]}]

#FMC_LPC_LA25_P, MOSI, R54.1, J101(5), 227
set_property PACKAGE_PIN AC26 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[1]}]

#FMC_LPC_LA11_P, SCLK, R71.1, J101(7), 228
set_property PACKAGE_PIN AE25 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[2]}]

#FMC_LPC_LA11_N, 74138GA(CS), R72.1, J101(8), 229
set_property PACKAGE_PIN AF25 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[3]}]

#FMC_LPC_LA14_P, 74138GB, R9.1, J101(37), 230
set_property PACKAGE_PIN AD21 [get_ports {gpio_tri_io[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[4]}]

#FMC_LPC_LA15_N, 74138GC, R74.1, J101(10), 231
set_property PACKAGE_PIN AD24 [get_ports {gpio_tri_io[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[5]}]

#FMC_LPC_LA10_P, SPI_S0, R7.1, J101(11), 232
set_property PACKAGE_PIN AJ24 [get_ports {gpio_tri_io[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[6]}]

#FMC_LPC_LA10_N, SPI_S1, R8.1, J101(12), 233
set_property PACKAGE_PIN AK25 [get_ports {gpio_tri_io[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[7]}]

#FMC_LPC_LA08_P, SPI_S2, R44.1, J101(13), 234
set_property PACKAGE_PIN AJ22 [get_ports {gpio_tri_io[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[8]}]

#FMC_LPC_LA08_N, SPI_S3, R45.1, J101(14), 235
set_property PACKAGE_PIN AJ23 [get_ports {gpio_tri_io[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[9]}]

#FMC_HPC_LA32_P, SOMI_1, R85.1, J86(6), 236
set_property PACKAGE_PIN D21 [get_ports {gpio_tri_io[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[10]}]

#FMC_HPC_LA32_N, MOSI_1, R86.1, J86(5), 237
set_property PACKAGE_PIN C21 [get_ports {gpio_tri_io[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[11]}]

#FMC_HPC_LA28_P, SCLK_1, R81.1, J86(7), 238
set_property PACKAGE_PIN D16 [get_ports {gpio_tri_io[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[12]}]

#GPIO_LED_6_LS, 74138GA(CS)_1, undefined, J86(8), 239
set_property PACKAGE_PIN E18 [get_ports {gpio_tri_io[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[13]}]

#HDMI_R_D16, 74138GB_1, undefined, J86(37), 240
set_property PACKAGE_PIN K19 [get_ports {gpio_tri_io[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[14]}]

#GPIO_LED_7_LS, 74138GC_1, undefined, J86(10), 241
set_property PACKAGE_PIN F16 [get_ports {gpio_tri_io[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[15]}]

#HDMI_R_CLK, SPI_S0_1, undefined, J86(11), 242
set_property PACKAGE_PIN K18 [get_ports {gpio_tri_io[16]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[16]}]

#HDMI_R_HSYNC, SPI_S1_1, undefined, J86(12), 243
set_property PACKAGE_PIN J18 [get_ports {gpio_tri_io[17]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[17]}]

#HDMI_R_VSYNC, SPI_S2_1, undefined, J86(13), 244
set_property PACKAGE_PIN H20 [get_ports {gpio_tri_io[18]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[18]}]

#HDMI_SPDIF_OUT_LS, SPI_S3_1, undefined, J86(14), 245
set_property PACKAGE_PIN G20 [get_ports {gpio_tri_io[19]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[19]}]

#FMC_HPC_HA01_CC_P, SOMI_2, R101.1, J94(6), 246
set_property PACKAGE_PIN H14 [get_ports {gpio_tri_io[20]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[20]}]

#FMC_HPC_HA00_CC_P, MOSI_2, R114.1, J94(5), 247
set_property PACKAGE_PIN D12 [get_ports {gpio_tri_io[21]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[21]}]

#FMC_HPC_HA00_CC_N, SCLK_2, R115.1, J94(7), 248
set_property PACKAGE_PIN D13 [get_ports {gpio_tri_io[22]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[22]}]

#FMC_HPC_HA01_CC_N, 74138GA(CS)_2, R102.1, J94(8), 249
set_property PACKAGE_PIN G14 [get_ports {gpio_tri_io[23]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[23]}]

#FMC_HPC_HA14_P, 74138GB_2, R132.1, J94(37), 250
set_property PACKAGE_PIN J16 [get_ports {gpio_tri_io[24]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[24]}]

#FMC_HPC_HA03_P, 74138GC_2, R126.1, J94(10), 251
set_property PACKAGE_PIN C12 [get_ports {gpio_tri_io[25]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[25]}]

#FMC_HPC_HA02_N, SPI_S0_2, R139.1, J94(11), 252
set_property PACKAGE_PIN C11 [get_ports {gpio_tri_io[26]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[26]}]

#FMC_HPC_HA03_N, SPI_S1_2, R127.1, J94(12), 253
set_property PACKAGE_PIN B12 [get_ports {gpio_tri_io[27]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[27]}]

#FMC_HPC_HA04_P, SPI_S2_2, R116.1, J94(13), 254
set_property PACKAGE_PIN F11 [get_ports {gpio_tri_io[28]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[28]}]

#FMC_HPC_HA05_P, SPI_S3_2, R103.1, J94(14), 255
set_property PACKAGE_PIN F15 [get_ports {gpio_tri_io[29]}]
set_property IOSTANDARD LVCMOS25 [get_ports {gpio_tri_io[29]}]
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
