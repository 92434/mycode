# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {

}

proc update_PARAM_VALUE.OPT_MEM_ADDR_BITS { PARAM_VALUE.OPT_MEM_ADDR_BITS } {
	# Procedure called to update OPT_MEM_ADDR_BITS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.OPT_MEM_ADDR_BITS { PARAM_VALUE.OPT_MEM_ADDR_BITS } {
	# Procedure called to validate OPT_MEM_ADDR_BITS
	return true
}

proc update_PARAM_VALUE.SDA_DEL_LEN { PARAM_VALUE.SDA_DEL_LEN } {
	# Procedure called to update SDA_DEL_LEN when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SDA_DEL_LEN { PARAM_VALUE.SDA_DEL_LEN } {
	# Procedure called to validate SDA_DEL_LEN
	return true
}

proc update_PARAM_VALUE.SCL_DEL_LEN { PARAM_VALUE.SCL_DEL_LEN } {
	# Procedure called to update SCL_DEL_LEN when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SCL_DEL_LEN { PARAM_VALUE.SCL_DEL_LEN } {
	# Procedure called to validate SCL_DEL_LEN
	return true
}

proc update_PARAM_VALUE.DEB_I2C_LEN { PARAM_VALUE.DEB_I2C_LEN } {
	# Procedure called to update DEB_I2C_LEN when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DEB_I2C_LEN { PARAM_VALUE.DEB_I2C_LEN } {
	# Procedure called to validate DEB_I2C_LEN
	return true
}

proc update_PARAM_VALUE.I2C_ADDRESS { PARAM_VALUE.I2C_ADDRESS } {
	# Procedure called to update I2C_ADDRESS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.I2C_ADDRESS { PARAM_VALUE.I2C_ADDRESS } {
	# Procedure called to validate I2C_ADDRESS
	return true
}


proc update_MODELPARAM_VALUE.I2C_ADDRESS { MODELPARAM_VALUE.I2C_ADDRESS PARAM_VALUE.I2C_ADDRESS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I2C_ADDRESS}] ${MODELPARAM_VALUE.I2C_ADDRESS}
}

proc update_MODELPARAM_VALUE.DEB_I2C_LEN { MODELPARAM_VALUE.DEB_I2C_LEN PARAM_VALUE.DEB_I2C_LEN } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DEB_I2C_LEN}] ${MODELPARAM_VALUE.DEB_I2C_LEN}
}

proc update_MODELPARAM_VALUE.SCL_DEL_LEN { MODELPARAM_VALUE.SCL_DEL_LEN PARAM_VALUE.SCL_DEL_LEN } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SCL_DEL_LEN}] ${MODELPARAM_VALUE.SCL_DEL_LEN}
}

proc update_MODELPARAM_VALUE.SDA_DEL_LEN { MODELPARAM_VALUE.SDA_DEL_LEN PARAM_VALUE.SDA_DEL_LEN } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SDA_DEL_LEN}] ${MODELPARAM_VALUE.SDA_DEL_LEN}
}

proc update_MODELPARAM_VALUE.OPT_MEM_ADDR_BITS { MODELPARAM_VALUE.OPT_MEM_ADDR_BITS PARAM_VALUE.OPT_MEM_ADDR_BITS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.OPT_MEM_ADDR_BITS}] ${MODELPARAM_VALUE.OPT_MEM_ADDR_BITS}
}

