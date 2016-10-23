# -*- coding: utf-8 -*-
import re

def kc705_pins_key(x):
	#print x
	info = x[3]
	pattern = re.compile(r'([^\d]+)(\d+)')
	g = pattern.match(info)
	r = g.groups()
	return r[0], int(r[1])

def gen_demo_board_list_kc705_net_group_part_pin():
	list_kc705_net_group_part_pin = []

	content = ''
	with open('kc705_signals.txt') as f:
		content = f.read()
	lines = content.splitlines()

	state = 0
	signal = None
	signal_pattern = re.compile(r'^\*SIGNAL\* (.*) \d+ \d+')
	group_part_pin_pattern = re.compile(r'^([^-\s]+)-([^\.]+)\.([^\s]+)')
	for line in lines:
		if len(line) == 0:
			state = 0
			signal = None
			continue

		if state == 0:
			list_signal = signal_pattern.findall(line)
			if len(list_signal):
				signal = list_signal[0]
				state = 1
		if state == 1:
			list_group_part_pin = group_part_pin_pattern.findall(line)
			for group, part, pin in list_group_part_pin:
				item = (signal, group, part, pin)
				list_kc705_net_group_part_pin.append(item)
	list_kc705_net_group_part_pin.append(('FMC_LPC_PRSNT_M2C_B_LS', 'J2', 'E', 'H2'))
	list_kc705_net_group_part_pin.append(('FMC_HPC_PRSNT_M2C_B_LS', 'J22', 'I', 'H2'))
	list_kc705_net_group_part_pin.append(('FMC_HPC_PG_M2C_LS', 'J22', 'F', 'F1'))
	list_kc705_net_group_part_pin.append(('FMC_C2M_PG_LS', 'J22', 'D', 'D1'))

	list_kc705_net_group_part_pin = sorted(list_kc705_net_group_part_pin, key = lambda x:(kc705_pins_key(x)))
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

def new_board_list_fmc_part_no_key(x):
	#print x
	part = x[1]
	no = x[2]
	return part, int(no)

def gen_new_board_list_fmc_part_no():
	list_fmc_part_no = []

	content = ''
	with open('kc705_new_board_fmc.txt') as f:
		content = f.read()
	lines = content.splitlines()

	state = 0
	signal = None
	signal_pattern = re.compile(r'^\*SIGNAL\* (.*) \d+ \d+')
	part_no_pattern = re.compile(r'^(J[^\.]+)\.(\d+)')
	for line in lines:
		if len(line) == 0:
			state = 0
			signal = None
			continue

		if state == 0:
			list_signal = signal_pattern.findall(line)
			if len(list_signal):
				signal = list_signal[0]
				state = 1
		if state == 1:
			list_part_no = part_no_pattern.findall(line)
			for part, no in list_part_no:
				item = (signal, part, no)
				list_fmc_part_no.append(item)

	list_fmc_part_no = sorted(list_fmc_part_no, key = lambda x:(new_board_list_fmc_part_no_key(x)))
	print '-' * 100
	print 'list_fmc_part_no info'
	print '-' * 100
	for i in list_fmc_part_no:
		print i
	print 'total:', len(list_fmc_part_no)
	return list_fmc_part_no

def gen_new_board_list_slot_list_portnum_pin_net(list_fmc_part_no, list_pin_net, list_kc705_net_group_part_pin):
	list_slot_list_portnum_pin_net = []
	set_slot = set()
	for net, slot, portnum in list_fmc_part_no:
		set_slot.add(slot)
	set_slot = sorted(set_slot)
	for slot in set_slot:
		list_portnum_pin_net = []
		for net, slot_, portnum in list_fmc_part_no:
			if slot == slot_:
				pin = None
				for pin_, net_ in list_pin_net:
					if net == net_:
						pin = pin_
						item = (int(portnum), pin, net)				
						if item not in list_portnum_pin_net:
							list_portnum_pin_net.append(item)
						break
				if not pin:
					for net_, group_, part_, pin_ in list_kc705_net_group_part_pin:
						if net == net_ and group_ == "U1":
							pin = pin_
							item = (int(portnum), pin, net)				
							if item not in list_portnum_pin_net:
								list_portnum_pin_net.append(item)
							break
				if not pin:
					print "(%s, %s, %s) is not in list_pin_net" %(net, slot, portnum)
		item = (slot, list_portnum_pin_net)
		list_slot_list_portnum_pin_net.append(item)

	print '-' * 100
	print 'list_slot_list_portnum_pin_net info'
	print '-' * 100
	for slot, list_portnum_pin_net in list_slot_list_portnum_pin_net:
		print "%s:" %(slot)
		for i in list_portnum_pin_net:
			print "\t%s" %(str(i))
	print 'total:', len(list_slot_list_portnum_pin_net)
	return list_slot_list_portnum_pin_net

