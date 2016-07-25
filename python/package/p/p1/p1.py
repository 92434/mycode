# -*- coding: utf-8 -*-
import traceback
import sys
print traceback.extract_stack()[-1][:-1]

def p1_main(argv):
	print traceback.extract_stack()[-1][:-1]
	print argv

	print '*' * 100
	d = {}
	dd = dir(d)
	for i in dd:
		j = getattr(d, i, None)
		print "str:\033[31m$%s\033[0m\ntype:\033[31m$%s\033[0m\n" %(str(j), type(j))
	print '*' * 100

if __name__ == "__main__":
	py_main(sys.argv)
