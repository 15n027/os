#!/usr/bin/python
import subprocess
import sys
import os

QEMU = "~/qemu/bin/qemu-system-x86_64"
#QEMU=/home/jwhite/qemu/bin/qemu-system-i386
TFTPDIR = "~/os/obj/targets/tftp"
#PXEROM=~/ipxe/src/bin/808610ea.rom
#ISO=~/ipxe/src/bin/ipxe.iso
ISO = "~/os/obj/targets/boot.iso"

QEMU = os.path.expanduser(QEMU)
ISO = os.path.expanduser(ISO)
TFTPDIR = os.path.expanduser(TFTPDIR)

DBG = None
if len(sys.argv) > 1:
    DBG = ["-s", "-S"]

ARGS = [
    "-smp", "4",
    "-enable-kvm",
    "-cpu", "max,+fsgsbase,+monitor",
    "-overcommit", "cpu-pm=on",
    "-machine", "type=q35,kernel_irqchip=off",
    "-m", "8",
    "-cdrom", ISO, "-boot", "d",
    "-vga", "std",
    "-monitor", "/dev/stdout",
    "-display", "sdl",
    "-netdev", "user,id=bort,ipv4=on,net=172.16.0.0/16,dhcpstart=172.16.1.1,tftp=$TFTPDIR,bootfile=ipxe.default",
    "-device", "virtio-net,netdev=bort,mac=52:54:00:12:aa:bb",
    "-device", "isa-serial,chardev=serial0",
    "-chardev", "file,append=off,id=serial0,path=serial.out",
    "-trace", "apic_deliver_irq",
]
if DBG:
    ARGS.extend(DBG)

subprocess.call([QEMU] + ARGS, shell=False)
subprocess.call(["reset"], shell=False)

#    -chardev socket,id=serial0,port=9999,telnet,server,nowait,host=0.0.0.0 \
