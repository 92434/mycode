#!/usr/bin/python

from __future__ import with_statement 
import os

import ConfigParser 
config=ConfigParser.ConfigParser() 
with open('testcfg.cfg','w+') as cfgfile: 
	config.readfp(cfgfile) 

	config.add_section('info')
	config.set('info','name','xiaofei') 
	config.set('info','gender','male') 
	config.set('info','age','21') 
	config.add_section('dist')
	config.set('dist','mm','100') 

	config.write(cfgfile)

with open('testcfg.cfg','w+') as cfgfile: 
	config.readfp(cfgfile) 

	name=config.get('info','name') 
	gender=config.get('info','gender') 
	age=config.get('info','age') 
	config.set('dist','mm','101') 
	print name 
	print gender
	print age 
	config.remove_section('info')
	config.write(cfgfile);
