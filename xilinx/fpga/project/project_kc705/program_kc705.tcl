set bitstream_file /home/action/vivadoworkspace/kc705/kc705.runs/impl_1/design_1_wrapper.bit
set debug_net_file /home/action/vivadoworkspace/kc705/kc705.runs/impl_1/debug_nets.ltx

close_hw
open_hw
connect_hw_server
open_hw_target [lindex [get_hw_targets -of_objects [get_hw_servers localhost]] 0]
refresh_hw_device [lindex [get_hw_devices] 0]
set_property PROGRAM.FILE $bitstream_file [lindex [get_hw_devices] 0]
set_property PROBES.FILE $debug_net_file [lindex [get_hw_devices] 0]
program_hw_devices [lindex [get_hw_devices] 0]
refresh_hw_device [lindex [get_hw_devices] 0]
