#!/bin/sh

echo -ne "\\" | hexdump -C

echo "'" | sed -n "
=
s/'/sss/g
p
=
"
echo "\'" | sed -n "s/\\\'/xiaofei/p"
