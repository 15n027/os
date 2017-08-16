TOPDIR ?= ../..
include $(TOPDIR)/build/defs.mk

MYARCH := x86_64
ifeq ($(ARCH), i686)
	ARCH := i386
endif

HEADERDIR := $(OBJDIRPREFIX)

HOME=$(shell pwd)

WHAT=	\
	$(OBJDIR)/libdietlibc.a
# $(OBJDIR)/liblatin1.a \
#	$(OBJDIR)/libcompat.a

all: $(WHAT) install install-headers


profiling: $(OBJDIR)/libgmon.a $(OBJDIR)/pstart.o

INC=-I. -isystem include

VPATH=lib:libstdio:libugly:libcruft:libcrypt:libshell:liblatin1:libcompat:libdl:librpc:libregex:libm:profiling


LIBOBJ=$(patsubst lib/%.c,$(OBJDIR)/%.o,$(wildcard lib/*.c))
LIBUGLYOBJ=$(patsubst libugly/%.c,$(OBJDIR)/%.o,$(wildcard libugly/*.c))
LIBSTDIOOBJ=$(patsubst libstdio/%.c,$(OBJDIR)/%.o,$(wildcard libstdio/*.c))
LIBCRUFTOBJ=$(patsubst libcruft/%.c,$(OBJDIR)/%.o,$(wildcard libcruft/*.c))
LIBCRYPTOBJ=$(patsubst libcrypt/%.c,$(OBJDIR)/%.o,$(wildcard libcrypt/*.c))
LIBSHELLOBJ=$(patsubst libshell/%.c,$(OBJDIR)/%.o,$(wildcard libshell/*.c))
LIBMATH=$(patsubst libm/%.c,%.o,$(wildcard libm/*.c))

LIBREGEXOBJ=$(patsubst libregex/%.c,$(OBJDIR)/%.o,$(wildcard libregex/*.c))

LIBGMON_OBJS=$(OBJDIR)/__mcount.o $(OBJDIR)/monitor.o $(OBJDIR)/profil.o


LIBMATHOBJ=$(patsubst %,$(OBJDIR)/%,$(LIBMATH))

PWD=$(shell pwd)
HEADERS=$(shell find include -name \*.h)
INSTALLEDHEADERS=$(addprefix $(HEADERDIR)/,$(HEADERS))

.SUFFIXES:
.SUFFIXES: .S .c


$(OBJDIR):
	mkdir -p $@

% :: %,v

$(OBJDIR)/%: $(OBJDIR)

$(OBJDIR)/pstart.o: start.S
	@$(CROSS)$(CC) $(INC) $(CFLAGS) -DPROFILING -c $< -o $@

$(OBJDIR)/%.o: %.c
	@$(CROSS)$(CC) $(INC) $(CFLAGS) -c $< -o $@ -D__dietlibc__


CC+=-D__dietlibc__

#DIETLIBC_OBJ = $(OBJDIR)/unified.o \
#$(LIBOBJ) $(LIBSTDIOOBJ) \
#$(OBJDIR)/__longjmp.o $(OBJDIR)/setjmp.o
DIETLIBC_OBJ := $(LIBOBJ) $(LIBSTDIOOBJ)

$(OBJDIR)/libdietlibc.a: $(DIETLIBC_OBJ)
	@$(CROSS)ar cru $@ $(DIETLIBC_OBJ)


LIBLATIN1_OBJS=$(patsubst liblatin1/%.c,$(OBJDIR)/%.o,$(wildcard liblatin1/*.c))
$(OBJDIR)/liblatin1.a: $(LIBLATIN1_OBJS)
	$(CROSS)ar cru $@ $^

$(OBJDIR2)/libgmon.a: $(LIBGMON_OBJS)
	$(CROSS)ar cru $@ $^

$(OBJDIR)/libcompat.a: $(LIBCOMPATOBJ)
	$(CROSS)ar cru $@ $(LIBCOMPATOBJ)

$(OBJDIR)/libm.a: $(LIBMATHOBJ)
	$(CROSS)ar cru $@ $(LIBMATHOBJ)

LD_UNSET = env -u LD_RUN_PATH



#VERSION=dietlibc-$(shell head -n 1 CHANGES|sed 's/://')
#CURNAME=$(notdir $(shell pwd))


clean:
	rm -f *.o *.a t t1 compile load elftrunc exports mapfile libdietc.so
	rm -rf bin-* pic-*
	$(MAKE) -C examples clean
	$(MAKE) -C dynlinker clean
	$(MAKE) -C libdl clean


install: $(LIBDIR)/libdietlibc.a

$(LIBDIR)/libdietlibc.a: $(OBJDIR)/libdietlibc.a
	@$(INSTALLLIB) $<

install-headers: $(INSTALLEDHEADERS)

$(HEADERDIR)/%.h: %.h
	install -m 644 -D $< $@

.PHONY: i386 x86_64

x86_64:
	$(MAKE) ARCH=$@ CROSS=$@-linux- all


i386:
ifeq ($(MYARCH),x86_64)
	$(MAKE) ARCH=$@ CC="$(CC) -m32" all
else
	$(MAKE) ARCH=$@ CROSS=$@-linux- all
endif




# these depend on dietfeatures.h for large file backward compatibility

# these depend on dietfeatures.h for thread support
$(OBJDIR)/alloc.o $(OBJDIR)/perror.o $(OBJDIR)/logging.o $(OBJDIR)/unified.o $(OBJDIR)/clone.o $(OBJDIR)/set_errno.o: dietfeatures.h

# these depend on dietfeatures.h for linker warnings
$(OBJDIR)/assert_fail.o $(OBJDIR)/sprintf.o $(OBJDIR)/vsnprintf.o $(OBJDIR)/___div.o $(OBJDIR)/fflush.o $(OBJDIR)/setvbuf.o $(OBJDIR)/system.o $(OBJDIR)/sendfile.o $(OBJDIR)/setenv.o: dietfeatures.h

# these depend on dietfeatures.h for buffered stdio
fclose.o $(OBJDIR)/fdglue.o $(OBJDIR)/fflush.o $(OBJDIR)/fgetc.o $(OBJDIR)/fputc.o $(OBJDIR)/fread.o $(OBJDIR)/fseek.o $(OBJDIR)/printf.o $(OBJDIR)/setvbuf.o $(OBJDIR)/stderr.o $(OBJDIR)/stdin.o $(OBJDIR)/stdout.o $(OBJDIR)/fwrite.o $(OBJDIR)/puts.o: dietfeatures.h

# these depend on dietfeatures.h for fast string routines
strcasecmp.o $(OBJDIR)/strcat.o $(OBJDIR)/strchr.o $(OBJDIR)/strcmp.o $(OBJDIR)/strcpy.o $(OBJDIR)/strlen.o $(OBJDIR)/strncasecmp.o $(OBJDIR)/strncat.o $(OBJDIR)/strrchr.o $(OBJDIR)/memchr.o: dietfeatures.h

# this depends on dietfeatures.h for WANT_NON_COMPLIANT_STRNCAT
$(OBJDIR)/strncpy.o: dietfeatures.h

# these depend on dietfeatures.h for /proc
$(OBJDIR)/ttyname.o $(OBJDIR)/sysconf_cpus.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_TZFILE_PARSER
$(OBJDIR)/localtime_r.o $(OBJDIR)/strftime.o $(OBJDIR)/tzfile.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_SMALL_STDIO_BUFS
$(LIBSTDIOOBJ): dietfeatures.h include/stdio.h dietstdio.h

# these depend on dietfeatures.h for WANT_FULL_RESOLV_CONF
$(OBJDIR)/dnscruft.o $(OBJDIR)/dnscruft2.o $(OBJDIR)/res_search.o $(OBJDIR)/res_query.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_THREAD_SAFE and errno
$(LIBRPCOBJ) $(OBJDIR)/logging.o $(OBJDIR)/alloc.o $(OBJDIR)/cfsetospeed.o $(OBJDIR)/cfsetispeed.o \
$(OBJDIR)/execl.o $(OBJDIR)/execlp.o $(OBJDIR)/execv.o $(OBJDIR)/execvp.o $(OBJDIR)/isatty.o \
$(OBJDIR)/lockf.o $(OBJDIR)/perror.o $(OBJDIR)/remove.o $(OBJDIR)/set_errno.o $(OBJDIR)/sigemptyset.o \
$(OBJDIR)/tcsetattr.o $(OBJDIR)/dnscruft2.o $(OBJDIR)/dnscruft.o $(OBJDIR)/entlib.o \
$(OBJDIR)/gethostbyaddr.o $(OBJDIR)/gethostbyaddr_r.o $(OBJDIR)/gethostbyname.o \
$(OBJDIR)/gethostbyname_r.o $(OBJDIR)/gethostbyname2.o $(OBJDIR)/gethostbyname2_r.o \
$(OBJDIR)/inet_pton.o $(OBJDIR)/mkstemp.o $(OBJDIR)/mktemp.o $(OBJDIR)/tempnam.o \
$(OBJDIR)/glob.o $(OBJDIR)/realpath.o $(OBJDIR)/fdglue.o $(OBJDIR)/fdglue2.o \
$(OBJDIR)/getaddrinfo.o $(OBJDIR)/getnameinfo.o $(OBJDIR)/getprotoent.o \
$(OBJDIR)/getservent.o $(OBJDIR)/iconv.o $(OBJDIR)/iconv_open.o \
$(OBJDIR)/netent.o $(OBJDIR)/system.o $(OBJDIR)/stdin.o $(OBJDIR)/stdout.o \
$(OBJDIR)/stderr.o: dietfeatures.h


# these depend on dietfeatures.h for WANT_FREAD_OPTIMIZATION
$(OBJDIR)/fread.o $(OBJDIR)/fwrite.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_DYNAMIC
$(OBJDIR)/start.o $(OBJDIR)/dyn_start.o $(OBJDIR)/dyn_stop.o: dietfeatures.h

$(OBJDIR)/unified.o: dietuglyweaks.h

$(OBJDIR)/adjtimex.o: include/sys/timex.h

$(OBJDIR)/fgetc_unlocked.o $(OBJDIR)/fread.o $(OBJDIR)/ungetc.o: dietstdio.h

# these depend on dietfeatures.h for WANT_LINKER_WARNINGS
$(OBJDIR)/setlinebuf.o $(OBJDIR)/bzero.o $(OBJDIR)/setegid.o \
$(OBJDIR)/seteuid.o $(OBJDIR)/toascii.o: dietfeatures.h

# these depend on dietfeatures.h for WANT_FULL_POSIX_COMPAT
$(OBJDIR)/strncpy.o: dietfeatures.h
$(OBJDIR)/strxfrm.o: dietfeatures.h

$(OBJDIR)/stat.o $(OBJDIR)/fstat.o $(OBJDIR)/lstat.o: include/sys/stat.h
$(OBJDIR)/stat64.o $(OBJDIR)/fstat64.o $(OBJDIR)/lstat64.o: include/sys/stat.h

# these depend on dietfeatures.h for WANT_INET_ADDR_DNS
$(OBJDIR)/gethostbyname_r.o: dietfeatures.h

# WANT_PLUGPLAY_DNS
$(OBJDIR)/getaddrinfo.o: dietfeatures.h

$(OBJDIR)/strsignal.o: include/signal.h

$(LIBPTHREAD_OBJS): include/pthread.h

# CFLAGS+=-W -Wshadow -Wid-clash-31 -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings

# WANT_LARGEFILE_BACKCOMPAT
$(OBJDIR)/fcntl64.o: dietfeatures.h

# WANT_SSP
# This facepalm brought to you by: Ubuntu!
$(OBJDIR)/stackgap.o: lib/stackgap.c dietfeatures.h
	@$(CROSS)$(CC) $(INC) $(CFLAGS) -c lib/stackgap.c -o $@ -D__dietlibc__ -fno-stack-protector
	$(COMMENT) -$(CROSS)strip -x -R .comment -R .note $@

# WANT_MALLOC_ZERO
$(OBJDIR)/strndup.o: dietfeatures.h