def get_list_slot_list_portnum_pin_net():
	list_slot_list_portnum_pin_net = [
		(
			'J102',
			[
				(5, 'Y30', 'FMC_LPC_LA32_P'),
				(6, 'AA30', 'FMC_LPC_LA32_N'),
				(7, 'AB29', 'FMC_LPC_LA30_P'),
				(8, 'AB30', 'FMC_LPC_LA30_N'),
				(9, 'AC29', 'FMC_LPC_LA33_P'),
				(10, 'AC30', 'FMC_LPC_LA33_N'),
				(11, 'AB27', 'FMC_LPC_LA17_CC_P'),
				(12, 'AC27', 'FMC_LPC_LA17_CC_N'),
				(13, 'AD29', 'FMC_LPC_LA31_P'),
				(14, 'AE29', 'FMC_LPC_LA31_N'),
				(17, 'AE30', 'FMC_LPC_LA28_P'),
				(18, 'AF30', 'FMC_LPC_LA28_N'),
				(19, 'AE28', 'FMC_LPC_LA29_P'),
				(20, 'AF28', 'FMC_LPC_LA29_N'),
				(21, 'AG30', 'FMC_LPC_LA24_P'),
				(22, 'AH30', 'FMC_LPC_LA24_N'),
				(23, 'AK29', 'FMC_LPC_LA26_P'),
				(24, 'AK30', 'FMC_LPC_LA26_N'),
				(25, 'AJ28', 'FMC_LPC_LA27_P'),
				(26, 'AJ29', 'FMC_LPC_LA27_N'),
				(29, 'AG27', 'FMC_LPC_LA21_P'),
				(30, 'AG28', 'FMC_LPC_LA21_N'),
				(31, 'AH26', 'FMC_LPC_LA23_P'),
				(32, 'AH27', 'FMC_LPC_LA23_N'),
				(33, 'AJ27', 'FMC_LPC_LA22_P'),
				(34, 'AK28', 'FMC_LPC_LA22_N'),
				(35, 'AJ26', 'FMC_LPC_LA19_P'),
				(36, 'AK26', 'FMC_LPC_LA19_N'),
				(37, 'AF26', 'FMC_LPC_LA20_P'),
				(38, 'AF27', 'FMC_LPC_LA20_N'),
			]
		),
		(
			'J101',
			[
				(5, 'AC26', 'FMC_LPC_LA25_P'),
				(6, 'AD26', 'FMC_LPC_LA25_N'),
				(7, 'AE25', 'FMC_LPC_LA11_P'),
				(8, 'AF25', 'FMC_LPC_LA11_N'),
				(9, 'AC24', 'FMC_LPC_LA15_P'),
				(10, 'AD24', 'FMC_LPC_LA15_N'),
				(11, 'AJ24', 'FMC_LPC_LA10_P'),
				(12, 'AK25', 'FMC_LPC_LA10_N'),
				(13, 'AJ22', 'FMC_LPC_LA08_P'),
				(14, 'AJ23', 'FMC_LPC_LA08_N'),
				(17, 'AG22', 'FMC_LPC_LA05_P'),
				(18, 'AH22', 'FMC_LPC_LA05_N'),
				(19, 'AD23', 'FMC_LPC_LA00_CC_P'),
				(20, 'AE24', 'FMC_LPC_LA00_CC_N'),
				(21, 'AC22', 'FMC_LPC_LA16_P'),
				(22, 'AD22', 'FMC_LPC_LA16_N'),
				(23, 'AF20', 'FMC_LPC_LA02_P'),
				(24, 'AF21', 'FMC_LPC_LA02_N'),
				(25, 'AG20', 'FMC_LPC_LA03_P'),
				(26, 'AH20', 'FMC_LPC_LA03_N'),
				(29, 'AK20', 'FMC_LPC_LA06_P'),
				(30, 'AK21', 'FMC_LPC_LA06_N'),
				(31, 'AE23', 'FMC_LPC_LA01_CC_P'),
				(32, 'AF23', 'FMC_LPC_LA01_CC_N'),
				(33, 'AB24', 'FMC_LPC_LA13_P'),
				(34, 'AC25', 'FMC_LPC_LA13_N'),
				(35, 'AK23', 'FMC_LPC_LA09_P'),
				(36, 'AK24', 'FMC_LPC_LA09_N'),
				(37, 'AD21', 'FMC_LPC_LA14_P'),
				(38, 'AE21', 'FMC_LPC_LA14_N'),
			]
		),
		(
			'J96',
			[
				(5, 'D17', 'FMC_HPC_CLK1_M2C_P'),
				(6, 'D27', 'FMC_HPC_CLK0_M2C_P'),
				(7, 'D18', 'FMC_HPC_CLK1_M2C_N'),
				(8, 'C27', 'FMC_HPC_CLK0_M2C_N'),
				(9, 'C25', 'FMC_HPC_LA00_CC_P'),
				(10, 'D26', 'FMC_HPC_LA01_CC_P'),
				(11, 'B25', 'FMC_HPC_LA00_CC_N'),
				(12, 'C26', 'FMC_HPC_LA01_CC_N'),
				(13, 'H24', 'FMC_HPC_LA02_P'),
				(14, 'H26', 'FMC_HPC_LA03_P'),
				(17, 'H25', 'FMC_HPC_LA02_N'),
				(18, 'H27', 'FMC_HPC_LA03_N'),
				(19, 'G28', 'FMC_HPC_LA04_P'),
				(20, 'G29', 'FMC_HPC_LA05_P'),
				(21, 'F28', 'FMC_HPC_LA04_N'),
				(22, 'F30', 'FMC_HPC_LA05_N'),
				(23, 'H30', 'FMC_HPC_LA06_P'),
				(24, 'E28', 'FMC_HPC_LA07_P'),
				(25, 'G30', 'FMC_HPC_LA06_N'),
				(26, 'D28', 'FMC_HPC_LA07_N'),
				(29, 'E29', 'FMC_HPC_LA08_P'),
				(30, 'B30', 'FMC_HPC_LA09_P'),
				(31, 'E30', 'FMC_HPC_LA08_N'),
				(32, 'A30', 'FMC_HPC_LA09_N'),
				(33, 'D29', 'FMC_HPC_LA10_P'),
				(34, 'G27', 'FMC_HPC_LA11_P'),
				(35, 'C30', 'FMC_HPC_LA10_N'),
				(36, 'F27', 'FMC_HPC_LA11_N'),
				(37, 'C29', 'FMC_HPC_LA12_P'),
				(38, 'B29', 'FMC_HPC_LA12_N'),
			]
		),
		(
			'J86',
			[
				(5, 'D21', 'FMC_HPC_LA32_P'),
				(6, 'C21', 'FMC_HPC_LA32_N'),
				(7, 'D16', 'FMC_HPC_LA28_P'),
				(8, 'E18', 'GPIO_LED_6_LS'),
				(9, 'C16', 'FMC_HPC_LA28_N'),
				(10, 'F16', 'GPIO_LED_7_LS'),
				(11, 'K18', 'HDMI_R_CLK'),
				(12, 'J18', 'HDMI_R_HSYNC'),
				(13, 'H20', 'HDMI_R_VSYNC'),
				(14, 'G20', 'HDMI_SPDIF_OUT_LS'),
				(17, 'J17', 'HDMI_R_SPDIF'),
				(18, 'H17', 'HDMI_R_DE'),
				(19, 'B23', 'HDMI_R_D0'),
				(20, 'A23', 'HDMI_R_D1'),
				(21, 'E23', 'HDMI_R_D2'),
				(22, 'D23', 'HDMI_R_D3'),
				(23, 'F25', 'HDMI_R_D4'),
				(24, 'E25', 'HDMI_R_D5'),
				(25, 'E24', 'HDMI_R_D6'),
				(26, 'D24', 'HDMI_R_D7'),
				(29, 'F26', 'HDMI_R_D8'),
				(30, 'E26', 'HDMI_R_D9'),
				(31, 'G23', 'HDMI_R_D10'),
				(32, 'G24', 'HDMI_R_D11'),
				(33, 'J19', 'HDMI_R_D12'),
				(34, 'H19', 'HDMI_R_D13'),
				(35, 'L17', 'HDMI_R_D14'),
				(36, 'L18', 'HDMI_R_D15'),
				(37, 'K19', 'HDMI_R_D16'),
				(38, 'K20', 'HDMI_R_D17'),
			]
		),
		(
			'J97',
			[
				(5, 'A25', 'FMC_HPC_LA13_P'),
				(6, 'B28', 'FMC_HPC_LA14_P'),
				(7, 'A26', 'FMC_HPC_LA13_N'),
				(8, 'A28', 'FMC_HPC_LA14_N'),
				(9, 'C24', 'FMC_HPC_LA15_P'),
				(10, 'B27', 'FMC_HPC_LA16_P'),
				(11, 'B24', 'FMC_HPC_LA15_N'),
				(12, 'A27', 'FMC_HPC_LA16_N'),
				(13, 'F20', 'FMC_HPC_LA17_CC_P'),
				(14, 'F21', 'FMC_HPC_LA18_CC_P'),
				(17, 'E20', 'FMC_HPC_LA17_CC_N'),
				(18, 'E21', 'FMC_HPC_LA18_CC_N'),
				(19, 'G18', 'FMC_HPC_LA19_P'),
				(20, 'E19', 'FMC_HPC_LA20_P'),
				(21, 'F18', 'FMC_HPC_LA19_N'),
				(22, 'D19', 'FMC_HPC_LA20_N'),
				(23, 'A20', 'FMC_HPC_LA21_P'),
				(24, 'C20', 'FMC_HPC_LA22_P'),
				(25, 'A21', 'FMC_HPC_LA21_N'),
				(26, 'B20', 'FMC_HPC_LA22_N'),
				(29, 'B22', 'FMC_HPC_LA23_P'),
				(30, 'A16', 'FMC_HPC_LA24_P'),
				(31, 'A22', 'FMC_HPC_LA23_N'),
				(32, 'A17', 'FMC_HPC_LA24_N'),
				(33, 'G17', 'FMC_HPC_LA25_P'),
				(34, 'B18', 'FMC_HPC_LA26_P'),
				(35, 'F17', 'FMC_HPC_LA25_N'),
				(36, 'A18', 'FMC_HPC_LA26_N'),
				(37, 'C19', 'FMC_HPC_LA27_P'),
				(38, 'B19', 'FMC_HPC_LA27_N'),
			]
		),
		(
			'J95',
			[
				(5, 'H15', 'FMC_HPC_HA15_P'),
				(6, 'L15', 'FMC_HPC_HA16_P'),
				(7, 'G15', 'FMC_HPC_HA15_N'),
				(8, 'K15', 'FMC_HPC_HA16_N'),
				(9, 'G13', 'FMC_HPC_HA17_CC_P'),
				(10, 'K14', 'FMC_HPC_HA18_P'),
				(11, 'F13', 'FMC_HPC_HA17_CC_N'),
				(12, 'J14', 'FMC_HPC_HA18_N'),
				(13, 'H11', 'FMC_HPC_HA19_P'),
				(14, 'K13', 'FMC_HPC_HA20_P'),
				(17, 'H12', 'FMC_HPC_HA19_N'),
				(18, 'J13', 'FMC_HPC_HA20_N'),
				(19, 'J11', 'FMC_HPC_HA21_P'),
				(20, 'L11', 'FMC_HPC_HA22_P'),
				(21, 'J12', 'FMC_HPC_HA21_N'),
				(22, 'K11', 'FMC_HPC_HA22_N'),
				(23, 'L12', 'FMC_HPC_HA23_P'),
				(25, 'L13', 'FMC_HPC_HA23_N'),
				(26, 'G19', 'GPIO_LED_5_LS'),
				(29, 'C17', 'FMC_HPC_LA29_P'),
				(30, 'D22', 'FMC_HPC_LA30_P'),
				(31, 'B17', 'FMC_HPC_LA29_N'),
				(32, 'C22', 'FMC_HPC_LA30_N'),
				(33, 'G22', 'FMC_HPC_LA31_P'),
				(35, 'F22', 'FMC_HPC_LA31_N'),
				(37, 'H21', 'FMC_HPC_LA33_P'),
				(38, 'H22', 'FMC_HPC_LA33_N'),
			]
		),
		(
			'J94',
			[
				(5, 'D12', 'FMC_HPC_HA00_CC_P'),
				(6, 'H14', 'FMC_HPC_HA01_CC_P'),
				(7, 'D13', 'FMC_HPC_HA00_CC_N'),
				(8, 'G14', 'FMC_HPC_HA01_CC_N'),
				(9, 'D11', 'FMC_HPC_HA02_P'),
				(10, 'C12', 'FMC_HPC_HA03_P'),
				(11, 'C11', 'FMC_HPC_HA02_N'),
				(12, 'B12', 'FMC_HPC_HA03_N'),
				(13, 'F11', 'FMC_HPC_HA04_P'),
				(14, 'F15', 'FMC_HPC_HA05_P'),
				(17, 'E11', 'FMC_HPC_HA04_N'),
				(18, 'E16', 'FMC_HPC_HA05_N'),
				(19, 'D14', 'FMC_HPC_HA06_P'),
				(20, 'B14', 'FMC_HPC_HA07_P'),
				(21, 'C14', 'FMC_HPC_HA06_N'),
				(22, 'A15', 'FMC_HPC_HA07_N'),
				(23, 'E14', 'FMC_HPC_HA08_P'),
				(24, 'F12', 'FMC_HPC_HA09_P'),
				(25, 'E15', 'FMC_HPC_HA08_N'),
				(26, 'E13', 'FMC_HPC_HA09_N'),
				(29, 'A11', 'FMC_HPC_HA10_P'),
				(30, 'B13', 'FMC_HPC_HA11_P'),
				(31, 'A12', 'FMC_HPC_HA10_N'),
				(32, 'A13', 'FMC_HPC_HA11_N'),
				(33, 'C15', 'FMC_HPC_HA12_P'),
				(34, 'L16', 'FMC_HPC_HA13_P'),
				(35, 'B15', 'FMC_HPC_HA12_N'),
				(36, 'K16', 'FMC_HPC_HA13_N'),
				(37, 'J16', 'FMC_HPC_HA14_P'),
				(38, 'H16', 'FMC_HPC_HA14_N'),
			]
		),
		(
			'J82',
			[
				(3, 'N30', 'PHY_INT'),
				(5, 'U30', 'PHY_RXD0'),
				(6, 'L20', 'PHY_RESET'),
				(7, 'T25', 'PHY_RXD2'),
				(8, 'U25', 'PHY_RXD1'),
				(9, 'R19', 'PHY_RXD4'),
				(10, 'U28', 'PHY_RXD3'),
				(11, 'T26', 'PHY_RXD6'),
				(12, 'T27', 'PHY_RXD5'),
				(13, 'R28', 'PHY_RXCTL_RXDV'),
				(14, 'T28', 'PHY_RXD7'),
				(17, 'V26', 'PHY_RXER'),
				(18, 'U27', 'PHY_RXCLK'),
				(19, 'R30', 'PHY_CRS'),
				(20, 'W19', 'PHY_COL'),
				(21, 'J21', 'PHY_MDIO'),
				(22, 'R23', 'PHY_MDC'),
				(23, 'K30', 'PHY_TXC_GTXCLK'),
				(24, 'M27', 'PHY_TXCTL_TXEN'),
				(25, 'N29', 'PHY_TXER'),
				(26, 'M28', 'PHY_TXCLK'),
				(29, 'N25', 'PHY_TXD1'),
				(30, 'N27', 'PHY_TXD0'),
				(31, 'L28', 'PHY_TXD3'),
				(32, 'M29', 'PHY_TXD2'),
				(33, 'K26', 'PHY_TXD5'),
				(34, 'J26', 'PHY_TXD4'),
				(35, 'J28', 'PHY_TXD7'),
				(36, 'L30', 'PHY_TXD6'),
				(37, 'J29', 'FMC_HPC_PG_M2C_LS'),
			]
		),
		(
			'J98',
			[
				(5, 'L25', 'USER_SMA_CLOCK_P'),
				(6, 'AA26', 'ROTARY_PUSH'),
				(7, 'K25', 'USER_SMA_CLOCK_N'),
				(8, 'Y26', 'ROTARY_INCA'),
				(9, 'M20', 'FMC_HPC_PRSNT_M2C_B_LS'),
				(10, 'Y25', 'ROTARY_INCB'),
				(11, 'P19', 'SFP_LOS_LS'),
				(18, 'J23', 'XADC_VAUX0P_R'),
				(20, 'J24', 'XADC_VAUX0N_R'),
				(21, 'L22', 'XADC_VAUX8P_R'),
				(23, 'L23', 'XADC_VAUX8N_R'),
				(25, 'AB25', 'XADC_GPIO_0'),
				(26, 'AA25', 'XADC_GPIO_1'),
				(29, 'AB28', 'XADC_GPIO_2'),
				(30, 'AA27', 'XADC_GPIO_3'),
				(31, 'AH24', 'HDMI_INT'),
				(32, 'Y14', 'PMBUS_DATA_LS'),
				(33, 'Y20', 'SFP_TX_DISABLE'),
				(34, 'AB14', 'PMBUS_ALERT_LS'),
				(36, 'AG17', 'PMBUS_CLK_LS'),
			]
		),
		(
			'J87',
			[
				(3, 'G7', 'SGMIICLK_Q0_N'),
				(4, 'B1', 'FMC_HPC_DP2_C2M_N'),
				(5, 'G8', 'SGMIICLK_Q0_P'),
				(6, 'B2', 'FMC_HPC_DP2_C2M_P'),
				(7, 'A8', 'FMC_HPC_DP3_M2C_P'),
				(8, 'B5', 'FMC_HPC_DP2_M2C_N'),
				(9, 'A4', 'FMC_HPC_DP3_C2M_P'),
				(10, 'B6', 'FMC_HPC_DP2_M2C_P'),
				(13, 'A3', 'FMC_HPC_DP3_C2M_N'),
				(14, 'A7', 'FMC_HPC_DP3_M2C_N'),
				(15, 'C8', 'FMC_HPC_GBTCLK0_M2C_C_P'),
				(16, 'E8', 'FMC_HPC_GBTCLK1_M2C_C_P'),
				(17, 'C7', 'FMC_HPC_GBTCLK0_M2C_C_N'),
				(18, 'E7', 'FMC_HPC_GBTCLK1_M2C_C_N'),
			]
		),
	]

	list_slot_list_portnum_pin_net = sorted(list_slot_list_portnum_pin_net, key = lambda x:x[0])

	print '-' * 100
	print 'list_slot_list_portnum_pin_net info'
	print '-' * 100
	for slot, list_portnum_pin_net in list_slot_list_portnum_pin_net:
		print "%s:" %(slot)
		for i in list_portnum_pin_net:
			print "\t%s" %(str(i))
	print 'total:', len(list_slot_list_portnum_pin_net)
	return list_slot_list_portnum_pin_net

