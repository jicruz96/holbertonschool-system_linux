#include "hreadelf.h"

/**
 * print_ELF_file_type - prints ELF file type
 * @e_type: ELF header e_type value
 **/
void print_ELF_file_type(uint16_t e_type)
{
	macro_matcher_t types[] = {
		{ET_NONE, "NONE (Unknown type)"},
		{ET_REL, "REL (Relocatable file)"},
		{ET_EXEC, "EXEC (Executable file)"},
		{ET_DYN, "DYN (Shared object file)"},
		{ET_CORE, "CORE (Core file)"}
	};
	unsigned int i;

	for (i = 0; i < sizeof(types) / sizeof(macro_matcher_t); i++)
	{
		if (types[i].macro == e_type)
		{
			puts(types[i].macro_string);
			return;
		}
	}

	printf("<unknown: %u %x>\n", e_type, e_type);

}
