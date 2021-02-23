#include "hreadelf.h"

/**
 * print_machine - prints machine information for ELF header
 * @e_machine: value
 **/
void print_machine(uint16_t e_machine)
{
	macro_matcher_t machines[] = {
		{EM_M32, "AT&T WE 32100"},
		{EM_SPARC, "Sparc"},
		{EM_386, "Intel 80386"},
		{EM_68K, "Motorola 68000"},
		{EM_88K, "Motorola 88000"},
		{EM_860, "Intel 80860"},
		{EM_MIPS, "MIPS RS3000"},
		{EM_PARISC, "HP/PA"},
		{EM_SPARC32PLUS, "SPARC with enhanced instruction set"},
		{EM_PPC, "PowerPC"},
		{EM_PPC64, "PowerPC 64-bit"},
		{EM_S390, "IBM S/390"},
		{EM_ARM, "Advanced RISC Machines"},
		{EM_SH, "Renesas SuperH"},
		{EM_SPARCV9, "SPARC v9 64-bit"},
		{EM_IA_64, "Intel Itanium"},
		{EM_X86_64, "Advanced Micro Devices X86-64"},
		{EM_VAX, "DEC Vax"}
	};
	unsigned int i;

	for (i = 0; i < sizeof(machines) / sizeof(macro_matcher_t); i++)
		if (e_machine == machines[i].macro)
		{
			puts(machines[i].macro_string);
			return;
		}

	printf("<unknown: %x>\n", e_machine);
}
