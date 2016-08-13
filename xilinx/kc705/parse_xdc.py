# *-* coding: utf-8 *-*
import sys
import re

def match_result(m):
	if m:
		print 'm.group():%s' %m.group()
		j = 0
		print 'm.group(\'%d\'):%s' %(j, m.group(j))
		for i in m.groupdict():
			print 'm.group(\'%s\'):%s' %(i, m.group(i))

		j += 1
		for i in m.groupdict():
			print 'm.group(\'%d\'):%s' %(j, m.group(j))
			j += 1

		print 'm.groups():%s' %str(m.groups())
		print 'm.groupdict():%s' %str(m.groupdict())
	else:
		print 'm:%s' %m

def main():
	str_file_path = ''
	if len(sys.argv) == 2:
		str_file_path = sys.argv[1]
	txt = ''
	with open(str_file_path) as f:
		txt = f.read()
	pattern = re.compile(r'#[^,]+,\s+(?P<des>[^,]+),[^\n]+\nset_property\s+PACKAGE_PIN\s+([^\s]+)\s+\[get_ports\s+{gpio.*}\]')
	l = pattern.findall(txt)
	for i in l:
		i = (i[1], i[0])
		print '%s,'  %(str(i))
	#l = txt.splitlines()
	#for i in l:
	#	pattern = re.compile(r'set_property PACKAGE_PIN\s+(?P<pin>[^\s]+)\s+\[get_ports\s+{(?P<port>[^}]+)}\]')
	#	m = pattern.match(i)
	#	if m:
	#		#match_result(m)
	#		print '%s,' %(str(m.groups()))

if __name__ == '__main__':
	main()
