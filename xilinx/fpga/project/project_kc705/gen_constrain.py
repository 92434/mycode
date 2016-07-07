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

io_des_map = {
		'FMC_LPC_LA30_P' : 'I2C_SDA0',
		'FMC_LPC_LA30_N' : 'I2C_SCL0',
		'FMC_LPC_LA32_P' : 'SPI_CS',
		'FMC_LPC_LA20_N' : 'SPI_MISO',
		'FMC_LPC_LA23_N' : 'SPI_MOSI',
		'FMC_LPC_LA23_P' : 'SPI_SCK',
}

io_des_map = {}

def get_gpios_pin_no_for_driver(constrain, hpc_lpc_pins_des, fmc_type, start, count):
	print '-' * 100
	print '%s gpio pin number for driver' %(fmc_type)
	print '-' * 100

	print 'start', start
	if (start % (32 * 2)) != 0:
		start = 32 * 2 * ((start / (32 * 2)) + 1)

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

	
	for i in hpc_lpc_pins_des:
		#print '%s\t%s\t%s' %(io_pin_no_map.get(i[0][0]), i[1], str(i))
		pin_no = io_pin_no_map.get(i[0][0])
		if pin_no:
			des = io_des_map.get(i[0][0])
			des = '(%s)' %des if des != None else ''
			print '%-20s%-20s%-20s%-20s' %(pin_no, i[1], i[0][0], des)

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
	print '-' * 100
	print 'fmc_pin_resistor info'
	print '-' * 100
	for i in fmc_pin_resistor:
		print i
	print 'total:', len(fmc_pin_resistor)
	return fmc_pin_resistor

def gen_kc705_signals():
	content = ''

	with open('kc705_signals.txt') as f:
		content = f.read()
	pattern = re.compile(r'\*SIGNAL\* (.*) \d+ \d+\r\n\s*([^\-\s]+)-([^\.]+)\.([^\s]+)')
	result = pattern.findall(content)
	kc705_signals = sorted(result, key = lambda x:(kc705_pins_key(x)))
	kc705_signals.append(('FMC_LPC_PRSNT_M2C_B_LS', 'J2', 'E', 'H2'))
	kc705_signals.append(('FMC_HPC_PRSNT_M2C_B_LS', 'J22', 'I', 'H2'))
	kc705_signals.append(('FMC_HPC_PG_M2C_LS', 'J22', 'F', 'F1'))
	kc705_signals.append(('FMC_C2M_PG_LS', 'J22', 'D', 'D1'))#not valid
	print '-' * 100
	print 'kc705_signals info'
	print '-' * 100
	for i in kc705_signals:
		print i
	print 'total:', len(kc705_signals)
	return kc705_signals

def gen_kc705_pins_resistor(unsupport_loc, kc705_signals, fmc_pin_resistor):
	kc705_pins_resistor = []
	fmc_pins = [x[0] for x in fmc_pin_resistor]
	print 'fmc_pins:', fmc_pins
	pin_resistor_map = {}
	for i in fmc_pin_resistor:
		item = {i[0] : i[1]}
		pin_resistor_map.update(item)

	for i in kc705_signals:
		if i[3] in unsupport_loc:
			print '%s can not be controled!' %(str(i))
			continue
		if not i[1] in ['J2', 'J22']:
			print '%s is not for fmc!' %(str(i))
			continue
		if not i[3] in fmc_pins:
			print '%s can not find resistor!' %(str(i))
			continue

		kc705_pins_resistor.append(
			(
				i,
				pin_resistor_map.get(i[3])
			)
		)

	kc705_pins_resistor = sorted(kc705_pins_resistor, key = lambda x:(gpio_pins_resistor_key(x)), reverse = True)
	print '-' * 100
	print 'kc705_pins_resistor info'
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

	print '-' * 100
	print 'resistor_kc705_pins_resistor_map info'
	print '-' * 100
	for i in resistor_kc705_pins_resistor_map.items():
		print i

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

	for i in hpc_lpc_pins_resistor_map.items():
		print '-' * 100
		print 'hpc_lpc_pins_resistor_map %s info' %(i[0])
		print '-' * 100
		for j in i[1]:
			print j

	return hpc_lpc_pins_resistor_map

