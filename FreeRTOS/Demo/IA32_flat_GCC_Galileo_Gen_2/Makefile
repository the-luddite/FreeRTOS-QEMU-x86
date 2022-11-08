ELFFILE = frtos.elf

# application
__c_target = main.c
__c_target += Blinky_Demo/main_blinky.c
__c_target += $(wildcard Full_Demo/*.c)
__c_target += $(wildcard Support_Files/*.c)

__s_target = $(wildcard Full_Demo/*.S)
__s_target += Qemu/startup.S

# kernel
__c_target += $(wildcard ../../Source/*.c)
__c_target += $(wildcard ../../Source/portable/GCC/IA32_flat/*.c)
__c_target += $(wildcard ../../Source/portable/MemMang/heap_4.c)
# __c_target += $(wildcard ../Common/Minimal/*.c)

__c_target += ../Common/Minimal/AbortDelay.c
__c_target += ../Common/Minimal/BlockQ.c
__c_target += ../Common/Minimal/blocktim.c
__c_target += ../Common/Minimal/countsem.c
__c_target += ../Common/Minimal/death.c
__c_target += ../Common/Minimal/dynamic.c
__c_target += ../Common/Minimal/EventGroupsDemo.c
__c_target += ../Common/Minimal/flop.c
__c_target += ../Common/Minimal/GenQTest.c
__c_target += ../Common/Minimal/integer.c
__c_target += ../Common/Minimal/IntSemTest.c
__c_target += ../Common/Minimal/PollQ.c
__c_target += ../Common/Minimal/QPeek.c
__c_target += ../Common/Minimal/QueueOverwrite.c
__c_target += ../Common/Minimal/QueueSet.c
__c_target += ../Common/Minimal/QueueSetPolling.c
__c_target += ../Common/Minimal/recmutex.c
__c_target += ../Common/Minimal/semtest.c
__c_target += ../Common/Minimal/StaticAllocation.c
__c_target += ../Common/Minimal/TaskNotify.c
__c_target += ../Common/Minimal/TimerDemo.c
__c_target += ../Common/Minimal/IntQueue.c

# __c_target += Qemu/early_serial_console.c
# __c_target += Qemu/tty.c
__c_target += Qemu/debugcon_printf.c
__c_target += Qemu/debug_io.c

__s_target += $(wildcard ../../Source/portable/GCC/IA32_flat/*.S)


__include = -I.
__include += -ISupport_Files
__include += -IFull_Demo
__include += -I../Common/include
__include += -I../../Source/include
__include += -I../../Source/portable/GCC/IA32_flat


#__CFLAGS = -m32 -g -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only -ggdb3 $(CFLAGS) $(INCLUDE) $(__include)
M32 = -m32
__CFLAGS = $(M32) -Iinclude -nostartfiles -nostdlib -ffreestanding -g $(CFLAGS) $(INCLUDE) $(__include)
__SFLAGS = $(M32) $(INCLUDE) $(__include) $(SFLAGS)
__LDFLAGS = -nostdlib $(LDFLAGS)

OBJECTS = $(__c_target:.c=.o) $(__s_target:.S=.o)

%.o: %.c
	$(CC) $(__CFLAGS) -c $< -o $@ 

%.o: %.S
	$(CC) $(__SFLAGS) -c $< -o $@

all: elf

clean:
	rm -rf $(OBJECTS) $(ELFFILE)

elf:$(OBJECTS)
	@echo $(OBJECTS)
	$(CC) $(M32) -T Qemu/linker.ld $(OBJECTS) -o $(ELFFILE) -nostdlib -ffreestanding -lgcc

qemu:
	qemu-system-i386 -kernel $(ELFFILE) -s -S -chardev stdio,id=seabios -device isa-debugcon,iobase=0x402,chardev=seabios
