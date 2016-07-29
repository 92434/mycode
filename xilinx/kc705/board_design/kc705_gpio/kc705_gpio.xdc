# ----------------------------------------------------------------------------------------------------
# kc705 default constrain
# ----------------------------------------------------------------------------------------------------

# Sys Clock Pins
set_property PACKAGE_PIN AD11 [get_ports MIG_SYS_CLK_clk_n]
set_property IOSTANDARD DIFF_SSTL15 [get_ports MIG_SYS_CLK_clk_n]

set_property PACKAGE_PIN AD12 [get_ports MIG_SYS_CLK_clk_p]
set_property IOSTANDARD DIFF_SSTL15 [get_ports MIG_SYS_CLK_clk_p]

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
# ----------------------------------------------------------------------------------------------------
# gpio constrain
# ----------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------
# gpio constrain for HPC
# ----------------------------------------------------------------------------------------------------

#FMC_HPC_LA11_P, None, R71.1, 148
set_property PACKAGE_PIN G27 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[0]}]

#FMC_HPC_LA04_P, None, R67.1, 149
set_property PACKAGE_PIN G28 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[1]}]

#FMC_HPC_LA05_P, None, R22.1, 150
set_property PACKAGE_PIN G29 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[2]}]

#FMC_HPC_LA31_N, None, R59.1, 151
set_property PACKAGE_PIN F22 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[3]}]

#FMC_HPC_LA18_CC_P, None, R11.1, 152
set_property PACKAGE_PIN F21 [get_ports {gpio_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[4]}]

#FMC_HPC_LA31_P, None, R58.1, 153
set_property PACKAGE_PIN G22 [get_ports {gpio_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[5]}]

#FMC_HPC_HA01_CC_N, None, R102.1, 154
set_property PACKAGE_PIN G14 [get_ports {gpio_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[6]}]

#FMC_HPC_HA17_CC_P, None, R144.1, 155
set_property PACKAGE_PIN G13 [get_ports {gpio_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[7]}]

#FMC_HPC_HA16_N, None, R110.1, 156
set_property PACKAGE_PIN K15 [get_ports {gpio_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[8]}]

