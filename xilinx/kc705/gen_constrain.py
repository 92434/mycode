# -*- coding: utf-8 -*-
import re

def kc705_pins_key(x):
	#print x
	info = x[3]
	pattern = re.compile(r'([^\d]+)(\d+)')
	g = pattern.match(info)
	r = g.groups()
	return r[0], int(r[1])

def gen_list_kc705_net_group_part_pin():
	content = ''

	with open('kc705_signals.txt') as f:
		content = f.read()
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n\s*([^\-\s]+)-([^\.]+)\.([^\s]+)')
	result = pattern.findall(content)
	list_kc705_net_group_part_pin = sorted(result, key = lambda x:(kc705_pins_key(x)))
	list_kc705_net_group_part_pin.append(('FMC_LPC_PRSNT_M2C_B_LS', 'J2', 'E', 'H2'))
	list_kc705_net_group_part_pin.append(('FMC_HPC_PRSNT_M2C_B_LS', 'J22', 'I', 'H2'))
	list_kc705_net_group_part_pin.append(('FMC_HPC_PG_M2C_LS', 'J22', 'F', 'F1'))
	list_kc705_net_group_part_pin.append(('FMC_C2M_PG_LS', 'J22', 'D', 'D1'))#not valid
	print '-' * 100
	print 'list_kc705_net_group_part_pin info'
	print '-' * 100
	for i in list_kc705_net_group_part_pin:
		print i
	print 'total:', len(list_kc705_net_group_part_pin)
	return list_kc705_net_group_part_pin

def fmc_pins_key(x):
	#print x
	info = x[5]
	pattern = re.compile(r'([^\d]+)(\d+)')
	g = pattern.match(info)
	r = g.groups()
	return r[0], int(r[1])

def gen_list_fmc_pin_resistor():
	content = ''

	with open('kc705FMC.txt') as f:
		content = f.read()
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n(R[^\.]+)\.(\d+)\s+([^\-\s]+)-([^\.]+)\.([^\s]+)')
	result = pattern.findall(content)
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n([^\-\s]+)-([^\.]+)\.([^\s]+)\s+(R[^\.]+)\.(\d+)')
	result.extend([(x[0], x[4], x[5], x[1], x[2], x[3]) for x in pattern.findall(content)])
	fmc_pins = sorted(result, key = lambda x:(fmc_pins_key(x)))
	list_fmc_pin_resistor = []
	for i in fmc_pins:
		list_fmc_pin_resistor.append((i[5], i[1] + '.' + i[2]))
	list_fmc_pin_resistor.append(('F1', 'R113.1'))
	list_fmc_pin_resistor.append(('H2', 'R62.1'))
	print '-' * 100
	print 'list_fmc_pin_resistor info'
	print '-' * 100
	for i in list_fmc_pin_resistor:
		print i
	print 'total:', len(list_fmc_pin_resistor)
	return list_fmc_pin_resistor

def gen_map_kc705_pin_net():
	map_kc705_pin_net = {}
	lines = []
	with open('kc705_package_pin_nets.txt') as f:
		lines = f.read().splitlines()

	for i in lines:
		l = i.split()
		item = {l[0] : l[1]}
		if len(l) == 2:
			map_kc705_pin_net.update(item)

	print '-' * 100
	print 'map_kc705_pin_net info'
	print '-' * 100
	for i in map_kc705_pin_net.items():
		print i
	print 'total:', len(map_kc705_pin_net)

	return map_kc705_pin_net

def gen_map_kc705_pin_iotype():
	map_kc705_pin_iotype = {}
	lines = []
	with open('kc705_io_package_pins.txt') as f:
		lines = f.read().splitlines()

	for i in lines:
		l = i.split()
		item = {l[0] : l[1]}
		if len(l) == 8:
			map_kc705_pin_iotype.update(item)

	print '-' * 100
	print 'map_kc705_pin_iotype info'
	print '-' * 100
	for i in map_kc705_pin_iotype.items():
		print i
	print 'total:', len(map_kc705_pin_iotype)

	return map_kc705_pin_iotype

def remove_unused_pin(map_kc705_pin_net):
	list_unsupport_pin_net = []
	unsupport_pin = [
		'F2',
		'F5',
		'F6',
		'C8',
		'C3',
		'C7',
		'C4',
		'F1',
		'B5',
		'B6',
		'B1',
		'B2',
		'E8',
		'E4',
		'E7',
		'E3',
		'N8',
		'N7',
		'A3',
		'A4',
		'A7',
		'A8',
		'D6',
		'D5',
		'D2',
		'D1',
	]
	
	for i in unsupport_pin:
		if i in map_kc705_pin_net.keys():
			net = map_kc705_pin_net.pop(i)
			item = (i, net)
			list_unsupport_pin_net.append(item)
	print '-' * 100
	print 'list_unsupport_pin_net info'
	print '-' * 100
	for i in list_unsupport_pin_net:
		print i
	print 'total:', len(list_unsupport_pin_net)

