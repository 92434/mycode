# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  set Component_Name  [  ipgui::add_param $IPINST -name "Component_Name" -display_name {Component Name}]
  set_property tooltip {Component Name} ${Component_Name}
  #Adding Page
  set Page_0  [  ipgui::add_page $IPINST -name "Page 0" -display_name {Page 0}]
  set_property tooltip {Page 0} ${Page_0}
  set IIC_SLAVE_ADDR_WIDTH  [  ipgui::add_param $IPINST -name "IIC_SLAVE_ADDR_WIDTH" -parent ${Page_0} -display_name {Iic Slave Addr Width}]
  set_property tooltip {Iic Slave Addr Width} ${IIC_SLAVE_ADDR_WIDTH}
  set IIC_SLAVE_DATA_WIDTH  [  ipgui::add_param $IPINST -name "IIC_SLAVE_DATA_WIDTH" -parent ${Page_0} -display_name {Iic Slave Data Width}]
  set_property tooltip {Iic Slave Data Width} ${IIC_SLAVE_DATA_WIDTH}
  set SDA_DEL_LEN  [  ipgui::add_param $IPINST -name "SDA_DEL_LEN" -parent ${Page_0} -display_name {Sda Del Len}]
  set_property tooltip {Sda Del Len} ${SDA_DEL_LEN}
  set SCL_DEL_LEN  [  ipgui::add_param $IPINST -name "SCL_DEL_LEN" -parent ${Page_0} -display_name {Scl Del Len}]
  set_property tooltip {Scl Del Len} ${SCL_DEL_LEN}
  set DEB_I2C_LEN  [  ipgui::add_param $IPINST -name "DEB_I2C_LEN" -parent ${Page_0} -display_name {Deb I2c Len}]
  set_property tooltip {Deb I2c Len} ${DEB_I2C_LEN}
  set I2C_ADDRESS  [  ipgui::add_param $IPINST -name "I2C_ADDRESS" -parent ${Page_0} -display_name {I2c Address}]
  set_property tooltip {I2c Address} ${I2C_ADDRESS}


}

proc update_PARAM_VALUE.IIC_SLAVE_ADDR_WIDTH { PARAM_VALUE.IIC_SLAVE_ADDR_WIDTH } {
	# Procedure called to update IIC_SLAVE_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.IIC_SLAVE_ADDR_WIDTH { PARAM_VALUE.IIC_SLAVE_ADDR_WIDTH } {
	# Procedure called to validate IIC_SLAVE_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.IIC_SLAVE_DATA_WIDTH { PARAM_VALUE.IIC_SLAVE_DATA_WIDTH } {
	# Procedure called to update IIC_SLAVE_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.IIC_SLAVE_DATA_WIDTH { PARAM_VALUE.IIC_SLAVE_DATA_WIDTH } {
	# Procedure called to validate IIC_SLAVE_DATA_WIDTH
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

proc update_MODELPARAM_VALUE.IIC_SLAVE_DATA_WIDTH { MODELPARAM_VALUE.IIC_SLAVE_DATA_WIDTH PARAM_VALUE.IIC_SLAVE_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.IIC_SLAVE_DATA_WIDTH}] ${MODELPARAM_VALUE.IIC_SLAVE_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.IIC_SLAVE_ADDR_WIDTH { MODELPARAM_VALUE.IIC_SLAVE_ADDR_WIDTH PARAM_VALUE.IIC_SLAVE_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.IIC_SLAVE_ADDR_WIDTH}] ${MODELPARAM_VALUE.IIC_SLAVE_ADDR_WIDTH}
}

