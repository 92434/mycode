#!/usr/bin/python
# -*- coding: utf-8 -*-

import re
from log import *

def match_result(m):
	logger.debug('')
	if m:
		print 'm.group():%s' %m.group()
		j = 0
		print 'm.group(\'%d\'):%s' %(j, m.group(j))
		j += 1
		for i in m.groupdict():
			print 'm.group(\'%s\'):%s' %(i, m.group(i))
			print 'm.group(\'%d\'):%s' %(j, m.group(j))
			j += 1
		print 'm.groups():%s' %str(m.groups())
		print 'm.groupdict():%s' %str(m.groupdict())
	else:
		print 'm:%s' %m


def named_group():
	logger.debug('')
	pattern = re.compile(r'(?P<quote>[\'\"]).*?(?P=quote)')
	m = pattern.match(r'"pattern1"')
	match_result(m)
	pattern = re.compile(r'^(?P<x>.*)(?P=x)haha(?P<h>.*)$')
	m = pattern.match(r'xiaofeixiaofeihahahaha')
	match_result(m)
	pattern = re.compile(r'^(?P<x>.*)(?P=x)(?P<h>.*)(?P=h)$')
	m = pattern.match(r'xiaofeixiaofeihahahaha')
	match_result(m)

def positive_lookahead_assertion():
	logger.debug('')
	r = re.compile(r'^haha(?=xiaofei)')
	m = r.match(r'hahaxiaofei')
	match_result(m)

def negative_lookahead_assertion():
	logger.debug('')
	r = re.compile(r'^.*ha(?!xiaofee)')
	m = r.match(r'hahaxiaofeixiaofei')
	match_result(m)

def positive_lookbehind_assertion():
	logger.debug('')
	r = re.compile(r'(?<=xiaofei).*')
	m = r.search(r'xiaofeihaha')
	match_result(m)

def negative_lookbehind_assertion():
	logger.debug('')
	r = re.compile(r'(?<!xiaofee)haha')
	m = r.search(r'xiaofeihaha')
	match_result(m)

if __name__ == '__main__':
	named_group()
	positive_lookahead_assertion()
	negative_lookahead_assertion()
	positive_lookbehind_assertion()
	negative_lookbehind_assertion()