def get_list_ip_net_pin_port_des(map_kc705_pin_net):
	list_ip_net_pin_port_des = []

	map_extra_net_property_port_property = {}
	list_net_port = []
	map_extra_net_property_net_property = {}
	list_pin_port = []

	#map_extra_net_property_port_property = {
	#	'mpeg_clk': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	'fs_0p5_en': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'symbol_2x_oe' : ['slew FAST'],

	#	'symbol_2x_re_out[0]' : ['slew FAST'],
	#	'symbol_2x_re_out[1]' : ['slew FAST'],
	#	'symbol_2x_re_out[2]' : ['slew FAST'],
	#	'symbol_2x_re_out[3]' : ['slew FAST'],
	#	'symbol_2x_re_out[4]' : ['slew FAST'],
	#	'symbol_2x_re_out[5]' : ['slew FAST'],
	#	'symbol_2x_re_out[6]' : ['slew FAST'],
	#	'symbol_2x_re_out[7]' : ['slew FAST'],
	#	'symbol_2x_re_out[8]' : ['slew FAST'],
	#	'symbol_2x_re_out[9]' : ['slew FAST'],
	#	'symbol_2x_re_out[10]' : ['slew FAST'],
	#	'symbol_2x_re_out[11]' : ['slew FAST'],
	#	'symbol_2x_re_out[12]' : ['slew FAST'],
	#	'symbol_2x_re_out[13]' : ['slew FAST'],
	#	'symbol_2x_re_out[14]' : ['slew FAST'],
	#	'symbol_2x_re_out[15]' : ['slew FAST'],

	#	'symbol_2x_im_out[0]' : ['slew FAST'],
	#	'symbol_2x_im_out[1]' : ['slew FAST'],
	#	'symbol_2x_im_out[2]' : ['slew FAST'],
	#	'symbol_2x_im_out[3]' : ['slew FAST'],
	#	'symbol_2x_im_out[4]' : ['slew FAST'],
	#	'symbol_2x_im_out[5]' : ['slew FAST'],
	#	'symbol_2x_im_out[6]' : ['slew FAST'],
	#	'symbol_2x_im_out[7]' : ['slew FAST'],
	#	'symbol_2x_im_out[8]' : ['slew FAST'],
	#	'symbol_2x_im_out[9]' : ['slew FAST'],
	#	'symbol_2x_im_out[10]' : ['slew FAST'],
	#	'symbol_2x_im_out[11]' : ['slew FAST'],
	#	'symbol_2x_im_out[12]' : ['slew FAST'],
	#	'symbol_2x_im_out[13]' : ['slew FAST'],
	#	'symbol_2x_im_out[14]' : ['slew FAST'],
	#	'symbol_2x_im_out[15]' : ['slew FAST'],
	#}
	#
	#list_net_port = [
	#	('FMC_LPC_LA25_P', 'mpeg_clk'),
	#	('FMC_LPC_LA29_P', 'mpeg_valid'),
	#	('FMC_LPC_LA24_P', 'mpeg_data[0]'),
	#	('FMC_LPC_LA24_N', 'mpeg_data[1]'),
	#	('FMC_LPC_LA26_P', 'mpeg_data[2]'),
	#	('FMC_LPC_LA31_P', 'mpeg_data[3]'),
	#	('FMC_LPC_LA31_N', 'mpeg_data[4]'),
	#	('FMC_LPC_LA28_P', 'mpeg_data[5]'),
	#	('FMC_LPC_LA28_N', 'mpeg_data[6]'),
	#	('FMC_LPC_LA29_N', 'mpeg_data[7]'),
	#	('FMC_LPC_LA25_N', 'mpeg_sync'),

	#	#('FMC_HPC_HA23_N', 'ts_out_sync'),
	#	#('FMC_HPC_HA23_P', 'slot0_out_dump_flag'),

	#	('FMC_LPC_LA10_P', 'symbol_2x_oe'),
	#		
	#	('FMC_LPC_LA00_CC_P', 'symbol_2x_re_out[0]'),
	#	('FMC_LPC_LA02_P', 'symbol_2x_re_out[1]'),
	#	('FMC_LPC_LA00_CC_N', 'symbol_2x_re_out[2]'),
	#	('FMC_LPC_LA02_N', 'symbol_2x_re_out[3]'),
	#	('FMC_LPC_LA03_P', 'symbol_2x_re_out[4]'),
	#	('FMC_LPC_LA04_P', 'symbol_2x_re_out[5]'),
	#	('FMC_LPC_LA03_N', 'symbol_2x_re_out[6]'),
	#	('FMC_LPC_LA04_N', 'symbol_2x_re_out[7]'),
	#	('FMC_LPC_LA07_P', 'symbol_2x_re_out[8]'),
	#	('FMC_LPC_LA08_P', 'symbol_2x_re_out[9]'),
	#	('FMC_LPC_LA01_CC_P', 'symbol_2x_re_out[10]'),
	#	('FMC_LPC_LA01_CC_N', 'symbol_2x_re_out[11]'),
	#	('FMC_LPC_LA06_P', 'symbol_2x_re_out[12]'),
	#	('FMC_LPC_LA06_N', 'symbol_2x_re_out[13]'),
	#	('FMC_LPC_LA05_P', 'symbol_2x_re_out[14]'),
	#	('FMC_LPC_LA05_N', 'symbol_2x_re_out[15]'),

	#	('FMC_LPC_LA10_N', 'symbol_2x_im_out[0]'),
	#	('FMC_LPC_LA09_P', 'symbol_2x_im_out[1]'),
	#	('FMC_LPC_LA09_N', 'symbol_2x_im_out[2]'),
	#	('FMC_LPC_LA13_P', 'symbol_2x_im_out[3]'),
	#	('FMC_LPC_LA14_P', 'symbol_2x_im_out[4]'),
	#	('FMC_LPC_LA13_N', 'symbol_2x_im_out[5]'),
	#	('FMC_LPC_LA14_N', 'symbol_2x_im_out[6]'),
	#	('FMC_LPC_LA07_N', 'symbol_2x_im_out[7]'),
	#	('FMC_LPC_LA08_N', 'symbol_2x_im_out[8]'),
	#	('FMC_LPC_LA12_P', 'symbol_2x_im_out[9]'),
	#	('FMC_LPC_LA11_P', 'symbol_2x_im_out[10]'),
	#	('FMC_LPC_LA12_N', 'symbol_2x_im_out[11]'),
	#	('FMC_LPC_LA11_N', 'symbol_2x_im_out[12]'),
	#	('FMC_LPC_LA16_P', 'symbol_2x_im_out[13]'),
	#	('FMC_LPC_LA16_N', 'symbol_2x_im_out[14]'),
	#	('FMC_LPC_LA15_P', 'symbol_2x_im_out[15]'),

	#	('FMC_LPC_LA22_N', 'clk_out2'),
	#	('FMC_LPC_LA21_P', 'clk_out3'),

	#	('FMC_LPC_LA20_P', 'clk_out4'),

	#	('FMC_LPC_LA32_N', 'asi_out_p'),
	#	('FMC_LPC_LA33_N', 'asi_out_n'),

	#	('FMC_LPC_CLK0_M2C_P', 'fs_0p5_en'),

	#	#('XADC_GPIO_0', 'clk_out1'),
	#	#('USER_SMA_GPIO_P', 'asi_out'),
	#	#('LCD_DB4_LS', 'asi_out'),
	#	#('GPIO_SW_E', 'asi_out'),
	#	('LCD_E_LS', 'lcm_din'),
	#	('LCD_RS_LS', 'lcm_lp'),
	#	('LCD_RW_LS', 'lcm_xscl'),
	#	('XADC_GPIO_0', 'lcm_data[0]'),
	#	('XADC_GPIO_1', 'lcm_data[1]'),
	#	('XADC_GPIO_2', 'lcm_data[2]'),
	#	('XADC_GPIO_3', 'lcm_data[3]'),
	#	('LCD_DB4_LS', 'lcm_data[4]'),
	#	('LCD_DB5_LS', 'lcm_data[5]'),
	#	('LCD_DB6_LS', 'lcm_data[6]'),
	#	('LCD_DB7_LS', 'lcm_data[7]'),
	#]

	#map_extra_net_property_port_property = {
	#	'iic_scl': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#}

	#list_net_port = [
	#	('FMC_HPC_LA28_P', 'iic_scl'),
	#	('FMC_HPC_LA21_P', 'iic_sda'),
	#]

	#map_extra_net_property_port_property = {
	#	'bclk[0]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[0]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[0]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[1]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[1]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[1]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[2]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[2]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[2]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[3]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[3]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[3]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[4]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[4]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[4]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[5]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[5]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[5]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[6]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[6]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[6]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[7]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[7]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[7]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[8]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[8]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[8]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[9]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[9]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[9]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[10]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[10]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[10]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[11]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[11]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[11]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[12]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[12]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[12]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[13]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[13]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[13]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[14]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[14]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[14]': ['CLOCK_DEDICATED_ROUTE FALSE'],

	#	'bclk[15]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'lrclk[15]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#	#'sdata[15]': ['CLOCK_DEDICATED_ROUTE FALSE'],
	#}


	map_extra_net_property_port_property = {
		'mpeg_clk': ['CLOCK_DEDICATED_ROUTE FALSE'],
		'fs_0p5_en': ['CLOCK_DEDICATED_ROUTE FALSE'],
	}

	#list_net_port = [
	#	('FMC_HPC_HA00_CC_P', 'mpeg_clk'),
	#	('FMC_HPC_HA01_CC_P', 'mpeg_sync'),
	#	('FMC_HPC_HA05_N', 'mpeg_valid'),
	#	('FMC_HPC_HA00_CC_N', 'mpeg_data[0]'),
	#	('FMC_HPC_HA01_CC_N', 'mpeg_data[1]'),
	#	('FMC_HPC_HA02_P', 'mpeg_data[2]'),
	#	('FMC_HPC_HA04_P', 'mpeg_data[3]'),
	#	('FMC_HPC_HA03_N', 'mpeg_data[4]'),
	#	('FMC_HPC_HA04_N', 'mpeg_data[5]'),
	#	('FMC_HPC_HA05_P', 'mpeg_data[6]'),
	#	('FMC_HPC_HA06_P', 'mpeg_data[7]'),

	#	('FMC_HPC_HA10_P', 'asi_out_p'),
	#	('FMC_HPC_HA10_N', 'asi_out_n'),

	#	('FMC_LPC_CLK0_M2C_P', 'fs_0p5_en'),

	#	('FMC_LPC_LA10_P', 'symbol_2x_oe'),
	#      
	#	('FMC_LPC_LA00_CC_P', 'symbol_2x_re_out[0]'),
	#	('FMC_LPC_LA02_P', 'symbol_2x_re_out[1]'),
	#	('FMC_LPC_LA00_CC_N', 'symbol_2x_re_out[2]'),
	#	('FMC_LPC_LA02_N', 'symbol_2x_re_out[3]'),
	#	('FMC_LPC_LA03_P', 'symbol_2x_re_out[4]'),
	#	('FMC_LPC_LA04_P', 'symbol_2x_re_out[5]'),
	#	('FMC_LPC_LA03_N', 'symbol_2x_re_out[6]'),
	#	('FMC_LPC_LA04_N', 'symbol_2x_re_out[7]'),
	#	('FMC_LPC_LA07_P', 'symbol_2x_re_out[8]'),
	#	('FMC_LPC_LA08_P', 'symbol_2x_re_out[9]'),
	#	('FMC_LPC_LA01_CC_P', 'symbol_2x_re_out[10]'),
	#	('FMC_LPC_LA01_CC_N', 'symbol_2x_re_out[11]'),
	#	('FMC_LPC_LA06_P', 'symbol_2x_re_out[12]'),
	#	('FMC_LPC_LA06_N', 'symbol_2x_re_out[13]'),
	#	('FMC_LPC_LA05_P', 'symbol_2x_re_out[14]'),
	#	('FMC_LPC_LA05_N', 'symbol_2x_re_out[15]'),

	#	('FMC_LPC_LA10_N', 'symbol_2x_im_out[0]'),
	#	('FMC_LPC_LA09_P', 'symbol_2x_im_out[1]'),
	#	('FMC_LPC_LA09_N', 'symbol_2x_im_out[2]'),
	#	('FMC_LPC_LA13_P', 'symbol_2x_im_out[3]'),
	#	('FMC_LPC_LA14_P', 'symbol_2x_im_out[4]'),
	#	('FMC_LPC_LA13_N', 'symbol_2x_im_out[5]'),
	#	('FMC_LPC_LA14_N', 'symbol_2x_im_out[6]'),
	#	('FMC_LPC_LA07_N', 'symbol_2x_im_out[7]'),
	#	('FMC_LPC_LA08_N', 'symbol_2x_im_out[8]'),
	#	('FMC_LPC_LA12_P', 'symbol_2x_im_out[9]'),
	#	('FMC_LPC_LA11_P', 'symbol_2x_im_out[10]'),
	#	('FMC_LPC_LA12_N', 'symbol_2x_im_out[11]'),
	#	('FMC_LPC_LA11_N', 'symbol_2x_im_out[12]'),
	#	('FMC_LPC_LA16_P', 'symbol_2x_im_out[13]'),
	#	('FMC_LPC_LA16_N', 'symbol_2x_im_out[14]'),
	#	('FMC_LPC_LA15_P', 'symbol_2x_im_out[15]'),
	#]


	#list_pin_port = [
	#	('AG22', 'bclk[0]'),
	#	('AH22', 'lrclk[0]'),
	#	('AD23', 'sdata[0]'),
	#	('AE24', 'bclk[1]'),
	#	('AC22', 'lrclk[1]'),
	#	('AD22', 'sdata[1]'),
	#	('AF20', 'bclk[2]'),
	#	('AF21', 'lrclk[2]'),
	#	('AG20', 'sdata[2]'),
	#	('AH20', 'bclk[3]'),
	#	('AK20', 'lrclk[3]'),
	#	('AK21', 'sdata[3]'),
	#	('AE23', 'bclk[4]'),
	#	('AF23', 'lrclk[4]'),
	#	('AB24', 'sdata[4]'),
	#	('AC25', 'bclk[5]'),
	#	('AK23', 'lrclk[5]'),
	#	('AK24', 'sdata[5]'),
	#	('Y30', 'bclk[6]'),
	#	('AA30', 'lrclk[6]'),
	#	('AB29', 'sdata[6]'),
	#	('AB30', 'bclk[7]'),
	#	('AC29', 'lrclk[7]'),
	#	('AC30', 'sdata[7]'),
	#	('AB27', 'bclk[8]'),
	#	('AC27', 'lrclk[8]'),
	#	('AD29', 'sdata[8]'),
	#	('AE29', 'bclk[9]'),
	#	('AE30', 'lrclk[9]'),
	#	('AF30', 'sdata[9]'),
	#	('AE28', 'bclk[10]'),
	#	('AF28', 'lrclk[10]'),
	#	('AG30', 'sdata[10]'),
	#	('AH30', 'bclk[11]'),
	#	('AK29', 'lrclk[11]'),
	#	('AK30', 'sdata[11]'),
	#	('AJ28', 'bclk[12]'),
	#	('AJ29', 'lrclk[12]'),
	#	('AG27', 'sdata[12]'),
	#	('AG28', 'bclk[13]'),
	#	('AH26', 'lrclk[13]'),
	#	('AH27', 'sdata[13]'),
	#	('AJ27', 'bclk[14]'),
	#	('AK28', 'lrclk[14]'),
	#	('AJ26', 'sdata[14]'),
	#	('AK26', 'bclk[15]'),
	#	('AF26', 'lrclk[15]'),
	#	('AF27', 'sdata[15]'),
	#]

	list_pin_port = [
		('D12', 'mpeg_clk'),
		('H14', 'mpeg_sync'),
		('E16', 'mpeg_valid'),
		('D13', 'mpeg_data[0]'),
		('G14', 'mpeg_data[1]'),
		('D11', 'mpeg_data[2]'),
		('F11', 'mpeg_data[3]'),
		('B12', 'mpeg_data[4]'),
		('E11', 'mpeg_data[5]'),
		('F15', 'mpeg_data[6]'),
		('D14', 'mpeg_data[7]'),

		('A11', 'asi_out_p'),
		('A12', 'asi_out_n'),

		('C29', 'fs_0p5_en'),

		('H25', 'symbol_2x_oe'),
	      
		('H24', 'symbol_2x_re_out[0]'),
		('H26', 'symbol_2x_re_out[1]'),
		('B25', 'symbol_2x_re_out[2]'),
		('C26', 'symbol_2x_re_out[3]'),
		('C25', 'symbol_2x_re_out[4]'),
		('D26', 'symbol_2x_re_out[5]'),
		('D18', 'symbol_2x_re_out[6]'),
		('C27', 'symbol_2x_re_out[7]'),
		('D17', 'symbol_2x_re_out[8]'),
		('D27', 'symbol_2x_re_out[9]'),
		('K19', 'symbol_2x_re_out[10]'),
		('K20', 'symbol_2x_re_out[11]'),
		('L17', 'symbol_2x_re_out[12]'),
		('L18', 'symbol_2x_re_out[13]'),
		('J19', 'symbol_2x_re_out[14]'),
		('H19', 'symbol_2x_re_out[15]'),

		('C30', 'symbol_2x_im_out[0]'),
		('F27', 'symbol_2x_im_out[1]'),
		('D29', 'symbol_2x_im_out[2]'),
		('G27', 'symbol_2x_im_out[3]'),
		('E30', 'symbol_2x_im_out[4]'),
		('A30', 'symbol_2x_im_out[5]'),
		('E29', 'symbol_2x_im_out[6]'),
		('B30', 'symbol_2x_im_out[7]'),
		('G30', 'symbol_2x_im_out[8]'),
		('D28', 'symbol_2x_im_out[9]'),
		('H30', 'symbol_2x_im_out[10]'),
		('E28', 'symbol_2x_im_out[11]'),
		('F28', 'symbol_2x_im_out[12]'),
		('F30', 'symbol_2x_im_out[13]'),
		('G28', 'symbol_2x_im_out[14]'),
		('G29', 'symbol_2x_im_out[15]'),
	]

	for pin, port in list_pin_port:
		list_extra_des = []
		list_extra_net_property_property = map_extra_net_property_port_property.get(port, None)
		if list_extra_net_property_property:
			for i in list_extra_net_property_property:
				list_extra_des.append('set_property %s [get_nets {%s}]' %(i, port))

		net = map_kc705_pin_net.pop(pin, None)
		if net:
			item = (net, pin, port, list_extra_des)
			list_ip_net_pin_port_des.append(item)
		else:
			print '(%s, %s) is not in map_kc705_pin_net!' %(pin, port)
	
	for net, port in list_net_port:
		list_extra_des = []

		list_extra_net_property_property = map_extra_net_property_port_property.get(port)
		if list_extra_net_property_property:
			for i in list_extra_net_property_property:
				list_extra_des.append('set_property %s [get_nets {%s}]' %(i, port))

		for i, j in map_kc705_pin_net.items():
			if j == net:
				pin = i
				map_kc705_pin_net.pop(pin, None)

				item = (net, pin, port, list_extra_des)
				list_ip_net_pin_port_des.append(item)

	print '-' * 100
	print 'list_ip_net_pin_port_des info'
	print '-' * 100
	for i in list_ip_net_pin_port_des:
		print i
	print 'total:', len(list_ip_net_pin_port_des)

	return list_ip_net_pin_port_des