def gen_kc705_list_pin_iotype():
	list_pin_iotype = []
	lines = []
	with open('kc705_io_package_pins.txt') as f:
		lines = f.read().splitlines()

	for i in lines:
		l = i.split()
		if len(l) == 8:
			pin_iotype = (l[0], l[1])
			list_pin_iotype.append(pin_iotype)

	print '-' * 100
	print 'list_pin_iotype info'
	print '-' * 100
	for i in list_pin_iotype:
		print i
	print 'total:', len(list_pin_iotype)

	return list_pin_iotype

def kc705_gen_list_pin_net(list_pin_iotype):
	list_pin_net = []
	lines = []
	with open('kc705_package_pin_nets.txt') as f:
		lines = f.read().splitlines()

	for i in lines:
		l = i.split()
		if len(l) == 2:
			for pin, iotype in list_pin_iotype:
				if pin == l[0]:
					pin_net = (l[0], l[1])
					list_pin_net.append(pin_net)
					break

	print '-' * 100
	print 'list_pin_net info'
	print '-' * 100
	for i in list_pin_net:
		print i
	print 'total:', len(list_pin_net)

	return list_pin_net

def remove_unused_pin(list_pin_net):
	list_unsupport_pin_net = []
	unsupport_pin = [
	]
	
	for i in unsupport_pin:
		for pin_net in list_pin_net:
			if pin_net[0] == i:
				list_pin_net.remove(pin_net)
				list_unsupport_pin_net.append(pin_net)
				break
	print '-' * 100
	print 'list_unsupport_pin_net info'
	print '-' * 100
	for i in list_unsupport_pin_net:
		print i
	print 'total:', len(list_unsupport_pin_net)

