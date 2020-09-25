#!/bin/bash

# build all supported architectures
ARCHS="arm7tdmi-s   cortex-m3   cortex-m0"

for ARCH in $ARCHS
do
	make -f Makefile.old ARCH=$ARCH distclean 2>&1 | grep -Ei ': warning:|: note:|: error:'
	if [ ${PIPESTATUS[0]} -ne 0 ]; then break; fi
	make -f Makefile.old ARCH=$ARCH clean 2>&1  | grep -Ei ': warning:|: note:|: error:'
	if [ ${PIPESTATUS[0]} -ne 0 ]; then break; fi
	make -f Makefile.old ARCH=$ARCH 2>&1 | grep -Ei ': warning:|: note:|: error:'
	if [ ${PIPESTATUS[0]} -ne 0 ]; then break; fi
	make -f Makefile.old ARCH=$ARCH install 2>&1 | grep -Ei ': warning:|: note:|: error:'
	if [ ${PIPESTATUS[0]} -ne 0 ]; then break; fi
	make -f Makefile.old ARCH=$ARCH clean 2>&1  | grep -Ei ': warning:|: note:|: error:'
	if [ ${PIPESTATUS[0]} -ne 0 ]; then break; fi
done

#pause
