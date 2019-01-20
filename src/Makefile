# WinARM template makefile
# by Martin Thomas, Kaiserslautern, Germany
# <eversmith(at)heizung-thomas(dot)de>
#
# Released to the Public Domain
#
# Modified by Mario Ivancic for libc

# On command line:
# make ARCH=atm7tdmi-s all = Make software.
# make ARCH=cortex-m3 clean = Clean out built project files.
# make ARCH=cortex-m0 install = Install liblpc.a and some .h files in lib/$(MCU).


# Project options
# set before compile & install
# ARCH type
#ARCH := arm7-tdmi
#ARCH := cortex-m3
#ARCH := cortex-m0

PROJECT_OPTIONS := -D__LIBC__
PROJECT_OPTIONS = -DGMTIME_OPTIMIZE_SIZE
#PROJECT_OPTIONS = -DGMTIME_OPTIMIZE_SPEED
#PROJECT_OPTIONS = -DGMTIME_LOOP_DAYS
#PROJECT_OPTIONS = -DSYSTEM_TIME_USE_CLOCK_GETTIME
#PROJECT_OPTIONS = -D__LIBC_PRINTF_SHIFT_AND_ADD_MULBY10__
#PROJECT_OPTIONS = -D__LIBC_PRINTF_LONG__
PROJECT_OPTIONS = -D__LIBC_PRINTF_LONGLONG__


ifndef SRCDIR
SRCDIR := .
endif

# Toolchain prefix (arm-elf- -> arm-elf-gcc.exe)
#TCHAIN_PREFIX := C:/bin/winarm/arm-elf-gcc-4.1.1/bin/arm-elf-
#TCHAIN_PREFIX := C:/bin/winarm/arm-elf-gcc-4.2.2/bin/arm-elf-
#TCHAIN_PREFIX := C:/bin/winarm/arm-elf-gcc-4.3.3/bin/arm-elf-
#TCHAIN_PREFIX := C:/bin/winarm/arm-elf-gcc-4.4.2/bin/arm-elf-
#TCHAIN_PREFIX := C:/bin/winarm/arm-eabi-gcc-4.3.0/bin/arm-eabi-
#TCHAIN_PREFIX := C:/bin/winarm/arm-eabi-gcc-4.5.0/bin/arm-none-eabi-
#TCHAIN_PREFIX := C:/bin/winarm/arm-eabi-gcc-4.6.2/bin/arm-none-eabi-
#TCHAIN_PREFIX := C:/bin/winarm/arm-eabi-gcc-4.7.0/bin/arm-none-eabi-
TCHAIN_PREFIX := arm-none-eabi-

# ARCH name and submodel

ifeq ($(ARCH),arm7tdmi-s)
    THUMB    := -mthumb
    THUMB_IW := -mthumb-interwork
    CLASS_NAME := ARM7TDMI
    CLASS_DIR := arm7
    #PROJECT_OPTIONS = -DSYSTEM_TIME_HAVE_DIV_INST
endif


ifeq ($(ARCH),cortex-m3)
    THUMB    := -mthumb
    THUMB_IW :=
    THUMB_ONLY := 1
    CLASS_NAME := CORTEXM3
    CLASS_DIR := cm3
    PROJECT_OPTIONS = -DSYSTEM_TIME_HAVE_DIV_INST
endif


ifeq ($(ARCH),cortex-m0)
    THUMB    := -mthumb
    THUMB_IW :=
    THUMB_ONLY := 1
    CLASS_NAME := CORTEXM0
    CLASS_DIR := cm0
    #PROJECT_OPTIONS = -DSYSTEM_TIME_HAVE_DIV_INST
endif

PROJECT_OPTIONS += -D$(CLASS_NAME)

# direktorij gdje idu obj fajlovi
#OBJDIR := obj/
#OBJDIR := obj

# Target file name (without extension).
TARGETLIB := libc

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS := ./inc ./inc/$(CLASS_DIR)


