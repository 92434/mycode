#close_hw
open_hw
connect_hw_server
open_hw_target [lindex [get_hw_targets -of_objects [get_hw_servers localhost]] 0]

#set bitstream_file D:/xf/fpga/ip_repo/download/xapp1171/design_1_wrapper.bit
#set mcs_file D:/xf/fpga/ip_repo/download/xapp1171/KC705.mcs
#set bitstream_file D:/xf/fpga/ip_repo/download/dma_performance_demo/pcie_7x_0_example/pcie_7x_0_example.runs/impl_1/xilinx_pcie_2_1_ep_7x.bit
#set mcs_file D:/xf/fpga/ip_repo/download/dma_performance_demo/pcie_7x_0_example/KC705.mcs
#set bitstream_file /home/action/vivadoworkspace/rdf0281-kc705-base-trd-2014-3/k7_pcie_dma_ddr3_base/ready_to_test/k7_pcie_dma_ddr3_base_x4_gen2.bit
#set mcs_file D:/xf/fpga/ip_repo/download/rdf0281-kc705-base-trd-2014-3/k7_pcie_dma_ddr3_base/ready_to_test/KC705.mcs
#set bitstream_file D:/xf/fpga/ip_repo/download/xilinx_pcie_2_1_ep_7x.bit
#set bitstream_file /home/action/vivadoworkspace/project_1_axi_pcie_bridge/project_1_axi_pcie_bridge.runs/impl_1/design_1_wrapper.bit
#set bitstream_file /home/action/vivadoworkspace/xapp1171/project_1/project_1.runs/impl_1/design_1_wrapper.bit
#set bitstream_file /home/action/vivadoworkspace/test_kc705/project_1/project_1.runs/impl_1/design_1_wrapper.bit
#set bitstream_file /home/action/vivadoworkspace/project_kc705_cdma/project_kc705_cdma.runs/impl_1/design_1_wrapper.bit
set bitstream_file /home/action/vivadoworkspace/kc705/kc705.runs/impl_1/design_1_wrapper.bit
#set bitstream_file /home/action/vivadoworkspace/test_fmc_gpios.bit
#set bitstream_file /home/action/vivadoworkspace/test_tsp_i2c.bit
set mcs_file /home/action/vivadoworkspace/kc705.mcs
set cfgmem_part_name 28f00ap30t-bpi-x16

refresh_hw_device [lindex [get_hw_devices] 0]

if {![file exists $mcs_file]} {
	write_cfgmem -force -format MCS -size 128 -interface BPIx16 -loadbit "up 0x0 $bitstream_file" $mcs_file
}

create_hw_cfgmem -hw_device [lindex [get_hw_devices] 0] -mem_dev [lindex [get_cfgmem_parts $cfgmem_part_name] 0]

set_property PROGRAM.BLANK_CHECK 0 [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.ERASE 1 [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.CFG_PROGRAM 1 [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.VERIFY 1 [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.ADDRESS_RANGE {use_file} [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.FILE_1 $mcs_file [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0]]
set_property PROGRAM.UNUSED_PIN_TERMINATION {pull-none} [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.BPI_RS_PINS {none} [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]

startgroup 

if {
	![
		string equal [
			get_property PROGRAM.HW_CFGMEM_TYPE [lindex [get_hw_devices] 0]
		] [
			get_property MEM_TYPE [
				get_property CFGMEM_PART [
					get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]
				]
			]
		]
	]
} {
	create_hw_bitstream -hw_device [lindex [get_hw_devices] 0] [get_property PROGRAM.HW_CFGMEM_BITFILE [ lindex [get_hw_devices] 0]]
	program_hw_devices [lindex [get_hw_devices] 0]; 
}

program_hw_cfgmem -hw_cfgmem [get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]

set nky_file /home/action/vivadoworkspace/kc705/kc705.runs/impl_1/design_1_wrapper.nky
set_property ENCRYPTION.FILE $nky_file [get_property PROGRAM.HW_BITSTREAM [lindex [get_hw_devices] 0]]
program_hw_devices -key {bbr} [lindex [get_hw_devices] 0]


file delete $mcs_file