def gen_hpc_lpc_pins_des_map(unsupport_loc, kc705_package_pin_nets):
	hpc_lpc_pins_des_map = {}
	hpc_lpc_pins_des_map.update({'HPC': []})
	hpc_lpc_pins_des_map.update({'LPC': []})
	hpc_lpc_pins_des_map.update({'unused_fmc': []})

	gpio_pins = [
		('Y30', 'SOMI'),
		('AA30', 'MOSI'),
		('AB29', 'SCLK'),
		('AB30', '74138GA(CS)'),
		('AC29', '74138GB'),
		('AC30', '74138GC'),
		('AB27', 'SPI_S0'),
		('AC27', 'SPI_S1'),
		('AD29', 'SPI_S2'),
		('AE29', 'SPI_S3'),
	]

	for i in gpio_pins:
		if i[0] in unsupport_loc:
			continue

		if not i[0] in kc705_package_pin_nets.keys():
			print '%s can not find this package pin!' %(str(i))
			continue

		net = kc705_package_pin_nets.get(i[0])
		v = ((net,'NULL', 'NULL', i[0]), i[1])
		if net.startswith('FMC_HPC'):
			hpc_lpc_pins_des_map.get('HPC').append(v)
		elif net.startswith('FMC_LPC'):
			hpc_lpc_pins_des_map.get('LPC').append(v)
		else:
			hpc_lpc_pins_des_map.get('unused_fmc').append(v)
			print 'ERROR:pin %s is not for hpc or lpc!' %(str(v))

	for i in hpc_lpc_pins_des_map.items():
		print '-' * 100
		print 'hpc_lpc_pins_des_map %s info' %(i[0])
		print '-' * 100
		for j in i[1]:
			print j

	return hpc_lpc_pins_des_map

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


def gen_fmc_gpio_constrain(kc705_package_pin_nets, hpc_lpc_pins, fmc_type, start):
	constrain = []

	ports = gen_gpio_ports(gpio_groups)

	if (start % 32) != 0:
		start = 32 * ((start / 32) + 1)

	ports = ports[start:]

	for i in hpc_lpc_pins:
		if not i[0] in kc705_package_pin_nets.values():
			hpc_lpc_pins.remove(i)
		else:
			try:
				kc705_package_pin_nets.pop(i[3])
			except:
				#print '#%s not in kc705_package_pin_nets' %(str(i))
				hpc_lpc_pins.remove(i)

	if len(hpc_lpc_pins) <= len(ports):
		ports = ports[:len(hpc_lpc_pins)]
	else:
		print "gpio bank is not enough!"
		return constrain

	for i in range(len(ports)):
		gpio = ports[i]
		pins = hpc_lpc_pins[i]
		io = pins[0]
		pin = pins[3]

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

