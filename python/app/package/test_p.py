# -*- coding: utf-8 -*-
from p.p1.p1 import get_dict_type, p1_main
from log import *

if __name__ == "__main__":
	logger.debug('')
	print dir(p1_main)
	d = {}
	get_dict_type(d)
	print dir()
