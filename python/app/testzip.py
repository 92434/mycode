#!/usr/bin/env python

def myzip(*args):
	it = [iter(i) for i in args]
	while it:
		print "i am here"
		result = []
		for i in it:
			e = next(i, None)
			if(e == None):
				return
			result.append(e)
		yield tuple(result)

if '__main__' == __name__:
	r = myzip("1234", "5678")
	for i in r:
		print i
	
