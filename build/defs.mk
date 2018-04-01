ARCH ?= i686
VERBOSE ?= 0
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

CC := $(TOPDIR)/build/cc.sh $(VERBOSE) $(PFX)gcc
CROSS_GCC := $(CC)
LD := $(PFX)ld
STRIP := $(PFX)strip
AR := $(PFX)ar
AS := $(PFX)as
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

DEBUG ?= 1
#-ftrapv
# -finstrument-functions
CFLAGS := -Wall -std=gnu99 -mno-sse -mno-mmx -nostdlib -nostdinc -static \
	-Wstack-usage=0 -ffreestanding -fbuiltin -mindirect-branch=thunk \
	-mindirect-branch-register -mgeneral-regs-only -mmitigate-rop
CPPFLAGS := -I $(OBJDIRPREFIX)/include
LDFLAGS := -L $(LIBDIR) -nostdlib -static $(LIBGCC)

ifeq ($(DEBUG), 0)
CFLAGS += -fomit-frame-pointer -Os
else
CFLAGS +=  -g -ggdb -g3 -DINCLUDE_ASSERTS -fverbose-asm
CFLAGS += -fno-omit-frame-pointer -O
endif

ifeq ($(ARCH), x86_64)
	CFLAGS += -mno-red-zone -mcmodel=kernel
	CFLAGS += -DARCH_BITS=64
else
	CFLAGS += -DARCH_BITS=32
endif

