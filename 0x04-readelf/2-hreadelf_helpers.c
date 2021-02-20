#include "hreadelf.h"

/**
 * print_elf32_program_header_table - prints ELF32 section header table
 * @ELF_fd: ELF file descriptor
 **/
void print_elf32_program_header_table(int ELF_fd)
{
	Elf32_Ehdr h;
	Elf32_Phdr ph;
	unsigned int i, j;
	char *st, tmp[512];
	Elf32_Addr *paddrs;
	uint32_t *fileszs;

	read(ELF_fd, &h, sizeof(h));
	if (h.e_phnum == 0)
	{
		printf("\nThere are no program headers in this file.\n");
		return;
	}
	printf("\nElf file type is "), print_ELF_file_type(h.e_type);
	printf("Entry point 0x%x\n", (unsigned int)h.e_entry);
	printf(PH_SUMMARY, h.e_phnum, (int)h.e_phoff);
	puts(PH_INTRO), puts(PH_FIRST_ROW);
	lseek(ELF_fd, h.e_phoff, 0);
	paddrs = malloc(sizeof(Elf64_Addr) * h.e_phnum);
	fileszs = malloc(sizeof(uint64_t) * h.e_phnum);
	for (i = 0; i < h.e_phnum; i++)
	{
		read(ELF_fd, &ph, sizeof(ph));
		paddrs[i] = ph.p_paddr, fileszs[i] = ph.p_filesz;
		if (print_ELF32_program_header_row(ph) == IS_INTERP_ROW)
		{
			lseek(ELF_fd, ph.p_offset, 0), read(ELF_fd, tmp, ph.p_filesz);
			printf("      [Requesting program interpreter: %s]\n", tmp);
			lseek(ELF_fd, h.e_phoff + ((i + 1) * h.e_phentsize), 0);
		}
	}
	puts("\n Section to Segment mapping:\n  Segment Sections...");
	st = get_string_table(ELF_fd, h.e_shoff + (h.e_shentsize * h.e_shstrndx));
	for (i = 0, j = 0; i < h.e_phnum; printf("\n"), i++)
	{
		lseek(ELF_fd, h.e_shoff, 0), printf("   %02d     ", i);
		for (j = 0; j < h.e_shnum; j++)
			print_segment64(ELF_fd, paddrs[i], fileszs[i], st);
	}
	free(st), free(paddrs), free(fileszs);
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
 * print_ELF32_program_header_row - prints one row of a program header table
 * @ph: program header struct
 * Return: 0 almost alsways | 1 if program header is of type "INTERP"
 **/
int print_ELF32_program_header_row(Elf32_Phdr ph)
{
	int flag;

	flag = print_p_type(ph.p_type);
	printf("0x%06x ",  (unsigned int)ph.p_offset);
	printf("0x%08x ", (unsigned int)ph.p_vaddr);
	printf("0x%08x ", (unsigned int)ph.p_paddr);
	printf("0x%05x ",  (unsigned int)ph.p_filesz);
	printf("0x%05x ",  (unsigned int)ph.p_memsz);
	print_p_flags(ph.p_flags);
	printf("%#x\n",   (unsigned int)ph.p_align);
	return (flag);
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
