# -*- coding: utf-8 -*-
import sys

def gen_test_data(data_file):
	txt = ''
	for l in range(0, 100, 1):
		for i in range(0, 188, 1):
			if i == 0:
				txt += '47 '
			elif i == 1:
				txt += '15 '
			elif i == 2:
				txt += '7f '
			else:
				txt += '00 '
	with open(data_file, 'w') as f:
		f.write(txt)

def get_pid(data_file):
	list_data = []

	with open(data_file) as f:
		list_data = f.read().split()
	
	index = 0
	len_list_data = len(list_data)
	map_id_tuple_count_list_packetpos = {}
	i = 0

	while True:
		if index + 188 >= len_list_data:
			break
		
		for i in range(index, len_list_data, 1):
			if list_data[i] == '47' and list_data[i + 188] == '47':
				break

		index = i

		if index == len_list_data - 1:
			break

		for i in range(index, len_list_data, 188):
			if not list_data[i + 0] == '47':
				break
			
			int_id = int(list_data[i + 1] + list_data[i + 2], 16)
			tuple_count_list_packetpos = map_id_tuple_count_list_packetpos.get(int_id, None)
			count = 0
			list_packetpos = []
			if tuple_count_list_packetpos:
				count, list_packetpos = tuple_count_list_packetpos

			count += 1
			list_packetpos.append(i)

			map_id_tuple_count_list_packetpos.update({int_id : (count, list_packetpos)})

		index = i

	return list_data, map_id_tuple_count_list_packetpos

if __name__ == "__main__":
	list_data, map_id_tuple_count_list_packetpos = get_pid(sys.argv[1])

	print 'ids:'
	for int_id, tuple_count_list_packetpos in map_id_tuple_count_list_packetpos.items():
		count, list_packetpos = tuple_count_list_packetpos
		txt = 'id:%04x : count:%d' %(int_id & 0x1fff, count)
		print txt

		txt = ''
		for i in list_packetpos:
			txt = ''
			for j in range(i, i + 188, 1):
				txt += '%s ' %(list_data[j])
			print txt