def gen_myip_constrain(kc705_package_pin_nets):
	myip_io_des_signals = [
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

		('FMC_LPC_CLK0_M2C_P', 'fs_0p5_en'),
	]

	myip_io_des_signals = []

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

	myip_extra_signal = []

	extra_property = {
		#'i2s_receiver_bclk': ['CLOCK_DEDICATED_ROUTE FALSE'],
		'mpeg_clk': ['CLOCK_DEDICATED_ROUTE FALSE'],
		'fs_0p5_en': ['CLOCK_DEDICATED_ROUTE FALSE'],

		'symbol_2x_oe' : ['slew FAST'],

		'symbol_2x_re_out[0]' : ['slew FAST'],
		'symbol_2x_re_out[1]' : ['slew FAST'],
		'symbol_2x_re_out[2]' : ['slew FAST'],
		'symbol_2x_re_out[3]' : ['slew FAST'],
		'symbol_2x_re_out[4]' : ['slew FAST'],
		'symbol_2x_re_out[5]' : ['slew FAST'],
		'symbol_2x_re_out[6]' : ['slew FAST'],
		'symbol_2x_re_out[7]' : ['slew FAST'],
		'symbol_2x_re_out[8]' : ['slew FAST'],
		'symbol_2x_re_out[9]' : ['slew FAST'],
		'symbol_2x_re_out[10]' : ['slew FAST'],
		'symbol_2x_re_out[11]' : ['slew FAST'],
		'symbol_2x_re_out[12]' : ['slew FAST'],
		'symbol_2x_re_out[13]' : ['slew FAST'],
		'symbol_2x_re_out[14]' : ['slew FAST'],
		'symbol_2x_re_out[15]' : ['slew FAST'],

		'symbol_2x_im_out[0]' : ['slew FAST'],
		'symbol_2x_im_out[1]' : ['slew FAST'],
		'symbol_2x_im_out[2]' : ['slew FAST'],
		'symbol_2x_im_out[3]' : ['slew FAST'],
		'symbol_2x_im_out[4]' : ['slew FAST'],
		'symbol_2x_im_out[5]' : ['slew FAST'],
		'symbol_2x_im_out[6]' : ['slew FAST'],
		'symbol_2x_im_out[7]' : ['slew FAST'],
		'symbol_2x_im_out[8]' : ['slew FAST'],
		'symbol_2x_im_out[9]' : ['slew FAST'],
		'symbol_2x_im_out[10]' : ['slew FAST'],
		'symbol_2x_im_out[11]' : ['slew FAST'],
		'symbol_2x_im_out[12]' : ['slew FAST'],
		'symbol_2x_im_out[13]' : ['slew FAST'],
		'symbol_2x_im_out[14]' : ['slew FAST'],
		'symbol_2x_im_out[15]' : ['slew FAST'],
	}

        extra_property = {}

	fmc_io_stand_except = [
	]


	print '#', '-' * 100
	print '#', 'generator constrain for ip signals'
	print '#', '-' * 100

	io_pin_map = {}
	for pin, io in kc705_package_pin_nets.items():
		io_pin_map.update({io: pin})

	myip_package_pins_des = [
		('AE30', 'BRCK[0]'),
		('AF30', 'LRCK[0]'),
		('AG30', 'DOUT[0]'),

		('AH30', 'BRCK[1]'),
		('AK29', 'LRCK[1]'),
		('AK30', 'DOUT[1]'),

		('AJ28', 'BRCK[2]'),
		('AJ29', 'LRCK[2]'),
		('AG27', 'DOUT[2]'),

		('AG28', 'BRCK[3]'),
		('AH26', 'LRCK[3]'),
		('AH27', 'DOUT[3]'),

		('AJ27', 'BRCK[4]'),
		('AK28', 'LRCK[4]'),
		('AJ26', 'DOUT[4]'),

		('AK26', 'BRCK[5]'),
		('AF26', 'LRCK[5]'),
		('AF27', 'DOUT[5]'),

		('AC26', 'BRCK[6]'),
		('AD26', 'LRCK[6]'),
		('AE25', 'DOUT[6]'),

		('AF25', 'BRCK[7]'),
		('AC24', 'LRCK[7]'),
		('AD24', 'DOUT[7]'),

		('AJ24', 'BRCK[8]'),
		('AK25', 'LRCK[8]'),
		('AJ22', 'DOUT[8]'),

		('AJ23', 'BRCK[9]'),
		('AG22', 'LRCK[9]'),
		('AH22', 'DOUT[9]'),

		('AD23', 'BRCK[10]'),
		('AE24', 'LRCK[10]'),
		('AC22', 'DOUT[10]'),

		('AD22', 'BRCK[11]'),
		('AF20', 'LRCK[11]'),
		('AF21', 'DOUT[11]'),

		('AG20', 'BRCK[12]'),
		('AH20', 'LRCK[12]'),
		('AK20', 'DOUT[12]'),

		('AK21', 'BRCK[13]'),
		('AE23', 'LRCK[13]'),
		('AF23', 'DOUT[13]'),

		('AB24', 'BRCK[14]'),
		('AC25', 'LRCK[14]'),
		('AK23', 'DOUT[14]'),

		('AK24', 'BRCK[15]'),
		('AD21', 'LRCK[15]'),
		('AE21', 'DOUT[15]'),
	]

	io_stand_except = []

	for io, signal in myip_io_des_signals:
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

	for pin, signal in myip_package_pins_des:
		io = kc705_package_pin_nets.pop(pin)
		if not io:
			print 'xiaofei:pin not in kc705_package_pin_nets!'
			continue

		print '\n#%s\nset_property PACKAGE_PIN %s [get_ports {%s}]' %(io, pin, signal)
		if not io in io_stand_except:
			print 'set_property IOSTANDARD LVCMOS15 [get_ports {%s}]' %(signal)
		extra_list = extra_property.get(signal)
		if not extra_list:
			continue
		for p in extra_list:
			print 'set_property %s [get_nets {%s}]' %(p, signal)


