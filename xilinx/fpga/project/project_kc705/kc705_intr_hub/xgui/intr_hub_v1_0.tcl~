# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {

}

proc update_PARAM_VALUE.INT_NUM { PARAM_VALUE.INT_NUM } {
	# Procedure called to update INT_NUM when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.INT_NUM { PARAM_VALUE.INT_NUM } {
	# Procedure called to validate INT_NUM
	return true
}


proc update_MODELPARAM_VALUE.INT_NUM { MODELPARAM_VALUE.INT_NUM PARAM_VALUE.INT_NUM } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.INT_NUM}] ${MODELPARAM_VALUE.INT_NUM}
}

