# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
	set Component_Name [ipgui::add_param $IPINST -name Component_Name]
	set Page0 [ipgui::add_page $IPINST -name "Page 0" -layout vertical]
}


