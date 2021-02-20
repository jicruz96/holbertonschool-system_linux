#include "hreadelf.h"

/**
 * main - replica of readelf -W -h <file>, where <file> is argv[1]
 * @argc: argument count
 * @argv: argument array
 * Return: 0 on success or no arguments passed | 1 on failure
 **/
int main(int argc, char *argv[])
{
	int ELF_fd;

	if (argc > 1)
	{
		if (open_ELF_file(&ELF_fd, argv[1], "readelf") == ELFCLASS64)
			print_elf64_program_header_table(ELF_fd);
		else
			print_elf32_program_header_table(ELF_fd);
	}

	return (EXIT_SUCCESS);
}

/**
 * print_elf64_program_header_table - prints ELF64 section header table
 * @ELF_fd: ELF file descriptor
 **/
void print_elf64_program_header_table(int ELF_fd)
{
	Elf64_Ehdr h;
	Elf64_Phdr ph;
	unsigned int i, j;
	char *st, tmp[512];
	Elf64_Addr *paddrs;
	uint64_t *fileszs;

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
		if (print_ELF64_program_header_row(ph) == IS_INTERP_ROW)
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
 * print_ELF64_program_header_row - prints one row of a program header table
 * @ph: program header struct
 * Return: 0 almost alsways | 1 if program header is of type "INTERP"
 **/
int print_ELF64_program_header_row(Elf64_Phdr ph)
{
	int flag;

	flag = print_p_type(ph.p_type);
	printf("0x%06x ",  (unsigned int)ph.p_offset);
	printf("0x%016x ", (unsigned int)ph.p_vaddr);
	printf("0x%016x ", (unsigned int)ph.p_paddr);
	printf("0x%06x ",  (unsigned int)ph.p_filesz);
	printf("0x%06x ",  (unsigned int)ph.p_memsz);
	print_p_flags(ph.p_flags);
	printf("0x%x\n",   (unsigned int)ph.p_align);
	return (flag);
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
