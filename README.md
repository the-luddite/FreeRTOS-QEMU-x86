## Prerequisite:
    * Install `zypper in gcc7-32bit`

## How to build:
    * cd qemu-freertos/FreeRTOS/Demo/IA32_flat_GCC_Galileo_Gen_2
    * make CC=gcc-7

## Run on Qemu:
    * cd qemu-freertos/FreeRTOS/Demo/IA32_flat_GCC_Galileo_Gen_2
    * make qemu

## How to debug:
    * cd qemu-freertos/FreeRTOS/Demo/IA32_flat_GCC_Galileo_Gen_2

### Run qemu in debug mode:
    * make QEMU_DEBUG=1 qemu

### Connect GDB
    * gdb frtos.elf
    * (gdb) target remote localhost:1234
    * (gdb) c

More informaion: https://qemu-project.gitlab.io/qemu/system/gdb.html