# Thumb and ARM Assembler source files ...
# ASRC, ASRCARM
# SRCOs, SRCO0, SRCO1, SRCO2, SRCO3 for Thumb code with
#  s,      0,     1,     2,     3   optimization level
# SRCARMOs, SRCARMO0, SRCARMO1, SRCARMO2, SRCARMO3 for ARM code with
#    s,         0,       1,         2,        3    optimisation level
# CPPSRCOs, CPPSRCO0, CPPSRCO1, CPPSRCO2, CPPSRCO3 for Thumb C++ code with
#    s,         0,       1,        2,        3     optimisation level
# CPPSRCARMOs, CPPSRCARMO0, CPPSRCARMO1, CPPSRCARMO2, CPPSRCARMO3 for ARM C++ code with
#     s,            0,           1,           2,           3      optimisation level

# these must be Simply expanded variables
ASRC :=
SRCOs :=
SRCO0 :=
SRCO1 :=
SRCO2 :=
SRCO3 :=
CPPSRCOs :=
CPPSRCO0 :=
CPPSRCO1 :=
CPPSRCO2 :=
CPPSRCO3 :=
ASRCARM :=
SRCARMOs :=
SRCARMO0 :=
SRCARMO1 :=
SRCARMO2 :=
SRCARMO3 :=
CPPSRCARMOs :=
CPPSRCARMO0 :=
CPPSRCARMO1 :=
CPPSRCARMO2 :=
CPPSRCARMO3 :=


SRCOs += abs.c
SRCOs += atoi.c
SRCOs += atol.c
SRCOs += atoll.c
SRCOs += bsearch.c
SRCOs += ctype.c
SRCOs += div.c
SRCOs += itoa.c
SRCOs += ldiv.c
SRCOs += lldiv.c
SRCOs += lltoa.c
SRCOs += ltoa.c
SRCOs += memccpy.c
SRCO2 += memccpy_fast.c
SRCOs += memchr.c
SRCO2 += memchr_fast.c
SRCOs += memcmp.c
SRCO2 += memcmp_fast.c
SRCOs += memcpy.c
SRCO2 += memcpy_fast.c
SRCOs += memmove.c
SRCO2 += memmove_fast.c
SRCOs += memset.c
SRCO2 += memset_fast.c
SRCOs += msp_printf.c
SRCOs += rand.c
SRCOs += reverse.c
SRCOs += strcasecmp.c
SRCO2 += strcat.c
SRCO2 += strchr.c
SRCOs += strcmp.c
SRCO2 += strcmp_fast.c
SRCO2 += strcpy.c
SRCOs += strcspn.c
SRCOs += strlcat.c
SRCOs += strlcpy.c
SRCO2 += strlen.c
SRCOs += strncasecmp.c
SRCOs += strncat.c
SRCOs += strncmp.c
SRCO2 += strncmp_fast.c
SRCOs += strncpy.c
SRCO2 += strncpy_fast.c
SRCOs += strnlen.c
SRCOs += strpbrk.c
SRCOs += strrchr.c
SRCOs += strrev.c
SRCOs += strsep.c
SRCOs += strspn.c
SRCO2 += strspn_fast.c
SRCOs += strstr.c
SRCOs += strtok.c
SRCO2 += strtol.c
SRCO2 += strtoll.c
SRCO2 += strtoul.c
SRCO2 += strtoull.c
SRCOs += strupr.c
SRCOs += tfp_printf.c

SRCOs += asctime.c
SRCOs += clock_getres.c
SRCOs += clock_gettime.c
SRCOs += clock_nanosleep.c
SRCOs += clock_settime.c
SRCOs += ctime.c
SRCOs += gmtime.c
SRCOs += localtime.c
SRCOs += mktime.c
SRCOs += systime_ms.c
SRCOs += systime_sec.c
SRCOs += systime_tick.c
SRCOs += time.c
SRCOs += time_get_timezone.c
SRCOs += time_month_days.c
SRCOs += tzset.c
SRCOs += usleep.c



