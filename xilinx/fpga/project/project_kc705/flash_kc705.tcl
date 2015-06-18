set bitstream_file D:/xf/fpga/ip_repo/download/xapp1171/design_1_wrapper.bit
set mcs_file D:/xf/fpga/ip_repo/download/xapp1171/KC705.mcs
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
