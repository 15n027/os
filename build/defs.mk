ARCH ?= i686
VERBOSE ?= 1
TOPDIR ?= $(abspath .)

ifeq ($(origin ARCH), undefined)
$(error ARCH not defined)
endif
ifeq ($(origin TOPDIR), undefined)
$(error TOPDIR not defined)
endif

TOPDIR := $(abspath $(TOPDIR))

PFX := $(TOPDIR)/toolchain/installed/bin/$(ARCH)-elf-
TOOLCHAINLIBDIR := $(TOPDIR)/toolchain/installed/lib

DEBUG ?= 1

CC := $(TOPDIR)/build/cc.sh $(VERBOSE) $(DEBUG) $(PFX)gcc
CROSS_GCC := $(CC)
LD := $(PFX)ld
STRIP := $(PFX)strip
AR := $(PFX)ar
AS := $(PFX)gcc
NM := $(PFX)nm
RANLIB := $(PFX)ranlib
READELF := $(PFX)readelf
OBJCOPY := $(PFX)objcopy
OBJDUMP := $(PFX)objdump
LIBGCC := $(shell $(PFX)gcc -print-libgcc-file-name)
OBJTOPDIR := obj
OBJDIRPREFIX64 := $(TOPDIR)/$(OBJTOPDIR)/obj-x86_64
OBJDIRPREFIX32 := $(TOPDIR)/$(OBJTOPDIR)/obj-i686
OBJDIRPREFIX := $(TOPDIR)/$(OBJTOPDIR)/obj-$(ARCH)
OBJDIR := $(OBJDIRPREFIX)$(CURDIR)
LIBDIR := $(OBJDIRPREFIX)/lib
TARGETDIR := $(TOPDIR)/$(OBJTOPDIR)/targets

# -Wl, --no-gc-sections
INSTALL := $(TOPDIR)/build/install.sh $(VERBOSE) $(OBJDIRPREFIX)/..
INSTALLLIB := $(INSTALL) $(LIBDIR)
INSTALLTARGET := $(INSTALL) $(TARGETDIR)

#-ftrapv
# -finstrument-functions
RETPOLINE := -mindirect-branch=thunk -mindirect-branch-register
CFLAGS := -Wall -std=gnu99 -mno-sse -mno-mmx -nostdlib -nostdinc -static \
	-ffreestanding -fbuiltin -mgeneral-regs-only -fno-exceptions \
	-fno-unwind-tables -fno-asynchronous-unwind-tables -ffunction-sections -fdata-sections
GCCROOT := $(dir $(shell $(PFX)gcc -print-libgcc-file-name))
CPPFLAGS := -ffreestanding -I $(OBJDIRPREFIX)/include -D__JWOS__ \
	-I $(GCCROOT)/include
LDFLAGS := -L $(LIBDIR) -nostdlib -static $(LIBGCC)

ifeq ($(DEBUG), 0)
CFLAGS += -fomit-frame-pointer -Os
else
CFLAGS += -DINCLUDE_ASSERTS -fverbose-asm
CFLAGS += -fno-omit-frame-pointer -O
endif
CFLAGS += -g -ggdb -g3

ifeq ($(ARCH), x86_64)
	CFLAGS += -mno-red-zone -mcmodel=kernel
	CFLAGS += -DARCH_BITS=64
else
	CFLAGS += -DARCH_BITS=32
endif