def get_map_gpio_if_list_net_pin_des_resistor(map_kc705_pin_net, list_kc705_net_group_part_pin, list_fmc_pin_resistor):
	map_gpio_if_list_net_pin_des_resistor = {}
	map_gpio_if_list_net_pin_des_resistor.update({'HPC': []})
	map_gpio_if_list_net_pin_des_resistor.update({'LPC': []})
	map_gpio_if_list_net_pin_des_resistor.update({'OTHER': []})

	list_pin_des = []
	list_net_des = []

	#list_pin_des = [
	#	('AD26', 'SOMI'),
	#	('AC26', 'MOSI'),
	#	('AE25', 'SCLK'),
	#	('AF25', '74138GA(CS)'),
	#	('AD21', '74138GB'),
	#	('AD24', '74138GC'),
	#	('AJ24', 'SPI_S0'),
	#	('AK25', 'SPI_S1'),
	#	('AJ22', 'SPI_S2'),
	#	('AJ23', 'SPI_S3'),
	#]

	list_pin_des = [
		('C12', 'i2c_sck'),
		('C11', 'i2c_sda'),

		('A23', 'spi_clk'),
		('D23', 'spi_mosi'),
		('E25', 'spi_miso'),

		('F26', '74138G2A'),
		('E23', 'spi_s0'),
		('F25', 'spi_s1'),
		('E24', 'spi_s2'),

		('C14', 'lnb1_on_off'),
		('E14', 'TUNB_3.3V_ON'),
		('AF27', 'AD9125_INTB'),
		('AH29', 'AD5375_DSOP'),

		('E15', 'undefined'),
		('C15', 'undefined'),
		('B15', 'undefined'),
		('J16', 'undefined'),
		('B14', 'undefined'),
		('A15', 'undefined'),
		('F12', 'undefined'),
		('E13', 'undefined'),
		('B13', 'undefined'),
		('A13', 'undefined'),
		('L16', 'undefined'),
		('K16', 'undefined'),
		('H16', 'undefined'),
	]

	#list_net_des = [
	#	('FMC_HPC_LA30_P', 'master_scl'),
	#	('FMC_HPC_LA24_P', 'master_sda'),
	#]
	#list_net_des = [
	#	('FMC_HPC_HA03_P', 'i2c_sck'),
	#	('FMC_HPC_HA02_N', 'i2c_sda'),

	#	('FMC_LPC_LA21_N', 'spi_clk'),
	#	('FMC_LPC_LA19_P', 'spi_mosi'),
	#	('FMC_LPC_LA19_N', 'spi_miso'),

	#	('FMC_LPC_LA22_P', '74138G2A'),
	#	('FMC_LPC_LA15_N', 'spi_s0'),
	#	('FMC_LPC_CLK0_M2C_N', 'spi_s1'),
	#	('FMC_LPC_PRSNT_M2C_B_LS', 'spi_s2'),

	#	('FMC_HPC_HA06_N', 'lnb1_on_off'),
	#	('FMC_HPC_HA08_P', 'TUNB_3.3V_ON'),
	#	('FMC_LPC_LA20_N', 'AD9125_INTB'),
	#	('FMC_LPC_CLK1_M2C_N', 'AD5375_DSOP'),

	#	('FMC_HPC_HA08_N', 'undefined'),
	#	('FMC_HPC_HA12_P', 'undefined'),
	#	('FMC_HPC_HA12_N', 'undefined'),
	#	('FMC_HPC_HA14_P', 'undefined'),
	#	('FMC_HPC_HA07_P', 'undefined'),
	#	('FMC_HPC_HA07_N', 'undefined'),
	#	('FMC_HPC_HA09_P', 'undefined'),
	#	('FMC_HPC_HA09_N', 'undefined'),
	#	('FMC_HPC_HA11_P', 'undefined'),
	#	('FMC_HPC_HA11_N', 'undefined'),
	#	('FMC_HPC_HA13_P', 'undefined'),
	#	('FMC_HPC_HA13_N', 'undefined'),
	#	('FMC_HPC_HA14_N', 'undefined'),
	#]

	for pin, des in list_pin_des:
	#des = None
	#for pin, net in map_kc705_pin_net.items():
		net = map_kc705_pin_net.pop(pin, None)
		if net:
			resistor = 'undefined'
			for i, group, part, j in list_kc705_net_group_part_pin:
				if net == i:
					if group in ['J2', 'J22']:
						for k, l in list_fmc_pin_resistor:
							if j == k:
								resistor = l
			v = (net, pin, des, resistor)
			if net.startswith('FMC_HPC'):
				map_gpio_if_list_net_pin_des_resistor.get('HPC').append(v)
			elif net.startswith('FMC_LPC'):
				map_gpio_if_list_net_pin_des_resistor.get('LPC').append(v)
			else:
				map_gpio_if_list_net_pin_des_resistor.get('OTHER').append(v)

	for net, des in list_net_des:
		for i, j in map_kc705_pin_net.items():
			if j == net:
				map_kc705_pin_net.pop(i, None)
				resistor = 'undefined'
				pin = i
				for k, group, part, l in list_kc705_net_group_part_pin:
					if net == k:
						if group in ['J2', 'J22']:
							for m, n in list_fmc_pin_resistor:
								if l == m:
									resistor = n
				v = (net, pin, des, resistor)
				if net.startswith('FMC_HPC'):
					map_gpio_if_list_net_pin_des_resistor.get('HPC').append(v)
				elif net.startswith('FMC_LPC'):
					map_gpio_if_list_net_pin_des_resistor.get('LPC').append(v)
				else:
					map_gpio_if_list_net_pin_des_resistor.get('OTHER').append(v)

	for i in map_gpio_if_list_net_pin_des_resistor.items():
		print '-' * 100
		print 'map_gpio_if_list_net_pin_des_resistor %s info' %(i[0])
		print '-' * 100
		for j in i[1]:
			print j
		print 'total:', len(i[1])

	return map_gpio_if_list_net_pin_des_resistor

