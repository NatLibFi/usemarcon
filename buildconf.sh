#!/bin/sh
set -x
dir=`aclocal --print-ac-dir`
if [ -f $dir/usemarcon.m4 ]; then
	aclocal
else
	aclocal -I . 
fi
glibtoolize --force 
automake -a 
automake -a 
autoconf
if [ -f config.cache ]; then
	rm config.cache
fi
