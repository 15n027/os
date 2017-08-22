#!/bin/bash
QEMU=/home/jwhite/qemu/bin/qemu-system-i386
TFTPDIR=~/os/tftp
#PXEROM=~/ipxe/src/bin/808610ea.rom
#ISO=~/ipxe/src/bin/ipxe.iso
ISO=~/os/obj-i686/targets/boot.iso
if [ -n "$1" ]; then
DBG="-s -S"
fi
$QEMU \
    -machine q35 \
    -cdrom $ISO \
    -boot d \
    -m 64 \
    -enable-kvm \
    -display sdl  $DBG
#    -netdev user,id=bort,ipv4=on,net=172.16.0.0/16,dhcpstart=172.16.1.1,tftp=$TFTPDIR,bootfile=default \
#    -device virtio-net,netdev=bort,mac=52:54:00:12:aa:bb
