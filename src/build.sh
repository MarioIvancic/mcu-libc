#!/bin/sh
make ARCH=atm7tdmi-s distclean
make ARCH=atm7tdmi-s
make ARCH=atm7tdmi-s install
make ARCH=atm7tdmi-s clean
make ARCH=cortex-m3 distclean
make ARCH=cortex-m3
make ARCH=cortex-m3 install
make ARCH=cortex-m3 clean
make ARCH=cortex-m0 distclean
make ARCH=cortex-m0
make ARCH=cortex-m0 install
make ARCH=cortex-m0 clean
#pause
