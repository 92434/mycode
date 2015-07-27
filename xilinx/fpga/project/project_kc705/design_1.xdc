# PCIe Refclk Pins
set_property PACKAGE_PIN U8 [get_ports IBUF_DS_P]
set_property PACKAGE_PIN U7 [get_ports IBUF_DS_N]

# Sys Reset Pins
set_property PACKAGE_PIN AB7 [get_ports reset]
set_property IOSTANDARD LVCMOS15 [get_ports reset]

# LED Pins
set_property PACKAGE_PIN AB8 [get_ports MSI_enable]
set_property IOSTANDARD LVCMOS15 [get_ports MSI_enable]

#mig Sys Clock Pins
#set_property PACKAGE_PIN AD12 [get_ports sys_diff_clock_clk_p]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports sys_diff_clock_clk_p]

#set_property PACKAGE_PIN AD11 [get_ports sys_diff_clock_clk_n]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports sys_diff_clock_clk_n]
