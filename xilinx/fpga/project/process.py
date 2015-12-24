# *-* coding: utf-8 *-*

def process(f1, f2):
	t1 = ''
	with open(f1) as f:
		t1 = f.read()
	t2 = ''
	with open(f2) as f:
		t2 = f.read()
	l = t1.strip().splitlines()
	l1 = []
	for i in l:
		l1.append(i.split()[0])
	
	d2 = {}
	l = t2.strip().splitlines()
	for i in l:
		pin, io = i.split()
		d2[pin] = io
	for i in l1:
		print '%-10s%-30s' %(i, d2.get(i))


if __name__ == '__main__':
	process('pins.txt', 'nets.txt')
