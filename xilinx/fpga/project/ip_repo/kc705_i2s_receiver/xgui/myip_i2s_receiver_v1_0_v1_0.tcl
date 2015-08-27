# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  set Component_Name  [  ipgui::add_param $IPINST -name "Component_Name" -display_name {Component Name}]
  set_property tooltip {Component Name} ${Component_Name}
  #Adding Page
  set Page_0  [  ipgui::add_page $IPINST -name "Page 0" -display_name {Page 0}]
  set_property tooltip {Page 0} ${Page_0}
  set C_M00_AXIS_START_COUNT  [  ipgui::add_param $IPINST -name "C_M00_AXIS_START_COUNT" -parent ${Page_0} -display_name {C M00 Axis Start Count}]
  set_property tooltip {C M00 Axis Start Count} ${C_M00_AXIS_START_COUNT}
  set C_M00_AXIS_TDATA_WIDTH  [  ipgui::add_param $IPINST -name "C_M00_AXIS_TDATA_WIDTH" -parent ${Page_0} -display_name {C M00 Axis Tdata Width}]
  set_property tooltip {C M00 Axis Tdata Width} ${C_M00_AXIS_TDATA_WIDTH}
  set NUMBER_OF_OUTPUT_WORDS  [  ipgui::add_param $IPINST -name "NUMBER_OF_OUTPUT_WORDS" -parent ${Page_0} -display_name {Number Of Output Words}]
  set_property tooltip {Number Of Output Words} ${NUMBER_OF_OUTPUT_WORDS}
  set I2S_DATA_BIT_WIDTH  [  ipgui::add_param $IPINST -name "I2S_DATA_BIT_WIDTH" -parent ${Page_0} -display_name {I2s Data Bit Width}]
  set_property tooltip {I2s Data Bit Width} ${I2S_DATA_BIT_WIDTH}


}

proc update_PARAM_VALUE.C_M00_AXIS_START_COUNT { PARAM_VALUE.C_M00_AXIS_START_COUNT } {
	# Procedure called to update C_M00_AXIS_START_COUNT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_M00_AXIS_START_COUNT { PARAM_VALUE.C_M00_AXIS_START_COUNT } {
	# Procedure called to validate C_M00_AXIS_START_COUNT
	return true
}

proc update_PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH { PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH } {
	# Procedure called to update C_M00_AXIS_TDATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH { PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH } {
	# Procedure called to validate C_M00_AXIS_TDATA_WIDTH
	return true
}

proc update_PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS { PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS } {
	# Procedure called to update NUMBER_OF_OUTPUT_WORDS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS { PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS } {
	# Procedure called to validate NUMBER_OF_OUTPUT_WORDS
	return true
}

proc update_PARAM_VALUE.I2S_DATA_BIT_WIDTH { PARAM_VALUE.I2S_DATA_BIT_WIDTH } {
	# Procedure called to update I2S_DATA_BIT_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.I2S_DATA_BIT_WIDTH { PARAM_VALUE.I2S_DATA_BIT_WIDTH } {
	# Procedure called to validate I2S_DATA_BIT_WIDTH
	return true
}


proc update_MODELPARAM_VALUE.I2S_DATA_BIT_WIDTH { MODELPARAM_VALUE.I2S_DATA_BIT_WIDTH PARAM_VALUE.I2S_DATA_BIT_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I2S_DATA_BIT_WIDTH}] ${MODELPARAM_VALUE.I2S_DATA_BIT_WIDTH}
}

proc update_MODELPARAM_VALUE.NUMBER_OF_OUTPUT_WORDS { MODELPARAM_VALUE.NUMBER_OF_OUTPUT_WORDS PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.NUMBER_OF_OUTPUT_WORDS}] ${MODELPARAM_VALUE.NUMBER_OF_OUTPUT_WORDS}
}

proc update_MODELPARAM_VALUE.C_M00_AXIS_TDATA_WIDTH { MODELPARAM_VALUE.C_M00_AXIS_TDATA_WIDTH PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_M00_AXIS_TDATA_WIDTH}] ${MODELPARAM_VALUE.C_M00_AXIS_TDATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_M00_AXIS_START_COUNT { MODELPARAM_VALUE.C_M00_AXIS_START_COUNT PARAM_VALUE.C_M00_AXIS_START_COUNT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_M00_AXIS_START_COUNT}] ${MODELPARAM_VALUE.C_M00_AXIS_START_COUNT}
}

