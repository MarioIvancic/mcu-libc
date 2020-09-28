# Top level Makefile for LibC

# usage:
#	to build and install LibC for all archs: 	make
#	to clean working directory: 				make clean
# 	to uninstall LibC for all archs:			make distclean
#	to build LibC for arch A:					make A
#	Supported archs: cortex-m0, cortex-m3, arm7tdmi-s
#	During testing it's convenient to test on one arch, for instance cortex-m3:
#	make clean
#	make cortex-m3

L2MAKEFILE = Makefile.2

# default target is to build LibC for all architectures
.PHONY: all
all: arm7tdmi-s-libc cortex-m3-libc cortex-m0-libc



# target for building LibC for arm7tdmi-s architecture
.PHONY: arm7tdmi-s-libc arm7tdmi-s-distclean arm7tdmi-s arm7tdmi-s-install arm7tdmi-s-clean arm7tdmi
arm7tdmi-s-libc: arm7tdmi-s-distclean arm7tdmi-s arm7tdmi-s-install arm7tdmi-s-clean
arm7tdmi-s-distclean:
	@make -s -f $(L2MAKEFILE) ARCH=arm7tdmi-s distclean

arm7tdmi: arm7tdmi-s
arm7tdmi-s:
	@make -s -f $(L2MAKEFILE) ARCH=arm7tdmi-s
	
arm7tdmi-s-install:
	@make -s -f $(L2MAKEFILE) ARCH=arm7tdmi-s install
	
arm7tdmi-s-clean:
	@make -s -f $(L2MAKEFILE) ARCH=arm7tdmi-s clean



# target for building LibC for cortex-m0 architecture
.PHONY: cortex-m0-libc cortex-m0-distclean cortex-m0 cortex-m0-install cortex-m0-clean
cortex-m0-libc: cortex-m0-distclean cortex-m0 cortex-m0-install cortex-m0-clean
cortex-m0-distclean:
	@make -s -f $(L2MAKEFILE) ARCH=cortex-m0 distclean

cortex-m0:
	@make -s -f $(L2MAKEFILE) ARCH=cortex-m0
	
cortex-m0-install:
	@make -s -f $(L2MAKEFILE) ARCH=cortex-m0 install
	
cortex-m0-clean:
	@make -s -f $(L2MAKEFILE) ARCH=cortex-m0 clean



# target for building LibC for cortex-m3 architecture
.PHONY: cortex-m3-libc cortex-m3-distclean cortex-m3 cortex-m3-install cortex-m3-clean
cortex-m3-libc: cortex-m3-distclean cortex-m3 cortex-m3-install cortex-m3-clean
cortex-m3-distclean:
	@make -s -f $(L2MAKEFILE) ARCH=cortex-m3 distclean

cortex-m3:
	@make -s -f $(L2MAKEFILE) ARCH=cortex-m3
	
cortex-m3-install:
	@make -s -f $(L2MAKEFILE) ARCH=cortex-m3 install
	
cortex-m3-clean:
	@make -s -f $(L2MAKEFILE) ARCH=cortex-m3 clean


# target to clean working directory
.PHONY: clean
clean: cortex-m3-clean cortex-m0-clean arm7tdmi-s-clean


# target to uninstall LibC for all archs
.PHONY: distclean
distclean: cortex-m3-distclean cortex-m0-distclean arm7tdmi-s-distclean

