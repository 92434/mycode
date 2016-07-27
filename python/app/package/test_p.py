# -*- coding: utf-8 -*-
import traceback
from p.p1.p1 import get_dict_type, p1_main

if __name__ == "__main__":
	print traceback.extract_stack()[-1][:-1]
	print dir(p1_main)
	d = {}
	get_dict_type(d)
	print dir()
