# -*- coding: utf-8 -*-
import re

gpio_groups = [
	'gpio_tri_io',
	'gpio2_tri_io',
	'gpio_1_tri_io',
	'gpio2_1_tri_io',
	'gpio_2_tri_io',
	'gpio2_2_tri_io',
	'gpio_3_tri_io',
	'gpio2_3_tri_io',
]

package_pins = {
	'H29': 'FMC_C2M_PG_LS',
	'C27': 'FMC_HPC_CLK0_M2C_N',
	'D27': 'FMC_HPC_CLK0_M2C_P',
	'D18': 'FMC_HPC_CLK1_M2C_N',
	'D17': 'FMC_HPC_CLK1_M2C_P',
	'D1': 'FMC_HPC_DP0_C2M_N',
	'D2': 'FMC_HPC_DP0_C2M_P',
	'E3': 'FMC_HPC_DP0_M2C_N',
	'E4': 'FMC_HPC_DP0_M2C_P',
	'C3': 'FMC_HPC_DP1_C2M_N',
	'C4': 'FMC_HPC_DP1_C2M_P',
	'D5': 'FMC_HPC_DP1_M2C_N',
	'D6': 'FMC_HPC_DP1_M2C_P',
	'B1': 'FMC_HPC_DP2_C2M_N',
	'B2': 'FMC_HPC_DP2_C2M_P',
	'B5': 'FMC_HPC_DP2_M2C_N',
	'B6': 'FMC_HPC_DP2_M2C_P',
	'A3': 'FMC_HPC_DP3_C2M_N',
	'A4': 'FMC_HPC_DP3_C2M_P',
	'A7': 'FMC_HPC_DP3_M2C_N',
	'A8': 'FMC_HPC_DP3_M2C_P',
	'C7': 'FMC_HPC_GBTCLK0_M2C_C_N',
	'C8': 'FMC_HPC_GBTCLK0_M2C_C_P',
	'E7': 'FMC_HPC_GBTCLK1_M2C_C_N',
	'E8': 'FMC_HPC_GBTCLK1_M2C_C_P',
	'D13': 'FMC_HPC_HA00_CC_N',
	'D12': 'FMC_HPC_HA00_CC_P',
	'G14': 'FMC_HPC_HA01_CC_N',
	'H14': 'FMC_HPC_HA01_CC_P',
	'C11': 'FMC_HPC_HA02_N',
	'D11': 'FMC_HPC_HA02_P',
	'B12': 'FMC_HPC_HA03_N',
	'C12': 'FMC_HPC_HA03_P',
	'E11': 'FMC_HPC_HA04_N',
	'F11': 'FMC_HPC_HA04_P',
	'E16': 'FMC_HPC_HA05_N',
	'F15': 'FMC_HPC_HA05_P',
	'C14': 'FMC_HPC_HA06_N',
	'D14': 'FMC_HPC_HA06_P',
	'A15': 'FMC_HPC_HA07_N',
	'B14': 'FMC_HPC_HA07_P',
	'E15': 'FMC_HPC_HA08_N',
	'E14': 'FMC_HPC_HA08_P',
	'E13': 'FMC_HPC_HA09_N',
	'F12': 'FMC_HPC_HA09_P',
	'A12': 'FMC_HPC_HA10_N',
	'A11': 'FMC_HPC_HA10_P',
	'A13': 'FMC_HPC_HA11_N',
	'B13': 'FMC_HPC_HA11_P',
	'B15': 'FMC_HPC_HA12_N',
	'C15': 'FMC_HPC_HA12_P',
	'K16': 'FMC_HPC_HA13_N',
	'L16': 'FMC_HPC_HA13_P',
	'H16': 'FMC_HPC_HA14_N',
	'J16': 'FMC_HPC_HA14_P',
	'G15': 'FMC_HPC_HA15_N',
	'H15': 'FMC_HPC_HA15_P',
	'K15': 'FMC_HPC_HA16_N',
	'L15': 'FMC_HPC_HA16_P',
	'F13': 'FMC_HPC_HA17_CC_N',
	'G13': 'FMC_HPC_HA17_CC_P',
	'J14': 'FMC_HPC_HA18_N',
	'K14': 'FMC_HPC_HA18_P',
	'H12': 'FMC_HPC_HA19_N',
	'H11': 'FMC_HPC_HA19_P',
	'J13': 'FMC_HPC_HA20_N',
	'K13': 'FMC_HPC_HA20_P',
	'J12': 'FMC_HPC_HA21_N',
	'J11': 'FMC_HPC_HA21_P',
	'K11': 'FMC_HPC_HA22_N',
	'L11': 'FMC_HPC_HA22_P',
	'L13': 'FMC_HPC_HA23_N',
	'L12': 'FMC_HPC_HA23_P',
	'B25': 'FMC_HPC_LA00_CC_N',
	'C25': 'FMC_HPC_LA00_CC_P',
	'C26': 'FMC_HPC_LA01_CC_N',
	'D26': 'FMC_HPC_LA01_CC_P',
	'H25': 'FMC_HPC_LA02_N',
	'H24': 'FMC_HPC_LA02_P',
	'H27': 'FMC_HPC_LA03_N',
	'H26': 'FMC_HPC_LA03_P',
	'F28': 'FMC_HPC_LA04_N',
	'G28': 'FMC_HPC_LA04_P',
	'F30': 'FMC_HPC_LA05_N',
	'G29': 'FMC_HPC_LA05_P',
	'G30': 'FMC_HPC_LA06_N',
	'H30': 'FMC_HPC_LA06_P',
	'D28': 'FMC_HPC_LA07_N',
	'E28': 'FMC_HPC_LA07_P',
	'E30': 'FMC_HPC_LA08_N',
	'E29': 'FMC_HPC_LA08_P',
	'A30': 'FMC_HPC_LA09_N',
	'B30': 'FMC_HPC_LA09_P',
	'C30': 'FMC_HPC_LA10_N',
	'D29': 'FMC_HPC_LA10_P',
	'F27': 'FMC_HPC_LA11_N',
	'G27': 'FMC_HPC_LA11_P',
	'B29': 'FMC_HPC_LA12_N',
	'C29': 'FMC_HPC_LA12_P',
	'A26': 'FMC_HPC_LA13_N',
	'A25': 'FMC_HPC_LA13_P',
	'A28': 'FMC_HPC_LA14_N',
	'B28': 'FMC_HPC_LA14_P',
	'B24': 'FMC_HPC_LA15_N',
	'C24': 'FMC_HPC_LA15_P',
	'A27': 'FMC_HPC_LA16_N',
	'B27': 'FMC_HPC_LA16_P',
	'E20': 'FMC_HPC_LA17_CC_N',
	'F20': 'FMC_HPC_LA17_CC_P',
	'E21': 'FMC_HPC_LA18_CC_N',
	'F21': 'FMC_HPC_LA18_CC_P',
	'F18': 'FMC_HPC_LA19_N',
	'G18': 'FMC_HPC_LA19_P',
	'D19': 'FMC_HPC_LA20_N',
	'E19': 'FMC_HPC_LA20_P',
	'A21': 'FMC_HPC_LA21_N',
	'A20': 'FMC_HPC_LA21_P',
	'B20': 'FMC_HPC_LA22_N',
	'C20': 'FMC_HPC_LA22_P',
	'A22': 'FMC_HPC_LA23_N',
	'B22': 'FMC_HPC_LA23_P',
	'A17': 'FMC_HPC_LA24_N',
	'A16': 'FMC_HPC_LA24_P',
	'F17': 'FMC_HPC_LA25_N',
	'G17': 'FMC_HPC_LA25_P',
	'A18': 'FMC_HPC_LA26_N',
	'B18': 'FMC_HPC_LA26_P',
	'B19': 'FMC_HPC_LA27_N',
	'C19': 'FMC_HPC_LA27_P',
	'C16': 'FMC_HPC_LA28_N',
	'D16': 'FMC_HPC_LA28_P',
	'B17': 'FMC_HPC_LA29_N',
	'C17': 'FMC_HPC_LA29_P',
	'C22': 'FMC_HPC_LA30_N',
	'D22': 'FMC_HPC_LA30_P',
	'F22': 'FMC_HPC_LA31_N',
	'G22': 'FMC_HPC_LA31_P',
	'C21': 'FMC_HPC_LA32_N',
	'D21': 'FMC_HPC_LA32_P',
	'H22': 'FMC_HPC_LA33_N',
	'H21': 'FMC_HPC_LA33_P',
	'J29': 'FMC_HPC_PG_M2C_LS',
	'M20': 'FMC_HPC_PRSNT_M2C_B_LS',
	'AG23': 'FMC_LPC_CLK0_M2C_N',
	'AF22': 'FMC_LPC_CLK0_M2C_P',
	'AH29': 'FMC_LPC_CLK1_M2C_N',
	'AG29': 'FMC_LPC_CLK1_M2C_P',
	'F1': 'FMC_LPC_DP0_C2M_N',
	'F2': 'FMC_LPC_DP0_C2M_P',
	'F5': 'FMC_LPC_DP0_M2C_N',
	'F6': 'FMC_LPC_DP0_M2C_P',
	'N7': 'FMC_LPC_GBTCLK0_M2C_C_N',
	'N8': 'FMC_LPC_GBTCLK0_M2C_C_P',
	'AE24': 'FMC_LPC_LA00_CC_N',
	'AD23': 'FMC_LPC_LA00_CC_P',
	'AF23': 'FMC_LPC_LA01_CC_N',
	'AE23': 'FMC_LPC_LA01_CC_P',
	'AF21': 'FMC_LPC_LA02_N',
	'AF20': 'FMC_LPC_LA02_P',
	'AH20': 'FMC_LPC_LA03_N',
	'AG20': 'FMC_LPC_LA03_P',
	'AJ21': 'FMC_LPC_LA04_N',
	'AH21': 'FMC_LPC_LA04_P',
	'AH22': 'FMC_LPC_LA05_N',
	'AG22': 'FMC_LPC_LA05_P',
	'AK21': 'FMC_LPC_LA06_N',
	'AK20': 'FMC_LPC_LA06_P',
	'AH25': 'FMC_LPC_LA07_N',
	'AG25': 'FMC_LPC_LA07_P',
	'AJ23': 'FMC_LPC_LA08_N',
	'AJ22': 'FMC_LPC_LA08_P',
	'AK24': 'FMC_LPC_LA09_N',
	'AK23': 'FMC_LPC_LA09_P',
	'AK25': 'FMC_LPC_LA10_N',
	'AJ24': 'FMC_LPC_LA10_P',
	'AF25': 'FMC_LPC_LA11_N',
	'AE25': 'FMC_LPC_LA11_P',
	'AB20': 'FMC_LPC_LA12_N',
	'AA20': 'FMC_LPC_LA12_P',
	'AC25': 'FMC_LPC_LA13_N',
	'AB24': 'FMC_LPC_LA13_P',
	'AE21': 'FMC_LPC_LA14_N',
	'AD21': 'FMC_LPC_LA14_P',
	'AD24': 'FMC_LPC_LA15_N',
	'AC24': 'FMC_LPC_LA15_P',
	'AD22': 'FMC_LPC_LA16_N',
	'AC22': 'FMC_LPC_LA16_P',
	'AC27': 'FMC_LPC_LA17_CC_N',
	'AB27': 'FMC_LPC_LA17_CC_P',
	'AD28': 'FMC_LPC_LA18_CC_N',
	'AD27': 'FMC_LPC_LA18_CC_P',
	'AK26': 'FMC_LPC_LA19_N',
	'AJ26': 'FMC_LPC_LA19_P',
	'AF27': 'FMC_LPC_LA20_N',
	'AF26': 'FMC_LPC_LA20_P',
	'AG28': 'FMC_LPC_LA21_N',
	'AG27': 'FMC_LPC_LA21_P',
	'AK28': 'FMC_LPC_LA22_N',
	'AJ27': 'FMC_LPC_LA22_P',
	'AH27': 'FMC_LPC_LA23_N',
	'AH26': 'FMC_LPC_LA23_P',
	'AH30': 'FMC_LPC_LA24_N',
	'AG30': 'FMC_LPC_LA24_P',
	'AD26': 'FMC_LPC_LA25_N',
	'AC26': 'FMC_LPC_LA25_P',
	'AK30': 'FMC_LPC_LA26_N',
	'AK29': 'FMC_LPC_LA26_P',
	'AJ29': 'FMC_LPC_LA27_N',
	'AJ28': 'FMC_LPC_LA27_P',
	'AF30': 'FMC_LPC_LA28_N',
	'AE30': 'FMC_LPC_LA28_P',
	'AF28': 'FMC_LPC_LA29_N',
	'AE28': 'FMC_LPC_LA29_P',
	'AB30': 'FMC_LPC_LA30_N',
	'AB29': 'FMC_LPC_LA30_P',
	'AE29': 'FMC_LPC_LA31_N',
	'AD29': 'FMC_LPC_LA31_P',
	'AA30': 'FMC_LPC_LA32_N',
	'Y30': 'FMC_LPC_LA32_P',
	'AC30': 'FMC_LPC_LA33_N',
	'AC29': 'FMC_LPC_LA33_P',
	'J22': 'FMC_LPC_PRSNT_M2C_B_LS',
	'J27': 'FMC_VADJ_ON_B_LS',
}

