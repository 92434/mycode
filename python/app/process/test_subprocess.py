# -*- coding: utf-8 -*-
from log import *
import subprocess
import chardet

def main():
	p = subprocess.Popen(['ls', '-lth'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	o, e = p.communicate()
	r = chardet.detect(o)
	enc = r.get('encoding')
	txt = '\n'
	txt += 'encoding:%s\n' %(enc)
	txt += '%s\n' %(o.decode(enc))
	txt = color(txt)
	logger.debug(txt)
	

if __name__ == "__main__":
	main()
