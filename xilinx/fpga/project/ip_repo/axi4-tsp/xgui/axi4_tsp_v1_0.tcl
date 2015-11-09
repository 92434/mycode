# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  set Component_Name  [  ipgui::add_param $IPINST -name "Component_Name" -display_name {Component Name}]
  set_property tooltip {Component Name} ${Component_Name}
  #Adding Page
  set Page_0  [  ipgui::add_page $IPINST -name "Page 0" -display_name {Page 0}]
  set_property tooltip {Page 0} ${Page_0}
  set MONITOR_FILTER_NUM  [  ipgui::add_param $IPINST -name "MONITOR_FILTER_NUM" -parent ${Page_0} -display_name {MONITOR_FILTER_NUM}]
  set_property tooltip {MONITOR_FILTER_NUM} ${MONITOR_FILTER_NUM}
  set REPLACER_FILTER_NUM  [  ipgui::add_param $IPINST -name "REPLACER_FILTER_NUM" -parent ${Page_0} -display_name {REPLACER_FILTER_NUM}]
  set_property tooltip {REPLACER_FILTER_NUM} ${REPLACER_FILTER_NUM}


}

proc update_PARAM_VALUE.C_S00_AXI_BUSER_WIDTH { PARAM_VALUE.C_S00_AXI_BUSER_WIDTH } {
	# Procedure called to update C_S00_AXI_BUSER_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_BUSER_WIDTH { PARAM_VALUE.C_S00_AXI_BUSER_WIDTH } {
	# Procedure called to validate C_S00_AXI_BUSER_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_RUSER_WIDTH { PARAM_VALUE.C_S00_AXI_RUSER_WIDTH } {
	# Procedure called to update C_S00_AXI_RUSER_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_RUSER_WIDTH { PARAM_VALUE.C_S00_AXI_RUSER_WIDTH } {
	# Procedure called to validate C_S00_AXI_RUSER_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_WUSER_WIDTH { PARAM_VALUE.C_S00_AXI_WUSER_WIDTH } {
	# Procedure called to update C_S00_AXI_WUSER_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_WUSER_WIDTH { PARAM_VALUE.C_S00_AXI_WUSER_WIDTH } {
	# Procedure called to validate C_S00_AXI_WUSER_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ARUSER_WIDTH { PARAM_VALUE.C_S00_AXI_ARUSER_WIDTH } {
	# Procedure called to update C_S00_AXI_ARUSER_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ARUSER_WIDTH { PARAM_VALUE.C_S00_AXI_ARUSER_WIDTH } {
	# Procedure called to validate C_S00_AXI_ARUSER_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_AWUSER_WIDTH { PARAM_VALUE.C_S00_AXI_AWUSER_WIDTH } {
	# Procedure called to update C_S00_AXI_AWUSER_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_AWUSER_WIDTH { PARAM_VALUE.C_S00_AXI_AWUSER_WIDTH } {
	# Procedure called to validate C_S00_AXI_AWUSER_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S00_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S00_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to update C_S00_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S00_AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ID_WIDTH { PARAM_VALUE.C_S00_AXI_ID_WIDTH } {
	# Procedure called to update C_S00_AXI_ID_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ID_WIDTH { PARAM_VALUE.C_S00_AXI_ID_WIDTH } {
	# Procedure called to validate C_S00_AXI_ID_WIDTH
	return true
}

proc update_PARAM_VALUE.COMMON_REPLACE_DATA_GROUPS { PARAM_VALUE.COMMON_REPLACE_DATA_GROUPS } {
	# Procedure called to update COMMON_REPLACE_DATA_GROUPS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.COMMON_REPLACE_DATA_GROUPS { PARAM_VALUE.COMMON_REPLACE_DATA_GROUPS } {
	# Procedure called to validate COMMON_REPLACE_DATA_GROUPS
	return true
}

proc update_PARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT { PARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT } {
	# Procedure called to update COMMON_REPLACE_MATCH_PID_COUNT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT { PARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT } {
	# Procedure called to validate COMMON_REPLACE_MATCH_PID_COUNT
	return true
}

proc update_PARAM_VALUE.COMMON_REPLACER_FILTER_NUM { PARAM_VALUE.COMMON_REPLACER_FILTER_NUM } {
	# Procedure called to update COMMON_REPLACER_FILTER_NUM when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.COMMON_REPLACER_FILTER_NUM { PARAM_VALUE.COMMON_REPLACER_FILTER_NUM } {
	# Procedure called to validate COMMON_REPLACER_FILTER_NUM
	return true
}

