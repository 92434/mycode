#!/usr/bin/python

import re

r = re.compile(r'^(?P<x>.*)(?P=x)haha(?P<h>.*)$')
m = r.match(r'xiaofeixiaofeihahahaha')
print m.group('x')
print m.group('h')

r = re.compile(r'^haha(?=xiaofei)')
m = r.match(r'hahaxiaofei')
print m.group(0)

r = re.compile(r'^.*ha(?!xiaofee)')
m = r.match(r'hahaxiaofeixiaofei')
print m.group(0)

r = re.compile(r'(?<=xiaofei).*')
m = r.search(r'xiaofeihaha')
print m.group(0)

r = re.compile(r'(?<!xiaofee)haha')
m = r.search(r'xiaofeihaha')
print m.group(0)