ifdef THUMB_ONLY
    # no ARM mode for cortex-mx
    SRCOs += $(SRCARMOs)
    SRCO0 += $(SRCARMO0)
    SRCO1 += $(SRCARMO1)
    SRCO2 += $(SRCARMO2)
    SRCO3 += $(SRCARMO3)
    CPPSRCOs += $(CPPSRCARMOs)
    CPPSRCO0 += $(CPPSRCARMO0)
    CPPSRCO1 += $(CPPSRCARMO1)
    CPPSRCO2 += $(CPPSRCARMO2)
    CPPSRCO3 += $(CPPSRCARMO3)
    ASRCARM :=
    SRCARMOs :=
    SRCARMO0 :=
    SRCARMO1 :=
    SRCARMO2 :=
    SRCARMO3 :=
    CPPSRCARMOs :=
    CPPSRCARMO0 :=
    CPPSRCARMO1 :=
    CPPSRCARMO2 :=
    CPPSRCARMO3 :=
endif


# Optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)


# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#DEBUG := stabs
#DEBUG := dwarf-2
DEBUG := gdb


# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD := -std=gnu99

# Place -D or -U options for C here
CDEFS += $(PROJECT_OPTIONS)

# Place -I options here
CINCS +=

# Place -D or -U options for ASM here
ADEFS +=

# Compiler flags.
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
#
# Flags for C and C++ (arm-elf-gcc/arm-elf-g++)
CFLAGS += -g$(DEBUG)
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -mapcs-frame
CFLAGS += -fomit-frame-pointer
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wextra
CFLAGS += -Wimplicit
CFLAGS += -Wcast-align
CFLAGS += -Wcast-qual
CFLAGS += -Wpointer-arith
CFLAGS += -Wredundant-decls
CFLAGS += -Wshadow
#CFLAGS += -Wswitch
#CFLAGS += -Wunused
#CFLAGS += -Wreturn-type

CFLAGS += -nostdinc -nostdlib -nostartfiles

# print include paths
#CFLAGS += -Wp,-v

CFLAGS += -Wa,-adhlns=$(OBJDIR)$(subst $(suffix $<),.lst,$<)
#CFLAGS += -Wa,-adhlns=$(OBJDIR)/$(notdir $(subst $(suffix $<),.lst,$<))
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

# flags only for C
#CONLYFLAGS += -Wstrict-prototypes -Wmissing-declarations
#CONLYFLAGS += -Wmissing-prototypes -Wnested-externs
CONLYFLAGS += $(CSTANDARD)

# flags only for C++ (arm-elf-g++)
CPPFLAGS = -fno-rtti -fno-exceptions
#CPPFLAGS +=

# Assembler flags.
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
##ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
ASFLAGS += $(ADEFS) -Wa,-adhlns=$(OBJDIR)$(<:.S=.lst),-g$(DEBUG)
#ASFLAGS += $(ADEFS) -Wa,-adhlns=$(OBJDIR)/$(notdir $(<:.S=.lst)),-g$(DEBUG)

# ---------------------------------------------------------------------------

# Define directories, if needed.
DIRARM := c:/bin/WinARM/
DIRARMUTILS := $(DIRARM)utils/bin/

# Define programs and commands.
#SHELL := $(DIRARMUTILS)sh
#SHELL := cmd
CC      := $(TCHAIN_PREFIX)gcc
CPP     := $(TCHAIN_PREFIX)g++
AR      := $(TCHAIN_PREFIX)ar
OBJCOPY := $(TCHAIN_PREFIX)objcopy
OBJDUMP := $(TCHAIN_PREFIX)objdump
SIZE    := $(TCHAIN_PREFIX)size
NM      := $(TCHAIN_PREFIX)nm

#REMOVE := $(DIRARMUTILS)rm -f
#REMOVEDIR := $(DIRARMUTILS)rm -rf
#COPY := $(DIRARMUTILS)cp

REMOVE := rm -f
REMOVEDIR := rm -rf
COPY := cp

# test.exe [ ]
#TEST := C:/bin/UnixUtils/usr/local/wbin/test
TEST := test


