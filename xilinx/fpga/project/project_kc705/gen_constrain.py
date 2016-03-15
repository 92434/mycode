# -*- coding: utf-8 -*-
import re
import copy

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

def gen_gpio_ports(gpio_groups):
	ports = []
	for i in gpio_groups:
		for j in range(32):
			ports.append(i + '[' + str(j) + ']')
	return ports

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

def gen_support_package_pins(package_pins, unsupport_loc):
	print '-' * 100
	print 'all package_pins'
	print '-' * 100
	for i in package_pins.items():
		print '封装pin:%-20s网络:%-20s' %(i[0], i[1])

	pins = {}
	for i, j in package_pins.items():
		if not i in unsupport_loc:
			pins.update({i:j})
	return pins

def constrain_key_gpio(x):
	io, pin, gpio = x

	pattern = re.compile(r'_|\[|\]')
	gpio = pattern.sub(r' ', gpio)
	gpio = gpio.split()
	if len(gpio) == 4:
		gpio.insert(1, '0')
	chip = int(gpio[1])
	channel = 0 if gpio[0] == 'gpio' else 1
	pin = int(gpio[4])
	return chip, channel, pin


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
	info = x[1]
	pattern = re.compile(r'([^\d]+)(\d+)')
	g = pattern.match(info)
	r = g.groups()
	return r[0], int(r[1])

def get_gpio_pin_no(gpio, start, gpio_bank_base):
	chip_offset = start / (32 * 2)
	channel_offset = (start / 32) % 2

	pattern = re.compile(r'_|\[|\]')
	gpio = pattern.sub(r' ', gpio)
	gpio = gpio.split()
	if len(gpio) == 4:
		gpio.insert(1, '0')

	chip = int(gpio[1]) - chip_offset
	channel = 0 if gpio[0] == 'gpio' else 1
	channel = channel - channel_offset
	pin = int(gpio[4])

	index = chip * 2 + channel
	if gpio_bank_base[index] == 0:
		return

	pin_no = gpio_bank_base[index] + pin

	return pin_no

def get_gpios_pin_no_for_driver(constrain, hpc_lpc_pins_resistor_map, fmc_type, start, count):
	print '-' * 100
	print '%s gpio pin number for driver' %(fmc_type)
	print '-' * 100

	print 'start', start
	if (start % (32 * 2)) != 0:
		start = 32 * 2 * ((start / (32 * 2)) + 1)

	pins_resistor = hpc_lpc_pins_resistor_map.get(fmc_type)

	gpio_boundary = 180

	gpio_bank_base = []
	for i in range(count / 32):
		gpio_boundary -= 32
		gpio_bank_base.append(gpio_boundary)
	gpio_boundary -= (count % 32);
	gpio_bank_base.append(gpio_boundary)
	
	io_pin_no_map = {}
	for io, pin, gpio in constrain:
		pin_no = get_gpio_pin_no(gpio, start, gpio_bank_base)
		io_pin_no_map.update({io: pin_no})

	for i in pins_resistor:
		#print '%s\t%s\t%s' %(io_pin_no_map.get(i[0][0]), i[1], str(i))
		print '%s\t%s\t%s' %(io_pin_no_map.get(i[0][0]), i[1], i[0][0])

def kc705_pins_resistor_key_resistor(x):
	r = x[1]

	pattern = re.compile(r'R|\.')
	r = pattern.sub(r' ', r)
	r = r.split()
	return int(r[0])

def gen_fmc_pin_resistor():
	content = ''

	with open('kc705FMC.txt') as f:
		content = f.read()
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n(R[^\.]+)\.(\d+)\s+([^\-\s]+)-([^\.]+)\.([^\s]+)')
	result = pattern.findall(content)
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n([^\-\s]+)-([^\.]+)\.([^\s]+)\s+(R[^\.]+)\.(\d+)')
	result.extend([(x[0], x[4], x[5], x[1], x[2], x[3]) for x in pattern.findall(content)])
	fmc_pins = sorted(result, key = lambda x:(fmc_pins_key(x)))
	fmc_pin_resistor = []
	for i in fmc_pins:
		fmc_pin_resistor.append((i[5], i[1] + '.' + i[2]))
	fmc_pin_resistor.append(('F1', 'R113.1'))
	fmc_pin_resistor.append(('H2', 'R62.1'))
	return fmc_pin_resistor