def map_port_list_property_iic_slave():
	map_port_list_property = {
		'iic_scl': ['CLOCK_DEDICATED_ROUTE FALSE'],
	}

	return map_port_list_property

def list_net_port_iic_slave():
	list_net_port = [
		('FMC_HPC_LA28_P', 'iic_scl'),
		('FMC_HPC_LA21_P', 'iic_sda'),
	]

	return list_net_port

#list_port_width_list_port_name = [
#	(32, ['gpio']),
#	(32, ['gpio2']),
#]
#
#get_list_all_ports_name(2, list_port_width_list_port_name, "_tri_io")
def get_list_all_ports_name(ip_num, list_port_width_list_port_name, str_append):
	list_all_ports_name = []

	for ip_no in range(ip_num):
		if ip_no == 0:
			str_ip_no = ''
		else:
			str_ip_no = '_%d' %(ip_no)
		
		for port_width, list_port_name in list_port_width_list_port_name:
			str_port_bit_no = ''
			for port_bit_no in range(port_width):
				if port_width > 1:
					str_port_bit_no = "[%d]" %(port_bit_no)

				for port_name in list_port_name:
					item = "%s%s%s%s" %(port_name, str_ip_no, str_append, str_port_bit_no)
					list_all_ports_name.append(item)

	return list_all_ports_name

