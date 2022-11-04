TOPTARGETS := all clean
QEMU_ARGS = -m 1024 -machine virt -cpu cortex-a57 -kernel core/core.bin -nographic -display none -serial mon:stdio


__c_target = main.c
__c_target += $(wildcard Full_Demo/*.c)
__c_target += $(wildcard Support_Files/*.c)

__s_target = $(wildcard Full_Demo/*.S)
__s_target = $(wildcard Support_Files/*.S)


__include = -I.
__include += -ISupport_Files
__include += -I../Common/include
__include += -I../../Source/include
__include += -I../../Source/portable/GCC/IA32_flat


__CFLAGS = -m32 -Iinclude -g -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only $(CFLAGS) $(INCLUDE) $(__include)

OBJECTS = $(__c_target:.c=.o) $(__s_target:.S=.o)

%.o: %.c
	$(CC) $(__CFLAGS) -c $< -o $@ 

%.o: %.S
	$(CC) -m32 -c $< -o $@

all: frtos.bin

clean:
	rm -rf $(OBJECTS)

frtos.bin:$(OBJECTS)
	@echo $(OBJECTS)
	ld  -T elf_ia32_efi.lds $(OBJECTS) -o frtos.bin