def gen_kc705_pins():
	content = ''

	with open('kc705.txt') as f:
		content = f.read()
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n\s*([^\-\s]+)-([^\.]+)\.([^\s]+)')
	result = pattern.findall(content)
	kc705_pins = sorted(result, key = lambda x:(kc705_pins_key(x)))
	kc705_pins.append(('FMC_LPC_PRSNT_M2C_B_LS', 'J2', 'E', 'H2'))
	kc705_pins.append(('FMC_HPC_PRSNT_M2C_B_LS', 'J22', 'I', 'H2'))
	kc705_pins.append(('FMC_HPC_PG_M2C_LS', 'J22', 'F', 'F1'))
	kc705_pins.append(('FMC_C2M_PG_LS', 'J22', 'D', 'D1'))#not valid
	#print kc705_pins
	return kc705_pins

def gen_kc705_pins_resistor(support_package_pins, kc705_pins, pin_resistor_map):
	kc705_pins_resistor = []
	for i in kc705_pins:
		if not i[0] in support_package_pins.values():
			#print '%s can not be controled!' %(str(i))
			continue
		if not i[1] in ['J2', 'J22']:
			#print '%s is not for fmc!' %(str(i))
			continue
		if not i[3] in pin_resistor_map.keys():
			#print '%s can not find resistor!' %(str(i))
			continue

		kc705_pins_resistor.append(
			(
				i,
				pin_resistor_map.get(i[3])
			)
		)

	kc705_pins_resistor = sorted(kc705_pins_resistor, key = lambda x:(gpio_pins_resistor_key(x)), reverse = True)
	print '-' * 100
	print 'kc705 package pins info'
	print '-' * 100
	for i in kc705_pins_resistor:
		print i
	print 'total:', len(kc705_pins_resistor)

	return kc705_pins_resistor

def gen_resistor_kc705_pins_resistor_map(kc705_pins_resistor):
	#建立电阻id与fmc的package_pins之间的映射关系
	resistor_kc705_pins_resistor_map = {}
	for i in kc705_pins_resistor:
		r = i[1]
		pattern = re.compile(r'R(\d+).*')
		m = pattern.match(r)
		key = m.group(1)

		if not resistor_kc705_pins_resistor_map.has_key(int(key)):
			resistor_kc705_pins_resistor_map.update({int(key): []})

		resistor_kc705_pins_resistor_map.get(int(key)).append(i)

	return resistor_kc705_pins_resistor_map

def gen_hpc_lpc_pins_resistor_map(resistor_kc705_pins_resistor_map):
	hpc_lpc_pins_resistor_map = {}
	hpc_lpc_pins_resistor_map.update({'HPC': []})
	hpc_lpc_pins_resistor_map.update({'LPC': []})
	hpc_lpc_pins_resistor_map.update({'unused_resistor': []})

	pins_list = []
	for i in fmc_board_pins.strip().splitlines():
		pins_list.append(i.strip().split())

	for i in pins_list:
		for j in i:
			if not int(j) in resistor_kc705_pins_resistor_map.keys():
				#输出fmc板上用不到的pin(电阻)
				#print j, 'is not valid pin!!!'
				hpc_lpc_pins_resistor_map.get('unused_resistor').append(int(j))
				continue
			v = resistor_kc705_pins_resistor_map.get(int(j))
			for vv in v:
				if vv[0][1] == 'J22':
					hpc_lpc_pins_resistor_map.get('HPC').append(vv)
				elif vv[0][1] == 'J2':
					hpc_lpc_pins_resistor_map.get('LPC').append(vv)
				else:
					print 'ERROR:pin %s is not for hpc or lpc!' %(str(vv))
	return hpc_lpc_pins_resistor_map


