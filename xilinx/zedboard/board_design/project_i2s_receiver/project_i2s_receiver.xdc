set_property PACKAGE_PIN U14 [get_ports output_ready]
set_property IOSTANDARD LVCMOS18 [get_ports output_ready]
set_property PACKAGE_PIN U19 [get_ports buffer_full_error]
set_property IOSTANDARD LVCMOS18 [get_ports buffer_full_error]

set_property PACKAGE_PIN AB6 [get_ports i2s_sender_bclk]
set_property IOSTANDARD LVCMOS18 [get_ports i2s_sender_bclk]
#set_property PACKAGE_PIN AB7 [get_ports i2s_receiver_bclk]
set_property PACKAGE_PIN AB11 [get_ports i2s_receiver_bclk]
set_property IOSTANDARD LVCMOS18 [get_ports i2s_receiver_bclk]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets i2s_receiver_bclk]
set_property PACKAGE_PIN AA4 [get_ports i2s_sender_lrclk]
set_property IOSTANDARD LVCMOS18 [get_ports i2s_sender_lrclk]
#set_property PACKAGE_PIN Y4 [get_ports i2s_receiver_lrclk]
set_property PACKAGE_PIN AB10 [get_ports i2s_receiver_lrclk]
set_property IOSTANDARD LVCMOS18 [get_ports i2s_receiver_lrclk]
set_property PACKAGE_PIN T6 [get_ports i2s_sender_sdata]
set_property IOSTANDARD LVCMOS18 [get_ports i2s_sender_sdata]
#set_property PACKAGE_PIN R6 [get_ports i2s_receiver_sdata]
set_property PACKAGE_PIN AB9 [get_ports i2s_receiver_sdata]
set_property IOSTANDARD LVCMOS18 [get_ports i2s_receiver_sdata]

set_property PACKAGE_PIN Y11 [get_ports {gpio_tri_io[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {gpio_tri_io[0]}]
set_property PACKAGE_PIN AA11 [get_ports {gpio_tri_io[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {gpio_tri_io[1]}]
set_property PACKAGE_PIN Y10 [get_ports {gpio_tri_io[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {gpio_tri_io[2]}]
set_property PACKAGE_PIN AA9 [get_ports {gpio_tri_io[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {gpio_tri_io[3]}]