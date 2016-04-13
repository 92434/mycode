
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
  set im [ create_bd_port -dir O -from 15 -to 0 im ]
  set oe [ create_bd_port -dir O oe ]
  set oe1 [ create_bd_port -dir O oe1 ]
  set re [ create_bd_port -dir O -from 15 -to 0 re ]
  set reset [ create_bd_port -dir I -type rst reset ]
  set_property -dict [ list CONFIG.POLARITY {ACTIVE_HIGH}  ] $reset
  set sys_clk [ create_bd_port -dir I sys_clk ]

  # Create instance: adi_pattern_gen_0, and set properties
  set adi_pattern_gen_0 [ create_bd_cell -type ip -vlnv xiaofei:user:adi_pattern_gen:1.0 adi_pattern_gen_0 ]

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
  set_property -dict [ list CONFIG.NUM_MI {10} CONFIG.NUM_SI {1}  ] $axi_interconnect_0

  # Create instance: axi_pcie_0, and set properties
  set axi_pcie_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_pcie:2.5 axi_pcie_0 ]
  set_property -dict [ list CONFIG.AXIBAR2PCIEBAR_0 {0x00000000} CONFIG.AXIBAR_AS_0 {true} CONFIG.AXIBAR_AS_1 {true} CONFIG.AXIBAR_NUM {2} CONFIG.BAR0_ENABLED {true} CONFIG.BAR0_SCALE {Kilobytes} CONFIG.BAR0_SIZE {64} CONFIG.BAR0_TYPE {Memory} CONFIG.BAR1_ENABLED {false} CONFIG.BAR_64BIT {true} CONFIG.DEVICE_ID {0x7012} CONFIG.MAX_LINK_SPEED {2.5_GT/s} CONFIG.M_AXI_DATA_WIDTH {64} CONFIG.NO_OF_LANES {X4} CONFIG.PCIEBAR2AXIBAR_0 {0x81000000} CONFIG.PCIE_CAP_SLOT_IMPLEMENTED {true} CONFIG.S_AXI_DATA_WIDTH {64} CONFIG.XLNX_REF_BOARD {None}  ] $axi_pcie_0

  # Create instance: kc705_pcie_ext_0, and set properties
  set kc705_pcie_ext_0 [ create_bd_cell -type ip -vlnv xiaofei:user:kc705_pcie_ext:1.0 kc705_pcie_ext_0 ]

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
  connect_bd_intf_net -intf_net axi_gpio_0_GPIO2 [get_bd_intf_ports GPIO2] [get_bd_intf_pins axi_gpio_0/GPIO2]
  connect_bd_intf_net -intf_net axi_gpio_1_GPIO [get_bd_intf_ports GPIO_1] [get_bd_intf_pins axi_gpio_1/GPIO]
  connect_bd_intf_net -intf_net axi_gpio_1_GPIO2 [get_bd_intf_ports GPIO2_1] [get_bd_intf_pins axi_gpio_1/GPIO2]
  connect_bd_intf_net -intf_net axi_gpio_2_GPIO [get_bd_intf_ports GPIO_2] [get_bd_intf_pins axi_gpio_2/GPIO]
  connect_bd_intf_net -intf_net axi_interconnect_0_M00_AXI [get_bd_intf_pins axi_interconnect_0/M00_AXI] [get_bd_intf_pins axi_pcie_0/S_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M01_AXI [get_bd_intf_pins axi_interconnect_0/M01_AXI] [get_bd_intf_pins axi_pcie_0/S_AXI_CTL]
  connect_bd_intf_net -intf_net axi_interconnect_0_M02_AXI [get_bd_intf_pins axi_gpio_0/S_AXI] [get_bd_intf_pins axi_interconnect_0/M02_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M03_AXI [get_bd_intf_pins axi_gpio_1/S_AXI] [get_bd_intf_pins axi_interconnect_0/M03_AXI]
  connect_bd_intf_net -intf_net axi_interconnect_0_M04_AXI [get_bd_intf_pins axi_gpio_2/S_AXI] [get_bd_intf_pins axi_interconnect_0/M04_AXI]

  # Create port connections
  connect_bd_net -net ARESETN_1 [get_bd_pins axi_interconnect_0/ARESETN] [get_bd_pins axi_interconnect_0/M00_ARESETN] [get_bd_pins axi_interconnect_0/M01_ARESETN] [get_bd_pins axi_interconnect_0/M02_ARESETN] [get_bd_pins axi_interconnect_0/M03_ARESETN] [get_bd_pins axi_interconnect_0/M04_ARESETN] [get_bd_pins axi_interconnect_0/M05_ARESETN] [get_bd_pins axi_interconnect_0/M06_ARESETN] [get_bd_pins axi_interconnect_0/M07_ARESETN] [get_bd_pins axi_interconnect_0/M08_ARESETN] [get_bd_pins axi_interconnect_0/M09_ARESETN] [get_bd_pins axi_interconnect_0/S00_ARESETN] [get_bd_pins proc_sys_reset_0/interconnect_aresetn]
  connect_bd_net -net EXT_PCIE_REFCLK_N_1 [get_bd_ports EXT_PCIE_REFCLK_N] [get_bd_pins kc705_pcie_ext_0/EXT_PCIE_REFCLK_N]
  connect_bd_net -net EXT_PCIE_REFCLK_P_1 [get_bd_ports EXT_PCIE_REFCLK_P] [get_bd_pins kc705_pcie_ext_0/EXT_PCIE_REFCLK_P]
  connect_bd_net -net adi_pattern_gen_0_im [get_bd_ports im] [get_bd_pins adi_pattern_gen_0/im]
  connect_bd_net -net adi_pattern_gen_0_oe [get_bd_ports oe] [get_bd_pins adi_pattern_gen_0/oe]
  connect_bd_net -net adi_pattern_gen_0_oe1 [get_bd_ports oe1] [get_bd_pins adi_pattern_gen_0/oe1]
  connect_bd_net -net adi_pattern_gen_0_re [get_bd_ports re] [get_bd_pins adi_pattern_gen_0/re]
  connect_bd_net -net axi_pcie_0_axi_aclk_out [get_bd_pins axi_gpio_0/s_axi_aclk] [get_bd_pins axi_gpio_1/s_axi_aclk] [get_bd_pins axi_gpio_2/s_axi_aclk] [get_bd_pins axi_interconnect_0/ACLK] [get_bd_pins axi_interconnect_0/M00_ACLK] [get_bd_pins axi_interconnect_0/M02_ACLK] [get_bd_pins axi_interconnect_0/M03_ACLK] [get_bd_pins axi_interconnect_0/M04_ACLK] [get_bd_pins axi_interconnect_0/M05_ACLK] [get_bd_pins axi_interconnect_0/M06_ACLK] [get_bd_pins axi_interconnect_0/M07_ACLK] [get_bd_pins axi_interconnect_0/M08_ACLK] [get_bd_pins axi_interconnect_0/M09_ACLK] [get_bd_pins axi_interconnect_0/S00_ACLK] [get_bd_pins axi_pcie_0/axi_aclk_out] [get_bd_pins kc705_pcie_ext_0/pcie_clk_125MHz]
  connect_bd_net -net axi_pcie_0_axi_ctl_aclk_out [get_bd_pins axi_interconnect_0/M01_ACLK] [get_bd_pins axi_pcie_0/axi_ctl_aclk_out]
  connect_bd_net -net axi_pcie_0_mmcm_lock [get_bd_pins axi_pcie_0/mmcm_lock] [get_bd_pins kc705_pcie_ext_0/pcie_mmcm_locked]
  connect_bd_net -net kc705_pcie_ext_0_EXT_LEDS [get_bd_ports EXT_LEDS] [get_bd_pins kc705_pcie_ext_0/EXT_LEDS]
  connect_bd_net -net kc705_pcie_ext_0_mmcms_locked [get_bd_pins kc705_pcie_ext_0/mmcms_locked] [get_bd_pins proc_sys_reset_0/dcm_locked]
  connect_bd_net -net kc705_pcie_ext_0_pcie_refclk_100MHz [get_bd_pins axi_pcie_0/REFCLK] [get_bd_pins kc705_pcie_ext_0/pcie_refclk_100MHz] [get_bd_pins proc_sys_reset_0/slowest_sync_clk]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins adi_pattern_gen_0/rst_n] [get_bd_pins axi_gpio_0/s_axi_aresetn] [get_bd_pins axi_gpio_1/s_axi_aresetn] [get_bd_pins axi_gpio_2/s_axi_aresetn] [get_bd_pins axi_pcie_0/axi_aresetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]
  connect_bd_net -net reset_1 [get_bd_ports reset] [get_bd_pins kc705_pcie_ext_0/EXT_SYS_RST] [get_bd_pins proc_sys_reset_0/ext_reset_in]
  connect_bd_net -net sys_clk_1 [get_bd_ports sys_clk] [get_bd_pins adi_pattern_gen_0/sys_clk]
  connect_bd_net -net xlconstant_0_dout [get_bd_pins axi_pcie_0/MSI_Vector_Num] [get_bd_pins xlconstant_0/dout]
  connect_bd_net -net xlconstant_1_dout [get_bd_pins kc705_pcie_ext_0/ddr_clk_100MHz] [get_bd_pins kc705_pcie_ext_0/ddr_rdy] [get_bd_pins kc705_pcie_ext_0/ddr_rst] [get_bd_pins xlconstant_1/dout]

  # Create address segments
  create_bd_addr_seg -range 0x1000 -offset 0x81002000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_gpio_0/S_AXI/Reg] SEG_axi_gpio_0_Reg
  create_bd_addr_seg -range 0x1000 -offset 0x81003000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_gpio_1/S_AXI/Reg] SEG_axi_gpio_1_Reg
  create_bd_addr_seg -range 0x1000 -offset 0x81004000 [get_bd_addr_spaces axi_pcie_0/M_AXI] [get_bd_addr_segs axi_gpio_2/S_AXI/Reg] SEG_axi_gpio_2_Reg
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


