# FreeRTOS demo application running on QEMU (x86)

A tiny FreeRTOS demo application that is supposed to create two tasks 
that wake up one by one sending a message via xQueueReceive.

The demo is based on FreeRTOS "main_blinky" demo
(Demo/IA32_flat_GCC_Galileo_Gen_2) for Intel Gallileo 
platform (x86 Quark SoC X1000) that is, with some tweaks and tricks, 
has been ported to QEMU.

See https://www.freertos.org/RTOS_Intel_Quark_Galileo_GCC.html

Instead of blinking LED of the hardware, two created FreeRTOS tasks
send/receive a message and print a message to QEMU debug console 
(aka debugcon: https://github.com/QEMU/QEMU/blob/master/hw/char/debugcon.c)


# Setting things up

The build was tested on openSUSE Tumbleweed.

GCC 7.5.0 (SUSE Linux)

You may need 32bit support in GCC, for openSUSE: 
* zypper in gcc7-32bit

QEMU emulator version 7.1.0 (openSUSE Tumbleweed)


## Build FreeRTOS and the application
    * make CC=gcc-7

## Run the emulator:
    * make QEMU

## Debug the application:
### Run QEMU in debug mode:
    * make QEMU_DEBUG=1 QEMU

### Connect GDB
    * gdb frtos.elf
    * (gdb) target remote localhost:1234
    * (gdb) c

More informaion: https://QEMU-project.gitlab.io/QEMU/system/gdb.html