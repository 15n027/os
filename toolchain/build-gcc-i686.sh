#!/bin/bash -x

GCC=gcc-8-20170813
BINUTILS=binutils-2.29

INSTALLDIR=`pwd`/installed-i686
export CFLAGS="-O0 -pipe"
export CXXFLAGS=${CFLAGS}

tar -xf ${BINUTILS}.tar.xz
pushd ${BINUTILS}
./configure --prefix=${INSTALLDIR} \
    --target=i686-elf \
    --disable-multilib \
    --enable-ld=yes --enable-bootstrap=no --enable-lto \
&& \
make \
&& \
make install
popd


tar -xf ${GCC}.tar.xz  
mkdir -p build-dir-i686
pushd build-dir-i686
../${GCC}/configure \
    --prefix=${INSTALLDIR} --enable-ld=yes --target=i686-elf \
    --enable-languages=c,c++ --disable-libstdcxx --disable-bootstrap \
    --disable-libssp --disable-libquadmath --disable-libada \
&& \
make  \
&& \
make install
popd
