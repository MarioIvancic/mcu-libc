Simple C library compiled from bits and peaces of NewLib, MSP430-LibC, Embedded Artistry LibC and Musl code.
It is intended to be customized for every project.

Currently supports arm7-dtmi, arm-cortex-m0, arm-cortex-m3, arm-cortex-m4 and arm-cortex-m33.

It has smaller memory footprint than Newlib-nano which is common for bare-metal ARM. Most importantly it does not use malloc/free in any internal function.

When compiled for arm-cortex-m33 (LPC55S69)...

Some not-so-trivial app linked against Newlib-nano:
*		Memory region	Used Size	Region Size	%age Used
*		PROGRAM_FLASH:	65616 B		630 KB     	10.17%
*		SRAM:       		30316 B     	256 KB     	11.56%


The same app linked against mcu-libc:
*		Memory region	Used Size	Region Size  %age Used
*		PROGRAM_FLASH:	56852 B		630 KB      	8.81%
*       SRAM:       		30296 B		256 KB     	11.56%

# Compiling

## Compiling libc

```
cd libc
cp project_make_options.sample project_make_options
nano project_make_options
```

To compile and install libc for all supported arch:

```
make
make clean
```

To compile and install libc for cortex-m3:

```
make cortex-m3
make cortex-m3-install
make clean
```

To remove libc for all arch:

```
make distclean
```

## Compiling libc++

```
cd ..
git submodule update --init --recursive
cd libcpp
cp project_make_options.sample project_make_options
cp __libcpp_config_site.sample __libcpp_config_site
nano project_make_options
nano __libcpp_config_site
```

To make libc++ for all supported arch:

```
make
make clean
```

To make libc++ for cortex-m3:

```
make cortex-m3
make cortex-m3-install
make clean
```

To remove libc++ for all arch:

```
make distclean
```

## TODOs
- Remove systime_tick from mcu-libc in the new lib.
- Remove malloc and free from mcu-libc in the new lib.
- Add unit tests.
