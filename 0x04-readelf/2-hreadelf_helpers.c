#include "hreadelf.h"


/**
 * print_segment64 - prints segment info for ELF program header table
 * @ELF_fd: ELF file descriptor
 * @paddr: current segment physical addr
 * @filesz: current segment file size
 * @st: string table
 **/
void print_segment64(int ELF_fd, Elf64_Addr paddr, uint64_t filesz, char *st)
{
	Elf64_Shdr sh;

	read(ELF_fd, &sh, sizeof(sh));
	if (sh.sh_addr >= paddr && sh.sh_addr < paddr + filesz)
	{
		if (printf("%s", st + sh.sh_name))
			putchar(' ');
		if (!strcmp(".data", st + sh.sh_name))
			printf(".bss ");
	}
}

/**
 * print_segment32 - prints segment info for ELF program header table
 * @ELF_fd: ELF file descriptor
 * @paddr: current segment physical addr
 * @filesz: current segment file size
 * @st: string table
 **/
void print_segment32(int ELF_fd, Elf32_Addr paddr, uint32_t filesz, char *st)
{
	Elf32_Shdr sh;

	read(ELF_fd, &sh, sizeof(sh));
	if (sh.sh_addr >= paddr && sh.sh_addr < paddr + filesz)
	{
		if (printf("%s", st + sh.sh_name))
			putchar(' ');
		if (!strcmp(".data", st + sh.sh_name))
			printf(".bss ");
	}
}

/**
 * print_p_flags - prints section header flags
 * @p_flags: flags value
 **/
void print_p_flags(uint32_t p_flags)
{
	unsigned int j, k;
	char flag_string[16];
	macro_matcher_t flags[] = {
		{PF_R, "R"},
		{PF_W, "W"},
		{PF_X, "E"}
	};

	for (j = 0, k = 0; j < sizeof(flags) / sizeof(macro_matcher_t); j++, k++)
		if (p_flags & flags[j].macro)
		{
			sprintf(flag_string + k, "%s", flags[j].macro_string);
			p_flags ^= flags[j].macro;
		}
		else
		{
			flag_string[k] = ' ';
		}
	flag_string[k] = '\0';
	printf("%3s ", flag_string);
}


/**
 * print_p_type - prints p_type info
 * @p_type: p_type value
 * Return: 1 if INTERP | 0 if not
 **/
int print_p_type(uint32_t p_type)
{
	macro_matcher_t types[] = {
		{PT_LOAD, "LOAD"},
		{PT_DYNAMIC, "DYNAMIC"},
		{PT_INTERP, "INTERP"},
		{PT_NOTE, "NOTE"},
		{PT_SHLIB, "SHLIB"},
		{PT_PHDR, "PHDR"},
		{PT_TLS, "TLS"},
		{PT_LOOS, "LOOS"},
		{PT_GNU_EH_FRAME, "GNU_EH_FRAME"},
		{PT_GNU_STACK, "GNU_STACK"},
		{PT_GNU_RELRO, "GNU_RELRO"},
		{PT_LOSUNW, "LOSUNW"},
		{PT_SUNWBSS, "SUNWBSS"},
		{PT_SUNWSTACK, "SUNWSTACK"},
		{PT_HISUNW, "HISUNW"},
		{PT_HIOS, "HIOS"},
		{PT_LOPROC, "LOPROC"},
		{PT_HIPROC, "HIPROC"}
		};

	int types_size = sizeof(types) / sizeof(macro_matcher_t), i;

	for (i = 0; i < types_size && p_type != (unsigned int)types[i].macro; i++)
		;

	printf("  %-14s ", i == types_size ? "NULL" : types[i].macro_string);
	return (types[i].macro == PT_INTERP);
}
