/*
 * link.lds
 *
 *  Created on: 2011-5-11
 *      Author: sangwei
 */
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	.text :	
	{
		*(.init)
		*(.text)
		*(.text.*)
	}

	. = ALIGN(4);
	.rodata : 
	{
		*(.rodata)
		*(.rodata.*)
	}

	. = ALIGN(4);
	.data :
	{
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
	}

	. = ALIGN(4);
	.nobss : { *(.nobss) }

	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) }
	__bss_end = .;
}
