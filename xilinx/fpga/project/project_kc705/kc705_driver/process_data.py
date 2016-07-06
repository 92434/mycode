# *-* coding: utf-8 *-*

import sys
import os

def process_data(args):
	filename = args[0]
	reportname = args[1]
	t = ''
	if os.path.exists(filename):
		with open(filename) as f:
			t = f.read()
	l = t.strip().splitlines()
	s = []
	d = {}
	for i in l:
		io, status = i.split()
		if not io in s:
			s.append(io)
		d[io] = status
	with open(reportname, 'w+') as f:
		for i in range(len(s)):
			print '%-30s%-10s' %(s[i], d.get(s[i]))
			f.write('%-30s%-10s\n' %(s[i], d.get(s[i])))
if __name__ == '__main__':
	process_data(sys.argv[1:])
