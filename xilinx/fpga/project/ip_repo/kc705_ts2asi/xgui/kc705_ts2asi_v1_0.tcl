# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  set Component_Name  [  ipgui::add_param $IPINST -name "Component_Name" -display_name {Component Name}]
  set_property tooltip {Component Name} ${Component_Name}

}

proc update_PARAM_VALUE.CNT_STATE4 { PARAM_VALUE.CNT_STATE4 } {
	# Procedure called to update CNT_STATE4 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CNT_STATE4 { PARAM_VALUE.CNT_STATE4 } {
	# Procedure called to validate CNT_STATE4
	return true
}

proc update_PARAM_VALUE.FIFO_DATA_WIDTH { PARAM_VALUE.FIFO_DATA_WIDTH } {
	# Procedure called to update FIFO_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.FIFO_DATA_WIDTH { PARAM_VALUE.FIFO_DATA_WIDTH } {
	# Procedure called to validate FIFO_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.CNT_STATE3 { PARAM_VALUE.CNT_STATE3 } {
	# Procedure called to update CNT_STATE3 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CNT_STATE3 { PARAM_VALUE.CNT_STATE3 } {
	# Procedure called to validate CNT_STATE3
	return true
}

proc update_PARAM_VALUE.CNT_STATE2 { PARAM_VALUE.CNT_STATE2 } {
	# Procedure called to update CNT_STATE2 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CNT_STATE2 { PARAM_VALUE.CNT_STATE2 } {
	# Procedure called to validate CNT_STATE2
	return true
}

proc update_PARAM_VALUE.CNT_STATE1 { PARAM_VALUE.CNT_STATE1 } {
	# Procedure called to update CNT_STATE1 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CNT_STATE1 { PARAM_VALUE.CNT_STATE1 } {
	# Procedure called to validate CNT_STATE1
	return true
}

proc update_PARAM_VALUE.CNT_RESET { PARAM_VALUE.CNT_RESET } {
	# Procedure called to update CNT_RESET when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CNT_RESET { PARAM_VALUE.CNT_RESET } {
	# Procedure called to validate CNT_RESET
	return true
}


proc update_MODELPARAM_VALUE.FIFO_DATA_WIDTH { MODELPARAM_VALUE.FIFO_DATA_WIDTH PARAM_VALUE.FIFO_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.FIFO_DATA_WIDTH}] ${MODELPARAM_VALUE.FIFO_DATA_WIDTH}
}