def map_port_list_property_new_i2s_board_multi():
	map_port_list_property = {}

	ip_num = 3
	list_port_width_list_port_name = [
		(16, ['bclk']),
	]
	str_append = ''
	list_all_ports_name = get_list_all_ports_name(ip_num, list_port_width_list_port_name, str_append)
	list_property = ['CLOCK_DEDICATED_ROUTE FALSE']
	for i in list_all_ports_name:
		item = {i : list_property}
		map_port_list_property.update(item)

	return map_port_list_property

def get_list_pin_net_from_list_slot_list_portnum_pin_net(list_slot_list_portnum_pin_net, list_list_slots, list_index_portnum):
	list_pin_net = []

	list_err_msg = []
	for i in range(len(list_list_slots)):
		for index, portnum in list_index_portnum:
			slot = list_list_slots[i][index]
			list_portnum_pin_net = None
			for slot_, list_portnum_pin_net_ in list_slot_list_portnum_pin_net:
				if slot_ == slot:
					list_portnum_pin_net = list_portnum_pin_net_
					item = None
					for portnum_, pin, net in list_portnum_pin_net:
						if portnum_ == portnum:
							item = (pin, net)
							list_pin_net.append(item)
							break
					if not item:
						err_msg = '\nslot:%s portnum:%s not exist!' %(slot, portnum)
						list_err_msg.append(err_msg)
					break
			if not list_portnum_pin_net:
				err_msg = '\nslot:%s not exist!' %(slot)
				list_err_msg.append(err_msg)
	if len(list_err_msg):
		err_txt = ''
		for err_msg in list_err_msg:
			err_txt += "\n%s" %(err_msg)
		raise Exception(err_txt)
	return list_pin_net

def list_pin_port_new_i2s_board_multi(list_slot_list_portnum_pin_net):
	list_pin_port = []

	list_list_slots = [
		['J101', 'J102'],
		['J86', 'J96'],
		['J94', 'J97']
	]

	J101 = 0
	J102 = 1
	list_index_portnum_for_i2s_16_inst = [
		(J101, 17),
		(J101, 18),
		(J101, 19),
		(J101, 20),
		(J101, 21),
		(J101, 22),
		(J101, 23),
		(J101, 24),
		(J101, 25),
		(J101, 26),
		(J101, 29),
		(J101, 30),
		(J101, 31),
		(J101, 32),
		(J101, 33),
		(J101, 34),
		(J101, 35),
		(J101, 36),
		(J102, 5),
		(J102, 6),
		(J102, 7),
		(J102, 8),
		(J102, 9),
		(J102, 10),
		(J102, 11),
		(J102, 12),
		(J102, 13),
		(J102, 14),
		(J102, 17),
		(J102, 18),
		(J102, 19),
		(J102, 20),
		(J102, 21),
		(J102, 22),
		(J102, 23),
		(J102, 24),
		(J102, 25),
		(J102, 26),
		(J102, 29),
		(J102, 30),
		(J102, 31),
		(J102, 32),
		(J102, 33),
		(J102, 34),
		(J102, 35),
		(J102, 36),
		(J102, 37),
		(J102, 38),
	]


	ip_num = len(list_list_slots)
	list_port_width_list_port_name = [
		(16, ['bclk', 'lrclk', 'sdata']),
	]
	str_append = ''
	list_all_ports_name = get_list_all_ports_name(ip_num, list_port_width_list_port_name, str_append)

	list_pin_net = get_list_pin_net_from_list_slot_list_portnum_pin_net(list_slot_list_portnum_pin_net, list_list_slots, list_index_portnum_for_i2s_16_inst)

	if len(list_all_ports_name) != len(list_pin_net):
		str_status = 'list_all_ports_name:%d list_pin_net:%d' %(len(list_all_ports_name), len(list_pin_net))
		raise Exception('')

	for i in range(len(list_all_ports_name)):
		item = (list_pin_net[i][0], list_all_ports_name[i])
		list_pin_port.append(item)
			
	return list_pin_port

def map_port_list_property_old_tsp_board_2ab42e394123204b24255388e7e131aab67b6328():
	map_port_list_property = {
		'mpeg_clk': ['CLOCK_DEDICATED_ROUTE FALSE'],
		#'fs_0p5_en': ['CLOCK_DEDICATED_ROUTE FALSE'],

		#'symbol_2x_oe' : ['slew FAST'],

		#'symbol_2x_re_out[0]' : ['slew FAST'],
		#'symbol_2x_re_out[1]' : ['slew FAST'],
		#'symbol_2x_re_out[2]' : ['slew FAST'],
		#'symbol_2x_re_out[3]' : ['slew FAST'],
		#'symbol_2x_re_out[4]' : ['slew FAST'],
		#'symbol_2x_re_out[5]' : ['slew FAST'],
		#'symbol_2x_re_out[6]' : ['slew FAST'],
		#'symbol_2x_re_out[7]' : ['slew FAST'],
		#'symbol_2x_re_out[8]' : ['slew FAST'],
		#'symbol_2x_re_out[9]' : ['slew FAST'],
		#'symbol_2x_re_out[10]' : ['slew FAST'],
		#'symbol_2x_re_out[11]' : ['slew FAST'],
		#'symbol_2x_re_out[12]' : ['slew FAST'],
		#'symbol_2x_re_out[13]' : ['slew FAST'],
		#'symbol_2x_re_out[14]' : ['slew FAST'],
		#'symbol_2x_re_out[15]' : ['slew FAST'],

		#'symbol_2x_im_out[0]' : ['slew FAST'],
		#'symbol_2x_im_out[1]' : ['slew FAST'],
		#'symbol_2x_im_out[2]' : ['slew FAST'],
		#'symbol_2x_im_out[3]' : ['slew FAST'],
		#'symbol_2x_im_out[4]' : ['slew FAST'],
		#'symbol_2x_im_out[5]' : ['slew FAST'],
		#'symbol_2x_im_out[6]' : ['slew FAST'],
		#'symbol_2x_im_out[7]' : ['slew FAST'],
		#'symbol_2x_im_out[8]' : ['slew FAST'],
		#'symbol_2x_im_out[9]' : ['slew FAST'],
		#'symbol_2x_im_out[10]' : ['slew FAST'],
		#'symbol_2x_im_out[11]' : ['slew FAST'],
		#'symbol_2x_im_out[12]' : ['slew FAST'],
		#'symbol_2x_im_out[13]' : ['slew FAST'],
		#'symbol_2x_im_out[14]' : ['slew FAST'],
		#'symbol_2x_im_out[15]' : ['slew FAST'],
	}

	return map_port_list_property