# Define Messages
# English
MSG_ERRORS_NONE := Errors: none
MSG_BEGIN := -------- begin --------
MSG_END := --------  end  --------
MSG_SIZE_BEFORE := Size before:
MSG_SIZE_AFTER := Size after:
MSG_FLASH := Creating load file for Flash:
MSG_EXTENDED_LISTING := Creating Extended Listing:
MSG_FULL_DISASEMBLY := Creating Full Disasembly:
MSG_SYMBOL_TABLE := Creating Symbol Table:
MSG_LINKING := Linking:
MSG_COMPILING := Compiling C:
MSG_COMPILING_ARM := "Compiling C (ARM-only):"
MSG_COMPILINGCPP := Compiling C++:
MSG_COMPILINGCPP_ARM := "Compiling C++ (ARM-only):"
MSG_ASSEMBLING := Assembling:
MSG_ASSEMBLING_ARM := "Assembling (ARM-only):"
MSG_CLEANING := Cleaning project:


# Define all object files.
COBJOs      = $(SRCOs:.c=.o)
COBJO0      = $(SRCO0:.c=.o)
COBJO1      = $(SRCO1:.c=.o)
COBJO2      = $(SRCO2:.c=.o)
COBJO3      = $(SRCO3:.c=.o)
COBJARMOs   = $(SRCARMOs:.c=.o)
COBJARMO0   = $(SRCARMO0:.c=.o)
COBJARMO1   = $(SRCARMO1:.c=.o)
COBJARMO2   = $(SRCARMO2:.c=.o)
COBJARMO3   = $(SRCARMO3:.c=.o)
CPPOBJOs    = $(CPPSRCOs:.cpp=.o)
CPPOBJO0    = $(CPPSRCO0:.cpp=.o)
CPPOBJO1    = $(CPPSRCO1:.cpp=.o)
CPPOBJO2    = $(CPPSRCO2:.cpp=.o)
CPPOBJO3    = $(CPPSRCO3:.cpp=.o)
CPPOBJARMOs = $(CPPSRCARMOs:.cpp=.o)
CPPOBJARMO0 = $(CPPSRCARMO0:.cpp=.o)
CPPOBJARMO1 = $(CPPSRCARMO1:.cpp=.o)
CPPOBJARMO2 = $(CPPSRCARMO2:.cpp=.o)
CPPOBJARMO3 = $(CPPSRCARMO3:.cpp=.o)
AOBJ      = $(ASRC:.S=.o)
AOBJARM   = $(ASRCARM:.S=.o)

ALL_OBJ =  $(AOBJARM) $(AOBJ)
ALL_OBJ += $(COBJARMOs) $(COBJARMO0) $(COBJARMO1) $(COBJARMO2) $(COBJARMO3)
ALL_OBJ += $(COBJOs) $(COBJO0) $(COBJO1) $(COBJO2) $(COBJO3)
ALL_OBJ += $(CPPOBJARMOs) $(CPPOBJARMO0) $(CPPOBJARMO1) $(CPPOBJARMO2) $(CPPOBJARMO3)
ALL_OBJ += $(CPPOBJOs) $(CPPOBJO0) $(CPPOBJO1) $(CPPOBJO2) $(CPPOBJO3)
ALL_OBJ += $(EXTRAOBJS)

ALL_SRC =  $(ASRC) $(ASRCARM)
ALL_SRC += $(SRCOs) $(SRCO0) $(SRCO1) $(SRCO2) $(SRCO3)
ALL_SRC += $(SRCARMOs) $(SRCARMO0) $(SRCARMO1) $(SRCARMO2) $(SRCARMO3)
ALL_SRC += $(CPPSRCOs) $(CPPSRCO0) $(CPPSRCO1) $(CPPSRCO2) $(CPPSRCO3)
ALL_SRC += $(CPPSRCARMOs) $(CPPSRCARMO0) $(CPPSRCARMO1) $(CPPSRCARMO2) $(CPPSRCARMO3)

