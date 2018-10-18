ENTRY(_entry)

/* specify HF-RISC memory areas */

MEMORY 
{
send	: ORIGIN = 0x3FFFFF00, LENGTH = 128B
recv	: ORIGIN = 0x3FFFFF80, LENGTH = 128B
ram		: ORIGIN = 0x40000000, LENGTH = 2M	/* RAM area. */
}

/* define a global symbol _stack  */

_stack = 0x40200000;

/* now define the output sections  */

SECTIONS 
{
	. = 0;

	.text :
	{
		*(.text.init)
	}

	.text :
	{
		_text = .;
		*(.text)
		*(.text.*)
		_etext = .;
	} > ram

	.rodata :
	{
		_rodata = .;
		*(.rdata)
		*(.rodata)
		*(.rodata.*)
		_erodata = .;
	} > ram

	.data :
	{
		_data = .;
		*(.data)
		*(.data.*)
		_gp = . + 0x800;
		*(.sdata)
		*(.sdata.*)
		. = ALIGN(16); 
	} > ram

	_edata = .;

	.bss :
	{
		_bss_start = .;
		*(.sbss) 
		*(.sbss.*) 
		*(.bss) 
		*(.bss.*)
		. = ALIGN(4);
		_bss_end = . ;
	} > ram

	_end = .;
}

