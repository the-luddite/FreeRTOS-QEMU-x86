# FreeRTOS demo application running on QEMU (x86)

A tiny FreeRTOS demo application for QEMU x86
that creates two tasks that wake up one after one sending 
a message via xQueueReceive.

The demo is originally based on FreeRTOS demo for Intel Gallileo 
platform (x86 Quark SoC X1000) that is, with some tweaks and tricks, 
has been ported to QEMU.

Instead of blinking LED of the hardware, two created FreeRTOS tasks
send/receive a message and print a message in the QEMU debug console 
(https://github.com/QEMU/QEMU/blob/master/hw/char/debugcon.c)


# Setting things up

The build was tested on openSUSE Tumbleweed.

GCC 7.5.0 (SUSE Linux)

You may need 32bit support in GCC, for openSUSE: 
    zypper in gcc7-32bit

QEMU emulator version 7.1.0 (openSUSE Tumbleweed)


## Build FreeRTOS and the application
    make CC=gcc-7

## Run the emulator:
    make QEMU

## Debug the application:
### Run QEMU in debug mode:
    make QEMU_DEBUG=1 QEMU

### Connect GDB
    gdb frtos.elf
    (gdb) target remote localhost:1234
    (gdb) c

More informaion: https://QEMU-project.gitlab.io/QEMU/system/gdb.html