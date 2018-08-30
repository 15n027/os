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
	-enable-kvm \
    -smp 1 \
    -overcommit cpu-pm=on \
    -cpu max,+fsgsbase,+monitor \
    -machine type=q35,kernel_irqchip=off \
    -m 8 \
    $CDROM \
    -vga std \
    -monitor /dev/stdout \
    -display sdl  $DBG \
    -netdev user,id=bort,ipv4=on,net=172.16.0.0/16,dhcpstart=172.16.1.1,tftp=$TFTPDIR,bootfile=ipxe.default \
    -device virtio-net,netdev=bort,mac=52:54:00:12:aa:bb \
    -chardev file,id=serial0,path=serial.out \
    -device isa-serial,chardev=serial0

#    -chardev socket,id=serial0,port=9999,telnet,server,nowait,host=0.0.0.0 \


reset
