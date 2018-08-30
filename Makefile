include build/defs.mk

SUBDIRS64 := musl-1.1.16 acpica kernel64
SUBDIRS32 := musl-1.1.16 kernel32
SUBDIRS := iso

include build/post.mk
