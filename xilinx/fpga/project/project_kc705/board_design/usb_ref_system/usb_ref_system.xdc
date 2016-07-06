# (c) Copyright 2012 Xilinx, Inc. All rights reserved.
#
# This file contains confidential and proprietary information
# of Xilinx, Inc. and is protected under U.S. and 
# international copyright and other intellectual property
# laws.
#
# DISCLAIMER
# This disclaimer is not a license and does not grant any
# rights to the materials distributed herewith. Except as
# otherwise provided in a valid license issued to you by
# Xilinx, and to the maximum extent permitted by applicable
# law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
# WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
# AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
# BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
# INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
# (2) Xilinx shall not be liable (whether in contract or tort,
# including negligence, or under any other theory of
# liability) for any loss or damage of any kind or nature
# related to, arising under or in connection with these
# materials, including for any direct, or any indirect,
# special, incidental, or consequential loss or damage
# (including loss of data, profits, goodwill, or any type of
# loss or damage suffered as a result of any action brought
# by a third party) even if such damage or loss was
# reasonably foreseeable or Xilinx had been advised of the
# possibility of the same.
#
# CRITICAL APPLICATIONS
# Xilinx products are not designed or intended to be fail-
# safe, or for use in any application requiring fail-safe
# performance, such as life-support or safety devices or
# systems, Class III medical devices, nuclear facilities,
# applications related to the deployment of airbags, or any
# other applications that could lead to death, personal
# injury, or severe property or environmental damage
# (individually and collectively, "Critical
# Applications"). Customer assumes the sole risk and
# liability of any use of Xilinx products in Critical
# Applications, subject only to applicable laws and
# regulations governing limitations on product liability.
#
# THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
# PART OF THIS FILE AT ALL TIMES.
#--------------------------------------------------------------------------
#
# KC705 XDC constraints for IPI Example Design
#
################# USB Constraints ####################

set_property LOC AE28 [ get_ports ULPI_next]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_next]

set_property LOC AB29 [ get_ports ULPI_stop]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_stop]

create_clock -period "16.667" -name ULPI_clk [get_ports "ULPI_clk"]

set_property LOC AD27 [ get_ports ULPI_clk]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_clk]

set_property LOC AF28 [ get_ports ULPI_dir]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_dir]

set_property LOC AB30 [ get_ports ULPI_rst]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_rst]

set_property LOC AC26 [ get_ports ULPI_data_io[7]]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_data_io[7]]

set_property LOC AD26 [ get_ports ULPI_data_io[6]]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_data_io[6]]

set_property LOC AK29 [ get_ports ULPI_data_io[5]]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_data_io[5]]

set_property LOC AK30 [ get_ports ULPI_data_io[4]]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_data_io[4]]

set_property LOC AJ28 [ get_ports ULPI_data_io[3]]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_data_io[3]]

set_property LOC AJ29 [ get_ports ULPI_data_io[2]]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_data_io[2]]

set_property LOC AE30 [ get_ports ULPI_data_io[1]]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_data_io[1]]

set_property LOC AF30 [ get_ports ULPI_data_io[0]]
set_property IOSTANDARD LVCMOS25 [ get_ports ULPI_data_io[0]]

set ulpi_input {ULPI_data* ULPI_dir ULPI_next}
set ulpi_output {ULPI_data* ULPI_stop}
set_input_delay -max 6.7 -clock [get_clocks -of_objects [get_pins -hier *ULPI_clk]] $ulpi_input
set_output_delay -max 7 -clock [get_clocks -of_objects [get_pins -hier *ULPI_clk]] $ulpi_output
set_max_delay 24 -from [get_ports ULPI_dir] -to [get_ports ULPI_data_io[*]] -datapath_only