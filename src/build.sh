#!/bin/sh
make ARCH=arm7tdmi-s distclean
make ARCH=arm7tdmi-s clean
make ARCH=arm7tdmi-s
make ARCH=arm7tdmi-s install
make ARCH=arm7tdmi-s clean
make ARCH=cortex-m3 distclean
make ARCH=cortex-m3 clean
make ARCH=cortex-m3
make ARCH=cortex-m3 install
make ARCH=cortex-m3 clean
make ARCH=cortex-m0 distclean
make ARCH=cortex-m0 clean
make ARCH=cortex-m0
make ARCH=cortex-m0 install
make ARCH=cortex-m0 clean
#pause
