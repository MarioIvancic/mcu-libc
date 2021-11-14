Simple C library compiled from bits and peaces of NewLib, MSP430-LibC, Embedded Artistry LibC and Musl code.
It is intended to be customized for every project.
Currently it supports arm7-dtmi, arm-cortex-m3 and arm-cortex-m0.

Compiling libc:

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

Compiling libc++:

```
cd ..
git submodule update --init --recursive
cd libcpp
cp project_make_options.sample project_make_options
nano project_make_options
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