def gen_default_contrain():
	txt = """
# Sys Clock Pins
#set_property PACKAGE_PIN AD11 [get_ports EXT_SYS_CLK_clk_n]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports EXT_SYS_CLK_clk_n]

#set_property PACKAGE_PIN AD12 [get_ports EXT_SYS_CLK_clk_p]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports EXT_SYS_CLK_clk_p]

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
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[4]}]

set_property PACKAGE_PIN G19 [get_ports {EXT_LEDS[5]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[5]}]

set_property PACKAGE_PIN E18 [get_ports {EXT_LEDS[6]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[6]}]

set_property PACKAGE_PIN F16 [get_ports {EXT_LEDS[7]}]
set_property IOSTANDARD LVCMOS15 [get_ports {EXT_LEDS[7]}]
	"""

	print '#', '-' * 100
	print '#', 'kc705 default constrain'
	print '#', '-' * 100
	print txt

myip_fmc_signal = [
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

	('FMC_LPC_LA10_P', 'symbol_2x_oe'),
		
	('FMC_LPC_LA00_CC_P', 'symbol_2x_re_out[0]'),
	('FMC_LPC_LA02_P', 'symbol_2x_re_out[1]'),
	('FMC_LPC_LA00_CC_N', 'symbol_2x_re_out[2]'),
	('FMC_LPC_LA02_N', 'symbol_2x_re_out[3]'),
	('FMC_LPC_LA03_P', 'symbol_2x_re_out[4]'),
	('FMC_LPC_LA04_P', 'symbol_2x_re_out[5]'),
	('FMC_LPC_LA03_N', 'symbol_2x_re_out[6]'),
	('FMC_LPC_LA04_N', 'symbol_2x_re_out[7]'),
	('FMC_LPC_LA07_P', 'symbol_2x_re_out[8]'),
	('FMC_LPC_LA08_P', 'symbol_2x_re_out[9]'),
	('FMC_LPC_LA01_CC_P', 'symbol_2x_re_out[10]'),
	('FMC_LPC_LA01_CC_N', 'symbol_2x_re_out[11]'),
	('FMC_LPC_LA06_P', 'symbol_2x_re_out[12]'),
	('FMC_LPC_LA06_N', 'symbol_2x_re_out[13]'),
	('FMC_LPC_LA05_P', 'symbol_2x_re_out[14]'),
	('FMC_LPC_LA05_N', 'symbol_2x_re_out[15]'),

	('FMC_LPC_LA10_N', 'symbol_2x_im_out[0]'),
	('FMC_LPC_LA09_P', 'symbol_2x_im_out[1]'),
	('FMC_LPC_LA09_N', 'symbol_2x_im_out[2]'),
	('FMC_LPC_LA13_P', 'symbol_2x_im_out[3]'),
	('FMC_LPC_LA14_P', 'symbol_2x_im_out[4]'),
	('FMC_LPC_LA13_N', 'symbol_2x_im_out[5]'),
	('FMC_LPC_LA14_N', 'symbol_2x_im_out[6]'),
	('FMC_LPC_LA07_N', 'symbol_2x_im_out[7]'),
	('FMC_LPC_LA08_N', 'symbol_2x_im_out[8]'),
	('FMC_LPC_LA12_P', 'symbol_2x_im_out[9]'),
	('FMC_LPC_LA11_P', 'symbol_2x_im_out[10]'),
	('FMC_LPC_LA12_N', 'symbol_2x_im_out[11]'),
	('FMC_LPC_LA11_N', 'symbol_2x_im_out[12]'),
	('FMC_LPC_LA16_P', 'symbol_2x_im_out[13]'),
	('FMC_LPC_LA16_N', 'symbol_2x_im_out[14]'),
	('FMC_LPC_LA15_P', 'symbol_2x_im_out[15]'),

	('FMC_LPC_LA22_N', 'clk_out2'),
	('FMC_LPC_LA21_P', 'clk_out3'),

	('FMC_LPC_LA20_P', 'clk_out4'),

	('FMC_LPC_LA32_N', 'asi_out_p'),
	('FMC_LPC_LA33_N', 'asi_out_n'),

	('FMC_LPC_CLK0_M2C_P', 'dvbs2_sys_clk'),
]

