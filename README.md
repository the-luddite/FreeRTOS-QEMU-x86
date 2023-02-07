# FreeRTOS demo application for QEMU i386

A tiny FreeRTOS demo application for QEMU i386
that creates two tasks that wake up one after one sending 
a message via xQueueReceive.

The demo is originally based on FreeRTOS demo for Intel Gallileo 
platform (x86 Quark SoC X1000) that has, with some tweaks and tricks, 
been ported to QEMU i386.

Instead of blinking LED of the hardware, two FreeRTOS tasks
send/receive a message and then notify a user via the QEMU debug console 
(https://github.com/QEMU/QEMU/blob/master/hw/char/debugcon.c)

# Directories

    ├── FreeRTOS
    │   ├── Demo
    │   │   ├── Common      ~~> Files used by all the demo applications.
    │   │   └── QEMU_x86    ~~> Platform-specific code, as well as demo application code.
    │   └── Source          ~~> FreeRTOS kernel source code, see https://github.com/FreeRTOS/FreeRTOS-Kernel.
    ├── Makefile
    └── README.md


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
