ENTRY(_start)

SECTIONS {
  . = 1M;

  .text BLOCK(4K) : ALIGN(4K) {
    __text_start =  ABSOLUTE(.);
    *(.multiboot)
    *(.text)
    __text_end =  ABSOLUTE(.);
  }

  .data BLOCK(4K) : ALIGN(4K) {
    __data_start = ABSOLUTE(.);
  	__data_lma = LOADADDR(.data); 
  	__data_vma =  ABSOLUTE(.);
    *(.data)
    __data_end = ABSOLUTE(.);
  }

  .bss BLOCK(4K) : ALIGN(4K) {
    __bss_start =  ABSOLUTE(.);
    *(COMMON)
    *(.bss)
    __bss_end =  ABSOLUTE(.);
  }

  .stack BLOCK(4K) : ALIGN(4K) {
    *(.bootstrap_stack)
  }

  .boot_stack :
	{
		. += 2048;
    	__stack_for_main = ABSOLUTE( . );
   		. = ALIGN(1024);
 	}

}