def gen_default_contrain():
	txt = """
# Sys Clock Pins
#set_property PACKAGE_PIN AD11 [get_ports MIG_SYS_CLK_clk_n]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports MIG_SYS_CLK_clk_n]

#set_property PACKAGE_PIN AD12 [get_ports MIG_SYS_CLK_clk_p]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports MIG_SYS_CLK_clk_p]

# Sys Reset Pins
set_property PACKAGE_PIN AB7 [get_ports reset]
set_property IOSTANDARD LVCMOS15 [get_ports reset]

# PCIe Refclk Pins
set_property PACKAGE_PIN U8 [get_ports EXT_PCIE_REFCLK_P]
set_property PACKAGE_PIN U7 [get_ports EXT_PCIE_REFCLK_N]

# PCIe TX RX Pins
#set_property PACKAGE_PIN M6 [get_ports {EXT_PCIE_rxp[0]}]
#set_property PACKAGE_PIN M5 [get_ports {EXT_PCIE_rxn[0]}]
#set_property PACKAGE_PIN P6 [get_ports {EXT_PCIE_rxp[1]}]
#set_property PACKAGE_PIN P5 [get_ports {EXT_PCIE_rxn[1]}]
#set_property PACKAGE_PIN R4 [get_ports {EXT_PCIE_rxp[2]}]
#set_property PACKAGE_PIN R3 [get_ports {EXT_PCIE_rxn[2]}]
#set_property PACKAGE_PIN T6 [get_ports {EXT_PCIE_rxp[3]}]
#set_property PACKAGE_PIN T5 [get_ports {EXT_PCIE_rxn[3]}]
#set_property PACKAGE_PIN L4 [get_ports {EXT_PCIE_txp[0]}]
#set_property PACKAGE_PIN L3 [get_ports {EXT_PCIE_txn[0]}]
#set_property PACKAGE_PIN M2 [get_ports {EXT_PCIE_txp[1]}]
#set_property PACKAGE_PIN M1 [get_ports {EXT_PCIE_txn[1]}]
#set_property PACKAGE_PIN N4 [get_ports {EXT_PCIE_txp[2]}]
#set_property PACKAGE_PIN N3 [get_ports {EXT_PCIE_txn[2]}]
#set_property PACKAGE_PIN P2 [get_ports {EXT_PCIE_txp[3]}]
#set_property PACKAGE_PIN P1 [get_ports {EXT_PCIE_txn[3]}]

# LED Pins
set_property PACKAGE_PIN AB8 [get_ports {EXT_LEDS[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[0]}]

set_property PACKAGE_PIN AA8 [get_ports {EXT_LEDS[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[1]}]

set_property PACKAGE_PIN AC9 [get_ports {EXT_LEDS[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[2]}]

set_property PACKAGE_PIN AB9 [get_ports {EXT_LEDS[3]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[3]}]

set_property PACKAGE_PIN AE26 [get_ports {EXT_LEDS[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[4]}]

set_property PACKAGE_PIN G19 [get_ports {EXT_LEDS[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[5]}]

set_property PACKAGE_PIN E18 [get_ports {EXT_LEDS[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[6]}]

set_property PACKAGE_PIN F16 [get_ports {EXT_LEDS[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {EXT_LEDS[7]}]
	"""

	print '#', '-' * 100
	print '#', 'kc705 default constrain'
	print '#', '-' * 100
	print txt

