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
#CC := $(PFX)gcc
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

OPT := -fno-omit-frame-pointer -O2
DEBUG :=  -g -ggdb -g3 -DINCLUDE_ASSERTS
DEBUG :=
CFLAGS := -Wall -std=gnu99 -mno-sse -mno-mmx -nostdlib -nostdinc $(OPT) $(DEBUG) -static \
	-Wstack-usage=0
CPPFLAGS := -I $(OBJDIRPREFIX)/include
LDFLAGS := -L $(LIBDIR) -nostdlib -static $(LIBGCC)

ifeq ($(ARCH), x86_64)
	CFLAGS += -mno-red-zone -mcmodel=large
endif

