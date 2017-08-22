#!/bin/bash -x

GCC=gcc-8-20170813
BINUTILS=binutils-2.29

INSTALLDIR=`pwd`/installed
export CFLAGS="-O2 -pipe -fomit-frame-pointer"
export CXXFLAGS=${CFLAGS}
BINUTILSFLAGS="--prefix=${INSTALLDIR} --disable-multilib --enable-bootstrap=no --enable-lto --disable-bootstrap  --enable-compressed-debug-sections=all --with-sysroot --enable-host-shared --enable-vtable-verify"
GCCFLAGS="--prefix=${INSTALLDIR} --enable-ld=yes --enable-languages=c,c++ --disable-libstdcxx --disable-bootstrap --disable-libssp --disable-libquadmath --disable-libada"

rm -rf ${BINUTILS}
tar -xf ${BINUTILS}.tar.xz
pushd ${BINUTILS}
./configure $BINUTILSFLAGS --target=i686-elf \
&& make -j 4 && make install
popd
rm -rf ${BINUTILS}

tar -xf ${BINUTILS}.tar.xz
pushd ${BINUTILS}
./configure $BINUTILSFLAGS --target=x86_64-elf \
&& make -j 4 && make install
popd
rm -rf ${BINUTILS}


BUILDDIR=build-dir
rm -rf ${GCC}
tar -xf ${GCC}.tar.xz
rm -rf ${BUILDDIR}
mkdir -p ${BUILDDIR}
pushd ${BUILDDIR}
../${GCC}/configure $GCCFLAGS --target=i686-elf \
&& make -j 4 && make install
popd
rm -rf ${BUILDDIR}

rm -rf ${GCC}
tar -xf ${GCC}.tar.xz
mkdir -p ${BUILDDIR}
pushd ${BUILDDIR}
../${GCC}/configure $GCCFLAGS --target=x86_64-elf \
&& make -j 4 && make install
popd
rm -rf ${BUILDDIR}
