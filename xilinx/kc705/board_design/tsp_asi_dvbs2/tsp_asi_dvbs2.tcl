
################################################################
# This is a generated script based on design: design_1
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2014.3
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   puts "ERROR: This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source design_1_script.tcl

# If you do not already have a project created,
# you can create a project using the following command:
#    create_project project_1 myproj -part xc7k325tffg900-2
#    set_property BOARD_PART xilinx.com:kc705:part0:1.1 [current_project]


# CHANGE DESIGN NAME HERE
set design_name design_1

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# CHECKING IF PROJECT EXISTS
if { [get_projects -quiet] eq "" } {
   puts "ERROR: Please open or create a project!"
   return 1
}


# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "ERROR: Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      puts "INFO: Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   puts "INFO: Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   puts "INFO: Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   puts "INFO: Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

puts "INFO: Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   puts $errMsg
   return $nRet
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set GPIO [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 GPIO ]
  set GPIO2 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 GPIO2 ]
  set GPIO2_1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 GPIO2_1 ]
  set GPIO_1 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 GPIO_1 ]
  set GPIO_2 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 GPIO_2 ]

  # Create ports
  set EXT_LEDS [ create_bd_port -dir O -from 7 -to 0 EXT_LEDS ]
  set EXT_PCIE_REFCLK_N [ create_bd_port -dir I EXT_PCIE_REFCLK_N ]
  set EXT_PCIE_REFCLK_P [ create_bd_port -dir I EXT_PCIE_REFCLK_P ]
  set asi_out_n [ create_bd_port -dir O asi_out_n ]
  set asi_out_p [ create_bd_port -dir O asi_out_p ]
  set clk_out2 [ create_bd_port -dir O -type clk clk_out2 ]
  set clk_out3 [ create_bd_port -dir O -type clk clk_out3 ]
  set clk_out4 [ create_bd_port -dir O -type clk clk_out4 ]
  set fs_0p5_en [ create_bd_port -dir I fs_0p5_en ]
  set lcm_data [ create_bd_port -dir O -from 7 -to 0 lcm_data ]
  set lcm_din [ create_bd_port -dir O lcm_din ]
  set lcm_lp [ create_bd_port -dir O lcm_lp ]
  set lcm_xscl [ create_bd_port -dir O lcm_xscl ]
  set mpeg_clk [ create_bd_port -dir I mpeg_clk ]
  set mpeg_data [ create_bd_port -dir I -from 7 -to 0 mpeg_data ]
  set mpeg_sync [ create_bd_port -dir I mpeg_sync ]
  set mpeg_valid [ create_bd_port -dir I mpeg_valid ]
  set reset [ create_bd_port -dir I -type rst reset ]
  set_property -dict [ list CONFIG.POLARITY {ACTIVE_HIGH}  ] $reset
  set symbol_2x_im_out [ create_bd_port -dir O -from 15 -to 0 symbol_2x_im_out ]
  set symbol_2x_oe [ create_bd_port -dir O symbol_2x_oe ]
  set symbol_2x_re_out [ create_bd_port -dir O -from 15 -to 0 symbol_2x_re_out ]

  # Create instance: asi_dump_0, and set properties
  set asi_dump_0 [ create_bd_cell -type ip -vlnv xiaofei:user:asi_dump:1.0 asi_dump_0 ]
  set_property -dict [ list CONFIG.C_M00_AXIS_START_COUNT {1}  ] $asi_dump_0

  # Create instance: axi4_tsp_0, and set properties
  set axi4_tsp_0 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_0 ]

  # Create instance: axi_dma_0, and set properties
  set axi_dma_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.1 axi_dma_0 ]
  set_property -dict [ list CONFIG.c_include_sg {0}  ] $axi_dma_0

  # Create instance: axi_dma_1, and set properties
  set axi_dma_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.1 axi_dma_1 ]
  set_property -dict [ list CONFIG.c_include_sg {0}  ] $axi_dma_1

  # Create instance: axi_gpio_0, and set properties
  set axi_gpio_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_0 ]
  set_property -dict [ list CONFIG.C_GPIO2_WIDTH {32} CONFIG.C_GPIO_WIDTH {32} CONFIG.C_INTERRUPT_PRESENT {0} CONFIG.C_IS_DUAL {1}  ] $axi_gpio_0

  # Create instance: axi_gpio_1, and set properties
  set axi_gpio_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_1 ]
  set_property -dict [ list CONFIG.C_ALL_INPUTS {0} CONFIG.C_GPIO2_WIDTH {27} CONFIG.C_IS_DUAL {1}  ] $axi_gpio_1

  # Create instance: axi_gpio_2, and set properties
  set axi_gpio_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_2 ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {23} CONFIG.C_IS_DUAL {0}  ] $axi_gpio_2

  # Create instance: axi_interconnect_0, and set properties
  set axi_interconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_interconnect_0 ]
  set_property -dict [ list CONFIG.NUM_MI {10} CONFIG.NUM_SI {5}  ] $axi_interconnect_0

  # Create instance: axi_pcie_0, and set properties
  set axi_pcie_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_pcie:2.5 axi_pcie_0 ]
  set_property -dict [ list CONFIG.AXIBAR2PCIEBAR_0 {0x00000000} CONFIG.AXIBAR_AS_0 {true} CONFIG.AXIBAR_AS_1 {true} CONFIG.AXIBAR_NUM {2} CONFIG.BAR0_ENABLED {true} CONFIG.BAR0_SCALE {Kilobytes} CONFIG.BAR0_SIZE {64} CONFIG.BAR0_TYPE {Memory} CONFIG.BAR1_ENABLED {false} CONFIG.BAR_64BIT {true} CONFIG.DEVICE_ID {0x7012} CONFIG.MAX_LINK_SPEED {2.5_GT/s} CONFIG.M_AXI_DATA_WIDTH {64} CONFIG.NO_OF_LANES {X4} CONFIG.PCIEBAR2AXIBAR_0 {0x81000000} CONFIG.PCIE_CAP_SLOT_IMPLEMENTED {true} CONFIG.S_AXI_DATA_WIDTH {64} CONFIG.XLNX_REF_BOARD {None}  ] $axi_pcie_0

  # Create instance: clk_wiz_0, and set properties
  set clk_wiz_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:5.1 clk_wiz_0 ]
  set_property -dict [ list CONFIG.CLKOUT1_JITTER {117.101} CONFIG.CLKOUT1_PHASE_ERROR {142.582} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {135} CONFIG.CLKOUT2_JITTER {118.930} CONFIG.CLKOUT2_PHASE_ERROR {142.582} CONFIG.CLKOUT2_REQUESTED_OUT_FREQ {122.88} CONFIG.CLKOUT2_USED {true} CONFIG.CLKOUT3_JITTER {118.930} CONFIG.CLKOUT3_PHASE_ERROR {142.582} CONFIG.CLKOUT3_REQUESTED_OUT_FREQ {122.88} CONFIG.CLKOUT3_REQUESTED_PHASE {180.000} CONFIG.CLKOUT3_USED {true} CONFIG.CLKOUT4_JITTER {193.719} CONFIG.CLKOUT4_PHASE_ERROR {142.582} CONFIG.CLKOUT4_REQUESTED_OUT_FREQ {10.000} CONFIG.CLKOUT4_USED {true} CONFIG.NUM_OUT_CLKS {4} CONFIG.PRIMITIVE {MMCM}  ] $clk_wiz_0

  # Create instance: intr_hub_0, and set properties
  set intr_hub_0 [ create_bd_cell -type ip -vlnv xiaofei:user:intr_hub:1.0 intr_hub_0 ]
  set_property -dict [ list CONFIG.INT_NUM {4}  ] $intr_hub_0

  # Create instance: kc705_dvb_s2_0, and set properties
  set kc705_dvb_s2_0 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_dvb_s2:1.0 kc705_dvb_s2_0 ]
  set_property -dict [ list CONFIG.C_S00_AXI_ADDR_WIDTH {12}  ] $kc705_dvb_s2_0

  # Create instance: kc705_i2s_receiver_0, and set properties
  set kc705_i2s_receiver_0 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_i2s_receiver:1.0 kc705_i2s_receiver_0 ]
  set_property -dict [ list CONFIG.C_M00_AXIS_START_COUNT {1} CONFIG.I2S_RECEIVER_NUM {32}  ] $kc705_i2s_receiver_0

  # Create instance: kc705_pcie_ext_0, and set properties
  set kc705_pcie_ext_0 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_pcie_ext:1.0 kc705_pcie_ext_0 ]

  # Create instance: kc705_ts2asi_0, and set properties
  set kc705_ts2asi_0 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_0 ]

  # Create instance: lcm_controler_eg9013f_nz_0, and set properties
  set lcm_controler_eg9013f_nz_0 [ create_bd_cell -type ip -vlnv xiaofei:user:lcm_controler_eg9013f_nz:1.0 lcm_controler_eg9013f_nz_0 ]

  # Create instance: myip_i2s_sender_0, and set properties
  set myip_i2s_sender_0 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_i2s_sender:1.0 myip_i2s_sender_0 ]
  set_property -dict [ list CONFIG.I2S_RECEIVER_NUM {32}  ] $myip_i2s_sender_0

  # Create instance: proc_sys_reset_0, and set properties
  set proc_sys_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset_0 ]

  # Create instance: xlconcat_0, and set properties
  set xlconcat_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_0 ]
  set_property -dict [ list CONFIG.NUM_PORTS {4}  ] $xlconcat_0

  # Create instance: xlconstant_0, and set properties
  set xlconstant_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_0 ]
  set_property -dict [ list CONFIG.CONST_VAL {0} CONFIG.CONST_WIDTH {5}  ] $xlconstant_0

  # Create instance: xlconstant_1, and set properties
  set xlconstant_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_1 ]

  # Create interface connections
  connect_bd_intf_net -intf_net S00_AXI_1 [get_bd_intf_pins axi_interconnect_0/S00_AXI] [get_bd_intf_pins axi_pcie_0/M_AXI]
  connect_bd_intf_net -intf_net asi_dump_0_m00_axis [get_bd_intf_pins asi_dump_0/m00_axis] [get_bd_intf_pins axi_dma_1/S_AXIS_S2MM]
  connect_bd_intf_net -intf_net axi_dma_0_M_AXI_MM2S [get_bd_intf_pins axi_dma_0/M_AXI_MM2S] [get_bd_intf_pins axi_interconnect_0/S01_AXI]
  connect_bd_intf_net -intf_net axi_dma_0_M_AXI_S2MM [get_bd_intf_pins axi_dma_0/M_AXI_S2MM] [get_bd_intf_pins axi_interconnect_0/S02_AXI]
  connect_bd_intf_net -intf_net axi_dma_1_M_AXI_MM2S [get_bd_intf_pins axi_dma_1/M_AXI_MM2S] [get_bd_intf_pins axi_interconnect_0/S03_AXI]
  connect_bd_intf_net -intf_net axi_dma_1_M_AXI_S2MM [get_bd_intf_pins axi_dma_1/M_AXI_S2MM] [get_bd_intf_pins axi_interconnect_0/S04_AXI]
  connect_bd_intf_net -intf_net axi_gpio_0_GPIO [get_bd_intf_ports GPIO] [get_bd_intf_pins axi_gpio_0/GPIO]
  connect_bd_intf_net -intf_net axi_gpio_0_GPIO2 [get_bd_intf_ports GPIO2] [get_bd_intf_pins axi_gpio_0/GPIO2]
  connect_bd_intf_net -intf_net axi_gpio_1_GPIO [get_bd_intf_ports GPIO_1] [get_bd_intf_pins axi_gpio_1/GPIO]
  connect_bd_intf_net -intf_net axi_gpio_1_GPIO2 [get_bd_intf_ports GPIO2_1] [get_bd_intf_pins axi_gpio_1/GPIO2]
  connect_bd_intf_net -intf_net axi_gpio_2_GPIO [get_bd_intf_ports GPIO_2] [get_bd_intf_pins axi_gpio_2/GPIO]
  connect_bd_intf_net -intf_net axi_interconnect_0_M00_AXI [get_bd_intf_pins axi_interconnect_0/M00_AXI] [get_bd_intf_pins axi_pcie_0/S_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M01_AXI [get_bd_intf_pins axi_interconnect_0/M01_AXI] [get_bd_intf_pins axi_pcie_0/S_AXI_CTL]
  connect_bd_intf_net -intf_net axi_interconnect_0_M02_AXI [get_bd_intf_pins axi_dma_0/S_AXI_LITE] [get_bd_intf_pins axi_interconnect_0/M02_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M03_AXI [get_bd_intf_pins axi4_tsp_0/s00_axi] [get_bd_intf_pins axi_interconnect_0/M03_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M04_AXI [get_bd_intf_pins axi_gpio_0/S_AXI] [get_bd_intf_pins axi_interconnect_0/M04_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M05_AXI [get_bd_intf_pins axi_gpio_1/S_AXI] [get_bd_intf_pins axi_interconnect_0/M05_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M06_AXI [get_bd_intf_pins axi_gpio_2/S_AXI] [get_bd_intf_pins axi_interconnect_0/M06_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M07_AXI [get_bd_intf_pins axi_interconnect_0/M07_AXI] [get_bd_intf_pins kc705_dvb_s2_0/s00_axi]
  connect_bd_intf_net -intf_net axi_interconnect_0_M08_AXI [get_bd_intf_pins axi_dma_1/S_AXI_LITE] [get_bd_intf_pins axi_interconnect_0/M08_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M09_AXI [get_bd_intf_pins axi_interconnect_0/M09_AXI] [get_bd_intf_pins lcm_controler_eg9013f_nz_0/s00_axi]
  connect_bd_intf_net -intf_net kc705_i2s_receiver_0_m00_axis [get_bd_intf_pins axi_dma_0/S_AXIS_S2MM] [get_bd_intf_pins kc705_i2s_receiver_0/m00_axis]

  # Create port connections
  connect_bd_net -net ARESETN_1 [get_bd_pins axi_interconnect_0/ARESETN] [get_bd_pins axi_interconnect_0/M00_ARESETN] [get_bd_pins axi_interconnect_0/M01_ARESETN] [get_bd_pins axi_interconnect_0/M02_ARESETN] [get_bd_pins axi_interconnect_0/M03_ARESETN] [get_bd_pins axi_interconnect_0/M04_ARESETN] [get_bd_pins axi_interconnect_0/M05_ARESETN] [get_bd_pins axi_interconnect_0/M06_ARESETN] [get_bd_pins axi_interconnect_0/M07_ARESETN] [get_bd_pins axi_interconnect_0/M08_ARESETN] [get_bd_pins axi_interconnect_0/M09_ARESETN] [get_bd_pins axi_interconnect_0/S00_ARESETN] [get_bd_pins axi_interconnect_0/S01_ARESETN] [get_bd_pins axi_interconnect_0/S02_ARESETN] [get_bd_pins axi_interconnect_0/S03_ARESETN] [get_bd_pins axi_interconnect_0/S04_ARESETN] [get_bd_pins proc_sys_reset_0/interconnect_aresetn]
  connect_bd_net -net EXT_PCIE_REFCLK_N_1 [get_bd_ports EXT_PCIE_REFCLK_N] [get_bd_pins kc705_pcie_ext_0/EXT_PCIE_REFCLK_N]
  connect_bd_net -net EXT_PCIE_REFCLK_P_1 [get_bd_ports EXT_PCIE_REFCLK_P] [get_bd_pins kc705_pcie_ext_0/EXT_PCIE_REFCLK_P]
  connect_bd_net -net axi4_tsp_0_ts_out [get_bd_pins axi4_tsp_0/ts_out] [get_bd_pins kc705_ts2asi_0/din_8b]
  connect_bd_net -net axi4_tsp_0_ts_out_clk [get_bd_pins axi4_tsp_0/ts_out_clk] [get_bd_pins kc705_ts2asi_0/din_clk]
  connect_bd_net -net axi4_tsp_0_ts_out_sync [get_bd_pins axi4_tsp_0/ts_out_sync] [get_bd_pins kc705_ts2asi_0/sync]
  connect_bd_net -net axi4_tsp_0_ts_out_valid [get_bd_pins axi4_tsp_0/ts_out_valid] [get_bd_pins kc705_ts2asi_0/valid]
  connect_bd_net -net axi_dma_0_mm2s_introut [get_bd_pins axi_dma_0/mm2s_introut] [get_bd_pins xlconcat_0/In0]
  connect_bd_net -net axi_dma_0_s2mm_introut [get_bd_pins axi_dma_0/s2mm_introut] [get_bd_pins xlconcat_0/In1]
  connect_bd_net -net axi_dma_1_mm2s_introut [get_bd_pins axi_dma_1/mm2s_introut] [get_bd_pins xlconcat_0/In2]
  connect_bd_net -net axi_dma_1_s2mm_introut [get_bd_pins axi_dma_1/s2mm_introut] [get_bd_pins xlconcat_0/In3]
  connect_bd_net -net axi_pcie_0_axi_aclk_out [get_bd_pins asi_dump_0/m00_axis_aclk] [get_bd_pins axi4_tsp_0/s00_axi_aclk] [get_bd_pins axi_dma_0/m_axi_mm2s_aclk] [get_bd_pins axi_dma_0/m_axi_s2mm_aclk] [get_bd_pins axi_dma_0/s_axi_lite_aclk] [get_bd_pins axi_dma_1/m_axi_mm2s_aclk] [get_bd_pins axi_dma_1/m_axi_s2mm_aclk] [get_bd_pins axi_dma_1/s_axi_lite_aclk] [get_bd_pins axi_gpio_0/s_axi_aclk] [get_bd_pins axi_gpio_1/s_axi_aclk] [get_bd_pins axi_gpio_2/s_axi_aclk] [get_bd_pins axi_interconnect_0/ACLK] [get_bd_pins axi_interconnect_0/M00_ACLK] [get_bd_pins axi_interconnect_0/M02_ACLK] [get_bd_pins axi_interconnect_0/M03_ACLK] [get_bd_pins axi_interconnect_0/M04_ACLK] [get_bd_pins axi_interconnect_0/M05_ACLK] [get_bd_pins axi_interconnect_0/M06_ACLK] [get_bd_pins axi_interconnect_0/M07_ACLK] [get_bd_pins axi_interconnect_0/M08_ACLK] [get_bd_pins axi_interconnect_0/M09_ACLK] [get_bd_pins axi_interconnect_0/S00_ACLK] [get_bd_pins axi_interconnect_0/S01_ACLK] [get_bd_pins axi_interconnect_0/S02_ACLK] [get_bd_pins axi_interconnect_0/S03_ACLK] [get_bd_pins axi_interconnect_0/S04_ACLK] [get_bd_pins axi_pcie_0/axi_aclk_out] [get_bd_pins intr_hub_0/clk] [get_bd_pins kc705_dvb_s2_0/s00_axi_aclk] [get_bd_pins kc705_dvb_s2_0/sys_clk] [get_bd_pins kc705_i2s_receiver_0/m00_axis_aclk] [get_bd_pins kc705_pcie_ext_0/pcie_clk_125MHz] [get_bd_pins lcm_controler_eg9013f_nz_0/s00_axi_aclk] [get_bd_pins myip_i2s_sender_0/clk]
  connect_bd_net -net axi_pcie_0_axi_ctl_aclk_out [get_bd_pins axi_interconnect_0/M01_ACLK] [get_bd_pins axi_pcie_0/axi_ctl_aclk_out]
  connect_bd_net -net axi_pcie_0_mmcm_lock [get_bd_pins axi_pcie_0/mmcm_lock] [get_bd_pins kc705_pcie_ext_0/pcie_mmcm_locked]
  connect_bd_net -net clk_wiz_0_clk_out1 [get_bd_pins asi_dump_0/clk] [get_bd_pins clk_wiz_0/clk_out1] [get_bd_pins kc705_ts2asi_0/clk]
  connect_bd_net -net clk_wiz_0_clk_out2 [get_bd_ports clk_out2] [get_bd_pins clk_wiz_0/clk_out2]
  connect_bd_net -net clk_wiz_0_clk_out3 [get_bd_ports clk_out3] [get_bd_pins clk_wiz_0/clk_out3]
  connect_bd_net -net clk_wiz_0_clk_out4 [get_bd_ports clk_out4] [get_bd_pins clk_wiz_0/clk_out4]
  connect_bd_net -net fs_0p5_en_1 [get_bd_ports fs_0p5_en] [get_bd_pins kc705_dvb_s2_0/fs_0p5_en]
  connect_bd_net -net intr_hub_0_int_o [get_bd_pins axi_pcie_0/INTX_MSI_Request] [get_bd_pins intr_hub_0/int_o]
  connect_bd_net -net kc705_dvb_s2_0_symbol_2x_im_out [get_bd_ports symbol_2x_im_out] [get_bd_pins kc705_dvb_s2_0/symbol_2x_im_out]
  connect_bd_net -net kc705_dvb_s2_0_symbol_2x_oe [get_bd_ports symbol_2x_oe] [get_bd_pins kc705_dvb_s2_0/symbol_2x_oe]
  connect_bd_net -net kc705_dvb_s2_0_symbol_2x_re_out [get_bd_ports symbol_2x_re_out] [get_bd_pins kc705_dvb_s2_0/symbol_2x_re_out]
  connect_bd_net -net kc705_pcie_ext_0_EXT_LEDS [get_bd_ports EXT_LEDS] [get_bd_pins kc705_pcie_ext_0/EXT_LEDS]
  connect_bd_net -net kc705_pcie_ext_0_mmcms_locked [get_bd_pins kc705_pcie_ext_0/mmcms_locked] [get_bd_pins proc_sys_reset_0/dcm_locked]
  connect_bd_net -net kc705_pcie_ext_0_pcie_refclk_100MHz [get_bd_pins axi_pcie_0/REFCLK] [get_bd_pins clk_wiz_0/clk_in1] [get_bd_pins kc705_pcie_ext_0/pcie_refclk_100MHz] [get_bd_pins proc_sys_reset_0/slowest_sync_clk]
  connect_bd_net -net kc705_ts2asi_0_asi_out_n [get_bd_ports asi_out_n] [get_bd_pins kc705_ts2asi_0/asi_out_n]
  connect_bd_net -net kc705_ts2asi_0_asi_out_p [get_bd_ports asi_out_p] [get_bd_pins kc705_ts2asi_0/asi_out_p]
  connect_bd_net -net kc705_ts2asi_0_ce_R0 [get_bd_pins asi_dump_0/ce] [get_bd_pins kc705_ts2asi_0/ce_R0]
  connect_bd_net -net kc705_ts2asi_0_sout_data [get_bd_pins asi_dump_0/ts_data] [get_bd_pins kc705_ts2asi_0/sout_data]
  connect_bd_net -net kc705_ts2asi_0_ts_clk [get_bd_pins kc705_dvb_s2_0/ts_clk_h264out] [get_bd_pins kc705_ts2asi_0/ts_clk]
  connect_bd_net -net kc705_ts2asi_0_ts_data [get_bd_pins kc705_dvb_s2_0/ts_din_h264out] [get_bd_pins kc705_ts2asi_0/ts_data]
  connect_bd_net -net kc705_ts2asi_0_ts_sync [get_bd_pins kc705_dvb_s2_0/ts_syn_h264out] [get_bd_pins kc705_ts2asi_0/ts_sync]
  connect_bd_net -net kc705_ts2asi_0_ts_valid [get_bd_pins kc705_dvb_s2_0/ts_valid_h264out] [get_bd_pins kc705_ts2asi_0/ts_valid]
  connect_bd_net -net lcm_controler_eg9013f_nz_0_lcm_data [get_bd_ports lcm_data] [get_bd_pins lcm_controler_eg9013f_nz_0/lcm_data]
  connect_bd_net -net lcm_controler_eg9013f_nz_0_lcm_din [get_bd_ports lcm_din] [get_bd_pins lcm_controler_eg9013f_nz_0/lcm_din]
  connect_bd_net -net lcm_controler_eg9013f_nz_0_lcm_lp [get_bd_ports lcm_lp] [get_bd_pins lcm_controler_eg9013f_nz_0/lcm_lp]
  connect_bd_net -net lcm_controler_eg9013f_nz_0_lcm_xscl [get_bd_ports lcm_xscl] [get_bd_pins lcm_controler_eg9013f_nz_0/lcm_xscl]
  connect_bd_net -net mpeg_clk_1 [get_bd_ports mpeg_clk] [get_bd_pins axi4_tsp_0/mpeg_clk]
  connect_bd_net -net mpeg_data_1 [get_bd_ports mpeg_data] [get_bd_pins axi4_tsp_0/mpeg_data]
  connect_bd_net -net mpeg_sync_1 [get_bd_ports mpeg_sync] [get_bd_pins axi4_tsp_0/mpeg_sync]
  connect_bd_net -net mpeg_valid_1 [get_bd_ports mpeg_valid] [get_bd_pins axi4_tsp_0/mpeg_valid]
  connect_bd_net -net myip_i2s_sender_0_i2s_sender_bclk [get_bd_pins kc705_i2s_receiver_0/bclk] [get_bd_pins myip_i2s_sender_0/i2s_sender_bclk]
  connect_bd_net -net myip_i2s_sender_0_i2s_sender_lrclk [get_bd_pins kc705_i2s_receiver_0/lrclk] [get_bd_pins myip_i2s_sender_0/i2s_sender_lrclk]
  connect_bd_net -net myip_i2s_sender_0_i2s_sender_sdata [get_bd_pins kc705_i2s_receiver_0/sdata] [get_bd_pins myip_i2s_sender_0/i2s_sender_sdata]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins asi_dump_0/m00_axis_aresetn] [get_bd_pins axi4_tsp_0/s00_axi_aresetn] [get_bd_pins axi_dma_0/axi_resetn] [get_bd_pins axi_dma_1/axi_resetn] [get_bd_pins axi_gpio_0/s_axi_aresetn] [get_bd_pins axi_gpio_1/s_axi_aresetn] [get_bd_pins axi_gpio_2/s_axi_aresetn] [get_bd_pins axi_pcie_0/axi_aresetn] [get_bd_pins intr_hub_0/rst] [get_bd_pins kc705_dvb_s2_0/hard_rst_n] [get_bd_pins kc705_dvb_s2_0/s00_axi_aresetn] [get_bd_pins kc705_i2s_receiver_0/m00_axis_aresetn] [get_bd_pins kc705_ts2asi_0/rst_n] [get_bd_pins lcm_controler_eg9013f_nz_0/s00_axi_aresetn] [get_bd_pins myip_i2s_sender_0/rst_n] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]
  connect_bd_net -net proc_sys_reset_0_peripheral_reset [get_bd_pins clk_wiz_0/reset] [get_bd_pins proc_sys_reset_0/peripheral_reset]
  connect_bd_net -net reset_1 [get_bd_ports reset] [get_bd_pins kc705_pcie_ext_0/EXT_SYS_RST] [get_bd_pins proc_sys_reset_0/ext_reset_in]
  connect_bd_net -net xlconcat_0_dout [get_bd_pins intr_hub_0/int_i] [get_bd_pins xlconcat_0/dout]
  connect_bd_net -net xlconstant_0_dout [get_bd_pins axi_pcie_0/MSI_Vector_Num] [get_bd_pins xlconstant_0/dout]
  connect_bd_net -net xlconstant_1_dout [get_bd_pins kc705_pcie_ext_0/ddr_clk_100MHz] [get_bd_pins kc705_pcie_ext_0/ddr_rdy] [get_bd_pins kc705_pcie_ext_0/ddr_rst] [get_bd_pins xlconstant_1/dout]

  # Create address segments
  create_bd_addr_seg -range 0x10000 -offset 0x80000000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR0] SEG_axi_pcie_0_BAR0
  create_bd_addr_seg -range 0x10000 -offset 0x80000000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR0] SEG_axi_pcie_0_BAR0
  create_bd_addr_seg -range 0x10000 -offset 0x80010000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR1] SEG_axi_pcie_0_BAR1
  create_bd_addr_seg -range 0x10000 -offset 0x80010000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR1] SEG_axi_pcie_0_BAR1
  create_bd_addr_seg -range 0x10000 -offset 0x80000000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR0] SEG_axi_pcie_0_BAR0
  create_bd_addr_seg -range 0x10000 -offset 0x80000000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR0] SEG_axi_pcie_0_BAR0
  create_bd_addr_seg -range 0x10000 -offset 0x80010000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR1] SEG_axi_pcie_0_BAR1
  create_bd_addr_seg -range 0x10000 -offset 0x80010000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR1] SEG_axi_pcie_0_BAR1
  create_bd_addr_seg -range 0x1000 -offset 0x81001000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_0/s00_axi/reg0] SEG_axi4_tsp_0_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81005000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_dma_0/S_AXI_LITE/Reg] SEG_axi_dma_0_Reg
  create_bd_addr_seg -range 0x1000 -offset 0x81007000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_dma_1/S_AXI_LITE/Reg] SEG_axi_dma_1_Reg
  create_bd_addr_seg -range 0x1000 -offset 0x81002000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_gpio_0/S_AXI/Reg] SEG_axi_gpio_0_Reg
  create_bd_addr_seg -range 0x1000 -offset 0x81003000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_gpio_1/S_AXI/Reg] SEG_axi_gpio_1_Reg
  create_bd_addr_seg -range 0x1000 -offset 0x81004000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_gpio_2/S_AXI/Reg] SEG_axi_gpio_2_Reg
  create_bd_addr_seg -range 0x1000 -offset 0x81000000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_pcie_0/S_AXI_CTL/CTL0] SEG_axi_pcie_0_CTL0
  create_bd_addr_seg -range 0x1000 -offset 0x81006000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs kc705_dvb_s2_0/s00_axi/reg0] SEG_kc705_dvb_s2_0_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81008000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs lcm_controler_eg9013f_nz_0/s00_axi/reg0] SEG_lcm_controler_eg9013f_nz_0_reg0

  # Exclude Address Segments
  create_bd_addr_seg -range 0x1000 -offset 0x81001000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi4_tsp_0/s00_axi/reg0] SEG_axi4_tsp_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_axi4_tsp_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81005000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_dma_0/S_AXI_LITE/Reg] SEG_axi_dma_0_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_axi_dma_0_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81007000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_dma_1/S_AXI_LITE/Reg] SEG_axi_dma_1_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_axi_dma_1_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81002000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_gpio_0/S_AXI/Reg] SEG_axi_gpio_0_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_axi_gpio_0_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81003000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_gpio_1/S_AXI/Reg] SEG_axi_gpio_1_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_axi_gpio_1_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81004000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_gpio_2/S_AXI/Reg] SEG_axi_gpio_2_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_axi_gpio_2_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81000000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs axi_pcie_0/S_AXI_CTL/CTL0] SEG_axi_pcie_0_CTL0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_axi_pcie_0_CTL0]

  create_bd_addr_seg -range 0x1000 -offset 0x81006000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs kc705_dvb_s2_0/s00_axi/reg0] SEG_kc705_dvb_s2_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_kc705_dvb_s2_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81008000 [get_bd_addr_spaces axi_dma_0/Data_MM2S] [get_bd_addr_segs lcm_controler_eg9013f_nz_0/s00_axi/reg0] SEG_lcm_controler_eg9013f_nz_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_MM2S/SEG_lcm_controler_eg9013f_nz_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81001000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi4_tsp_0/s00_axi/reg0] SEG_axi4_tsp_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_axi4_tsp_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81005000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_dma_0/S_AXI_LITE/Reg] SEG_axi_dma_0_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_axi_dma_0_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81007000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_dma_1/S_AXI_LITE/Reg] SEG_axi_dma_1_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_axi_dma_1_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81002000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_gpio_0/S_AXI/Reg] SEG_axi_gpio_0_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_axi_gpio_0_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81003000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_gpio_1/S_AXI/Reg] SEG_axi_gpio_1_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_axi_gpio_1_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81004000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_gpio_2/S_AXI/Reg] SEG_axi_gpio_2_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_axi_gpio_2_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81000000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs axi_pcie_0/S_AXI_CTL/CTL0] SEG_axi_pcie_0_CTL0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_axi_pcie_0_CTL0]

  create_bd_addr_seg -range 0x1000 -offset 0x81006000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs kc705_dvb_s2_0/s00_axi/reg0] SEG_kc705_dvb_s2_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_kc705_dvb_s2_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81008000 [get_bd_addr_spaces axi_dma_0/Data_S2MM] [get_bd_addr_segs lcm_controler_eg9013f_nz_0/s00_axi/reg0] SEG_lcm_controler_eg9013f_nz_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_0/Data_S2MM/SEG_lcm_controler_eg9013f_nz_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81001000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi4_tsp_0/s00_axi/reg0] SEG_axi4_tsp_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_axi4_tsp_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81005000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi_dma_0/S_AXI_LITE/Reg] SEG_axi_dma_0_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_axi_dma_0_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81007000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi_dma_1/S_AXI_LITE/Reg] SEG_axi_dma_1_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_axi_dma_1_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81002000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi_gpio_0/S_AXI/Reg] SEG_axi_gpio_0_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_axi_gpio_0_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81003000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi_gpio_1/S_AXI/Reg] SEG_axi_gpio_1_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_axi_gpio_1_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81004000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi_gpio_2/S_AXI/Reg] SEG_axi_gpio_2_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_axi_gpio_2_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81000000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs axi_pcie_0/S_AXI_CTL/CTL0] SEG_axi_pcie_0_CTL0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_axi_pcie_0_CTL0]

  create_bd_addr_seg -range 0x1000 -offset 0x81006000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs kc705_dvb_s2_0/s00_axi/reg0] SEG_kc705_dvb_s2_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_kc705_dvb_s2_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81008000 [get_bd_addr_spaces axi_dma_1/Data_MM2S] [get_bd_addr_segs lcm_controler_eg9013f_nz_0/s00_axi/reg0] SEG_lcm_controler_eg9013f_nz_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_MM2S/SEG_lcm_controler_eg9013f_nz_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81001000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi4_tsp_0/s00_axi/reg0] SEG_axi4_tsp_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_axi4_tsp_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81005000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi_dma_0/S_AXI_LITE/Reg] SEG_axi_dma_0_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_axi_dma_0_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81007000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi_dma_1/S_AXI_LITE/Reg] SEG_axi_dma_1_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_axi_dma_1_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81002000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi_gpio_0/S_AXI/Reg] SEG_axi_gpio_0_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_axi_gpio_0_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81003000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi_gpio_1/S_AXI/Reg] SEG_axi_gpio_1_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_axi_gpio_1_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81004000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi_gpio_2/S_AXI/Reg] SEG_axi_gpio_2_Reg
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_axi_gpio_2_Reg]

  create_bd_addr_seg -range 0x1000 -offset 0x81000000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs axi_pcie_0/S_AXI_CTL/CTL0] SEG_axi_pcie_0_CTL0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_axi_pcie_0_CTL0]

  create_bd_addr_seg -range 0x1000 -offset 0x81006000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs kc705_dvb_s2_0/s00_axi/reg0] SEG_kc705_dvb_s2_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_kc705_dvb_s2_0_reg0]

  create_bd_addr_seg -range 0x1000 -offset 0x81008000 [get_bd_addr_spaces axi_dma_1/Data_S2MM] [get_bd_addr_segs lcm_controler_eg9013f_nz_0/s00_axi/reg0] SEG_lcm_controler_eg9013f_nz_0_reg0
  exclude_bd_addr_seg [get_bd_addr_segs axi_dma_1/Data_S2MM/SEG_lcm_controler_eg9013f_nz_0_reg0]

  create_bd_addr_seg -range 0x10000 -offset 0x80000000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR0] SEG_axi_pcie_0_BAR0
  exclude_bd_addr_seg [get_bd_addr_segs axi_pcie_0/M_AXI/SEG_axi_pcie_0_BAR0]

  create_bd_addr_seg -range 0x10000 -offset 0x80010000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR1] SEG_axi_pcie_0_BAR1
  exclude_bd_addr_seg [get_bd_addr_segs axi_pcie_0/M_AXI/SEG_axi_pcie_0_BAR1]

  

  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