def gen_ip_constrain(list_ip_net_pin_port_des):
	print '#', '-' * 100
	print '#', 'ip constrain'
	print '#', '-' * 100

	for net, pin, port, des in list_ip_net_pin_port_des:
		print '\n#%s\nset_property PACKAGE_PIN %s [get_ports {%s}]' %(net, pin, port)
		print 'set_property IOSTANDARD LVCMOS25 [get_ports {%s}]' %(port)
		if des:
			for i in des:
				print i

def gen_gpio_constrain(map_gpio_if_list_net_pin_des_resistor):
	list_net_pin_des_resistor_gpio_gpio_no = []
	start = 0
	top_pin_no = 256
	gpio_no = 0

	list_gpio_groups = [
		'gpio_tri_io',
		'gpio2_tri_io',
		'gpio_1_tri_io',
		'gpio2_1_tri_io',
		'gpio_2_tri_io',
		'gpio2_2_tri_io',
		'gpio_3_tri_io',
		'gpio2_3_tri_io',
		'gpio_4_tri_io',
		'gpio2_4_tri_io',
	]

	list_gpio_ports = []
	for i in list_gpio_groups:
		for j in range(32):
			list_gpio_ports.append(i + '[' + str(j) + ']')

	print '#', '-' * 100
	print '#', 'gpio constrain'
	print '#', '-' * 100

	common_list_net_pin_des_resistor = []
	for i, j in map_gpio_if_list_net_pin_des_resistor.items():
		common_list_net_pin_des_resistor.extend(j)
	map_common_list_net_pin_des_resistor = {}
	map_common_list_net_pin_des_resistor.update({'common gpio' : common_list_net_pin_des_resistor})

	#for i, j in map_gpio_if_list_net_pin_des_resistor.items():
	for i, j in map_common_list_net_pin_des_resistor.items():
		list_len = len(j)

		if list_len == 0:
			continue

		list_if_gpio_ports = list_gpio_ports[start : ]

		if len(list_if_gpio_ports) < list_len:
			#print , "gpio bank is not enough!"
			continue

		#print '#', '-' * 100
		#print '#', 'gpio constrain for %s' %(i)
		#print '#', '-' * 100

		base_pin_no = 0

		for k in range(list_len):
			bank = k / 32
			bank_gpio_num = 32 if (list_len - bank * 32 >= 32) else list_len - bank * 32
			base_pin_no = top_pin_no - bank * 32 - bank_gpio_num
			gpio_no = base_pin_no + k % 32
			net, pin, des, resistor = j[k]
			gpio = list_if_gpio_ports[k]
			item = (net, pin, des, resistor, gpio, gpio_no)
			list_net_pin_des_resistor_gpio_gpio_no.append(item)
			print '\n#%s, %s, %s, %d\nset_property PACKAGE_PIN %s [get_ports {%s}]' %(net, des, resistor, gpio_no, pin, gpio)
			print 'set_property IOSTANDARD LVCMOS25 [get_ports {%s}]' %(gpio)

		top_pin_no = base_pin_no

		start += list_len
		#if (start % 32) != 0:
		#	start = 32 * ((start / 32) + 1)

	return list_net_pin_des_resistor_gpio_gpio_no

