#!/bin/bash
QEMU=/home/jwhite/qemu/bin/qemu-system-x86_64
#QEMU=/home/jwhite/qemu/bin/qemu-system-i386
TFTPDIR=~/os/obj/targets/tftp
#PXEROM=~/ipxe/src/bin/808610ea.rom
#ISO=~/ipxe/src/bin/ipxe.iso
ISO=~/os/obj/targets/boot.iso
CDROM="-cdrom $ISO -boot d"
#CDROM=
if [ -n "$1" ]; then
DBG="-s -S"
#DBG=
fi
$QEMU \
    -machine q35 \
    -m 7168 \
    $CDROM \
    -enable-kvm \
    -vga std \
    -monitor /dev/stdout \
    -display sdl  $DBG \
    -netdev user,id=bort,ipv4=on,net=172.16.0.0/16,dhcpstart=172.16.1.1,tftp=$TFTPDIR,bootfile=ipxe.default \
    -device virtio-net,netdev=bort,mac=52:54:00:12:aa:bb
reset
