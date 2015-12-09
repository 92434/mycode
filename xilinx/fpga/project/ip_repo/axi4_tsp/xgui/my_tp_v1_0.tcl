# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  set Component_Name  [  ipgui::add_param $IPINST -name "Component_Name" -display_name {Component Name}]
  set_property tooltip {Component Name} ${Component_Name}
  #Adding Page
  set Page_0  [  ipgui::add_page $IPINST -name "Page 0" -display_name {Page 0}]
  set_property tooltip {Page 0} ${Page_0}
  set REPLACER_FILTER_NUM  [  ipgui::add_param $IPINST -name "REPLACER_FILTER_NUM" -parent ${Page_0} -display_name {Replacer Filter Num}]
  set_property tooltip {Replacer Filter Num} ${REPLACER_FILTER_NUM}
  set MONITOR_FILTER_NUM  [  ipgui::add_param $IPINST -name "MONITOR_FILTER_NUM" -parent ${Page_0} -display_name {Monitor Filter Num}]
  set_property tooltip {Monitor Filter Num} ${MONITOR_FILTER_NUM}
  set OPT_MEM_ADDR_BITS  [  ipgui::add_param $IPINST -name "OPT_MEM_ADDR_BITS" -parent ${Page_0} -display_name {Opt Mem Addr Bits}]
  set_property tooltip {Opt Mem Addr Bits} ${OPT_MEM_ADDR_BITS}
  set C_S_AXI_DATA_WIDTH  [  ipgui::add_param $IPINST -name "C_S_AXI_DATA_WIDTH" -parent ${Page_0} -display_name {C S Axi Data Width}]
  set_property tooltip {C S Axi Data Width} ${C_S_AXI_DATA_WIDTH}


}

proc update_PARAM_VALUE.REPLACER_FILTER_NUM { PARAM_VALUE.REPLACER_FILTER_NUM } {
	# Procedure called to update REPLACER_FILTER_NUM when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.REPLACER_FILTER_NUM { PARAM_VALUE.REPLACER_FILTER_NUM } {
	# Procedure called to validate REPLACER_FILTER_NUM
	return true
}

proc update_PARAM_VALUE.MONITOR_FILTER_NUM { PARAM_VALUE.MONITOR_FILTER_NUM } {
	# Procedure called to update MONITOR_FILTER_NUM when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MONITOR_FILTER_NUM { PARAM_VALUE.MONITOR_FILTER_NUM } {
	# Procedure called to validate MONITOR_FILTER_NUM
	return true
}

proc update_PARAM_VALUE.OPT_MEM_ADDR_BITS { PARAM_VALUE.OPT_MEM_ADDR_BITS } {
	# Procedure called to update OPT_MEM_ADDR_BITS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.OPT_MEM_ADDR_BITS { PARAM_VALUE.OPT_MEM_ADDR_BITS } {
	# Procedure called to validate OPT_MEM_ADDR_BITS
	return true
}

proc update_PARAM_VALUE.C_S_AXI_DATA_WIDTH { PARAM_VALUE.C_S_AXI_DATA_WIDTH } {
	# Procedure called to update C_S_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S_AXI_DATA_WIDTH { PARAM_VALUE.C_S_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S_AXI_DATA_WIDTH
	return true
}


proc update_MODELPARAM_VALUE.C_S_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S_AXI_DATA_WIDTH PARAM_VALUE.C_S_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.OPT_MEM_ADDR_BITS { MODELPARAM_VALUE.OPT_MEM_ADDR_BITS PARAM_VALUE.OPT_MEM_ADDR_BITS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.OPT_MEM_ADDR_BITS}] ${MODELPARAM_VALUE.OPT_MEM_ADDR_BITS}
}

proc update_MODELPARAM_VALUE.MONITOR_FILTER_NUM { MODELPARAM_VALUE.MONITOR_FILTER_NUM PARAM_VALUE.MONITOR_FILTER_NUM } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MONITOR_FILTER_NUM}] ${MODELPARAM_VALUE.MONITOR_FILTER_NUM}
}

proc update_MODELPARAM_VALUE.REPLACER_FILTER_NUM { MODELPARAM_VALUE.REPLACER_FILTER_NUM PARAM_VALUE.REPLACER_FILTER_NUM } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.REPLACER_FILTER_NUM}] ${MODELPARAM_VALUE.REPLACER_FILTER_NUM}
}