proc update_PARAM_VALUE.REPLACE_DATA_GROUPS { PARAM_VALUE.REPLACE_DATA_GROUPS } {
	# Procedure called to update REPLACE_DATA_GROUPS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.REPLACE_DATA_GROUPS { PARAM_VALUE.REPLACE_DATA_GROUPS } {
	# Procedure called to validate REPLACE_DATA_GROUPS
	return true
}

proc update_PARAM_VALUE.REPLACE_MATCH_PID_COUNT { PARAM_VALUE.REPLACE_MATCH_PID_COUNT } {
	# Procedure called to update REPLACE_MATCH_PID_COUNT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.REPLACE_MATCH_PID_COUNT { PARAM_VALUE.REPLACE_MATCH_PID_COUNT } {
	# Procedure called to validate REPLACE_MATCH_PID_COUNT
	return true
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


proc update_MODELPARAM_VALUE.MONITOR_FILTER_NUM { MODELPARAM_VALUE.MONITOR_FILTER_NUM PARAM_VALUE.MONITOR_FILTER_NUM } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MONITOR_FILTER_NUM}] ${MODELPARAM_VALUE.MONITOR_FILTER_NUM}
}

proc update_MODELPARAM_VALUE.REPLACER_FILTER_NUM { MODELPARAM_VALUE.REPLACER_FILTER_NUM PARAM_VALUE.REPLACER_FILTER_NUM } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.REPLACER_FILTER_NUM}] ${MODELPARAM_VALUE.REPLACER_FILTER_NUM}
}

proc update_MODELPARAM_VALUE.REPLACE_MATCH_PID_COUNT { MODELPARAM_VALUE.REPLACE_MATCH_PID_COUNT PARAM_VALUE.REPLACE_MATCH_PID_COUNT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.REPLACE_MATCH_PID_COUNT}] ${MODELPARAM_VALUE.REPLACE_MATCH_PID_COUNT}
}

proc update_MODELPARAM_VALUE.REPLACE_DATA_GROUPS { MODELPARAM_VALUE.REPLACE_DATA_GROUPS PARAM_VALUE.REPLACE_DATA_GROUPS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.REPLACE_DATA_GROUPS}] ${MODELPARAM_VALUE.REPLACE_DATA_GROUPS}
}

proc update_MODELPARAM_VALUE.COMMON_REPLACER_FILTER_NUM { MODELPARAM_VALUE.COMMON_REPLACER_FILTER_NUM PARAM_VALUE.COMMON_REPLACER_FILTER_NUM } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.COMMON_REPLACER_FILTER_NUM}] ${MODELPARAM_VALUE.COMMON_REPLACER_FILTER_NUM}
}

proc update_MODELPARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT { MODELPARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT PARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT}] ${MODELPARAM_VALUE.COMMON_REPLACE_MATCH_PID_COUNT}
}

proc update_MODELPARAM_VALUE.COMMON_REPLACE_DATA_GROUPS { MODELPARAM_VALUE.COMMON_REPLACE_DATA_GROUPS PARAM_VALUE.COMMON_REPLACE_DATA_GROUPS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.COMMON_REPLACE_DATA_GROUPS}] ${MODELPARAM_VALUE.COMMON_REPLACE_DATA_GROUPS}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ID_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ID_WIDTH PARAM_VALUE.C_S00_AXI_ID_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ID_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ID_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_AWUSER_WIDTH { MODELPARAM_VALUE.C_S00_AXI_AWUSER_WIDTH PARAM_VALUE.C_S00_AXI_AWUSER_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_AWUSER_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_AWUSER_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ARUSER_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ARUSER_WIDTH PARAM_VALUE.C_S00_AXI_ARUSER_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ARUSER_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ARUSER_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_WUSER_WIDTH { MODELPARAM_VALUE.C_S00_AXI_WUSER_WIDTH PARAM_VALUE.C_S00_AXI_WUSER_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_WUSER_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_WUSER_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_RUSER_WIDTH { MODELPARAM_VALUE.C_S00_AXI_RUSER_WIDTH PARAM_VALUE.C_S00_AXI_RUSER_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_RUSER_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_RUSER_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_BUSER_WIDTH { MODELPARAM_VALUE.C_S00_AXI_BUSER_WIDTH PARAM_VALUE.C_S00_AXI_BUSER_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_BUSER_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_BUSER_WIDTH}
}