def list_net_port_old_tsp_board_2ab42e394123204b24255388e7e131aab67b6328():
	list_net_port = [
		('FMC_LPC_LA25_P', 'mpeg_clk'),
		('FMC_LPC_LA29_P', 'mpeg_valid'),
		('FMC_LPC_LA24_P', 'mpeg_data[0]'),
		('FMC_LPC_LA24_N', 'mpeg_data[1]'),
		('FMC_LPC_LA26_P', 'mpeg_data[2]'),
		('FMC_LPC_LA31_P', 'mpeg_data[3]'),
		('FMC_LPC_LA31_N', 'mpeg_data[4]'),
		('FMC_LPC_LA28_P', 'mpeg_data[5]'),
		('FMC_LPC_LA28_N', 'mpeg_data[6]'),
		('FMC_LPC_LA29_N', 'mpeg_data[7]'),
		('FMC_LPC_LA25_N', 'mpeg_sync'),

		#('FMC_HPC_HA23_N', 'ts_out_sync'),
		#('FMC_HPC_HA23_P', 'slot0_out_dump_flag'),

		#('FMC_LPC_LA10_P', 'symbol_2x_oe'),
		#	
		#('FMC_LPC_LA00_CC_P', 'symbol_2x_re_out[0]'),
		#('FMC_LPC_LA02_P', 'symbol_2x_re_out[1]'),
		#('FMC_LPC_LA00_CC_N', 'symbol_2x_re_out[2]'),
		#('FMC_LPC_LA02_N', 'symbol_2x_re_out[3]'),
		#('FMC_LPC_LA03_P', 'symbol_2x_re_out[4]'),
		#('FMC_LPC_LA04_P', 'symbol_2x_re_out[5]'),
		#('FMC_LPC_LA03_N', 'symbol_2x_re_out[6]'),
		#('FMC_LPC_LA04_N', 'symbol_2x_re_out[7]'),
		#('FMC_LPC_LA07_P', 'symbol_2x_re_out[8]'),
		#('FMC_LPC_LA08_P', 'symbol_2x_re_out[9]'),
		#('FMC_LPC_LA01_CC_P', 'symbol_2x_re_out[10]'),
		#('FMC_LPC_LA01_CC_N', 'symbol_2x_re_out[11]'),
		#('FMC_LPC_LA06_P', 'symbol_2x_re_out[12]'),
		#('FMC_LPC_LA06_N', 'symbol_2x_re_out[13]'),
		#('FMC_LPC_LA05_P', 'symbol_2x_re_out[14]'),
		#('FMC_LPC_LA05_N', 'symbol_2x_re_out[15]'),

		#('FMC_LPC_LA10_N', 'symbol_2x_im_out[0]'),
		#('FMC_LPC_LA09_P', 'symbol_2x_im_out[1]'),
		#('FMC_LPC_LA09_N', 'symbol_2x_im_out[2]'),
		#('FMC_LPC_LA13_P', 'symbol_2x_im_out[3]'),
		#('FMC_LPC_LA14_P', 'symbol_2x_im_out[4]'),
		#('FMC_LPC_LA13_N', 'symbol_2x_im_out[5]'),
		#('FMC_LPC_LA14_N', 'symbol_2x_im_out[6]'),
		#('FMC_LPC_LA07_N', 'symbol_2x_im_out[7]'),
		#('FMC_LPC_LA08_N', 'symbol_2x_im_out[8]'),
		#('FMC_LPC_LA12_P', 'symbol_2x_im_out[9]'),
		#('FMC_LPC_LA11_P', 'symbol_2x_im_out[10]'),
		#('FMC_LPC_LA12_N', 'symbol_2x_im_out[11]'),
		#('FMC_LPC_LA11_N', 'symbol_2x_im_out[12]'),
		#('FMC_LPC_LA16_P', 'symbol_2x_im_out[13]'),
		#('FMC_LPC_LA16_N', 'symbol_2x_im_out[14]'),
		#('FMC_LPC_LA15_P', 'symbol_2x_im_out[15]'),

		#('FMC_LPC_LA22_N', 'clk_out2'),
		#('FMC_LPC_LA21_P', 'clk_out3'),

		#('FMC_LPC_LA20_P', 'clk_out4'),

		('FMC_LPC_LA32_N', 'asi_out_p'),
		('FMC_LPC_LA33_N', 'asi_out_n'),

		#('FMC_LPC_CLK0_M2C_P', 'fs_0p5_en'),

		##('XADC_GPIO_0', 'clk_out1'),
		##('USER_SMA_GPIO_P', 'asi_out'),
		##('LCD_DB4_LS', 'asi_out'),
		##('GPIO_SW_E', 'asi_out'),
		#('LCD_E_LS', 'lcm_din'),
		#('LCD_RS_LS', 'lcm_lp'),
		#('LCD_RW_LS', 'lcm_xscl'),
		#('XADC_GPIO_0', 'lcm_data[0]'),
		#('XADC_GPIO_1', 'lcm_data[1]'),
		#('XADC_GPIO_2', 'lcm_data[2]'),
		#('XADC_GPIO_3', 'lcm_data[3]'),
		#('LCD_DB4_LS', 'lcm_data[4]'),
		#('LCD_DB5_LS', 'lcm_data[5]'),
		#('LCD_DB6_LS', 'lcm_data[6]'),
		#('LCD_DB7_LS', 'lcm_data[7]'),
	]

	return list_net_port

def map_port_list_property_multi_tsp():
	map_port_list_property = {}

	ip_num = 4
	list_port_width_list_port_name = [
		(1, ['mpeg_clk']),
	]
	str_append = ''
	list_all_ports_name = get_list_all_ports_name(ip_num, list_port_width_list_port_name, str_append)
	list_property = ['CLOCK_DEDICATED_ROUTE FALSE']
	for i in list_all_ports_name:
		item = {i : list_property}
		map_port_list_property.update(item)

	return map_port_list_property

