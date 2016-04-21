# -*- coding: utf-8 -*-
import sys
import re

def get_ts_head_line(data_file):
	data_txt = ''

	with open(data_file) as f:
		data_txt = f.read()
	l = data_txt.splitlines()
	total = len(l)
	pre_line_no = 0
	
	for i in range(0, total):
		if l[i] == '0x47':
			if i + 188 < total:
				if l[i + 188] == '0x47':
					print 'line: %08d \t distance: %08d' %(i + 1, i - pre_line_no)
					pre_line_no = i

if __name__ == "__main__":
	data_file = sys.argv[1]
	if data_file != '':
		get_ts_head_line(data_file)
