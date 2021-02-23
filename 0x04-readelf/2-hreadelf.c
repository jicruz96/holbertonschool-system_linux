#include "hreadelf.h"
#define strindex(h) (h.e_shoff + (h.e_shentsize * h.e_shstrndx))

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
		if (open_ELF(&ELF_fd, argv[1], "readelf") == ELFCLASS64)
			print_elf64_program_header_table(ELF_fd);
		else
			print_elf32_program_header_table(ELF_fd);
	}

	return (EXIT_SUCCESS);
}

/**
 * print_elf64_program_header_table - prints ELF64 section header table
 * @ELF_fd: ELF file descriptor
 * Return: irrelevant
 **/
int print_elf64_program_header_table(int ELF_fd)
{
	Elf64_Ehdr h;
	Elf64_Phdr ph;
	unsigned int i, j;
	char *st, tmp[512];
	uint64_t paddrs[5125], fileszs[512];

	read(ELF_fd, &h, sizeof(h));
	if (h.e_ident[EI_DATA] == ELFDATA2MSB)
		lseek(ELF_fd, 0, 0), read_elf64_header_MSB(&h, ELF_fd);
	if (h.e_phnum == 0)
		return (printf("\nThere are no program headers in this file.\n"));
	printf("\nElf file type is "), print_elf_file_type(h.e_type);
	printf("Entry point 0x%x\n", (unsigned int)h.e_entry);
	printf(PH_SUMMARY, h.e_phnum, (int)h.e_phoff), puts(PH_INTRO);
	puts(PH_FIRST_ROW), lseek(ELF_fd, h.e_phoff, 0);
	for (i = 0; i < h.e_phnum; i++)
	{
		if (h.e_ident[EI_DATA] == ELFDATA2MSB)
			read_elf64_program_MSB(&ph, ELF_fd);
		else
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
	st = get_string_table64(ELF_fd, strindex(h), h.e_ident[EI_DATA]);
	for (i = 0, j = 0; i < h.e_phnum; printf("\n"), i++)
	{
		lseek(ELF_fd, h.e_shoff, 0), printf("   %02d     ", i);
		for (j = 0; j < h.e_shnum; j++)
			print_segment64(ELF_fd, paddrs[i], fileszs[i], st);
	}
	free(st);
	return (0);
}


/**
 * print_elf32_program_header_table - prints ELF32 section header table
 * @ELF_fd: ELF file descriptor
 * Return: irrelevant
 **/
int print_elf32_program_header_table(int ELF_fd)
{
	Elf32_Ehdr h;
	Elf32_Phdr ph;
	unsigned int i, j;
	char *st, tmp[512];
	uint32_t paddrs[512], fileszs[512];

	read(ELF_fd, &h, sizeof(h));
	if (h.e_ident[EI_DATA] == ELFDATA2MSB)
		lseek(ELF_fd, 0, 0), read_elf32_header_MSB(&h, ELF_fd);
	if (h.e_phnum == 0)
		return (printf("\nThere are no program headers in this file.\n"));
	printf("\nElf file type is "), print_elf_file_type(h.e_type);
	printf("Entry point 0x%x\n", (unsigned int)h.e_entry);
	printf(PH_SUMMARY, h.e_phnum, (int)h.e_phoff);
	puts(PH_INTRO), puts(PH_FIRST_ROW32), lseek(ELF_fd, h.e_phoff, 0);
	for (i = 0; i < h.e_phnum; i++)
	{
		if (h.e_ident[EI_DATA] == ELFDATA2MSB)
			read_elf32_program_MSB(&ph, ELF_fd);
		else
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
	st = get_string_table32(ELF_fd, strindex(h), h.e_ident[EI_DATA]);
	for (i = 0, j = 0; i < h.e_phnum; printf("\n"), i++)
	{
		lseek(ELF_fd, h.e_shoff, 0), printf("   %02d     ", i);
		for (j = 0; j < h.e_shnum; j++)
			print_segment64(ELF_fd, paddrs[i], fileszs[i], st);
	}
	free(st);
	return (0);
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
