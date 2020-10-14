/* putchar.c
 * Copyright (c) 2020
 * Mario Ivancic
 * All rights reserved.
 * 
 * Dummy putchar implementation as weak symbol.
 * To be usefull must be replaced by something real.
 */

#include <stdio.h>


// dummy putchar implementation, to be usefull must be replaced by something real
__attribute__((weak)) int putchar(int c){ return c; }


