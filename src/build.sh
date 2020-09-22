#!/bin/sh

# build all supported architectures
ARCHS="arm7tdmi-s   cortex-m3   cortex-m0"

for ARCH in $ARCHS
do
	make ARCH=$ARCH distclean
	if [ ! $? ]; then break; fi
	make ARCH=$ARCH clean
	if [ ! $? ]; then break; fi
	make ARCH=$ARCH
	if [ ! $? ]; then break; fi
	make ARCH=$ARCH install
	if [ ! $? ]; then break; fi
done

#pause
