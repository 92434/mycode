#!/bin/sh

for i in $(sudo find / -name "lib" -type d);do sudo find $i -name "*.so"|grep "libGL";done
