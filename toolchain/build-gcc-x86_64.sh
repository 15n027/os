#!/bin/bash

GCC=gcc-8-20170813
BINUTILS=binutils-2.29

INSTALLDIR=`pwd`/installed
export CFLAGS="-O0 -pipe"
export CXXFLAGS=${CFLAGS}

rm -rf ${BINUTILS}
tar -xf ${BINUTILS}.tar.xz
pushd ${BINUTILS}
./configure --prefix=${INSTALLDIR} \
    --target=x86_64-elf \
    --disable-multilib \
    --enable-ld=yes --enable-bootstrap=no --enable-lto \
&& \
make \
&& \
make install
popd


tar -xf ${GCC}.tar.xz
mkdir -p build-dir-x86_64
pushd build-dir-x86_64
../${GCC}/configure \
    --prefix=${INSTALLDIR} --enable-ld=yes --target=x86_64-elf \
    --enable-languages=c,c++ --disable-libstdcxx --disable-bootstrap \
    --disable-libssp --disable-libquadmath --disable-libada \
&& \
make  \
&& \
make install
popd
