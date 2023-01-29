DEMO_PATH=FreeRTOS/Demo/IA32_flat_GCC_Galileo_Gen_2
CC=gcc-7

all:
	$(MAKE) -C $(DEMO_PATH) CC=$(CC)

clean:
	$(MAKE) -C $(DEMO_PATH) clean


qemu:
	$(MAKE) -C $(DEMO_PATH) qemu