def list_pin_port_new_board_multi_tsp(list_slot_list_portnum_pin_net):
	list_pin_port = []

	list_list_slots = [
		['J94'],
		['J95'],
		['J96'],
		['J97']
	]

	J94 = 0
	list_index_portnum_for_i2s_16_inst = [
		(J94, 5),
		(J94, 6),
		(J94, 18),
		(J94, 7),
		(J94, 8),
		(J94, 9),
		(J94, 13),
		(J94, 12),
		(J94, 17),
		(J94, 14),
		(J94, 19),

		(J94, 29),
		(J94, 31),
	]


	ip_num = len(list_list_slots)
	list_port_width_list_port_name = [
		(1, ['mpeg_clk', 'mpeg_sync', 'mpeg_valid']),
		(8, ['mpeg_data']),
		(1, ['asi_out_p', 'asi_out_n']),
	]
	str_append = ''
	list_all_ports_name = get_list_all_ports_name(ip_num, list_port_width_list_port_name, str_append)

	list_pin_net = get_list_pin_net_from_list_slot_list_portnum_pin_net(list_slot_list_portnum_pin_net, list_list_slots, list_index_portnum_for_i2s_16_inst)

	if len(list_all_ports_name) != len(list_pin_net):
		str_status = 'list_all_ports_name:%d list_pin_net:%d' %(len(list_all_ports_name), len(list_pin_net))
		raise Exception(str_status)

	for i in range(len(list_all_ports_name)):
		item = (list_pin_net[i][0], list_all_ports_name[i])
		list_pin_port.append(item)
			
	return list_pin_port

def ip_get_list_net_pin_port_des(list_pin_net, list_slot_list_portnum_pin_net):
	list_net_pin_port_des = []

	map_port_list_property = {}
	list_net_port = []
	map_net_property = {}
	list_pin_port = []

	#map_port_list_property = map_port_list_property_new_i2s_board_multi()
	#list_pin_port = list_pin_port_new_i2s_board_multi(list_slot_list_portnum_pin_net)

	#map_port_list_property = map_port_list_property_old_tsp_board_2ab42e394123204b24255388e7e131aab67b6328()
	#list_net_port = list_net_port_old_tsp_board_2ab42e394123204b24255388e7e131aab67b6328()

	map_port_list_property = map_port_list_property_multi_tsp()
	list_pin_port = list_pin_port_new_board_multi_tsp(list_slot_list_portnum_pin_net)

	list_err_msg = []
	for pin, port in list_pin_port:
		list_extra_des = []
		list_property = map_port_list_property.get(port, None)
		if list_property:
			for i in list_property:
				list_extra_des.append('set_property %s [get_nets {%s}]' %(i, port))

		net = None
		for pin_net in list_pin_net:
			if pin_net[0] == pin:
				net = pin_net[1]
				list_pin_net.remove(pin_net)
				break
		if net:
			item = (net, pin, port, list_extra_des)
			list_net_pin_port_des.append(item)
		else:
			err_msg = '\n%s is not in list_pin_net for %s!' %(pin, port)
			list_err_msg.append(err_msg)
	
	for net, port in list_net_port:
		list_extra_des = []

		list_property = map_port_list_property.get(port)
		if list_property:
			for i in list_property:
				list_extra_des.append('set_property %s [get_nets {%s}]' %(i, port))

		pin = None
		for pin_net in list_pin_net:
			if pin_net[1] == net:
				pin = pin_net[0]
				list_pin_net.remove(pin_net)
				break

		if pin:
			item = (net, pin, port, list_extra_des)
			list_net_pin_port_des.append(item)
		else:
			err_msg = '\n%s is not in list_pin_net for %s!' %(net, port)
			list_err_msg.append(err_msg)
	if len(list_err_msg):
		err_txt = ''
		for err_msg in list_err_msg:
			err_txt += err_msg
		raise Exception(err_txt)

	print '-' * 100
	print 'list_net_pin_port_des info'
	print '-' * 100
	for i in list_net_pin_port_des:
		print i
	print 'total:', len(list_net_pin_port_des)

	return list_net_pin_port_des

def list_pin_des_new_i2s_board_multi(list_slot_list_portnum_pin_net):
	list_pin_des = []

	list_list_slots = [
		['J101', 'J102'],
		['J86', 'J96'],
		['J94', 'J97']
	]

	J101 = 0
	list_index_portnum_for_i2s_16_inst = [
		(J101, 6),
		(J101, 5),
		(J101, 7),
		(J101, 8),
		(J101, 37),
		(J101, 10),
		(J101, 11),
		(J101, 12),
		(J101, 13),
		(J101, 14),
	]

	ip_num = len(list_list_slots)
	list_port_width_list_des = [
		(1, ['SOMI', 'MOSI', 'SCLK', '74138GA(CS)', '74138GB', '74138GC', 'SPI_S0', 'SPI_S1', 'SPI_S2', 'SPI_S3']),
	]
	str_append = ''
	list_all_des = get_list_all_ports_name(ip_num, list_port_width_list_des, str_append)

	list_pin_net = get_list_pin_net_from_list_slot_list_portnum_pin_net(list_slot_list_portnum_pin_net, list_list_slots, list_index_portnum_for_i2s_16_inst)

	if len(list_all_des) != len(list_pin_net):
		str_status = 'list_all_des:%d list_pin_net:%d' %(len(list_all_des), len(list_pin_net))
		raise Exception(str_status)

	for i in range(len(list_all_des)):
		item = (list_pin_net[i][0], list_all_des[i])
		list_pin_des.append(item)

	return list_pin_des

def list_net_des_iic_slave():
	list_net_des = [
		('FMC_HPC_LA30_P', 'master_scl'),
		('FMC_HPC_LA24_P', 'master_sda'),
	]

	return list_net_des

def list_net_des_old_tsp_board_2ab42e394123204b24255388e7e131aab67b6328():
	list_net_des = [
		('FMC_LPC_LA30_N', 'i2c_sck'),
		('FMC_LPC_LA30_P', 'i2c_sda'),
		#('FMC_LPC_LA23_P', 'spi_clk'),
		#('FMC_LPC_LA23_N', 'spi_mosi'),
		#('FMC_LPC_LA20_N', 'spi_miso'),
		#('FMC_LPC_LA32_P', 'spi_cs'),
	]

	return list_net_des

def list_pin_des_multi_tsp(list_slot_list_portnum_pin_net):
	list_pin_des = []

	list_list_slots = [
		['J94'],
		['J95'],
		['J96'],
		['J97']
	]

	J94 = 0
	list_index_portnum_for_multi_tsp_inst = [
		(J94, 10),
		(J94, 11),
		(J94, 21),
		(J94, 22),
		(J94, 23),
	]

	ip_num = len(list_list_slots)
	list_port_width_list_des = [
		(1, ['i2c_sck', 'i2c_sda', 'lnb1_on_off', 'nim_reset', 'tunb_33_on_off']),
	]
	str_append = ''
	list_all_des = get_list_all_ports_name(ip_num, list_port_width_list_des, str_append)

	list_pin_net = get_list_pin_net_from_list_slot_list_portnum_pin_net(list_slot_list_portnum_pin_net, list_list_slots, list_index_portnum_for_multi_tsp_inst)

	if len(list_all_des) != len(list_pin_net):
		str_status = 'list_all_des:%d list_pin_net:%d' %(len(list_all_des), len(list_pin_net))
		raise Exception(str_status)

	for i in range(len(list_all_des)):
		item = (list_pin_net[i][0], list_all_des[i])
		list_pin_des.append(item)

	return list_pin_des