# Define all listing files.
LST =  $(ASRC:.S=.lst) $(ASRCARM:.S=.lst)
LST += $(SRCOs:.c=.lst) $(SRCARMOs:.c=.lst)
LST += $(SRCO0:.c=.lst) $(SRCARMO0:.c=.lst)
LST += $(SRCO1:.c=.lst) $(SRCARMO1:.c=.lst)
LST += $(SRCO2:.c=.lst) $(SRCARMO2:.c=.lst)
LST += $(SRCO3:.c=.lst) $(SRCARMO3:.c=.lst)
LST += $(CPPSRCOs:.cpp=.lst) $(CPPSRCARMOs:.cpp=.lst)
LST += $(CPPSRCO0:.cpp=.lst) $(CPPSRCARMO0:.cpp=.lst)
LST += $(CPPSRCO1:.cpp=.lst) $(CPPSRCARMO1:.cpp=.lst)
LST += $(CPPSRCO2:.cpp=.lst) $(CPPSRCARMO2:.cpp=.lst)
LST += $(CPPSRCO3:.cpp=.lst) $(CPPSRCARMO3:.cpp=.lst)

# Compiler flags to generate dependency files.
### GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mcpu=$(ARCH) $(THUMB_IW) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mcpu=$(ARCH) $(THUMB_IW) -I. -x assembler-with-cpp $(ASFLAGS)

ifdef OBJDIR
    OBJMAGIC = $< -o $(OBJDIR)/$(notdir $@)
else
    OBJMAGIC = $< -o $@
endif


# Default target.
all: begin gccversion buildlib finished end

#buildlib: objdir a copytarget
buildlib: objdir libc_version.h $(TARGETLIB).a

version: libc_version.h

# svn macro expansion
libc_version.h: libc_version.h.in
	cp libc_version.h.in libc_version.h
#	SubWCRev . libc_version.h.in libc_version.h

# pravi se OBJDIR ako je definisan
ifdef OBJDIR
objdir:
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
else
objdir:

endif

# kopira se target iz OBJDIR direktorija
copytarget:
	@echo "kopira se target iz OBJDIR direktorija"
	@if [ -d $(OBJDIR) -a -f $(OBJDIR)/$(TARGET).hex ]; then $(COPY) $(OBJDIR)/$(TARGET).hex . ; fi
	@if [ -d $(OBJDIR) -a -f $(OBJDIR)/$(TARGETLIB).a ]; then $(COPY) $(OBJDIR)/$(TARGETLIB).a . ; fi


# Eye candy.
begin:
	@echo
	@echo $(MSG_BEGIN)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo


# Display compiler version information.
gccversion :
	@$(CC) --version


# Link: create .a output file from object files.
$(TARGETLIB).a:  $(ALL_OBJ)
	@echo
	@echo $(MSG_LINKING) $@
#	@echo OBJDIR: $(OBJDIR)
#	@echo ALL_OBJ: $(ALL_OBJ)
#	@echo ALL_OBJ_NOTDIR: $(notdir $(ALL_OBJ))
#	@echo ALL_OBJ_PREFIXED: $(addprefix $(OBJDIR)/,$(notdir $(ALL_OBJ)))
ifdef OBJDIR
	@$(AR) rc $(TARGETLIB).a $(addprefix $(OBJDIR)/,$(notdir $(ALL_OBJ)))
else
	@$(AR) rc $(TARGETLIB).a $(ALL_OBJ)
endif


# Compile: create assembler files from C source files. ARM/Thumb
## does not work - TODO - hints welcome
##$(COBJ) : %.s : %.c
##	$(CC) $(THUMB) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files. ARM/Thumb
$(AOBJ) : %.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(THUMB) $(ALL_ASFLAGS) $< -o $(OBJDIR)$@


# Assemble: create object files from assembler source files. ARM-only
$(AOBJARM) : %.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING_ARM) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $(OBJDIR)$@


# Optimization -Os

# Compile: create object files from C source files. ARM/Thumb
$(COBJOs) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
#	$(CC) -c $(THUMB) $(ALL_CFLAGS) -Os $(CONLYFLAGS) $< -o $(OBJDIR)$@
	$(CC) -c $(THUMB) $(ALL_CFLAGS) -Os $(CONLYFLAGS) $(OBJMAGIC)

