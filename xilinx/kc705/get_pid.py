# -*- coding: utf-8 -*-
import sys

def get_pid(data_file):
	list_data = []

	with open(data_file) as f:
		list_data = f.read().split()
	
	#find first 0x47

	map_ids = {}
	while True:
		len_list_data = len(list_data)
		index = 0
		start = 0

		if len_list_data < 188:
			break
		
		for i in list_data:
			if i == '47' and list_data[index + 188] == '47':
				start = index
				break

			index += 1

		if index == len_list_data:
			break

		for i in range(start, len_list_data, 188):
			if not list_data[i + 0] == '47':
				break
			
			int_id = int(list_data[i + 1] + list_data[i + 2], 16)
			id_count = map_ids.get(int_id, None)
			if not id_count:
				id_count = 0
			map_ids.update({int_id : id_count + 1})
			index = i

		list_data = list_data[index : ]

	return map_ids

if __name__ == "__main__":
	map_ids = get_pid(sys.argv[1])
	print 'ids:'
	for i, j in map_ids.items():
		print "id:%04x : count:%d" %(i & 0x1fff, j)
