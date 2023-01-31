DEMO_PATH=FreeRTOS/Demo/QEMU_x86
CC=gcc-7

all:
	$(MAKE) -C $(DEMO_PATH) CC=$(CC)

clean:
	$(MAKE) -C $(DEMO_PATH) clean


qemu:
	$(MAKE) -C $(DEMO_PATH) qemu
