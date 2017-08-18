ARCH ?= i686
VERBOSE ?= 0
TOPDIR ?= $(abspath .)
#export TOPDIR

ifeq ($(origin ARCH), undefined)
$(error ARCH not set)
endif

PFX := $(abspath $(TOPDIR)/toolchain)/installed/bin/$(ARCH)-elf-
TOOLCHAINLIBDIR := $(abspath $(TOPDIR)/toolchain/installed)/lib

CC := $(TOPDIR)/build/cc.sh $(VERBOSE) $(PFX)gcc
#CC := $(PFX)gcc
LD := $(PFX)gcc
STRIP := $(PFX)strip
AR := $(PFX)ar
AS := $(PFX)as
NM := $(PFX)nm
RANLIB := $(PFX)ranlib
READELF := $(PFX)readelf
OBJCOPY := $(PFX)objcopy
OBJDUMP := $(PFX)objdump

CFLAGS := -Wall -std=gnu99 -pedantic -mno-sse -mno-mmx -nostdlib -nostdinc -fno-omit-frame-pointer

ifeq ($(ARCH), x86_64)
	CFLAGS += -mno-red-zone
endif

OBJDIRPREFIX := $(TOPDIR)/obj-$(ARCH)
OBJDIR := $(OBJDIRPREFIX)$(CURDIR)
LIBDIR := $(OBJDIRPREFIX)/lib
TARGETDIR := $(OBJDIRPREFIX)/targets
CPPFLAGS := -I $(OBJDIRPREFIX)/include -I$(TOPDIR)/kernel/include
LDFLAGS := -L $(LIBDIR) -nostdlib -Wl,--no-gc-sections
INSTALL := $(TOPDIR)/build/install.sh $(VERBOSE) $(OBJDIRPREFIX)/..
INSTALLLIB := $(INSTALL) $(LIBDIR)
INSTALLTARGET := $(INSTALL) $(TARGETDIR)

