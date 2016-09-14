
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

  # Create ports
  set EXT_LEDS [ create_bd_port -dir O -from 7 -to 0 EXT_LEDS ]
  set EXT_PCIE_REFCLK_N [ create_bd_port -dir I EXT_PCIE_REFCLK_N ]
  set EXT_PCIE_REFCLK_P [ create_bd_port -dir I EXT_PCIE_REFCLK_P ]
  set asi_out_n [ create_bd_port -dir O asi_out_n ]
  set asi_out_n_1 [ create_bd_port -dir O asi_out_n_1 ]
  set asi_out_n_2 [ create_bd_port -dir O asi_out_n_2 ]
  set asi_out_n_3 [ create_bd_port -dir O asi_out_n_3 ]
  set asi_out_n_4 [ create_bd_port -dir O asi_out_n_4 ]
  set asi_out_n_5 [ create_bd_port -dir O asi_out_n_5 ]
  set asi_out_n_6 [ create_bd_port -dir O asi_out_n_6 ]
  set asi_out_n_7 [ create_bd_port -dir O asi_out_n_7 ]
  set asi_out_p [ create_bd_port -dir O asi_out_p ]
  set asi_out_p_1 [ create_bd_port -dir O asi_out_p_1 ]
  set asi_out_p_2 [ create_bd_port -dir O asi_out_p_2 ]
  set asi_out_p_3 [ create_bd_port -dir O asi_out_p_3 ]
  set asi_out_p_4 [ create_bd_port -dir O asi_out_p_4 ]
  set asi_out_p_5 [ create_bd_port -dir O asi_out_p_5 ]
  set asi_out_p_6 [ create_bd_port -dir O asi_out_p_6 ]
  set asi_out_p_7 [ create_bd_port -dir O asi_out_p_7 ]
  set mpeg_clk [ create_bd_port -dir I mpeg_clk ]
  set mpeg_clk_1 [ create_bd_port -dir I mpeg_clk_1 ]
  set mpeg_clk_2 [ create_bd_port -dir I mpeg_clk_2 ]
  set mpeg_clk_3 [ create_bd_port -dir I mpeg_clk_3 ]
  set mpeg_clk_4 [ create_bd_port -dir I mpeg_clk_4 ]
  set mpeg_clk_5 [ create_bd_port -dir I mpeg_clk_5 ]
  set mpeg_clk_6 [ create_bd_port -dir I mpeg_clk_6 ]
  set mpeg_clk_7 [ create_bd_port -dir I mpeg_clk_7 ]
  set mpeg_data [ create_bd_port -dir I -from 7 -to 0 mpeg_data ]
  set mpeg_data_1 [ create_bd_port -dir I -from 7 -to 0 mpeg_data_1 ]
  set mpeg_data_2 [ create_bd_port -dir I -from 7 -to 0 mpeg_data_2 ]
  set mpeg_data_3 [ create_bd_port -dir I -from 7 -to 0 mpeg_data_3 ]
  set mpeg_data_4 [ create_bd_port -dir I -from 7 -to 0 mpeg_data_4 ]
  set mpeg_data_5 [ create_bd_port -dir I -from 7 -to 0 mpeg_data_5 ]
  set mpeg_data_6 [ create_bd_port -dir I -from 7 -to 0 mpeg_data_6 ]
  set mpeg_data_7 [ create_bd_port -dir I -from 7 -to 0 mpeg_data_7 ]
  set mpeg_sync [ create_bd_port -dir I mpeg_sync ]
  set mpeg_sync_1 [ create_bd_port -dir I mpeg_sync_1 ]
  set mpeg_sync_2 [ create_bd_port -dir I mpeg_sync_2 ]
  set mpeg_sync_3 [ create_bd_port -dir I mpeg_sync_3 ]
  set mpeg_sync_4 [ create_bd_port -dir I mpeg_sync_4 ]
  set mpeg_sync_5 [ create_bd_port -dir I mpeg_sync_5 ]
  set mpeg_sync_6 [ create_bd_port -dir I mpeg_sync_6 ]
  set mpeg_sync_7 [ create_bd_port -dir I mpeg_sync_7 ]
  set mpeg_valid [ create_bd_port -dir I mpeg_valid ]
  set mpeg_valid_1 [ create_bd_port -dir I mpeg_valid_1 ]
  set mpeg_valid_2 [ create_bd_port -dir I mpeg_valid_2 ]
  set mpeg_valid_3 [ create_bd_port -dir I mpeg_valid_3 ]
  set mpeg_valid_4 [ create_bd_port -dir I mpeg_valid_4 ]
  set mpeg_valid_5 [ create_bd_port -dir I mpeg_valid_5 ]
  set mpeg_valid_6 [ create_bd_port -dir I mpeg_valid_6 ]
  set mpeg_valid_7 [ create_bd_port -dir I mpeg_valid_7 ]
  set reset [ create_bd_port -dir I -type rst reset ]
  set_property -dict [ list CONFIG.POLARITY {ACTIVE_HIGH}  ] $reset

  # Create instance: axi4_tsp_0, and set properties
  set axi4_tsp_0 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_0 ]

  # Create instance: axi4_tsp_1, and set properties
  set axi4_tsp_1 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_1 ]

  # Create instance: axi4_tsp_2, and set properties
  set axi4_tsp_2 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_2 ]

  # Create instance: axi4_tsp_3, and set properties
  set axi4_tsp_3 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_3 ]

  # Create instance: axi4_tsp_4, and set properties
  set axi4_tsp_4 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_4 ]

  # Create instance: axi4_tsp_5, and set properties
  set axi4_tsp_5 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_5 ]

  # Create instance: axi4_tsp_6, and set properties
  set axi4_tsp_6 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_6 ]

  # Create instance: axi4_tsp_7, and set properties
  set axi4_tsp_7 [ create_bd_cell -type ip -vlnv xiaofei:user:axi4_tsp:1.0 axi4_tsp_7 ]

  # Create instance: axi_gpio_0, and set properties
  set axi_gpio_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_0 ]
  set_property -dict [ list CONFIG.C_GPIO_WIDTH {8} CONFIG.C_INTERRUPT_PRESENT {0} CONFIG.C_IS_DUAL {0}  ] $axi_gpio_0

  # Create instance: axi_interconnect_0, and set properties
  set axi_interconnect_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_interconnect_0 ]
  set_property -dict [ list CONFIG.NUM_MI {11} CONFIG.NUM_SI {1}  ] $axi_interconnect_0

  # Create instance: axi_pcie_0, and set properties
  set axi_pcie_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_pcie:2.5 axi_pcie_0 ]
  set_property -dict [ list CONFIG.AXIBAR2PCIEBAR_0 {0x00000000} CONFIG.AXIBAR_AS_0 {true} CONFIG.AXIBAR_AS_1 {true} CONFIG.AXIBAR_NUM {2} CONFIG.BAR0_ENABLED {true} CONFIG.BAR0_SCALE {Kilobytes} CONFIG.BAR0_SIZE {64} CONFIG.BAR0_TYPE {Memory} CONFIG.BAR1_ENABLED {false} CONFIG.BAR_64BIT {true} CONFIG.DEVICE_ID {0x7012} CONFIG.MAX_LINK_SPEED {2.5_GT/s} CONFIG.M_AXI_DATA_WIDTH {64} CONFIG.NO_OF_LANES {X4} CONFIG.PCIEBAR2AXIBAR_0 {0x81000000} CONFIG.PCIE_CAP_SLOT_IMPLEMENTED {true} CONFIG.S_AXI_DATA_WIDTH {64} CONFIG.XLNX_REF_BOARD {None}  ] $axi_pcie_0

  # Create instance: clk_wiz_0, and set properties
  set clk_wiz_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:5.1 clk_wiz_0 ]
  set_property -dict [ list CONFIG.CLKOUT1_JITTER {122.344} CONFIG.CLKOUT1_PHASE_ERROR {97.646} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {135.000}  ] $clk_wiz_0

  # Create instance: kc705_pcie_ext_0, and set properties
  set kc705_pcie_ext_0 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_pcie_ext:1.0 kc705_pcie_ext_0 ]

  # Create instance: kc705_ts2asi_0, and set properties
  set kc705_ts2asi_0 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_0 ]

  # Create instance: kc705_ts2asi_1, and set properties
  set kc705_ts2asi_1 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_1 ]

  # Create instance: kc705_ts2asi_2, and set properties
  set kc705_ts2asi_2 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_2 ]

  # Create instance: kc705_ts2asi_3, and set properties
  set kc705_ts2asi_3 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_3 ]

  # Create instance: kc705_ts2asi_4, and set properties
  set kc705_ts2asi_4 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_4 ]

  # Create instance: kc705_ts2asi_5, and set properties
  set kc705_ts2asi_5 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_5 ]

  # Create instance: kc705_ts2asi_6, and set properties
  set kc705_ts2asi_6 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_6 ]

  # Create instance: kc705_ts2asi_7, and set properties
  set kc705_ts2asi_7 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_ts2asi:1.0 kc705_ts2asi_7 ]

  # Create instance: proc_sys_reset_0, and set properties
  set proc_sys_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset_0 ]

  # Create instance: xlconstant_0, and set properties
  set xlconstant_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_0 ]
  set_property -dict [ list CONFIG.CONST_VAL {0} CONFIG.CONST_WIDTH {5}  ] $xlconstant_0

  # Create instance: xlconstant_1, and set properties
  set xlconstant_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_1 ]

  # Create interface connections
  connect_bd_intf_net -intf_net S00_AXI_1 [get_bd_intf_pins axi_interconnect_0/S00_AXI] [get_bd_intf_pins axi_pcie_0/M_AXI]
  connect_bd_intf_net -intf_net axi_gpio_0_GPIO [get_bd_intf_ports GPIO] [get_bd_intf_pins axi_gpio_0/GPIO]
  connect_bd_intf_net -intf_net axi_interconnect_0_M00_AXI [get_bd_intf_pins axi_interconnect_0/M00_AXI] [get_bd_intf_pins axi_pcie_0/S_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M01_AXI [get_bd_intf_pins axi_interconnect_0/M01_AXI] [get_bd_intf_pins axi_pcie_0/S_AXI_CTL]
  connect_bd_intf_net -intf_net axi_interconnect_0_M02_AXI [get_bd_intf_pins axi_gpio_0/S_AXI] [get_bd_intf_pins axi_interconnect_0/M02_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M03_AXI [get_bd_intf_pins axi4_tsp_0/s00_axi] [get_bd_intf_pins axi_interconnect_0/M03_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M04_AXI [get_bd_intf_pins axi4_tsp_1/s00_axi] [get_bd_intf_pins axi_interconnect_0/M04_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M05_AXI [get_bd_intf_pins axi4_tsp_2/s00_axi] [get_bd_intf_pins axi_interconnect_0/M05_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M06_AXI [get_bd_intf_pins axi4_tsp_3/s00_axi] [get_bd_intf_pins axi_interconnect_0/M06_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M07_AXI [get_bd_intf_pins axi4_tsp_4/s00_axi] [get_bd_intf_pins axi_interconnect_0/M07_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M08_AXI [get_bd_intf_pins axi4_tsp_5/s00_axi] [get_bd_intf_pins axi_interconnect_0/M08_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M09_AXI [get_bd_intf_pins axi4_tsp_6/s00_axi] [get_bd_intf_pins axi_interconnect_0/M09_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M10_AXI [get_bd_intf_pins axi4_tsp_7/s00_axi] [get_bd_intf_pins axi_interconnect_0/M10_AXI]

  # Create port connections
  connect_bd_net -net ARESETN_1 [get_bd_pins axi_interconnect_0/ARESETN] [get_bd_pins axi_interconnect_0/M00_ARESETN] [get_bd_pins axi_interconnect_0/M01_ARESETN] [get_bd_pins axi_interconnect_0/M02_ARESETN] [get_bd_pins axi_interconnect_0/M03_ARESETN] [get_bd_pins axi_interconnect_0/M04_ARESETN] [get_bd_pins axi_interconnect_0/M05_ARESETN] [get_bd_pins axi_interconnect_0/M06_ARESETN] [get_bd_pins axi_interconnect_0/M07_ARESETN] [get_bd_pins axi_interconnect_0/M08_ARESETN] [get_bd_pins axi_interconnect_0/M09_ARESETN] [get_bd_pins axi_interconnect_0/M10_ARESETN] [get_bd_pins axi_interconnect_0/S00_ARESETN] [get_bd_pins proc_sys_reset_0/interconnect_aresetn]
  connect_bd_net -net EXT_PCIE_REFCLK_N_1 [get_bd_ports EXT_PCIE_REFCLK_N] [get_bd_pins kc705_pcie_ext_0/EXT_PCIE_REFCLK_N]
  connect_bd_net -net EXT_PCIE_REFCLK_P_1 [get_bd_ports EXT_PCIE_REFCLK_P] [get_bd_pins kc705_pcie_ext_0/EXT_PCIE_REFCLK_P]
  connect_bd_net -net axi4_tsp_0_ts_out [get_bd_pins axi4_tsp_0/ts_out] [get_bd_pins kc705_ts2asi_0/din_8b]
  connect_bd_net -net axi4_tsp_0_ts_out_clk [get_bd_pins axi4_tsp_0/ts_out_clk] [get_bd_pins kc705_ts2asi_0/din_clk]
  connect_bd_net -net axi4_tsp_0_ts_out_sync [get_bd_pins axi4_tsp_0/ts_out_sync] [get_bd_pins kc705_ts2asi_0/sync]
  connect_bd_net -net axi4_tsp_0_ts_out_valid [get_bd_pins axi4_tsp_0/ts_out_valid] [get_bd_pins kc705_ts2asi_0/valid]
  connect_bd_net -net axi4_tsp_1_ts_out [get_bd_pins axi4_tsp_1/ts_out] [get_bd_pins kc705_ts2asi_1/din_8b]
  connect_bd_net -net axi4_tsp_1_ts_out_clk [get_bd_pins axi4_tsp_1/ts_out_clk] [get_bd_pins kc705_ts2asi_1/din_clk]
  connect_bd_net -net axi4_tsp_1_ts_out_sync [get_bd_pins axi4_tsp_1/ts_out_sync] [get_bd_pins kc705_ts2asi_1/sync]
  connect_bd_net -net axi4_tsp_1_ts_out_valid [get_bd_pins axi4_tsp_1/ts_out_valid] [get_bd_pins kc705_ts2asi_1/valid]
  connect_bd_net -net axi4_tsp_2_ts_out [get_bd_pins axi4_tsp_2/ts_out] [get_bd_pins kc705_ts2asi_2/din_8b]
  connect_bd_net -net axi4_tsp_2_ts_out_clk [get_bd_pins axi4_tsp_2/ts_out_clk] [get_bd_pins kc705_ts2asi_2/din_clk]
  connect_bd_net -net axi4_tsp_2_ts_out_sync [get_bd_pins axi4_tsp_2/ts_out_sync] [get_bd_pins kc705_ts2asi_2/sync]
  connect_bd_net -net axi4_tsp_2_ts_out_valid [get_bd_pins axi4_tsp_2/ts_out_valid] [get_bd_pins kc705_ts2asi_2/valid]
  connect_bd_net -net axi4_tsp_3_ts_out [get_bd_pins axi4_tsp_3/ts_out] [get_bd_pins kc705_ts2asi_3/din_8b]
  connect_bd_net -net axi4_tsp_3_ts_out_clk [get_bd_pins axi4_tsp_3/ts_out_clk] [get_bd_pins kc705_ts2asi_3/din_clk]
  connect_bd_net -net axi4_tsp_3_ts_out_sync [get_bd_pins axi4_tsp_3/ts_out_sync] [get_bd_pins kc705_ts2asi_3/sync]
  connect_bd_net -net axi4_tsp_3_ts_out_valid [get_bd_pins axi4_tsp_3/ts_out_valid] [get_bd_pins kc705_ts2asi_3/valid]
  connect_bd_net -net axi4_tsp_4_ts_out [get_bd_pins axi4_tsp_4/ts_out] [get_bd_pins kc705_ts2asi_4/din_8b]
  connect_bd_net -net axi4_tsp_4_ts_out_clk [get_bd_pins axi4_tsp_4/ts_out_clk] [get_bd_pins kc705_ts2asi_4/din_clk]
  connect_bd_net -net axi4_tsp_4_ts_out_sync [get_bd_pins axi4_tsp_4/ts_out_sync] [get_bd_pins kc705_ts2asi_4/sync]
  connect_bd_net -net axi4_tsp_4_ts_out_valid [get_bd_pins axi4_tsp_4/ts_out_valid] [get_bd_pins kc705_ts2asi_4/valid]
  connect_bd_net -net axi4_tsp_5_ts_out [get_bd_pins axi4_tsp_5/ts_out] [get_bd_pins kc705_ts2asi_5/din_8b]
  connect_bd_net -net axi4_tsp_5_ts_out_clk [get_bd_pins axi4_tsp_5/ts_out_clk] [get_bd_pins kc705_ts2asi_5/din_clk]
  connect_bd_net -net axi4_tsp_5_ts_out_sync [get_bd_pins axi4_tsp_5/ts_out_sync] [get_bd_pins kc705_ts2asi_5/sync]
  connect_bd_net -net axi4_tsp_5_ts_out_valid [get_bd_pins axi4_tsp_5/ts_out_valid] [get_bd_pins kc705_ts2asi_5/valid]
  connect_bd_net -net axi4_tsp_6_ts_out [get_bd_pins axi4_tsp_6/ts_out] [get_bd_pins kc705_ts2asi_6/din_8b]
  connect_bd_net -net axi4_tsp_6_ts_out_clk [get_bd_pins axi4_tsp_6/ts_out_clk] [get_bd_pins kc705_ts2asi_6/din_clk]
  connect_bd_net -net axi4_tsp_6_ts_out_sync [get_bd_pins axi4_tsp_6/ts_out_sync] [get_bd_pins kc705_ts2asi_6/sync]
  connect_bd_net -net axi4_tsp_6_ts_out_valid [get_bd_pins axi4_tsp_6/ts_out_valid] [get_bd_pins kc705_ts2asi_6/valid]
  connect_bd_net -net axi4_tsp_7_ts_out [get_bd_pins axi4_tsp_7/ts_out] [get_bd_pins kc705_ts2asi_7/din_8b]
  connect_bd_net -net axi4_tsp_7_ts_out_clk [get_bd_pins axi4_tsp_7/ts_out_clk] [get_bd_pins kc705_ts2asi_7/din_clk]
  connect_bd_net -net axi4_tsp_7_ts_out_sync [get_bd_pins axi4_tsp_7/ts_out_sync] [get_bd_pins kc705_ts2asi_7/sync]
  connect_bd_net -net axi4_tsp_7_ts_out_valid [get_bd_pins axi4_tsp_7/ts_out_valid] [get_bd_pins kc705_ts2asi_7/valid]
  connect_bd_net -net axi_pcie_0_axi_aclk_out [get_bd_pins axi4_tsp_0/s00_axi_aclk] [get_bd_pins axi4_tsp_1/s00_axi_aclk] [get_bd_pins axi4_tsp_2/s00_axi_aclk] [get_bd_pins axi4_tsp_3/s00_axi_aclk] [get_bd_pins axi4_tsp_4/s00_axi_aclk] [get_bd_pins axi4_tsp_5/s00_axi_aclk] [get_bd_pins axi4_tsp_6/s00_axi_aclk] [get_bd_pins axi4_tsp_7/s00_axi_aclk] [get_bd_pins axi_gpio_0/s_axi_aclk] [get_bd_pins axi_interconnect_0/ACLK] [get_bd_pins axi_interconnect_0/M00_ACLK] [get_bd_pins axi_interconnect_0/M02_ACLK] [get_bd_pins axi_interconnect_0/M03_ACLK] [get_bd_pins axi_interconnect_0/M04_ACLK] [get_bd_pins axi_interconnect_0/M05_ACLK] [get_bd_pins axi_interconnect_0/M06_ACLK] [get_bd_pins axi_interconnect_0/M07_ACLK] [get_bd_pins axi_interconnect_0/M08_ACLK] [get_bd_pins axi_interconnect_0/M09_ACLK] [get_bd_pins axi_interconnect_0/M10_ACLK] [get_bd_pins axi_interconnect_0/S00_ACLK] [get_bd_pins axi_pcie_0/axi_aclk_out] [get_bd_pins kc705_pcie_ext_0/pcie_clk_125MHz]
  connect_bd_net -net axi_pcie_0_axi_ctl_aclk_out [get_bd_pins axi_interconnect_0/M01_ACLK] [get_bd_pins axi_pcie_0/axi_ctl_aclk_out]
  connect_bd_net -net axi_pcie_0_mmcm_lock [get_bd_pins axi_pcie_0/mmcm_lock] [get_bd_pins kc705_pcie_ext_0/pcie_mmcm_locked]
  connect_bd_net -net clk_wiz_0_clk_out1 [get_bd_pins clk_wiz_0/clk_out1] [get_bd_pins kc705_ts2asi_0/clk] [get_bd_pins kc705_ts2asi_1/clk] [get_bd_pins kc705_ts2asi_2/clk] [get_bd_pins kc705_ts2asi_3/clk] [get_bd_pins kc705_ts2asi_4/clk] [get_bd_pins kc705_ts2asi_5/clk] [get_bd_pins kc705_ts2asi_6/clk] [get_bd_pins kc705_ts2asi_7/clk]
  connect_bd_net -net kc705_pcie_ext_0_EXT_LEDS [get_bd_ports EXT_LEDS] [get_bd_pins kc705_pcie_ext_0/EXT_LEDS]
  connect_bd_net -net kc705_pcie_ext_0_mmcms_locked [get_bd_pins kc705_pcie_ext_0/mmcms_locked] [get_bd_pins proc_sys_reset_0/dcm_locked]
  connect_bd_net -net kc705_pcie_ext_0_pcie_refclk_100MHz [get_bd_pins axi_pcie_0/REFCLK] [get_bd_pins clk_wiz_0/clk_in1] [get_bd_pins kc705_pcie_ext_0/pcie_refclk_100MHz] [get_bd_pins proc_sys_reset_0/slowest_sync_clk]
  connect_bd_net -net kc705_ts2asi_0_asi_out_n [get_bd_ports asi_out_n] [get_bd_pins kc705_ts2asi_0/asi_out_n]
  connect_bd_net -net kc705_ts2asi_0_asi_out_p [get_bd_ports asi_out_p] [get_bd_pins kc705_ts2asi_0/asi_out_p]
  connect_bd_net -net kc705_ts2asi_1_asi_out_n [get_bd_ports asi_out_n_1] [get_bd_pins kc705_ts2asi_1/asi_out_n]
  connect_bd_net -net kc705_ts2asi_1_asi_out_p [get_bd_ports asi_out_p_1] [get_bd_pins kc705_ts2asi_1/asi_out_p]
  connect_bd_net -net kc705_ts2asi_2_asi_out_n [get_bd_ports asi_out_n_2] [get_bd_pins kc705_ts2asi_2/asi_out_n]
  connect_bd_net -net kc705_ts2asi_2_asi_out_p [get_bd_ports asi_out_p_2] [get_bd_pins kc705_ts2asi_2/asi_out_p]
  connect_bd_net -net kc705_ts2asi_3_asi_out_n [get_bd_ports asi_out_n_3] [get_bd_pins kc705_ts2asi_3/asi_out_n]
  connect_bd_net -net kc705_ts2asi_3_asi_out_p [get_bd_ports asi_out_p_3] [get_bd_pins kc705_ts2asi_3/asi_out_p]
  connect_bd_net -net kc705_ts2asi_4_asi_out_n [get_bd_ports asi_out_n_4] [get_bd_pins kc705_ts2asi_4/asi_out_n]
  connect_bd_net -net kc705_ts2asi_4_asi_out_p [get_bd_ports asi_out_p_4] [get_bd_pins kc705_ts2asi_4/asi_out_p]
  connect_bd_net -net kc705_ts2asi_5_asi_out_n [get_bd_ports asi_out_n_5] [get_bd_pins kc705_ts2asi_5/asi_out_n]
  connect_bd_net -net kc705_ts2asi_5_asi_out_p [get_bd_ports asi_out_p_5] [get_bd_pins kc705_ts2asi_5/asi_out_p]
  connect_bd_net -net kc705_ts2asi_6_asi_out_n [get_bd_ports asi_out_n_6] [get_bd_pins kc705_ts2asi_6/asi_out_n]
  connect_bd_net -net kc705_ts2asi_6_asi_out_p [get_bd_ports asi_out_p_6] [get_bd_pins kc705_ts2asi_6/asi_out_p]
  connect_bd_net -net kc705_ts2asi_7_asi_out_n [get_bd_ports asi_out_n_7] [get_bd_pins kc705_ts2asi_7/asi_out_n]
  connect_bd_net -net kc705_ts2asi_7_asi_out_p [get_bd_ports asi_out_p_7] [get_bd_pins kc705_ts2asi_7/asi_out_p]
  connect_bd_net -net mpeg_clk_1 [get_bd_ports mpeg_clk] [get_bd_pins axi4_tsp_0/mpeg_clk]
  connect_bd_net -net mpeg_clk_1_1 [get_bd_ports mpeg_clk_1] [get_bd_pins axi4_tsp_1/mpeg_clk]
  connect_bd_net -net mpeg_clk_2_1 [get_bd_ports mpeg_clk_2] [get_bd_pins axi4_tsp_2/mpeg_clk]
  connect_bd_net -net mpeg_clk_3_1 [get_bd_ports mpeg_clk_3] [get_bd_pins axi4_tsp_3/mpeg_clk]
  connect_bd_net -net mpeg_clk_4_1 [get_bd_ports mpeg_clk_4] [get_bd_pins axi4_tsp_4/mpeg_clk]
  connect_bd_net -net mpeg_clk_5_1 [get_bd_ports mpeg_clk_5] [get_bd_pins axi4_tsp_5/mpeg_clk]
  connect_bd_net -net mpeg_clk_6_1 [get_bd_ports mpeg_clk_6] [get_bd_pins axi4_tsp_6/mpeg_clk]
  connect_bd_net -net mpeg_clk_7_1 [get_bd_ports mpeg_clk_7] [get_bd_pins axi4_tsp_7/mpeg_clk]
  connect_bd_net -net mpeg_data_1 [get_bd_ports mpeg_data] [get_bd_pins axi4_tsp_0/mpeg_data]
  connect_bd_net -net mpeg_data_1_1 [get_bd_ports mpeg_data_1] [get_bd_pins axi4_tsp_1/mpeg_data]
  connect_bd_net -net mpeg_data_2_1 [get_bd_ports mpeg_data_2] [get_bd_pins axi4_tsp_2/mpeg_data]
  connect_bd_net -net mpeg_data_3_1 [get_bd_ports mpeg_data_3] [get_bd_pins axi4_tsp_3/mpeg_data]
  connect_bd_net -net mpeg_data_4_1 [get_bd_ports mpeg_data_4] [get_bd_pins axi4_tsp_4/mpeg_data]
  connect_bd_net -net mpeg_data_5_1 [get_bd_ports mpeg_data_5] [get_bd_pins axi4_tsp_5/mpeg_data]
  connect_bd_net -net mpeg_data_6_1 [get_bd_ports mpeg_data_6] [get_bd_pins axi4_tsp_6/mpeg_data]
  connect_bd_net -net mpeg_data_7_1 [get_bd_ports mpeg_data_7] [get_bd_pins axi4_tsp_7/mpeg_data]
  connect_bd_net -net mpeg_sync_1 [get_bd_ports mpeg_sync] [get_bd_pins axi4_tsp_0/mpeg_sync]
  connect_bd_net -net mpeg_sync_1_1 [get_bd_ports mpeg_sync_1] [get_bd_pins axi4_tsp_1/mpeg_sync]
  connect_bd_net -net mpeg_sync_2_1 [get_bd_ports mpeg_sync_2] [get_bd_pins axi4_tsp_2/mpeg_sync]
  connect_bd_net -net mpeg_sync_3_1 [get_bd_ports mpeg_sync_3] [get_bd_pins axi4_tsp_3/mpeg_sync]
  connect_bd_net -net mpeg_sync_4_1 [get_bd_ports mpeg_sync_4] [get_bd_pins axi4_tsp_4/mpeg_sync]
  connect_bd_net -net mpeg_sync_5_1 [get_bd_ports mpeg_sync_5] [get_bd_pins axi4_tsp_5/mpeg_sync]
  connect_bd_net -net mpeg_sync_6_1 [get_bd_ports mpeg_sync_6] [get_bd_pins axi4_tsp_6/mpeg_sync]
  connect_bd_net -net mpeg_sync_7_1 [get_bd_ports mpeg_sync_7] [get_bd_pins axi4_tsp_7/mpeg_sync]
  connect_bd_net -net mpeg_valid_1 [get_bd_ports mpeg_valid] [get_bd_pins axi4_tsp_0/mpeg_valid]
  connect_bd_net -net mpeg_valid_1_1 [get_bd_ports mpeg_valid_1] [get_bd_pins axi4_tsp_1/mpeg_valid]
  connect_bd_net -net mpeg_valid_2_1 [get_bd_ports mpeg_valid_2] [get_bd_pins axi4_tsp_2/mpeg_valid]
  connect_bd_net -net mpeg_valid_3_1 [get_bd_ports mpeg_valid_3] [get_bd_pins axi4_tsp_3/mpeg_valid]
  connect_bd_net -net mpeg_valid_4_1 [get_bd_ports mpeg_valid_4] [get_bd_pins axi4_tsp_4/mpeg_valid]
  connect_bd_net -net mpeg_valid_5_1 [get_bd_ports mpeg_valid_5] [get_bd_pins axi4_tsp_5/mpeg_valid]
  connect_bd_net -net mpeg_valid_6_1 [get_bd_ports mpeg_valid_6] [get_bd_pins axi4_tsp_6/mpeg_valid]
  connect_bd_net -net mpeg_valid_7_1 [get_bd_ports mpeg_valid_7] [get_bd_pins axi4_tsp_7/mpeg_valid]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins axi4_tsp_0/s00_axi_aresetn] [get_bd_pins axi4_tsp_1/s00_axi_aresetn] [get_bd_pins axi4_tsp_2/s00_axi_aresetn] [get_bd_pins axi4_tsp_3/s00_axi_aresetn] [get_bd_pins axi4_tsp_4/s00_axi_aresetn] [get_bd_pins axi4_tsp_5/s00_axi_aresetn] [get_bd_pins axi4_tsp_6/s00_axi_aresetn] [get_bd_pins axi4_tsp_7/s00_axi_aresetn] [get_bd_pins axi_gpio_0/s_axi_aresetn] [get_bd_pins axi_pcie_0/axi_aresetn] [get_bd_pins kc705_ts2asi_0/rst_n] [get_bd_pins kc705_ts2asi_1/rst_n] [get_bd_pins kc705_ts2asi_2/rst_n] [get_bd_pins kc705_ts2asi_3/rst_n] [get_bd_pins kc705_ts2asi_4/rst_n] [get_bd_pins kc705_ts2asi_5/rst_n] [get_bd_pins kc705_ts2asi_6/rst_n] [get_bd_pins kc705_ts2asi_7/rst_n] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]
  connect_bd_net -net proc_sys_reset_0_peripheral_reset [get_bd_pins clk_wiz_0/reset] [get_bd_pins proc_sys_reset_0/peripheral_reset]
  connect_bd_net -net reset_1 [get_bd_ports reset] [get_bd_pins kc705_pcie_ext_0/EXT_SYS_RST] [get_bd_pins proc_sys_reset_0/ext_reset_in]
  connect_bd_net -net xlconstant_0_dout [get_bd_pins axi_pcie_0/MSI_Vector_Num] [get_bd_pins xlconstant_0/dout]
  connect_bd_net -net xlconstant_1_dout [get_bd_pins kc705_pcie_ext_0/ddr_clk_100MHz] [get_bd_pins kc705_pcie_ext_0/ddr_rdy] [get_bd_pins kc705_pcie_ext_0/ddr_rst] [get_bd_pins xlconstant_1/dout]

  # Create address segments
  create_bd_addr_seg -range 0x1000 -offset 0x81002000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_0/s00_axi/reg0] SEG_axi4_tsp_0_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81003000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_1/s00_axi/reg0] SEG_axi4_tsp_1_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81004000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_2/s00_axi/reg0] SEG_axi4_tsp_2_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81005000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_3/s00_axi/reg0] SEG_axi4_tsp_3_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81006000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_4/s00_axi/reg0] SEG_axi4_tsp_4_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81007000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_5/s00_axi/reg0] SEG_axi4_tsp_5_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81008000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_6/s00_axi/reg0] SEG_axi4_tsp_6_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81009000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi4_tsp_7/s00_axi/reg0] SEG_axi4_tsp_7_reg0
  create_bd_addr_seg -range 0x1000 -offset 0x81001000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_gpio_0/S_AXI/Reg] SEG_axi_gpio_0_Reg
  create_bd_addr_seg -range 0x10000 -offset 0x80000000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR0] SEG_axi_pcie_0_BAR0
  create_bd_addr_seg -range 0x10000 -offset 0x80010000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_pcie_0/S_AXI/BAR1] SEG_axi_pcie_0_BAR1
  create_bd_addr_seg -range 0x1000 -offset 0x81000000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_pcie_0/S_AXI_CTL/CTL0] SEG_axi_pcie_0_CTL0
  

  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


