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
	map_ids = {}
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
			id_count = map_ids.get(int_id, None)
			if not id_count:
				id_count = 0

			map_ids.update({int_id : id_count + 1})

		index = i

	return map_ids

if __name__ == "__main__":
	map_ids = get_pid(sys.argv[1])
	print 'ids:'
	for i, j in map_ids.items():
		print "id:%04x : count:%d" %(i & 0x1fff, j)
