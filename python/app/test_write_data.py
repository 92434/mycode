def write_data_to_file(str_file, pos, str_format, data):
	import struct
	f = open(str_file, 'wb')
	f.seek(pos, 0)
	packed_data = struct.pack(str_format, data)
	f.write(packed_data)
	f.close()
def read_data_from_file(str_file, pos, str_format):
	import struct
	f = open(str_file, 'rb')
	f.seek(pos, 0)
	filedata = f.read(1)
	unpacked_data = struct.unpack(str_format, filedata)
	f.close()
	return unpacked_data

def get_mtd_info():
	mtd_proc_file = open('/proc/mtd', 'r')
	mtd_info = mtd_proc_file.readlines()
	mtd_proc_file.close()
	return mtd_info

	
def get_swap_mtd_blk():
	mtd_label = '"swap"'
	mtd_info = """\
dev:    size   erasesize  name
mtd0: 04a00000 00020000 "e2rootfs"
mtd1: 00040000 00020000 "swap"
mtd2: 000a0000 00010000 "u-boot"
mtd3: 00300000 00010000 "e2kernel"\
"""
	mtd_info = mtd_info.splitlines()

	#mtd_info = get_mtd_info()


	for i in mtd_info:
		i = i.split()
		if len(i) < 4:
			continue
		if i[3] != mtd_label:
			continue
		#return '/dev/' + i[0][:-1]
		return 'aa.bin'

def write_byte_mark(mark):
	str_mtd_dev = get_swap_mtd_blk()
	write_data_to_file(str_mtd_dev, 0, 'B', mark)
	print '%01x' %(read_data_from_file(str_mtd_dev, 0, 'B'))
	
write_byte_mark(0xaa)
