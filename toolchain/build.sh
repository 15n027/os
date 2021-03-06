#!/bin/bash -x

LOG=/home/jwhite/os/toolchain/build.log
GCC=gcc
BINUTILS=binutils

JOBS=6
INSTALLDIR=`pwd`/installed
export CFLAGS="-O3 -pipe -fomit-frame-pointer"
export CXXFLAGS=${CFLAGS}
BINUTILSFLAGS="--prefix=${INSTALLDIR} --disable-multilib --enable-bootstrap=no --enable-lto --disable-bootstrap  --enable-compressed-debug-sections=all --with-sysroot --enable-host-shared --enable-vtable-verify"
GCCFLAGS="--prefix=${INSTALLDIR} --enable-ld=yes --enable-languages=c,c++ --disable-libstdcxx --disable-bootstrap --disable-libssp --disable-libquadmath --disable-libada"
BUILDDIR=build-dir
ARCHES="i686-elf x86_64-elf"

for arch in $ARCHES; do
echo "Begin binutils $arch"
pushd ${BINUTILS}
git clean -f -d >> $LOG
./configure $BINUTILSFLAGS --target=$arch 2>1 >> $LOG \
&& make -j $JOBS 2>1 >> $LOG && make install 2>1 >> $LOG
if [ $? -eq 0 ]; then
    echo "binutils $arch done"
else
    echo "binutils $arch fail" && exit 1
fi
popd
done

for arch in $ARCHES; do
echo "Begin GCC $arch"
mkdir -p ${BUILDDIR}-$arch
pushd ${BUILDDIR}-$arch
../${GCC}/configure $GCCFLAGS --target=$arch >& $LOG \
&& make -j $JOBS >& $LOG && make install >& $LOG
if [ $? -eq 0 ]; then
    echo "gcc $arch done"
else
    echo "gcc $arch fail" && exit 1
fi
popd
done

