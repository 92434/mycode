# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  set Component_Name  [  ipgui::add_param $IPINST -name "Component_Name" -display_name {Component Name}]
  set_property tooltip {Component Name} ${Component_Name}
  #Adding Page
  set Page_0  [  ipgui::add_page $IPINST -name "Page 0" -display_name {Page 0}]
  set_property tooltip {Page 0} ${Page_0}
  set IO_DELAY_GROUP  [  ipgui::add_param $IPINST -name "IO_DELAY_GROUP" -parent ${Page_0} -display_name {Io Delay Group}]
  set_property tooltip {Io Delay Group} ${IO_DELAY_GROUP}
  set DAC_DATAPATH_DISABLE  [  ipgui::add_param $IPINST -name "DAC_DATAPATH_DISABLE" -parent ${Page_0} -display_name {Dac Datapath Disable}]
  set_property tooltip {Dac Datapath Disable} ${DAC_DATAPATH_DISABLE}
  set MMCM_OR_BUFIO_N  [  ipgui::add_param $IPINST -name "MMCM_OR_BUFIO_N" -parent ${Page_0} -display_name {Mmcm Or Bufio N}]
  set_property tooltip {Mmcm Or Bufio N} ${MMCM_OR_BUFIO_N}
  set SERDES_OR_DDR_N  [  ipgui::add_param $IPINST -name "SERDES_OR_DDR_N" -parent ${Page_0} -display_name {Serdes Or Ddr N}]
  set_property tooltip {Serdes Or Ddr N} ${SERDES_OR_DDR_N}
  set DEVICE_TYPE  [  ipgui::add_param $IPINST -name "DEVICE_TYPE" -parent ${Page_0} -display_name {Device Type}]
  set_property tooltip {Device Type} ${DEVICE_TYPE}
  set ID  [  ipgui::add_param $IPINST -name "ID" -parent ${Page_0} -display_name {Id}]
  set_property tooltip {Id} ${ID}


}

proc update_PARAM_VALUE.IO_DELAY_GROUP { PARAM_VALUE.IO_DELAY_GROUP } {
	# Procedure called to update IO_DELAY_GROUP when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.IO_DELAY_GROUP { PARAM_VALUE.IO_DELAY_GROUP } {
	# Procedure called to validate IO_DELAY_GROUP
	return true
}

proc update_PARAM_VALUE.DAC_DATAPATH_DISABLE { PARAM_VALUE.DAC_DATAPATH_DISABLE } {
	# Procedure called to update DAC_DATAPATH_DISABLE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DAC_DATAPATH_DISABLE { PARAM_VALUE.DAC_DATAPATH_DISABLE } {
	# Procedure called to validate DAC_DATAPATH_DISABLE
	return true
}

proc update_PARAM_VALUE.MMCM_OR_BUFIO_N { PARAM_VALUE.MMCM_OR_BUFIO_N } {
	# Procedure called to update MMCM_OR_BUFIO_N when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MMCM_OR_BUFIO_N { PARAM_VALUE.MMCM_OR_BUFIO_N } {
	# Procedure called to validate MMCM_OR_BUFIO_N
	return true
}

proc update_PARAM_VALUE.SERDES_OR_DDR_N { PARAM_VALUE.SERDES_OR_DDR_N } {
	# Procedure called to update SERDES_OR_DDR_N when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SERDES_OR_DDR_N { PARAM_VALUE.SERDES_OR_DDR_N } {
	# Procedure called to validate SERDES_OR_DDR_N
	return true
}

proc update_PARAM_VALUE.DEVICE_TYPE { PARAM_VALUE.DEVICE_TYPE } {
	# Procedure called to update DEVICE_TYPE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DEVICE_TYPE { PARAM_VALUE.DEVICE_TYPE } {
	# Procedure called to validate DEVICE_TYPE
	return true
}

proc update_PARAM_VALUE.ID { PARAM_VALUE.ID } {
	# Procedure called to update ID when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ID { PARAM_VALUE.ID } {
	# Procedure called to validate ID
	return true
}


proc update_MODELPARAM_VALUE.ID { MODELPARAM_VALUE.ID PARAM_VALUE.ID } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.ID}] ${MODELPARAM_VALUE.ID}
}

proc update_MODELPARAM_VALUE.DEVICE_TYPE { MODELPARAM_VALUE.DEVICE_TYPE PARAM_VALUE.DEVICE_TYPE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DEVICE_TYPE}] ${MODELPARAM_VALUE.DEVICE_TYPE}
}

proc update_MODELPARAM_VALUE.SERDES_OR_DDR_N { MODELPARAM_VALUE.SERDES_OR_DDR_N PARAM_VALUE.SERDES_OR_DDR_N } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SERDES_OR_DDR_N}] ${MODELPARAM_VALUE.SERDES_OR_DDR_N}
}

proc update_MODELPARAM_VALUE.MMCM_OR_BUFIO_N { MODELPARAM_VALUE.MMCM_OR_BUFIO_N PARAM_VALUE.MMCM_OR_BUFIO_N } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MMCM_OR_BUFIO_N}] ${MODELPARAM_VALUE.MMCM_OR_BUFIO_N}
}

proc update_MODELPARAM_VALUE.DAC_DATAPATH_DISABLE { MODELPARAM_VALUE.DAC_DATAPATH_DISABLE PARAM_VALUE.DAC_DATAPATH_DISABLE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DAC_DATAPATH_DISABLE}] ${MODELPARAM_VALUE.DAC_DATAPATH_DISABLE}
}

proc update_MODELPARAM_VALUE.IO_DELAY_GROUP { MODELPARAM_VALUE.IO_DELAY_GROUP PARAM_VALUE.IO_DELAY_GROUP } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.IO_DELAY_GROUP}] ${MODELPARAM_VALUE.IO_DELAY_GROUP}
}

