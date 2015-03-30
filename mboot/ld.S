SECTIONS
{
	/*
	 * check to see if we defined section starts in the makefile - if not,
	 * define them here.
	 *
	 * Align everything to a 16-byte boundary if you're specifying the
	 * addresses here.
	 */

	TEXT_START = DEFINED(TEXT_START) ? TEXT_START : 0x00000000;
	.text TEXT_START : AT (TEXT_START)
	{
		_text_start = .;
		*(.text)
		*(.text.*)
		_text_end = .;
	}

	. = ALIGN(4);
	.rodata :
	{
		*(.rodata)
		*(.rodata.*)
		*(.rodata1)
		*(.sdata2)
		*(.sdata2.*)
		*(.sbss2)
		*(.sbss2.*)
	}

	. = ALIGN(4);
	.data :
	{
		_data_start = .;
		*(.data)
		_data_end = .;
	}
	_end = .;

	.bss :
	{
		_bss_start = .;
		*(.sbss)
		*(.scommon)
		*(.dynbss)
		*(.bss)
		_stack_start = .;
		. = . + STACK_SIZE;
		_stack_end = .;
		*(COMMON);
		_bss_end = . ;
	}


	/*
	   These debug sections are here for information only - they're not going to be
	   included in the ROM-RAM copy because it only copies .text, .data, and .bss.
	 */
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }

	/* DWARF debug sections */
	.debug 0 : {*(.debug)}
	.debug_srcinfo 0 : {*(.debug_srcinfo)}
	.debug_aranges 0 : {*(.debug_aranges)}
	.debug_pubnames 0 : {*(.debug_pubnames)}
	.debug_sfnames 0 : {*(.debug_sfnames)}
	.line 0 : {*(.line)}
}