# Compile: create object files from C source files. ARM-only
$(COBJARMOs) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING_ARM) $<
	$(CC) -c $(ALL_CFLAGS) -Os $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM/Thumb
$(CPPOBJOs) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	$(CPP) -c $(THUMB) $(ALL_CFLAGS) -Os $(CPPFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM-only
$(CPPOBJARMOs) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $<
	$(CPP) -c $(ALL_CFLAGS) -Os $(CPPFLAGS) $< -o $(OBJDIR)$@


# Optimization -O0

# Compile: create object files from C source files. ARM/Thumb
$(COBJO0) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(THUMB) $(ALL_CFLAGS) -O0 $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C source files. ARM-only
$(COBJARMO0) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING_ARM) $<
	$(CC) -c $(ALL_CFLAGS) -O0 $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM/Thumb
$(CPPOBJO0) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	$(CPP) -c $(THUMB) $(ALL_CFLAGS) -O0 $(CPPFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM-only
$(CPPOBJARMO0) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $<
	$(CPP) -c $(ALL_CFLAGS) -O0 $(CPPFLAGS) $< -o $(OBJDIR)$@


# Optimization -O1

# Compile: create object files from C source files. ARM/Thumb
$(COBJO1) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(THUMB) $(ALL_CFLAGS) -O1 $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C source files. ARM-only
$(COBJARMO1) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING_ARM) $<
	$(CC) -c $(ALL_CFLAGS) -O1 $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM/Thumb
$(CPPOBJO1) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	$(CPP) -c $(THUMB) $(ALL_CFLAGS) -O1 $(CPPFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM-only
$(CPPOBJARMO1) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $<
	$(CPP) -c $(ALL_CFLAGS) -O1 $(CPPFLAGS) $< -o $(OBJDIR)$@


# Optimization -O2

# Compile: create object files from C source files. ARM/Thumb
$(COBJO2) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(THUMB) $(ALL_CFLAGS) -O2 $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C source files. ARM-only
$(COBJARMO2) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING_ARM) $<
	$(CC) -c $(ALL_CFLAGS) -O2 $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM/Thumb
