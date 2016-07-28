# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {

}

proc update_PARAM_VALUE.C_M00_AXIS_START_COUNT { PARAM_VALUE.C_M00_AXIS_START_COUNT } {
	# Procedure called to update C_M00_AXIS_START_COUNT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_M00_AXIS_START_COUNT { PARAM_VALUE.C_M00_AXIS_START_COUNT } {
	# Procedure called to validate C_M00_AXIS_START_COUNT
	return true
}

proc update_PARAM_VALUE.BULK_DEPTH { PARAM_VALUE.BULK_DEPTH } {
	# Procedure called to update BULK_DEPTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.BULK_DEPTH { PARAM_VALUE.BULK_DEPTH } {
	# Procedure called to validate BULK_DEPTH
	return true
}

proc update_PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH { PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH } {
	# Procedure called to update C_M00_AXIS_TDATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH { PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH } {
	# Procedure called to validate C_M00_AXIS_TDATA_WIDTH
	return true
}

proc update_PARAM_VALUE.BULK_OF_DATA { PARAM_VALUE.BULK_OF_DATA } {
	# Procedure called to update BULK_OF_DATA when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.BULK_OF_DATA { PARAM_VALUE.BULK_OF_DATA } {
	# Procedure called to validate BULK_OF_DATA
	return true
}

proc update_PARAM_VALUE.MPEG_DATA_WIDTH { PARAM_VALUE.MPEG_DATA_WIDTH } {
	# Procedure called to update MPEG_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MPEG_DATA_WIDTH { PARAM_VALUE.MPEG_DATA_WIDTH } {
	# Procedure called to validate MPEG_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS { PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS } {
	# Procedure called to update NUMBER_OF_OUTPUT_WORDS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS { PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS } {
	# Procedure called to validate NUMBER_OF_OUTPUT_WORDS
	return true
}


proc update_MODELPARAM_VALUE.MPEG_DATA_WIDTH { MODELPARAM_VALUE.MPEG_DATA_WIDTH PARAM_VALUE.MPEG_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MPEG_DATA_WIDTH}] ${MODELPARAM_VALUE.MPEG_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.NUMBER_OF_OUTPUT_WORDS { MODELPARAM_VALUE.NUMBER_OF_OUTPUT_WORDS PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS}] ${MODELPARAM_VALUE.NUMBER_OF_OUTPUT_WORDS}
}

proc update_MODELPARAM_VALUE.BULK_OF_DATA { MODELPARAM_VALUE.BULK_OF_DATA PARAM_VALUE.BULK_OF_DATA } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.BULK_OF_DATA}] ${MODELPARAM_VALUE.BULK_OF_DATA}
}

proc update_MODELPARAM_VALUE.BULK_DEPTH { MODELPARAM_VALUE.BULK_DEPTH PARAM_VALUE.BULK_DEPTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.BULK_DEPTH}] ${MODELPARAM_VALUE.BULK_DEPTH}
}

proc update_MODELPARAM_VALUE.C_M00_AXIS_TDATA_WIDTH { MODELPARAM_VALUE.C_M00_AXIS_TDATA_WIDTH PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH}] ${MODELPARAM_VALUE.C_M00_AXIS_TDATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_M00_AXIS_START_COUNT { MODELPARAM_VALUE.C_M00_AXIS_START_COUNT PARAM_VALUE.C_M00_AXIS_START_COUNT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_M00_AXIS_START_COUNT}] ${MODELPARAM_VALUE.C_M00_AXIS_START_COUNT}
}