unsupport_loc = [
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

pin_gpio_map = {}
unsupport_package_pins = []

def gen_port_constrain():
	ports = []
	for i in gpio_groups:
		for j in range(32):
			ports.append(i + '[' + str(j) + ']')
	ports = ports[:197]
	#print ports
	#print len(ports)

	for i in package_pins.items():
		print '封装pin:%-20s网络:%-20s' %(i[0], i[1])

	for i in unsupport_loc:
		unsupport_package_pins.append([i, package_pins.get(i)])
		package_pins.pop(i)
	#print len(package_pins)

	if len(package_pins) != len(ports):
		print "pin count error!"
		return

	print '-' * 200
	for i, j in package_pins.items():
		port = ports.pop(0)
		pin_gpio_map[j] = port
		print '\n#%s\nset_property PACKAGE_PIN %s [get_ports {%s}]' %(j, i, port)
		print 'set_property IOSTANDARD LVCMOS18 [get_ports {%s}]' %(port)

line_no = [
	95,
	98,
	101,
	233,
	239,
	242,
	245,
	296,
	383,
	386,
	389,
	392,
	443,
	446,
	449,
	452,
	614,
	620,
	680,
	683,
	686,
	689,
	701,
	704,
	707,
	710,
]

def get_spec_lines(line_no = []):
	lines = []
	with open('log') as f:
		lines = f.read().splitlines()
		print lines
	for i in line_no:
		print lines[i - 1]


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

def kc705_pins_key(x):
	#print x
	info = x[3]
	pattern = re.compile(r'([^\d]+)(\d+)')
	g = pattern.match(info)
	r = g.groups()
	return r[0], int(r[1])

def fmc_pins_key(x):
	#print x
	info = x[5]
	pattern = re.compile(r'([^\d]+)(\d+)')
	g = pattern.match(info)
	r = g.groups()
	return r[0], int(r[1])

def gpio_pins_resistor_key(x):
	info = x[2]
	pattern = re.compile(r'([^\d]+)(\d+)')
	g = pattern.match(info)
	r = g.groups()
	return r[0], int(r[1])

def gpio_pins_gpio_key(x):
	info = x[0]
	info = info.replace('_', ' ')
	info = info.replace('[', ' ')
	info = info.replace(']', ' ')
	info = info.split()
	if len(info) == 4:
		info.insert(1, '0')
	return int(info[1]), info[0], int(info[4])

def get_gpios_pin_no(gpios):
	gpio_bank_base = [148, 116, 84, 52, 20, 0, 15]
	pattern = re.compile(r'_|\[|\]')
	for i in gpios:
		gpio = pattern.sub(r' ', i)
		gpio = gpio.split()
		if len(gpio) == 4:
			gpio.insert(1, '0')
		chip = int(gpio[1])
		channel = 0 if gpio[0] == 'gpio' else 1
		pin = int(gpio[4])
		index = chip * 2 + channel
		if gpio_bank_base[index] == 0:
			#print 'invalid gpio %s' %(i)
			continue
		pin_no = gpio_bank_base[index] + pin

		print "%d" %(pin_no)

def get_nc_info():
	content = ''

	#从网表中提取fmc的pin脚与网络的关系
	with open('kc705FMC.txt') as f:
		content = f.read()
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n(R[^\.]+)\.(\d+)\s+([^\-\s]+)-([^\.]+)\.([^\s]+)')
	result = pattern.findall(content)
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n([^\-\s]+)-([^\.]+)\.([^\s]+)\s+(R[^\.]+)\.(\d+)')
	result.extend([(x[0], x[4], x[5], x[1], x[2], x[3]) for x in pattern.findall(content)])
	fmc_pins = sorted(result, key = lambda x:(fmc_pins_key(x)))
	pin_resistor_map = {}
	for i in fmc_pins:
		pin_resistor_map[i[5]] = i[1] + '.' + i[2]

	#从网表中提取kc705的pin脚与网络的关系
	with open('kc705.txt') as f:
		content = f.read()
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n\s*([^\-\s]+)-([^\.]+)\.([^\s]+)')
	result = pattern.findall(content)
	kc705_pins = sorted(result, key = lambda x:(kc705_pins_key(x)))

	#建立板上fmc端口与package_pins之间的关联
	result = []
	for i in kc705_pins:
		if not i[1] in ['J2', 'J22']:
			continue
		if i[0] in package_pins.values():
			result.append(
				(pin_gpio_map[i[0]] if i[0] in pin_gpio_map.keys() else '?' * 200, i, pin_resistor_map[i[3]] if i[3] in pin_resistor_map.keys() else '!' * 200)
			)

	#result = sorted(result, key = lambda x:(gpio_pins_resistor_key(x)))
	#for i in result:
	#	print i
	#print '-' * 200
	#result = sorted(result, key = lambda x:(gpio_pins_gpio_key(x)))
	#for i in result:
	#	print i

	#建立电阻id与fmc的package_pins之间的映射关系
	result_dict = {}
	for i in result:
		r = i[2]
		pattern = re.compile(r'R(\d+).*')
		m = pattern.match(r)
		key = m.group(1)

		if not result_dict.has_key(int(key)):
			result_dict[int(key)] = []

		result_dict[int(key)].append(i)

	pins_list = []
	for i in fmc_board_pins.strip().splitlines():
		pins_list.append(i.strip().split())

	#输出fmc板上电阻与gpio的映射关系
	print '-' * 200
	used_packagepin = []
	gpios_sequence = []
	for i in pins_list:
		for j in i:
			if not int(j) in result_dict.keys():
				#输出fmc板上用不到的pin(电阻)
				#print j, 'is not valid pin!!!'
				continue
			v = result_dict.get(int(j))
			for k in v:
				used_packagepin.append(k[1][0])
				gpios_sequence.append(k[0])
				print k
	
	#打印pin号
	print '-' * 200
	get_gpios_pin_no(gpios_sequence)

	#哪些package_pin没有用上？
	for i, j in package_pins.items():
		if not j in used_packagepin:
			unsupport_package_pins.append([i, j])

	print '-' * 200
	for i in unsupport_package_pins:
		print '封装pin:%-20s网络:%-20s' %(i[0], i[1])
		
	

if __name__ == "__main__":
	gen_port_constrain()
	get_nc_info()
