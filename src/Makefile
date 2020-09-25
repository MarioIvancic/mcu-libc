# Makefile for LibC


# default target is to build LibC for all architectures
.PHONY: all
all: arm7tdmi-s-libc cortex-m3-libc cortex-m0-libc



# target for building LibC for arm7tdmi-s architecture
.PHONY: arm7tdmi-s-libc arm7tdmi-s-distclean arm7tdmi-s arm7tdmi-s-install arm7tdmi-s-clean arm7tdmi
arm7tdmi-s-libc: arm7tdmi-s-distclean arm7tdmi-s arm7tdmi-s-install arm7tdmi-s-clean
arm7tdmi-s-distclean:
	@make -f arch/arm7tdmi-s/Makefile.arm7tdmi-s ARCH=arm7tdmi-s distclean

arm7tdmi: arm7tdmi-s
arm7tdmi-s:
	@make -f arch/arm7tdmi-s/Makefile.arm7tdmi-s ARCH=arm7tdmi-s
	
arm7tdmi-s-install:
	@make -f arch/arm7tdmi-s/Makefile.arm7tdmi-s ARCH=arm7tdmi-s install
	
arm7tdmi-s-clean:
	@make -f arch/arm7tdmi-s/Makefile.arm7tdmi-s ARCH=arm7tdmi-s clean



# target for building LibC for cortex-m0 architecture
.PHONY: cortex-m0-libc cortex-m0-distclean cortex-m0 cortex-m0-install cortex-m0-clean
cortex-m0-libc: cortex-m0-distclean cortex-m0 cortex-m0-install cortex-m0-clean
cortex-m0-distclean:
	@make -f arch/cortex-m0/Makefile.cortex-m0 ARCH=cortex-m0 distclean

cortex-m0:
	@make -f arch/cortex-m0/Makefile.cortex-m0 ARCH=cortex-m0
	
cortex-m0-install:
	@make -f arch/cortex-m0/Makefile.cortex-m0 ARCH=cortex-m0 install
	
cortex-m0-clean:
	@make -f arch/cortex-m0/Makefile.cortex-m0 ARCH=cortex-m0 clean



# target for building LibC for cortex-m3 architecture
.PHONY: cortex-m3-libc cortex-m3-distclean cortex-m3 cortex-m3-install cortex-m3-clean
cortex-m3-libc: cortex-m3-distclean cortex-m3 cortex-m3-install cortex-m3-clean
cortex-m3-distclean:
	@make -f arch/cortex-m3/Makefile.cortex-m3 ARCH=cortex-m3 distclean

cortex-m3:
	@make -f arch/cortex-m3/Makefile.cortex-m3 ARCH=cortex-m3
	
cortex-m3-install:
	@make -f arch/cortex-m3/Makefile.cortex-m3 ARCH=cortex-m3 install
	
cortex-m3-clean:
	@make -f arch/cortex-m3/Makefile.cortex-m3 ARCH=cortex-m3 clean