def gen_bitstream_constrain():
	txt = """
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 40 [current_design]
set_property BITSTREAM.CONFIG.SPI_FALL_EDGE YES [current_design]

##Encryption Settings
#set_property BITSTREAM.ENCRYPTION.ENCRYPT YES [current_design]
#set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT BBRAM [current_design]
##set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT eFUSE [current_design]
#set_property BITSTREAM.ENCRYPTION.KEY0 256'h12345678ABCDDCBA1234578ABCDDCBA1234578ABCDDCBA1234578ABCDDCBA [current_design]
	"""

	print '#', '-' * 100
	print '#', 'bitstream constrain'
	print '#', '-' * 100
	print txt

def gen_gpio_test_list(list_net_pin_des_resistor_gpio_gpio_no):
	fmc_board_pins = '''
	149 148 147 146 145 144 143 142 141 140 139 138
	84 83 137 136 135 134 133 132 131 130 129 128 127 126
	61 59 82 81 80 79 78 77 75 73 71 69 67 65 63
	86 53 58 57 56 55 54 76 74 72 70 68 66 64 62
	52 51 49 50 47 48 46 45 44 43 42 41 40 39 38
	85 60 125 124 123 122 121 120 119 118 117 116 115 114 113
	112 111 110 109 108 107 106 105 104 103 102 101 3 19 18 4
	37 36 35 34 33 32 31 30 29 28 27 26 25 24 5
	16 15 14 13 12 11 10 9 8 7 23 22 21 20 6 17
	98 97 96 95 94 93 99 100 92 91 90 89 88 87 1 2
	'''
	list_resistor_no = []
	l = fmc_board_pins.strip().splitlines()
	for i in l:
		for j in i.split():
			list_resistor_no.append(j)
	

	print '#', '-' * 100
	print '#', 'test fmc data'
	print '#', '-' * 100

	list_test_gpio_resistor_net = []

	for no in list_resistor_no:
		for net, pin, des, resistor, gpio, gpio_no in list_net_pin_des_resistor_gpio_gpio_no:
			if net.startswith('FMC_HPC'):
				pattern = re.compile(r'([^\d]+)(\d+)')
				g = pattern.match(resistor)
				if g:
					r, i = g.groups()
					if no == i:
						item = (gpio_no, resistor, net)
						list_test_gpio_resistor_net.append(item)

	for no in list_resistor_no:
		for net, pin, des, resistor, gpio, gpio_no in list_net_pin_des_resistor_gpio_gpio_no:
			if net.startswith('FMC_LPC'):
				pattern = re.compile(r'([^\d]+)(\d+)')
				g = pattern.match(resistor)
				if g:
					r, i = g.groups()
					if no == i:
						item = (gpio_no, resistor, net)
						list_test_gpio_resistor_net.append(item)

	for gpio_no, resistor, net in list_test_gpio_resistor_net:
		print "%s\t%s\t%s" %(gpio_no, resistor, net)

def gen_kc705_constrain():
	list_kc705_net_group_part_pin = gen_list_kc705_net_group_part_pin()

	list_fmc_pin_resistor = gen_list_fmc_pin_resistor()

	map_kc705_pin_net = gen_map_kc705_pin_net()

	map_kc705_pin_iotype = gen_map_kc705_pin_iotype()

	remove_unused_pin(map_kc705_pin_net)

	list_ip_net_pin_port_des = get_list_ip_net_pin_port_des(map_kc705_pin_net)

	map_gpio_if_list_net_pin_des_resistor = get_map_gpio_if_list_net_pin_des_resistor(map_kc705_pin_net, list_kc705_net_group_part_pin, list_fmc_pin_resistor)
	gen_default_contrain()

	gen_ip_constrain(list_ip_net_pin_port_des)

	list_net_pin_des_resistor_gpio_gpio_no = gen_gpio_constrain(map_gpio_if_list_net_pin_des_resistor)

	gen_bitstream_constrain()

	gen_gpio_test_list(list_net_pin_des_resistor_gpio_gpio_no)

if __name__ == "__main__":
	gen_kc705_constrain()
