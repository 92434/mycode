# -*- coding: utf-8 -*-
import traceback
import sys
print traceback.extract_stack()[-1][:-1]

def p1_main(argv):
	print traceback.extract_stack()[-1][:-1]
	print argv

if __name__ == "__main__":
	py_main(sys.argv)