#FMC_HPC_LA30_N, None, R84.1, 157
set_property PACKAGE_PIN C22 [get_ports {gpio_tri_io[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[9]}]

#FMC_HPC_LA22_P, None, R52.1, 158
set_property PACKAGE_PIN C20 [get_ports {gpio_tri_io[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[10]}]

#FMC_HPC_LA32_N, None, R86.1, 159
set_property PACKAGE_PIN C21 [get_ports {gpio_tri_io[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[11]}]

#FMC_HPC_LA01_CC_N, None, R21.1, 160
set_property PACKAGE_PIN C26 [get_ports {gpio_tri_io[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[12]}]

#FMC_HPC_CLK0_M2C_N, None, R64.1, 161
set_property PACKAGE_PIN C27 [get_ports {gpio_tri_io[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[13]}]

#FMC_HPC_LA23_N, None, R31.1, 162
set_property PACKAGE_PIN A22 [get_ports {gpio_tri_io[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[14]}]

#FMC_HPC_LA04_N, None, R68.1, 163
set_property PACKAGE_PIN F28 [get_ports {gpio_tri_io[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[15]}]

#FMC_HPC_LA06_N, None, R6.1, 164
set_property PACKAGE_PIN G30 [get_ports {gpio_tri_io[16]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[16]}]

#FMC_HPC_HA20_P, None, R111.1, 165
set_property PACKAGE_PIN K13 [get_ports {gpio_tri_io[17]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[17]}]

#FMC_HPC_LA12_N, None, R47.1, 166
set_property PACKAGE_PIN B29 [get_ports {gpio_tri_io[18]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[18]}]

#FMC_HPC_LA14_P, None, R9.1, 167
set_property PACKAGE_PIN B28 [get_ports {gpio_tri_io[19]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[19]}]

#FMC_HPC_LA16_P, None, R48.1, 168
set_property PACKAGE_PIN B27 [get_ports {gpio_tri_io[20]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[20]}]

#FMC_HPC_LA00_CC_N, None, R41.1, 169
set_property PACKAGE_PIN B25 [get_ports {gpio_tri_io[21]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[21]}]

#FMC_HPC_LA15_N, None, R74.1, 170
set_property PACKAGE_PIN B24 [get_ports {gpio_tri_io[22]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[22]}]

#FMC_HPC_LA23_P, None, R30.1, 171
set_property PACKAGE_PIN B22 [get_ports {gpio_tri_io[23]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[23]}]

#FMC_HPC_LA22_N, None, R53.1, 172
set_property PACKAGE_PIN B20 [get_ports {gpio_tri_io[24]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[24]}]

#FMC_HPC_LA14_N, None, R10.1, 173
set_property PACKAGE_PIN A28 [get_ports {gpio_tri_io[25]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[25]}]

#FMC_HPC_LA17_CC_P, None, R28.1, 174
set_property PACKAGE_PIN F20 [get_ports {gpio_tri_io[26]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[26]}]

#FMC_HPC_LA11_N, None, R72.1, 175
set_property PACKAGE_PIN F27 [get_ports {gpio_tri_io[27]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[27]}]

#FMC_HPC_PG_M2C_LS, None, R113.1, 176
set_property PACKAGE_PIN J29 [get_ports {gpio_tri_io[28]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[28]}]

#FMC_HPC_HA16_P, None, R109.1, 177
set_property PACKAGE_PIN L15 [get_ports {gpio_tri_io[29]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[29]}]

#FMC_HPC_HA13_P, None, R107.1, 178
set_property PACKAGE_PIN L16 [get_ports {gpio_tri_io[30]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[30]}]

#FMC_HPC_HA22_P, None, R136.1, 179
set_property PACKAGE_PIN L11 [get_ports {gpio_tri_io[31]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_tri_io[31]}]

#FMC_HPC_HA23_P, None, R148.1, 116
set_property PACKAGE_PIN L12 [get_ports {gpio2_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[0]}]

#FMC_HPC_HA23_N, None, R149.1, 117
set_property PACKAGE_PIN L13 [get_ports {gpio2_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[1]}]

#FMC_HPC_HA19_P, None, R124.1, 118
set_property PACKAGE_PIN H11 [get_ports {gpio2_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[2]}]

#FMC_HPC_HA19_N, None, R125.1, 119
set_property PACKAGE_PIN H12 [get_ports {gpio2_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[3]}]

#FMC_HPC_HA15_P, None, R122.1, 120
set_property PACKAGE_PIN H15 [get_ports {gpio2_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[4]}]

#FMC_HPC_HA14_N, None, R133.1, 121
set_property PACKAGE_PIN H16 [get_ports {gpio2_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[5]}]

#FMC_HPC_HA06_P, None, R140.1, 122
set_property PACKAGE_PIN D14 [get_ports {gpio2_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[6]}]

#FMC_HPC_HA02_P, None, R138.1, 123
set_property PACKAGE_PIN D11 [get_ports {gpio2_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[7]}]

#FMC_HPC_HA00_CC_P, None, R114.1, 124
set_property PACKAGE_PIN D12 [get_ports {gpio2_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[8]}]

#FMC_HPC_CLK1_M2C_N, None, R39.1, 125
set_property PACKAGE_PIN D18 [get_ports {gpio2_tri_io[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[9]}]

#FMC_HPC_LA20_N, None, R51.1, 126
set_property PACKAGE_PIN D19 [get_ports {gpio2_tri_io[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[10]}]

#FMC_HPC_LA02_P, None, R65.1, 127
set_property PACKAGE_PIN H24 [get_ports {gpio2_tri_io[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[11]}]

#FMC_HPC_PRSNT_M2C_B_LS, None, R62.1, 128
set_property PACKAGE_PIN M20 [get_ports {gpio2_tri_io[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[12]}]

#FMC_HPC_LA17_CC_N, None, R29.1, 129
set_property PACKAGE_PIN E20 [get_ports {gpio2_tri_io[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[13]}]

#FMC_HPC_LA18_CC_N, None, R12.1, 130
set_property PACKAGE_PIN E21 [get_ports {gpio2_tri_io[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[14]}]

#FMC_HPC_LA07_P, None, R69.1, 131
set_property PACKAGE_PIN E28 [get_ports {gpio2_tri_io[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[15]}]

#FMC_HPC_LA08_P, None, R44.1, 132
set_property PACKAGE_PIN E29 [get_ports {gpio2_tri_io[16]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[16]}]

#FMC_HPC_LA19_P, None, R75.1, 133
set_property PACKAGE_PIN G18 [get_ports {gpio2_tri_io[17]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[17]}]

#FMC_HPC_HA15_N, None, R123.1, 134
set_property PACKAGE_PIN G15 [get_ports {gpio2_tri_io[18]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[18]}]

#FMC_HPC_LA21_P, None, R77.1, 135
set_property PACKAGE_PIN A20 [get_ports {gpio2_tri_io[19]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[19]}]

#FMC_HPC_LA13_P, None, R26.1, 136
set_property PACKAGE_PIN A25 [get_ports {gpio2_tri_io[20]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[20]}]

#FMC_HPC_LA13_N, None, R27.1, 137
set_property PACKAGE_PIN A26 [get_ports {gpio2_tri_io[21]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[21]}]

#FMC_HPC_LA16_N, None, R49.1, 138
set_property PACKAGE_PIN A27 [get_ports {gpio2_tri_io[22]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[22]}]

#FMC_HPC_HA18_P, None, R134.1, 139
set_property PACKAGE_PIN K14 [get_ports {gpio2_tri_io[23]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[23]}]

#FMC_HPC_HA13_N, None, R108.1, 140
set_property PACKAGE_PIN K16 [get_ports {gpio2_tri_io[24]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[24]}]

#FMC_HPC_LA27_P, None, R13.1, 141
set_property PACKAGE_PIN C19 [get_ports {gpio2_tri_io[25]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[25]}]

#FMC_HPC_HA03_P, None, R126.1, 142
set_property PACKAGE_PIN C12 [get_ports {gpio2_tri_io[26]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[26]}]

#FMC_HPC_HA02_N, None, R139.1, 143
set_property PACKAGE_PIN C11 [get_ports {gpio2_tri_io[27]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[27]}]

#FMC_HPC_LA29_P, None, R56.1, 144
set_property PACKAGE_PIN C17 [get_ports {gpio2_tri_io[28]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[28]}]

#FMC_HPC_LA28_N, None, R82.1, 145
set_property PACKAGE_PIN C16 [get_ports {gpio2_tri_io[29]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[29]}]

#FMC_HPC_HA12_P, None, R120.1, 146
set_property PACKAGE_PIN C15 [get_ports {gpio2_tri_io[30]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[30]}]

#FMC_HPC_HA06_N, None, R141.1, 147
set_property PACKAGE_PIN C14 [get_ports {gpio2_tri_io[31]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_tri_io[31]}]

#FMC_HPC_LA15_P, None, R73.1, 84
set_property PACKAGE_PIN C24 [get_ports {gpio_1_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[0]}]

#FMC_HPC_LA00_CC_P, None, R40.1, 85
set_property PACKAGE_PIN C25 [get_ports {gpio_1_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[1]}]

#FMC_HPC_LA06_P, None, R5.1, 86
set_property PACKAGE_PIN H30 [get_ports {gpio_1_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[2]}]

#FMC_HPC_HA07_N, None, R129.1, 87
set_property PACKAGE_PIN A15 [get_ports {gpio_1_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[3]}]

#FMC_HPC_LA12_P, None, R46.1, 88
set_property PACKAGE_PIN C29 [get_ports {gpio_1_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[4]}]

#FMC_HPC_LA33_N, None, R61.1, 89
set_property PACKAGE_PIN H22 [get_ports {gpio_1_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[5]}]

#FMC_HPC_LA05_N, None, R23.1, 90
set_property PACKAGE_PIN F30 [get_ports {gpio_1_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[6]}]

#FMC_HPC_HA20_N, None, R112.1, 91
set_property PACKAGE_PIN J13 [get_ports {gpio_1_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[7]}]

#FMC_HPC_LA28_P, None, R81.1, 92
set_property PACKAGE_PIN D16 [get_ports {gpio_1_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[8]}]

#FMC_HPC_CLK1_M2C_P, None, R38.1, 93
set_property PACKAGE_PIN D17 [get_ports {gpio_1_tri_io[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[9]}]

#FMC_HPC_LA10_N, None, R8.1, 94
set_property PACKAGE_PIN C30 [get_ports {gpio_1_tri_io[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[10]}]

#FMC_HPC_HA00_CC_N, None, R115.1, 95
set_property PACKAGE_PIN D13 [get_ports {gpio_1_tri_io[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[11]}]

#FMC_HPC_LA02_N, None, R66.1, 96
set_property PACKAGE_PIN H25 [get_ports {gpio_1_tri_io[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[12]}]

#FMC_HPC_LA03_P, None, R42.1, 97
set_property PACKAGE_PIN H26 [get_ports {gpio_1_tri_io[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[13]}]

#FMC_HPC_LA33_P, None, R60.1, 98
set_property PACKAGE_PIN H21 [get_ports {gpio_1_tri_io[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[14]}]

#FMC_HPC_LA09_P, None, R24.1, 99
set_property PACKAGE_PIN B30 [get_ports {gpio_1_tri_io[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[15]}]

#FMC_HPC_LA19_N, None, R76.1, 100
set_property PACKAGE_PIN F18 [get_ports {gpio_1_tri_io[16]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[16]}]

#FMC_HPC_HA09_P, None, R105.1, 101
set_property PACKAGE_PIN F12 [get_ports {gpio_1_tri_io[17]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[17]}]

#FMC_HPC_HA17_CC_N, None, R145.1, 102
set_property PACKAGE_PIN F13 [get_ports {gpio_1_tri_io[18]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[18]}]

#FMC_HPC_HA04_P, None, R116.1, 103
set_property PACKAGE_PIN F11 [get_ports {gpio_1_tri_io[19]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[19]}]

#FMC_HPC_LA25_N, None, R55.1, 104
set_property PACKAGE_PIN F17 [get_ports {gpio_1_tri_io[20]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[20]}]

#FMC_HPC_HA05_P, None, R103.1, 105
set_property PACKAGE_PIN F15 [get_ports {gpio_1_tri_io[21]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[21]}]

#FMC_HPC_LA03_N, None, R43.1, 106
set_property PACKAGE_PIN H27 [get_ports {gpio_1_tri_io[22]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[22]}]

#FMC_HPC_LA21_N, None, R78.1, 107
set_property PACKAGE_PIN A21 [get_ports {gpio_1_tri_io[23]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[23]}]

#FMC_HPC_LA29_N, None, R57.1, 108
set_property PACKAGE_PIN B17 [get_ports {gpio_1_tri_io[24]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[24]}]

#FMC_HPC_HA07_P, None, R128.1, 109
set_property PACKAGE_PIN B14 [get_ports {gpio_1_tri_io[25]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[25]}]

#FMC_HPC_HA12_N, None, R121.1, 110
set_property PACKAGE_PIN B15 [get_ports {gpio_1_tri_io[26]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[26]}]

#FMC_HPC_HA03_N, None, R127.1, 111
set_property PACKAGE_PIN B12 [get_ports {gpio_1_tri_io[27]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[27]}]

#FMC_HPC_HA11_P, None, R130.1, 112
set_property PACKAGE_PIN B13 [get_ports {gpio_1_tri_io[28]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[28]}]

#FMC_HPC_LA26_P, None, R32.1, 113
set_property PACKAGE_PIN B18 [get_ports {gpio_1_tri_io[29]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[29]}]

#FMC_HPC_LA27_N, None, R14.1, 114
set_property PACKAGE_PIN B19 [get_ports {gpio_1_tri_io[30]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[30]}]

#FMC_HPC_LA09_N, None, R25.1, 115
set_property PACKAGE_PIN A30 [get_ports {gpio_1_tri_io[31]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_1_tri_io[31]}]

#FMC_HPC_HA14_P, None, R132.1, 58
set_property PACKAGE_PIN J16 [get_ports {gpio2_1_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[0]}]

#FMC_HPC_HA18_N, None, R135.1, 59
set_property PACKAGE_PIN J14 [get_ports {gpio2_1_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[1]}]

#FMC_HPC_HA21_N, None, R147.1, 60
set_property PACKAGE_PIN J12 [get_ports {gpio2_1_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[2]}]

#FMC_HPC_HA21_P, None, R146.1, 61
set_property PACKAGE_PIN J11 [get_ports {gpio2_1_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[3]}]

#FMC_HPC_LA10_P, None, R7.1, 62
set_property PACKAGE_PIN D29 [get_ports {gpio2_1_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[4]}]

#FMC_HPC_LA07_N, None, R70.1, 63
set_property PACKAGE_PIN D28 [get_ports {gpio2_1_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[5]}]

#FMC_HPC_LA32_P, None, R85.1, 64
set_property PACKAGE_PIN D21 [get_ports {gpio2_1_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[6]}]

#FMC_HPC_LA30_P, None, R83.1, 65
set_property PACKAGE_PIN D22 [get_ports {gpio2_1_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[7]}]

#FMC_HPC_CLK0_M2C_P, None, R63.1, 66
set_property PACKAGE_PIN D27 [get_ports {gpio2_1_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[8]}]

#FMC_HPC_LA01_CC_P, None, R20.1, 67
set_property PACKAGE_PIN D26 [get_ports {gpio2_1_tri_io[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[9]}]

#FMC_HPC_LA20_P, None, R50.1, 68
set_property PACKAGE_PIN E19 [get_ports {gpio2_1_tri_io[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[10]}]

#FMC_HPC_HA04_N, None, R117.1, 69
set_property PACKAGE_PIN E11 [get_ports {gpio2_1_tri_io[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[11]}]

#FMC_HPC_HA09_N, None, R106.1, 70
set_property PACKAGE_PIN E13 [get_ports {gpio2_1_tri_io[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[12]}]

#FMC_HPC_HA08_N, None, R119.1, 71
set_property PACKAGE_PIN E15 [get_ports {gpio2_1_tri_io[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[13]}]

#FMC_HPC_LA24_N, None, R80.1, 72
set_property PACKAGE_PIN A17 [get_ports {gpio2_1_tri_io[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[14]}]

#FMC_HPC_LA24_P, None, R79.1, 73
set_property PACKAGE_PIN A16 [get_ports {gpio2_1_tri_io[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[15]}]

#FMC_HPC_HA10_P, None, R142.1, 74
set_property PACKAGE_PIN A11 [get_ports {gpio2_1_tri_io[16]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[16]}]

#FMC_HPC_HA11_N, None, R131.1, 75
set_property PACKAGE_PIN A13 [get_ports {gpio2_1_tri_io[17]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[17]}]

#FMC_HPC_HA10_N, None, R143.1, 76
set_property PACKAGE_PIN A12 [get_ports {gpio2_1_tri_io[18]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[18]}]

#FMC_HPC_HA08_P, None, R118.1, 77
set_property PACKAGE_PIN E14 [get_ports {gpio2_1_tri_io[19]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[19]}]

#FMC_HPC_LA26_N, None, R33.1, 78
set_property PACKAGE_PIN A18 [get_ports {gpio2_1_tri_io[20]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[20]}]

#FMC_HPC_HA22_N, None, R137.1, 79
set_property PACKAGE_PIN K11 [get_ports {gpio2_1_tri_io[21]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[21]}]

#FMC_HPC_LA08_N, None, R45.1, 80
set_property PACKAGE_PIN E30 [get_ports {gpio2_1_tri_io[22]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[22]}]

#FMC_HPC_LA25_P, None, R54.1, 81
set_property PACKAGE_PIN G17 [get_ports {gpio2_1_tri_io[23]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[23]}]

#FMC_HPC_HA05_N, None, R104.1, 82
set_property PACKAGE_PIN E16 [get_ports {gpio2_1_tri_io[24]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[24]}]

#FMC_HPC_HA01_CC_P, None, R101.1, 83
set_property PACKAGE_PIN H14 [get_ports {gpio2_1_tri_io[25]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_1_tri_io[25]}]
# ----------------------------------------------------------------------------------------------------
# gpio constrain for LPC
# ----------------------------------------------------------------------------------------------------

#FMC_LPC_PRSNT_M2C_B_LS, None, R62.1, 148
set_property PACKAGE_PIN J22 [get_ports {gpio_2_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[0]}]

#FMC_LPC_LA24_N, None, R80.1, 149
set_property PACKAGE_PIN AH30 [get_ports {gpio_2_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[1]}]

#FMC_LPC_LA26_P, None, R32.1, 150
set_property PACKAGE_PIN AK29 [get_ports {gpio_2_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[2]}]

#FMC_LPC_LA21_N, None, R78.1, 151
set_property PACKAGE_PIN AG28 [get_ports {gpio_2_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[3]}]

#FMC_LPC_CLK1_M2C_P, None, R38.1, 152
set_property PACKAGE_PIN AG29 [get_ports {gpio_2_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[4]}]

#FMC_LPC_LA21_P, None, R77.1, 153
set_property PACKAGE_PIN AG27 [get_ports {gpio_2_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[5]}]

#FMC_LPC_LA07_P, None, R69.1, 154
set_property PACKAGE_PIN AG25 [get_ports {gpio_2_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[6]}]

#FMC_LPC_LA05_P, None, R22.1, 155
set_property PACKAGE_PIN AG22 [get_ports {gpio_2_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[7]}]

#FMC_LPC_CLK0_M2C_N, None, R64.1, 156
set_property PACKAGE_PIN AG23 [get_ports {gpio_2_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[8]}]

#FMC_LPC_LA03_P, None, R42.1, 157
set_property PACKAGE_PIN AG20 [get_ports {gpio_2_tri_io[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[9]}]

#FMC_LPC_LA28_P, None, R81.1, 158
set_property PACKAGE_PIN AE30 [get_ports {gpio_2_tri_io[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[10]}]

#FMC_LPC_LA00_CC_P, None, R40.1, 159
set_property PACKAGE_PIN AD23 [get_ports {gpio_2_tri_io[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[11]}]

#FMC_LPC_LA16_N, None, R49.1, 160
set_property PACKAGE_PIN AD22 [get_ports {gpio_2_tri_io[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[12]}]

#FMC_LPC_LA25_N, None, R55.1, 161
set_property PACKAGE_PIN AD26 [get_ports {gpio_2_tri_io[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[13]}]

#FMC_LPC_LA31_P, None, R58.1, 162
set_property PACKAGE_PIN AD29 [get_ports {gpio_2_tri_io[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[14]}]

#FMC_LPC_LA18_CC_N, None, R12.1, 163
set_property PACKAGE_PIN AD28 [get_ports {gpio_2_tri_io[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[15]}]

#FMC_LPC_LA30_N, None, R84.1, 164
set_property PACKAGE_PIN AB30 [get_ports {gpio_2_tri_io[16]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[16]}]

#FMC_LPC_LA33_N, None, R61.1, 165
set_property PACKAGE_PIN AC30 [get_ports {gpio_2_tri_io[17]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[17]}]

#FMC_LPC_LA26_N, None, R33.1, 166
set_property PACKAGE_PIN AK30 [get_ports {gpio_2_tri_io[18]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[18]}]

#FMC_LPC_LA01_CC_N, None, R21.1, 167
set_property PACKAGE_PIN AF23 [get_ports {gpio_2_tri_io[19]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[19]}]

#FMC_LPC_LA02_P, None, R65.1, 168
set_property PACKAGE_PIN AF20 [get_ports {gpio_2_tri_io[20]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[20]}]

#FMC_LPC_LA11_N, None, R72.1, 169
set_property PACKAGE_PIN AF25 [get_ports {gpio_2_tri_io[21]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[21]}]

#FMC_LPC_LA29_N, None, R57.1, 170
set_property PACKAGE_PIN AF28 [get_ports {gpio_2_tri_io[22]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[22]}]

#FMC_LPC_LA12_P, None, R46.1, 171
set_property PACKAGE_PIN AA20 [get_ports {gpio_2_tri_io[23]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[23]}]

#FMC_LPC_LA14_P, None, R9.1, 172
set_property PACKAGE_PIN AD21 [get_ports {gpio_2_tri_io[24]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[24]}]

#FMC_LPC_LA15_N, None, R74.1, 173
set_property PACKAGE_PIN AD24 [get_ports {gpio_2_tri_io[25]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[25]}]

#FMC_LPC_LA18_CC_P, None, R11.1, 174
set_property PACKAGE_PIN AD27 [get_ports {gpio_2_tri_io[26]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[26]}]

#FMC_LPC_LA19_P, None, R75.1, 175
set_property PACKAGE_PIN AJ26 [get_ports {gpio_2_tri_io[27]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[27]}]

#FMC_LPC_LA22_N, None, R53.1, 176
set_property PACKAGE_PIN AK28 [get_ports {gpio_2_tri_io[28]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[28]}]

#FMC_LPC_LA09_P, None, R24.1, 177
set_property PACKAGE_PIN AK23 [get_ports {gpio_2_tri_io[29]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[29]}]

#FMC_LPC_LA19_N, None, R76.1, 178
set_property PACKAGE_PIN AK26 [get_ports {gpio_2_tri_io[30]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[30]}]

#FMC_LPC_LA09_N, None, R25.1, 179
set_property PACKAGE_PIN AK24 [get_ports {gpio_2_tri_io[31]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_2_tri_io[31]}]

#FMC_LPC_LA10_N, None, R8.1, 116
set_property PACKAGE_PIN AK25 [get_ports {gpio2_2_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[0]}]

#FMC_LPC_LA00_CC_N, None, R41.1, 117
set_property PACKAGE_PIN AE24 [get_ports {gpio2_2_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[1]}]

#FMC_LPC_LA11_P, None, R71.1, 118
set_property PACKAGE_PIN AE25 [get_ports {gpio2_2_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[2]}]

#FMC_LPC_LA14_N, None, R10.1, 119
set_property PACKAGE_PIN AE21 [get_ports {gpio2_2_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[3]}]

#FMC_LPC_LA01_CC_P, None, R20.1, 120
set_property PACKAGE_PIN AE23 [get_ports {gpio2_2_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[4]}]

#FMC_LPC_LA29_P, None, R56.1, 121
set_property PACKAGE_PIN AE28 [get_ports {gpio2_2_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[5]}]

#FMC_LPC_LA31_N, None, R59.1, 122
set_property PACKAGE_PIN AE29 [get_ports {gpio2_2_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[6]}]

#FMC_LPC_CLK1_M2C_N, None, R39.1, 123
set_property PACKAGE_PIN AH29 [get_ports {gpio2_2_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[7]}]

#FMC_LPC_LA07_N, None, R70.1, 124
set_property PACKAGE_PIN AH25 [get_ports {gpio2_2_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[8]}]

#FMC_LPC_LA23_N, None, R31.1, 125
set_property PACKAGE_PIN AH27 [get_ports {gpio2_2_tri_io[9]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[9]}]

#FMC_LPC_LA23_P, None, R30.1, 126
set_property PACKAGE_PIN AH26 [get_ports {gpio2_2_tri_io[10]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[10]}]

#FMC_LPC_LA04_P, None, R67.1, 127
set_property PACKAGE_PIN AH21 [get_ports {gpio2_2_tri_io[11]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[11]}]

#FMC_LPC_LA03_N, None, R43.1, 128
set_property PACKAGE_PIN AH20 [get_ports {gpio2_2_tri_io[12]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[12]}]

#FMC_LPC_LA05_N, None, R23.1, 129
set_property PACKAGE_PIN AH22 [get_ports {gpio2_2_tri_io[13]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[13]}]

#FMC_LPC_LA16_P, None, R48.1, 130
set_property PACKAGE_PIN AC22 [get_ports {gpio2_2_tri_io[14]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[14]}]

#FMC_LPC_LA25_P, None, R54.1, 131
set_property PACKAGE_PIN AC26 [get_ports {gpio2_2_tri_io[15]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[15]}]

#FMC_LPC_LA17_CC_N, None, R29.1, 132
set_property PACKAGE_PIN AC27 [get_ports {gpio2_2_tri_io[16]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[16]}]

#FMC_LPC_LA15_P, None, R73.1, 133
set_property PACKAGE_PIN AC24 [get_ports {gpio2_2_tri_io[17]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[17]}]

#FMC_LPC_LA13_N, None, R27.1, 134
set_property PACKAGE_PIN AC25 [get_ports {gpio2_2_tri_io[18]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[18]}]

#FMC_LPC_LA33_P, None, R60.1, 135
set_property PACKAGE_PIN AC29 [get_ports {gpio2_2_tri_io[19]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[19]}]

#FMC_LPC_LA28_N, None, R82.1, 136
set_property PACKAGE_PIN AF30 [get_ports {gpio2_2_tri_io[20]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[20]}]

#FMC_LPC_LA27_N, None, R14.1, 137
set_property PACKAGE_PIN AJ29 [get_ports {gpio2_2_tri_io[21]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[21]}]

#FMC_LPC_LA27_P, None, R13.1, 138
set_property PACKAGE_PIN AJ28 [get_ports {gpio2_2_tri_io[22]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[22]}]

#FMC_LPC_LA22_P, None, R52.1, 139
set_property PACKAGE_PIN AJ27 [get_ports {gpio2_2_tri_io[23]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[23]}]

#FMC_LPC_LA24_P, None, R79.1, 140
set_property PACKAGE_PIN AG30 [get_ports {gpio2_2_tri_io[24]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[24]}]

#FMC_LPC_LA10_P, None, R7.1, 141
set_property PACKAGE_PIN AJ24 [get_ports {gpio2_2_tri_io[25]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[25]}]

#FMC_LPC_LA08_N, None, R45.1, 142
set_property PACKAGE_PIN AJ23 [get_ports {gpio2_2_tri_io[26]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[26]}]

#FMC_LPC_LA08_P, None, R44.1, 143
set_property PACKAGE_PIN AJ22 [get_ports {gpio2_2_tri_io[27]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[27]}]

#FMC_LPC_LA04_N, None, R68.1, 144
set_property PACKAGE_PIN AJ21 [get_ports {gpio2_2_tri_io[28]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[28]}]

#FMC_LPC_LA32_P, None, R85.1, 145
set_property PACKAGE_PIN Y30 [get_ports {gpio2_2_tri_io[29]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[29]}]

#FMC_LPC_LA32_N, None, R86.1, 146
set_property PACKAGE_PIN AA30 [get_ports {gpio2_2_tri_io[30]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[30]}]

#FMC_LPC_LA06_P, None, R5.1, 147
set_property PACKAGE_PIN AK20 [get_ports {gpio2_2_tri_io[31]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio2_2_tri_io[31]}]

#FMC_LPC_LA06_N, None, R6.1, 107
set_property PACKAGE_PIN AK21 [get_ports {gpio_3_tri_io[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[0]}]

#FMC_LPC_LA17_CC_P, None, R28.1, 108
set_property PACKAGE_PIN AB27 [get_ports {gpio_3_tri_io[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[1]}]

#FMC_LPC_LA13_P, None, R26.1, 109
set_property PACKAGE_PIN AB24 [get_ports {gpio_3_tri_io[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[2]}]

#FMC_LPC_LA12_N, None, R47.1, 110
set_property PACKAGE_PIN AB20 [get_ports {gpio_3_tri_io[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[3]}]

#FMC_LPC_LA30_P, None, R83.1, 111
set_property PACKAGE_PIN AB29 [get_ports {gpio_3_tri_io[4]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[4]}]

#FMC_LPC_CLK0_M2C_P, None, R63.1, 112
set_property PACKAGE_PIN AF22 [get_ports {gpio_3_tri_io[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[5]}]

#FMC_LPC_LA02_N, None, R66.1, 113
set_property PACKAGE_PIN AF21 [get_ports {gpio_3_tri_io[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[6]}]

#FMC_LPC_LA20_N, None, R51.1, 114
set_property PACKAGE_PIN AF27 [get_ports {gpio_3_tri_io[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[7]}]

#FMC_LPC_LA20_P, None, R50.1, 115
set_property PACKAGE_PIN AF26 [get_ports {gpio_3_tri_io[8]}]
set_property IOSTANDARD LVCMOS15 [get_ports {gpio_3_tri_io[8]}]
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
