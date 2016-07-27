# -*- coding: utf-8 -*-

import sys

def complement(v):
	print "*" * 100
	v = int(v, 16)
	if (v & 0x8000) == 0:
		print hex(v)
	else:
		v_i = v & 0x7fff;
		v_i = (~v_i) & 0xffff;
		print "%18s" %(bin(v))
		print "%18s" %(bin(v_i))
		v_c = (v_i + 1) & 0xffff;
		print "%18s" %(bin(v_c))
		print "%18s" %(hex(v_c))

if '__main__' == __name__:
	if len(sys.argv) > 1:
		for i in sys.argv[1 : ]:
			complement(i)