#myip_fmc_signal = []

def gen_constrain(hpc_lpc_pins_resistor_map, fmc_type, start):
	constrain = []

	pins_resistor = hpc_lpc_pins_resistor_map.get(fmc_type)
	myip_fmc_ios = []
	for io, signal in myip_fmc_signal:
		myip_fmc_ios.append(io)

	signal_index = []
	for i in range(len(pins_resistor)):
		pin, resistor = pins_resistor[i]
		io = pin[0]
		if io in myip_fmc_ios:
			signal_index.append(i)
	signal_index.reverse()
	for i in signal_index:
		pins_resistor.pop(i)

	ports = gen_gpio_ports(gpio_groups)

	if (start % 32) != 0:
		start = 32 * ((start / 32) + 1)

	ports = ports[start:]

	if len(pins_resistor) <= len(ports):
		ports = ports[:len(pins_resistor)]
	else:
		print "gpio bank is not enough!"
		return constrain

	io_pin_map = {}
	for pin, io in package_pins.items():
		io_pin_map.update({io: pin})

	for i in range(len(ports)):
		gpio = ports[i]
		pin_resistor = pins_resistor[i]
		io = pin_resistor[0][0]
		pin = io_pin_map.get(io)

		constrain.append((io, pin, gpio))

	print '#', '-' * 100
	print '#', 'generator constrain for %s' %(fmc_type)
	print '#', '-' * 100
	constrain = sorted(constrain, key = lambda x:(constrain_key_gpio(x)))
	for i in constrain:
		io, pin, gpio = i
		print '\n#%s\nset_property PACKAGE_PIN %s [get_ports {%s}]' %(io, pin, gpio)
		print 'set_property IOSTANDARD LVCMOS15 [get_ports {%s}]' %(gpio)

	return constrain

def gen_myip_constrain():
	myip_extra_signal = [
		#('XADC_GPIO_0', 'AB25', 'clk_out1', 'LVCMOS15'),
		#('USER_SMA_GPIO_P', 'Y23', 'asi_out', 'LVCMOS15'),
		#('LCD_DB4_LS', 'AA13', 'asi_out', 'LVCMOS15'),
		#('GPIO_SW_E', 'AG5', 'asi_out', 'LVCMOS15'),
		('IO_L7P_T1_33', 'AB10', 'lcm_din', 'LVCMOS15'),
		('IO_L4P_T0_33', 'Y11', 'lcm_lp', 'LVCMOS15'),
		('IO_L6N_T0_VREF_33', 'AB13', 'lcm_xscl', 'LVCMOS15'),
		('IO_L6N_T0_VREF_13', 'AB25', 'lcm_data[0]', 'LVCMOS15'),
		('IO_L6P_T0_13', 'AA25', 'lcm_data[1]', 'LVCMOS15'),
		('IO_L5N_T0_13', 'AB28', 'lcm_data[2]', 'LVCMOS15'),
		('IO_L5P_T0_13', 'AA27', 'lcm_data[3]', 'LVCMOS15'),
		('IO_L6P_T0_33', 'AA13', 'lcm_data[4]', 'LVCMOS15'),
		('IO_L5N_T0_33', 'AA10', 'lcm_data[5]', 'LVCMOS15'),
		('IO_L5P_T0_33', 'AA11', 'lcm_data[6]', 'LVCMOS15'),
		('IO_L4N_T0_33', 'Y10', 'lcm_data[7]', 'LVCMOS15'),
	]

	extra_property = {
		#'i2s_receiver_bclk': ['CLOCK_DEDICATED_ROUTE FALSE'],
		'mpeg_clk': ['CLOCK_DEDICATED_ROUTE FALSE'],
		'dvbs2_sys_clk': ['CLOCK_DEDICATED_ROUTE FALSE'],
	}

	fmc_io_stand_except = [
	]


	print '#', '-' * 100
	print '#', 'generator constrain for ip signals'
	print '#', '-' * 100

	io_pin_map = {}
	for pin, io in package_pins.items():
		io_pin_map.update({io: pin})

	for io, signal in myip_fmc_signal:
		pin = io_pin_map.get(io)
		print '\n#%s\nset_property PACKAGE_PIN %s [get_ports {%s}]' %(io, pin, signal)
		if not io in fmc_io_stand_except:
			print 'set_property IOSTANDARD LVCMOS15 [get_ports {%s}]' %(signal)
		extra_list = extra_property.get(signal)
		if not extra_list:
			continue
		for p in extra_list:
			print 'set_property %s [get_nets {%s}]' %(p, signal)

	for io, pin, signal, level in myip_extra_signal:
		print '\n#%s\nset_property PACKAGE_PIN %s [get_ports {%s}]' %(io, pin, signal)
		print 'set_property IOSTANDARD %s [get_ports {%s}]' %(level, signal)
		extra_list = extra_property.get(signal)
		if not extra_list:
			continue
		for p in extra_list:
			print 'set_property %s [get_nets {%s}]' %(p, signal)

