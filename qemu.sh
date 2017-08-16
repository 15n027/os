#!/bin/bash
QEMU=/home/jwhite/qemu/bin/qemu-system-x86_64
TFTPDIR=~/os/tftp
PXEROM=~/ipxe/src/bin/808610ea.rom
ISO=~/ipxe/src/bin/ipxe.iso
#ISO=~/os/obj-i686/targets/boot.iso
$QEMU \
    -cdrom $ISO \
    -m 512 \
    -display sdl \
    -enable-kvm \
    -vga std  \
    -netdev user,id=bort,ipv4=on,net=172.16.0.0/16,dhcpstart=172.16.1.1,tftp=$TFTPDIR,bootfile=default \
    -device e1000e,netdev=bort,mac=52:54:00:12:aa:bb,romfile=$PXEROM
