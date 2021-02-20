#include "hreadelf.h"

#define PH_SUMMARY "There are %d program headers, starting at offset %d\n\n"
#define PH_INTRO "Program Headers:"
#define PH_FIRST_ROW \
"  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align"

/**
 * main - replica of readelf -W -h <file>, where <file> is argv[1]
 * @argc: argument count
 * @argv: argument array
 * Return: 0 on success or no arguments passed | 1 on failure
 **/
int main(int argc, char *argv[])
{
	int elf_file;

	if (argc > 1)
	{
		if (open_ELF_file(&elf_file, argv[1], "readelf") == ELFCLASS64)
			print_elf64_program_header_table(elf_file);
		else
			print_elf32_program_header_table(elf_file);
	}

	return (EXIT_SUCCESS);
}


/**
 * print_elf64_program_header_table - prints ELF64 section header table
 * @elf_file: ELF file descriptor
 **/
void print_elf64_program_header_table(int elf_file)
{
	Elf64_Ehdr h;
	Elf64_Phdr ph;
	Elf64_Shdr sh;
	unsigned int flag, i, j;
	char *st;
	Elf64_Addr *paddrs;
	uint64_t *fileszs;

	read(elf_file, &h, sizeof(h));
	puts("");
	printf("Elf file type is ");
	print_ELF_file_type(h.e_type);
	printf("Entry point 0x%x\n", (unsigned int)h.e_entry);
	printf(PH_SUMMARY, h.e_phnum, (int)h.e_phoff);
	puts(PH_INTRO);
	puts(PH_FIRST_ROW);
	lseek(elf_file, h.e_phoff, 0);
	paddrs = malloc(sizeof(Elf64_Addr) * h.e_phnum);
	fileszs = malloc(sizeof(uint64_t) * h.e_phnum);
	for (i = 0; i < h.e_phnum; i++)
	{
		read(elf_file, &ph, sizeof(ph));
		flag = print_p_type(ph.p_type);
		printf("0x%06x ",  (unsigned int)ph.p_offset);
		printf("0x%016x ", (unsigned int)ph.p_vaddr);
		printf("0x%016x ", (unsigned int)ph.p_paddr);
		paddrs[i] = ph.p_paddr;
		printf("0x%06x ",  (unsigned int)ph.p_filesz);
		fileszs[i] = ph.p_filesz;
		printf("0x%06x ",  (unsigned int)ph.p_memsz);
		print_p_flags(ph.p_flags);
		printf("0x%x\n",   (unsigned int)ph.p_align);
		if (flag)
		{
			lseek(elf_file, ph.p_offset, 0);
			flag = 0;
			st = malloc(sizeof(char) * ph.p_filesz);
			read(elf_file, st, ph.p_filesz);
			printf("      [Requesting program interpreter: ");
			printf("%s]\n", st);
			lseek(elf_file, h.e_phoff + ((i + 1) * h.e_phentsize), 0);
		}
	}

	puts("\n Section to Segment mapping:");
	puts("  Segment Sections...");

	st = get_string_table(elf_file, h.e_shoff + (h.e_shentsize * h.e_shstrndx));
	for (i = 0, j = 0; i < h.e_phnum; i++)
	{
		printf("   %02d     ", i);
		lseek(elf_file, h.e_shoff, 0);
		for (j = 0; j < h.e_shnum; j++)
		{
			read(elf_file, &sh, sizeof(sh));
			if (sh.sh_addr >= paddrs[i] && sh.sh_addr < paddrs[i] + fileszs[i])
			{
				if (printf("%s", st + sh.sh_name))
					putchar(' ');
				if (!strcmp(".data", st + sh.sh_name))
					printf(".bss ");
			}
		}
		printf("\n");
	}
	free(st);
}

/**
 * print_elf32_program_header_table - prints ELF32 section header table
 * @elf_file: ELF file descriptor
 **/
void print_elf32_program_header_table(int elf_file)
{
	Elf32_Ehdr h;
	Elf32_Phdr ph;
	Elf32_Shdr sh;
	unsigned int i, j;
	char *st;
	Elf64_Addr *paddrs;
	uint64_t *fileszs;
	int flag;

	read(elf_file, &h, sizeof(h));
	puts("");
	printf("Elf file type is ");
	print_ELF_file_type(h.e_type);
	printf("Entry point 0x%x\n", (unsigned int)h.e_entry);
	printf(PH_SUMMARY, h.e_phnum, (int)h.e_phoff);
	puts(PH_INTRO);
	puts(PH_FIRST_ROW);
	lseek(elf_file, h.e_phoff, 0);
	paddrs = malloc(sizeof(Elf64_Addr) * h.e_phnum);
	fileszs = malloc(sizeof(uint64_t) * h.e_phnum);
	for (i = 0; i < h.e_phnum; i++)
	{
		read(elf_file, &ph, sizeof(ph));
		flag = print_p_type(ph.p_type);
		printf("0x%06x ",  (unsigned int)ph.p_offset);
		printf("0x%016x ", (unsigned int)ph.p_vaddr);
		printf("0x%016x ", (unsigned int)ph.p_paddr);
		paddrs[i] = ph.p_paddr;
		printf("0x%06x ",  (unsigned int)ph.p_filesz);
		fileszs[i] = ph.p_filesz;
		printf("0x%06x ",  (unsigned int)ph.p_memsz);
		print_p_flags(ph.p_flags);
		printf("0x%x\n",   (unsigned int)ph.p_align);
		if (flag)
		{
			lseek(elf_file, ph.p_offset, 0);
			flag = 0;
			st = malloc(sizeof(char) * ph.p_filesz);
			read(elf_file, st, ph.p_filesz);
			printf("      [Requesting program interpreter: ");
			printf("%s]\n", st);
			lseek(elf_file, h.e_phoff + ((i + 1) * h.e_phentsize), 0);
		}
	}

	puts("\n Section to Segment mapping:");
	puts("  Segment Sections...");

	st = get_string_table(elf_file, h.e_shoff + (h.e_shentsize * h.e_shstrndx));
	for (i = 0, j = 0; i < h.e_phnum; i++)
	{
		printf("   %02d     ", i);
		lseek(elf_file, h.e_shoff, 0);
		for (j = 0; j < h.e_shnum; j++)
		{
			read(elf_file, &sh, sizeof(sh));
			if (sh.sh_addr >= paddrs[i] && sh.sh_addr < paddrs[i] + fileszs[i])
			{
				if (printf("%s", st + sh.sh_name))
					putchar(' ');
				if (!strcmp(".data", st + sh.sh_name))
					printf(".bss ");
			}
		}
		printf("\n");
	}
	free(st);
}

/**
 * print_p_type - prints p_type info
 * @p_type: p_type value
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