def gen_encryption_constrain():
	txt = """
#Encryption Settings

#set_property BITSTREAM.ENCRYPTION.ENCRYPT YES [current_design]
#set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT BBRAM [current_design]
##set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT eFUSE [current_design]
#set_property BITSTREAM.ENCRYPTION.KEY0 256'h12345678ABCDDCBA1234578ABCDDCBA1234578ABCDDCBA1234578ABCDDCBA [current_design]
	"""

	print '#', '-' * 100
	print '#', 'bitstream encryption'
	print '#', '-' * 100
	print txt



def gen_unused_pin_io(support_package_pins, kc705_pins_resistor):
	print '-' * 100
	print 'package pins for unused'
	print '-' * 100
	for i, j in package_pins.items():
		if not j in support_package_pins.values():
			print '封装pin:%-20s网络:%-20s' %(i, j)
	ios = [ x[0][0] for x in kc705_pins_resistor ]
	for i, j in support_package_pins.items():
		if not j in ios:
			print '封装pin:%-20s网络:%-20s' %(i, j)

def gen_kc705_constrain():
	#从网表中提取kc705的pin脚与网络的关系
	kc705_pins = gen_kc705_pins()
	#print kc705_pins

	#从网表中提取fmc的pin脚与网络的关系
	fmc_pin_resistor = gen_fmc_pin_resistor()
	#print fmc_pin_resistor

	pin_resistor_map = {}
	for i in fmc_pin_resistor:
		pin_resistor_map.update({i[0]: i[1]})

	support_package_pins = gen_support_package_pins(package_pins, unsupport_loc)

	#建立板上package_pins与fmc端口之间的关联
	kc705_pins_resistor = gen_kc705_pins_resistor(support_package_pins, kc705_pins, pin_resistor_map)

	#哪些package_pin没有用上？
	gen_unused_pin_io(support_package_pins, kc705_pins_resistor)

	resistor_kc705_pins_resistor_map = gen_resistor_kc705_pins_resistor_map(kc705_pins_resistor)
	hpc_lpc_pins_resistor_map = gen_hpc_lpc_pins_resistor_map(resistor_kc705_pins_resistor_map)

	#输出固定约束
	gen_default_contrain()

	#输出fmc板上电阻与kc705的映射关系
	hpc_constrain = gen_constrain(hpc_lpc_pins_resistor_map, 'HPC', 0)
	lpc_constrain = gen_constrain(hpc_lpc_pins_resistor_map, 'LPC', len(hpc_constrain))
	
	gen_myip_constrain()

	gen_encryption_constrain()

	get_gpios_pin_no_for_driver(hpc_constrain, hpc_lpc_pins_resistor_map, 'HPC', 0, len(hpc_constrain))
	get_gpios_pin_no_for_driver(lpc_constrain, hpc_lpc_pins_resistor_map, 'LPC', len(hpc_constrain), len(lpc_constrain))
	print 'total', len(hpc_constrain) + len(lpc_constrain)

	#打印pin号

if __name__ == "__main__":
	gen_kc705_constrain()