$(CPPOBJO2) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	$(CPP) -c $(THUMB) $(ALL_CFLAGS) -O2 $(CPPFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM-only
$(CPPOBJARMO2) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $<
	$(CPP) -c $(ALL_CFLAGS) -O2 $(CPPFLAGS) $< -o $(OBJDIR)$@



# Optimization -O3

# Compile: create object files from C source files. ARM/Thumb
$(COBJO3) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(THUMB) $(ALL_CFLAGS) -O3 $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C source files. ARM-only
$(COBJARMO3) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING_ARM) $<
	$(CC) -c $(ALL_CFLAGS) -O3 $(CONLYFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM/Thumb
$(CPPOBJO3) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	$(CPP) -c $(THUMB) $(ALL_CFLAGS) -O3 $(CPPFLAGS) $< -o $(OBJDIR)$@

# Compile: create object files from C++ source files. ARM-only
$(CPPOBJARMO3) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $<
	$(CPP) -c $(ALL_CFLAGS) -O3 $(CPPFLAGS) $< -o $(OBJDIR)$@


# Target: install
install: libc_version.h $(OBJDIR)$(TARGETLIB).a
	@if [ ! -d ./../libc/$(CLASS_DIR) ]; then mkdir ./../libc/$(CLASS_DIR); fi
	@$(COPY) $(OBJDIR)$(TARGETLIB).a ./../libc/$(CLASS_DIR)
	@$(COPY) libc_version.h ./../libc/$(CLASS_DIR)
	@$(COPY) ./inc/*.h ./../libc/$(CLASS_DIR)
	@$(COPY) ./inc/$(CLASS_DIR)/*.h ./../libc/$(CLASS_DIR)


# Target: clean project.
#clean: begin clean_list finished end
clean: clean_list

distclean: clean_list
	@$(REMOVE) ./../libc/$(CLASS_DIR)/$(TARGETLIB).a
	@$(REMOVE) ./../libc/$(CLASS_DIR)/*.h
#	@$(REMOVEDIR) ./../libc/*

clean_list :
	@$(REMOVE) libc_version.h
	@$(REMOVE) $(TARGETLIB).obj
	@$(REMOVE) $(TARGETLIB).elf
	@$(REMOVE) $(TARGETLIB).map
	@$(REMOVE) $(TARGETLIB).obj
	@$(REMOVE) $(TARGETLIB).a90
	@$(REMOVE) $(TARGETLIB).sym
	@$(REMOVE) $(TARGETLIB).lnk
	@$(REMOVE) $(TARGETLIB).lss
	@$(REMOVE) $(TARGETLIB).dis
	@$(REMOVE) $(TARGETLIB).a
	@$(REMOVE) $(ALL_OBJ)
	@$(REMOVE) $(LST)
	@$(REMOVE) $(SRCOs:.c=.s)
	@$(REMOVE) $(SRCO0:.c=.s)
	@$(REMOVE) $(SRCO1:.c=.s)
	@$(REMOVE) $(SRCO2:.c=.s)
	@$(REMOVE) $(SRCO3:.c=.s)
	@$(REMOVE) $(SRCOs:.c=.d)
	@$(REMOVE) $(SRCO0:.c=.d)
	@$(REMOVE) $(SRCO1:.c=.d)
	@$(REMOVE) $(SRCO2:.c=.d)
	@$(REMOVE) $(SRCO3:.c=.d)
	@$(REMOVE) $(SRCARMOs:.c=.s)
	@$(REMOVE) $(SRCARMO0:.c=.s)
	@$(REMOVE) $(SRCARMO1:.c=.s)
	@$(REMOVE) $(SRCARMO2:.c=.s)
	@$(REMOVE) $(SRCARMO3:.c=.s)
	@$(REMOVE) $(SRCARMOs:.c=.d)
	@$(REMOVE) $(SRCARMO0:.c=.d)
	@$(REMOVE) $(SRCARMO1:.c=.d)
	@$(REMOVE) $(SRCARMO2:.c=.d)
	@$(REMOVE) $(SRCARMO3:.c=.d)
	@$(REMOVE) $(CPPSRCOs:.cpp=.s)
	@$(REMOVE) $(CPPSRCO0:.cpp=.s)
	@$(REMOVE) $(CPPSRCO1:.cpp=.s)
	@$(REMOVE) $(CPPSRCO2:.cpp=.s)
	@$(REMOVE) $(CPPSRCO3:.cpp=.s)
	@$(REMOVE) $(CPPSRCOs:.cpp=.d)
	@$(REMOVE) $(CPPSRCO0:.cpp=.d)
	@$(REMOVE) $(CPPSRCO1:.cpp=.d)
	@$(REMOVE) $(CPPSRCO2:.cpp=.d)
	@$(REMOVE) $(CPPSRCO3:.cpp=.d)
	@$(REMOVE) $(CPPSRCARMOs:.cpp=.s)
	@$(REMOVE) $(CPPSRCARMO0:.cpp=.s)
	@$(REMOVE) $(CPPSRCARMO1:.cpp=.s)
	@$(REMOVE) $(CPPSRCARMO2:.cpp=.s)
	@$(REMOVE) $(CPPSRCARMO3:.cpp=.s)
	@$(REMOVE) $(CPPSRCARMOs:.cpp=.d)
	@$(REMOVE) $(CPPSRCARMO0:.cpp=.d)
	@$(REMOVE) $(CPPSRCARMO1:.cpp=.d)
	@$(REMOVE) $(CPPSRCARMO2:.cpp=.d)
	@$(REMOVE) $(CPPSRCARMO3:.cpp=.d)
	@$(REMOVEDIR) .dep
	@$(REMOVEDIR) $(OBJDIR)

Debug: all
Release: all
cleanDebug: clean
cleanRelease: clean

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build buildlib a elf hex lss sym dis clean clean_list program install distclean \
Debug Release cleanDebug cleanRelease
