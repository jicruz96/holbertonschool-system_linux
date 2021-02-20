#include "hreadelf.h"


#define SH_SUMMARY "There are %d section headers, starting at offset 0x%x:\n\n"
#define SH_INTRO "Section Headers:"
#define SH_FIRST_ROW \
"  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al"
#define SH_FIRST_ROW32 \
"  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al"

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
			print_elf64_section_header_table(ELF_fd);
		else
			print_elf32_section_header_table(ELF_fd);
	}

	return (EXIT_SUCCESS);
}


/**
 * print_elf64_section_header_table - prints ELF64 section header table
 * @ELF_fd: ELF file descriptor
 **/
void print_elf64_section_header_table(int ELF_fd)
{
	Elf64_Ehdr h;
	Elf64_Shdr sh;
	unsigned int i;
	char *st;
	char *FLAG_KEY_LINES[] = {
		"Key to Flags:",
		"  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)",
		"  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)",
		"  O (extra OS processing required) o (OS specific), p (processor specific)"
	};

	/* Read ELF header */
	read(ELF_fd, &h, sizeof(h));
	st = get_string_table(ELF_fd, h.e_shoff + (h.e_shentsize * h.e_shstrndx));

	printf(SH_SUMMARY, h.e_shnum, (unsigned int)h.e_shoff);
	puts(SH_INTRO);
	puts(SH_FIRST_ROW);
	lseek(ELF_fd, h.e_shoff, 0);
	for (i = 0; i < h.e_shnum; i++)
	{
		read(ELF_fd, &sh, sizeof(sh));
		printf("  [%2d] %-17s ", i, st + sh.sh_name);
		print_sh_type(sh.sh_type);
		printf("%016x %06x ", (int)sh.sh_addr, (int)sh.sh_offset);
		printf("%06x %02x ", (int)sh.sh_size, (int)sh.sh_entsize);
		print_sh_flags(sh.sh_flags);
		printf("%2i %3i %2i\n", sh.sh_link, sh.sh_info, (int)sh.sh_addralign);
	}
	for (i = 0; i < sizeof(FLAG_KEY_LINES) / sizeof(char *); i++)
		puts(FLAG_KEY_LINES[i]);
	close(ELF_fd);
	free(st);
}

/**
 * print_elf32_section_header_table - prints ELF32 section header table
 * @ELF_fd: ELF file descriptor
 **/
void print_elf32_section_header_table(int ELF_fd)
{
	Elf32_Ehdr h;
	Elf32_Shdr sh;
	unsigned int i;
	char *st;
	char *FLAG_KEY_LINES[] = {
		"Key to Flags:",
		"  W (write), A (alloc), X (execute), M (merge), S (strings)",
		"  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)",
		"  O (extra OS processing required) o (OS specific), p (processor specific)"
	};

	/* Read ELF header */
	read(ELF_fd, &h, sizeof(h));
	st = get_string_table32(ELF_fd, h.e_shoff + (h.e_shentsize * h.e_shstrndx));

	printf(SH_SUMMARY, h.e_shnum, (unsigned int)h.e_shoff);
	puts(SH_INTRO);
	puts(SH_FIRST_ROW32);
	lseek(ELF_fd, h.e_shoff, 0);
	for (i = 0; i < h.e_shnum; i++)
	{
		read(ELF_fd, &sh, sizeof(sh));
		printf("  [%2d] %-17s ", i, st + sh.sh_name);
		print_sh_type(sh.sh_type);
		printf("%08x %06x ", (int)sh.sh_addr, (int)sh.sh_offset);
		printf("%06x %02x ", (int)sh.sh_size, (int)sh.sh_entsize);
		print_sh_flags(sh.sh_flags);
		printf("%2i %3i %2i\n", sh.sh_link, sh.sh_info, (int)sh.sh_addralign);
	}
	for (i = 0; i < sizeof(FLAG_KEY_LINES) / sizeof(char *); i++)
		puts(FLAG_KEY_LINES[i]);
	close(ELF_fd);
	free(st);
}

/**
 * print_sh_flags - prints section header flags
 * @sh_flags: flags value
 **/
void print_sh_flags(uint32_t sh_flags)
{
	unsigned int j, k;
	char flag_string[16];
	macro_matcher_t flags[] = {
		{SHF_WRITE, "W"},
		{SHF_ALLOC, "A"},
		{SHF_EXECINSTR, "X"},
		{SHF_MERGE, "M"},
		{SHF_STRINGS, "S"},
		{SHF_INFO_LINK, "I"},
		{SHF_LINK_ORDER, "L"},
		{SHF_GROUP, "G"},
		{SHF_TLS, "T"},
		{SHF_EXCLUDE, "E"},
		{SHF_MASKOS, "o"},
		{SHF_MASKPROC, "O"}
	};

	for (j = 0, k = 0; j < sizeof(flags) / sizeof(macro_matcher_t); j++)
		if (sh_flags & flags[j].macro)
		{
			sprintf(flag_string + k++, "%s", flags[j].macro_string);
			sh_flags ^= flags[j].macro;
		}
	if (sh_flags)
		flag_string[k++] = 'x';
	flag_string[k] = '\0';
	printf("%3s ", flag_string);
}

/**
 * print_sh_type - prints sh_type info
 * @sh_type: sh_type value
 **/
void print_sh_type(uint32_t sh_type)
{
	macro_matcher_t types[] = {
		{SHT_PROGBITS, "PROGBITS"},
		{SHT_SYMTAB, "SYMTAB"},
		{SHT_STRTAB, "STRTAB"},
		{SHT_RELA, "RELA"},
		{SHT_HASH, "HASH"},
		{SHT_DYNAMIC, "DYNAMIC"},
		{SHT_NOTE, "NOTE"},
		{SHT_NOBITS, "NOBITS"},
		{SHT_REL, "REL"},
		{SHT_SHLIB, "SHLIB"},
		{SHT_DYNSYM, "DYNSYM"},
		{SHT_INIT_ARRAY, "INIT_ARRAY"},
		{SHT_FINI_ARRAY, "FINI_ARRAY"},
		{SHT_PREINIT_ARRAY, "PREINIT_ARRAY"},
		{SHT_GROUP, "GROUP"},
		{SHT_SYMTAB_SHNDX, "SYMTAB_SHNDX"},
		{SHT_LOOS, "LOOS"},
		{SHT_GNU_ATTRIBUTES, "GNU_ATTRIBUTES"},
		{SHT_GNU_HASH, "GNU_HASH"},
		{SHT_GNU_LIBLIST, "GNU_LIBLIST"},
		{SHT_CHECKSUM, "CHECKSUM"},
		{SHT_LOSUNW, "LOSUNW"},
		{SHT_SUNW_move, "SUNW_move"},
		{SHT_SUNW_COMDAT, "SUNW_COMDAT"},
		{SHT_SUNW_syminfo, "SUNW_syminfo"},
		{SHT_GNU_verdef, "VERDEF"},
		{SHT_GNU_verneed, "VERNEED"},
		{SHT_GNU_versym, "VERSYM"},
		{SHT_HISUNW, "HISUNW"},
		{SHT_HIOS, "HIOS"},
		{SHT_LOPROC, "LOPROC"}, {SHT_HIPROC, "HIPROC"},
		{SHT_LOUSER, "LOUSER"}, {SHT_HIUSER, "HIUSER"}
	};
	int types_size = sizeof(types) / sizeof(macro_matcher_t), i;

	for (i = 0; i < types_size && sh_type != (unsigned int)types[i].macro; i++)
		;
	printf("%-15s ", i == types_size ? "NULL" : types[i].macro_string);
}
