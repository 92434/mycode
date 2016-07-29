# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {

}

proc update_PARAM_VALUE.COUNT { PARAM_VALUE.COUNT } {
	# Procedure called to update COUNT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.COUNT { PARAM_VALUE.COUNT } {
	# Procedure called to validate COUNT
	return true
}


proc update_MODELPARAM_VALUE.COUNT { MODELPARAM_VALUE.COUNT PARAM_VALUE.COUNT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.COUNT}] ${MODELPARAM_VALUE.COUNT}
}

