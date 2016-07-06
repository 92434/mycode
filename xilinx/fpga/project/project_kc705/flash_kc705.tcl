#close_hw
open_hw
connect_hw_server
open_hw_target [lindex [get_hw_targets -of_objects [get_hw_servers localhost]] 0]

#set bitstream_file /home/action/vivadoworkspace/kc705/kc705.runs/impl_1/design_1_wrapper.bit
set bitstream_file /home/action/vivadoworkspace/csa/csa.runs/impl_1/design_1_wrapper.bit
#set bitstream_file /home/action/vivadoworkspace/tsp_asi_dvbs2/tsp_asi_dvbs2.runs/impl_1/design_1_wrapper.bit
#set bitstream_file /home/action/vivadoworkspace/adc_9125_data/adc_9125_data.runs/impl_1/design_1_wrapper.bit
#set bitstream_file /home/action/vivadoworkspace/kc705_gpio/kc705_gpio.runs/impl_1/design_1_wrapper.bit
#set bitstream_file /home/action/vivadoworkspace/i2s_receiver_test/i2s_receiver_test.runs/impl_1/design_1_wrapper.bit

#set nky_file /home/action/vivadoworkspace/kc705/kc705.runs/impl_1/design_1_wrapper.nky

set mcs_file /home/action/vivadoworkspace/kc705.mcs
if {![file exists $mcs_file]} {
#	write_cfgmem -force -format MCS -size 1024 -interface BPIx16 -loadbit "up 0x0 $bitstream_file" $mcs_file
	write_cfgmem -force -format MCS -size 128 -interface SPIx4 -loadbit "up 0x0 $bitstream_file" $mcs_file
}

#set cfgmem_part_name 28f00ap30t-bpi-x16
set cfgmem_part_name n25q128-3.3v-spi-x1_x2_x4
create_hw_cfgmem -hw_device [lindex [get_hw_devices] 0] -mem_dev  [lindex [get_cfgmem_parts $cfgmem_part_name] 0]

#set_property PROGRAM.ADDRESS_RANGE  {entire_device} [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.ADDRESS_RANGE  {use_file} [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.FILES [list $mcs_file ] [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0]]
set_property PROGRAM.BPI_RS_PINS {none} [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.UNUSED_PIN_TERMINATION {pull-none} [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.BLANK_CHECK  0 [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.ERASE  1 [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.CFG_PROGRAM  1 [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]
set_property PROGRAM.VERIFY  1 [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]

startgroup 
if {
	![
		string equal [ get_property PROGRAM.HW_CFGMEM_TYPE [lindex [get_hw_devices] 0] ] [ get_property MEM_TYPE [ get_property CFGMEM_PART [ get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ] ] ] ]
	]
} {
	create_hw_bitstream -hw_device [lindex [get_hw_devices] 0] [get_property PROGRAM.HW_CFGMEM_BITFILE [ lindex [get_hw_devices] 0]]
	program_hw_devices [lindex [get_hw_devices] 0]; 
}


program_hw_cfgmem -hw_cfgmem [get_property PROGRAM.HW_CFGMEM [lindex [get_hw_devices] 0 ]]

#set_property ENCRYPTION.FILE $nky_file [get_property PROGRAM.HW_BITSTREAM [lindex [get_hw_devices] 0]]
#program_hw_devices -key {bbr} [lindex [get_hw_devices] 0]

refresh_hw_device [lindex [get_hw_devices] 0]

file delete $mcs_file

