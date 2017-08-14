ARCH ?= i686

PFX := $(abspath $(TOPDIR)/toolchain)/bin/$(ARCH)-elf-
TOOLCHAINLIBDIR := $(abspath $(TOPDIR)/toolchain)/lib

CC := $(PFX)gcc
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


OBJDIR := $(TOPDIR)/obj
CPPFLAGS := -I $(OBJDIR)/libc/include