def gen_bitstream_constrain():
	txt = """
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 33 [current_design]

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



def gen_unused_pin_io(kc705_package_pin_nets, kc705_pins_resistor):
	print '-' * 100
	print 'package pins for unused'
	print '-' * 100
	ios = [ x[0][0] for x in kc705_pins_resistor ]
	for i in kc705_package_pin_nets.items():
		if not i[1] in ios:
			print '封装pin:%-20s网络:%-20s' %(i[0], i[1])

def gen_kc705_package_pin_nets():
	kc705_package_pin_nets = {}
	lines = []
	with open('kc705_package_pin_nets.txt') as f:
		lines = f.read().splitlines()

	for i in lines:
		l = i.split()
		item = {l[0] : l[1]}
		if len(l) == 2:
			kc705_package_pin_nets.update(item)

	print '-' * 100
	print 'kc705_package_pin_nets info'
	print '-' * 100
	for i in kc705_package_pin_nets.items():
		print i
	print 'total:', len(kc705_package_pin_nets)

	return kc705_package_pin_nets

def gen_kc705_io_package_pins():
	kc705_io_package_pins = {}
	lines = []
	with open('kc705_io_package_pins.txt') as f:
		lines = f.read().splitlines()

	for i in lines:
		l = i.split()
		item = {l[0] : l[1]}
		if len(l) == 8:
			kc705_io_package_pins.update(item)

	print '-' * 100
	print 'kc705_io_package_pins info'
	print '-' * 100
	for i in kc705_io_package_pins.items():
		print i
	print 'total:', len(kc705_io_package_pins)

	return kc705_io_package_pins

def gen_kc705_constrain():
	kc705_signals = gen_kc705_signals()

	fmc_pin_resistor = gen_fmc_pin_resistor()

	kc705_package_pin_nets = gen_kc705_package_pin_nets()

	kc705_io_package_pins = gen_kc705_io_package_pins()

	#建立板上package_pins与fmc端口之间的关联
	kc705_pins_resistor = gen_kc705_pins_resistor(unsupport_loc, kc705_signals, fmc_pin_resistor)
	resistor_kc705_pins_resistor_map = gen_resistor_kc705_pins_resistor_map(kc705_pins_resistor)
	hpc_lpc_pins_des_map = gen_hpc_lpc_pins_resistor_map(resistor_kc705_pins_resistor_map)
	hpc_lpc_pins_des_map = gen_hpc_lpc_pins_des_map(unsupport_loc, kc705_package_pin_nets)
	hpc_pins = [x[0] for x in hpc_lpc_pins_des_map.get('HPC')]
	lpc_pins = [x[0] for x in hpc_lpc_pins_des_map.get('LPC')]
	hpc_pins_des = hpc_lpc_pins_des_map.get('HPC')
	lpc_pins_des = hpc_lpc_pins_des_map.get('LPC')

	##哪些package_pin没有用上？
	#gen_unused_pin_io(kc705_package_pin_nets, kc705_pins_resistor)

	#输出固定约束
	gen_default_contrain()

	gen_myip_constrain(kc705_package_pin_nets)

	#输出fmc板上电阻与kc705的映射关系
	hpc_constrain = gen_fmc_gpio_constrain(kc705_package_pin_nets, hpc_pins, 'HPC', 0)
	lpc_constrain = gen_fmc_gpio_constrain(kc705_package_pin_nets, lpc_pins, 'LPC', len(hpc_constrain))
	
	gen_bitstream_constrain()

	get_gpios_pin_no_for_driver(hpc_constrain, hpc_pins_des, 'HPC', 0, len(hpc_constrain))
	get_gpios_pin_no_for_driver(lpc_constrain, lpc_pins_des, 'LPC', len(hpc_constrain), len(lpc_constrain))
	print 'total', len(hpc_constrain) + len(lpc_constrain)

	#打印pin号

if __name__ == "__main__":
	gen_kc705_constrain()
