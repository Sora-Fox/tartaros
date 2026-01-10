ENTRY (_start)

_kernel_virtual_base = 0xC0000000;

SECTIONS
{
	. = 0x00100000;

	.multiboot : {
		*(.multiboot)
	}
	.bootstrap.text ALIGN (4K) : {
		*(.bootstrap.text)
	}

	. += _kernel_virtual_base;

	.text ALIGN (4K) : AT (ADDR (.text) - _kernel_virtual_base) {
		*(.text)
	}
	.init_array ALIGN (4K) : AT (ADDR (.init_array) - _kernel_virtual_base) {
		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
		KEEP(*(.init_array))
	}
	.fini_array ALIGN (4K) : AT (ADDR (.fini_array) - _kernel_virtual_base) {
		KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
		KEEP(*(.fini_array))
	 }
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - _kernel_virtual_base) {
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - _kernel_virtual_base) {
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - _kernel_virtual_base) {
		*(COMMON)
		*(.bss)
		. = ALIGN (16);
		. += 64K;
		_stack_top = .;
	}

	_init_array_begin  = ADDR(.init_array);
	_init_array_end    = ADDR(.init_array) + SIZEOF(.init_array);
	_fini_array_begin  = ADDR(.fini_array);
	_fini_array_end    = ADDR(.fini_array) + SIZEOF(.fini_array);

	_boostrap_start = LOADADDR(.bootstrap.text);
	_boostrap_end = LOADADDR(.bootstrap.text) + SIZEOF(.bootstrap.text);

	_kernel_phys_start = LOADADDR(.bootstrap.text);
	_kernel_phys_end   = LOADADDR(.bss) + SIZEOF(.bss);
	_kernel_virt_start = ADDR(.text);
	_kernel_virt_end   = ADDR(.bss) + SIZEOF(.bss);
}

