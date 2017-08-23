
PFX=/home/jwhite/os/toolchain/installed/bin/i686-elf-

export CC=${PFX}gcc
export CROSS_COMPILE=${PFX}
export CFLAGS="-O2 -march=native -mtune=native"
./configure --target=i686-elf --disable-shared --prefix=/home/jwhite/os/obj-i686
#make clean
make -j 8
make install