def get_map_gpioif_list_net_pin_des_resistor(list_pin_net, list_kc705_net_group_part_pin, list_fmc_pin_resistor, list_slot_list_portnum_pin_net):
	map_gpioif_list_net_pin_des_resistor = {}
	map_gpioif_list_net_pin_des_resistor.update({'HPC': []})
	map_gpioif_list_net_pin_des_resistor.update({'LPC': []})
	map_gpioif_list_net_pin_des_resistor.update({'OTHER': []})
	map_gpioif_list_net_pin_des_resistor.update({'ALL': []})

	list_pin_des = []
	list_net_des = []

	#list_pin_des = list_pin_des_new_i2s_board_multi(list_slot_list_portnum_pin_net)
	#list_net_des = list_net_des_old_tsp_board_2ab42e394123204b24255388e7e131aab67b6328()
	list_pin_des = list_pin_des_multi_tsp(list_slot_list_portnum_pin_net)

	for pin, des in list_pin_des:
		net = None
		for pin_net in list_pin_net:
			if pin_net[0] == pin:
				net = pin_net[1]
				list_pin_net.remove(pin_net)
				break
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
				map_gpioif_list_net_pin_des_resistor.get('HPC').append(v)
			elif net.startswith('FMC_LPC'):
				map_gpioif_list_net_pin_des_resistor.get('LPC').append(v)
			else:
				map_gpioif_list_net_pin_des_resistor.get('OTHER').append(v)

			map_gpioif_list_net_pin_des_resistor.get('ALL').append(v)

	for net, des in list_net_des:
		pin = None
		for pin_net in list_pin_net:
			if pin_net[1] == net:
				pin = pin_net[0]
				list_pin_net.remove(pin_net)
				break;
		if pin:
			resistor = 'undefined'
			for k, group, part, l in list_kc705_net_group_part_pin:
				if net == k:
					if group in ['J2', 'J22']:
						for m, n in list_fmc_pin_resistor:
							if l == m:
								resistor = n
			v = (net, pin, des, resistor)
			if net.startswith('FMC_HPC'):
				map_gpioif_list_net_pin_des_resistor.get('HPC').append(v)
			elif net.startswith('FMC_LPC'):
				map_gpioif_list_net_pin_des_resistor.get('LPC').append(v)
			else:
				map_gpioif_list_net_pin_des_resistor.get('OTHER').append(v)

	#for pin_net in list_pin_net:
	#	des = 'undefined'
	#	pin = pin_net[0]
	#	net = pin_net[1]
	#	resistor = 'undefined'
	#	for i, group, part, j in list_kc705_net_group_part_pin:
	#		if net == i:
	#			if group in ['J2', 'J22']:
	#				for k, l in list_fmc_pin_resistor:
	#					if j == k:
	#						resistor = l
	#	v = (net, pin, des, resistor)
	#	if net.startswith('FMC_HPC'):
	#		map_gpioif_list_net_pin_des_resistor.get('HPC').append(v)
	#	elif net.startswith('FMC_LPC'):
	#		map_gpioif_list_net_pin_des_resistor.get('LPC').append(v)
	#	else:
	#		map_gpioif_list_net_pin_des_resistor.get('OTHER').append(v)

	for i in map_gpioif_list_net_pin_des_resistor.items():
		print '-' * 100
		print 'map_gpioif_list_net_pin_des_resistor %s info' %(i[0])
		print '-' * 100
		for j in i[1]:
			print j
		print 'total:', len(i[1])

	#map_gpioif_list_net_pin_des_resistor.pop('OTHER', None)

	return map_gpioif_list_net_pin_des_resistor

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

def gen_ip_constrain(list_net_pin_port_des):
	print '#', '-' * 100
	print '#', 'ip constrain'
	print '#', '-' * 100

	for net, pin, port, des in list_net_pin_port_des:
		print '\n#%s\nset_property PACKAGE_PIN %s [get_ports {%s}]' %(net, pin, port)
		print 'set_property IOSTANDARD LVCMOS25 [get_ports {%s}]' %(port)
		if des:
			for i in des:
				print i

def gen_gpio_constrain(map_gpioif_list_net_pin_des_resistor):
	list_net_pin_des_resistor_gpio_gpio_no = []
	start = 0
	top_pin_no = 256
	#top_pin_no = 180
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
	for i, j in map_gpioif_list_net_pin_des_resistor.items():
		if i == 'ALL':
			common_list_net_pin_des_resistor.extend(j)
	map_common_list_net_pin_des_resistor = {}
	map_common_list_net_pin_des_resistor.update({'common gpio' : common_list_net_pin_des_resistor})

	#for i, j in map_gpioif_list_net_pin_des_resistor.items():
	for i, j in map_common_list_net_pin_des_resistor.items():
		list_len = len(j)

		if list_len == 0:
			continue

		list_if_gpio_ports = list_gpio_ports[start : ]

		if len(list_if_gpio_ports) < list_len:
			err_msg = 'len(list_if_gpio_ports):%d list_len:%d' %(len(list_if_gpio_ports), list_len)
			raise Exception(err_msg)

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
		#top_pin_no = 180

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
	list_kc705_net_group_part_pin = gen_demo_board_list_kc705_net_group_part_pin()

	list_fmc_pin_resistor = gen_list_fmc_pin_resistor()

	list_fmc_part_no = gen_new_board_list_fmc_part_no()

	list_pin_iotype = gen_kc705_list_pin_iotype()

	list_pin_net = kc705_gen_list_pin_net(list_pin_iotype)

	list_slot_list_portnum_pin_net = gen_new_board_list_slot_list_portnum_pin_net(list_fmc_part_no, list_pin_net, list_kc705_net_group_part_pin)
	#list_slot_list_portnum_pin_net = get_list_slot_list_portnum_pin_net()

	remove_unused_pin(list_pin_net)

	list_net_pin_port_des = ip_get_list_net_pin_port_des(list_pin_net, list_slot_list_portnum_pin_net)

	map_gpioif_list_net_pin_des_resistor = get_map_gpioif_list_net_pin_des_resistor(list_pin_net, list_kc705_net_group_part_pin, list_fmc_pin_resistor, list_slot_list_portnum_pin_net)

	gen_default_contrain()

	gen_ip_constrain(list_net_pin_port_des)

	list_net_pin_des_resistor_gpio_gpio_no = gen_gpio_constrain(map_gpioif_list_net_pin_des_resistor)

	gen_bitstream_constrain()

	gen_gpio_test_list(list_net_pin_des_resistor_gpio_gpio_no)

if __name__ == "__main__":
	gen_kc705_constrain()
