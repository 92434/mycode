# -*- coding: utf-8 -*-
import re
import sys
import os

def compare(argv):
	if not len(sys.argv) == 3:
		return
	file1 = argv[1]
	file2 = argv[2]

	if not os.path.exists(file1):
		return
	print file1

	if not os.path.exists(file2):
		return
	print file2

	txt1 = ''
	with open(file1) as f:
		txt1 = f.read()

	txt2 = ''
	with open(file2) as f:
		txt2 = f.read()
	
	pattern = re.compile(r'#(FMC[^,]+),.*')
	list_result1 = pattern.findall(txt1)
	print "list_result1:\n", list_result1

	pattern = re.compile(r'#(FMC[^\n]+)')
	list_result2 = pattern.findall(txt2)
	print "list_result2:\n", list_result2

	print 'pin only in 1:'
	out = ''
	for i in list_result1:
		find = False
		for j in list_result2:
			if i == j:
				find = True
		if not find:
			out += "%s " %(i)
	print out

	print 'pin only in 2:'
	out = ''
	for i in list_result2:
		find = False
		for j in list_result1:
			if i == j:
				find = True
		if not find:
			out += "%s " %(i)
	print out

if __name__ == "__main__":
	compare(sys.argv)
