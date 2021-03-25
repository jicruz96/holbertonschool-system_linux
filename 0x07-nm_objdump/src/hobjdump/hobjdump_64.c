#include <elf.h>
#include <stdio.h>
#include <bswap_64.h>
#include <unistd.h>
#include <hobjdump_64.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

/* BFD contains relocation entries.  */
#define HAS_RELOC                   0x1

/* BFD is directly executable.  */
#define EXEC_P                      0x2

/* BFD has symbols.  */
#define HAS_SYMS                   0x10

/* BFD is a dynamic object.  */
#define DYNAMIC                    0x40

#define D_PAGED                   0x100


#define comma ", "
/**
 * print_flags_64 - prints flags
 * @elf_fd: elf file descriptor
 * @elf_header: elf header
 **/
void print_flags_64(int elf_fd, Elf64_Ehdr *elf_header)
{
	char *delim = "";
	uint16_t i;
	uint32_t flags = 0;
	Elf64_Shdr blah;

	if (elf_header->e_type == ET_REL)
		flags |= HAS_RELOC;
	if (elf_header->e_type == ET_EXEC)
		flags |= EXEC_P;
	lseek(elf_fd, elf_header->e_shoff, SEEK_SET);
	for (i = 0; i < elf_header->e_shnum; i++)
	{
		read(elf_fd, &blah, sizeof(blah));
		if (elf_header->e_ident[EI_DATA] == ELFDATA2MSB)
			bswap_Elf64_Shdr(&blah);
		if (blah.sh_type == SHT_SYMTAB || blah.sh_type == SHT_DYNSYM)
		{
			flags |= HAS_SYMS;
			lseek(elf_fd, elf_header->e_ehsize, SEEK_SET);
			break;
		}
	}
	if (elf_header->e_type == ET_DYN)
		flags |= DYNAMIC;
	if (elf_header->e_type != ET_REL)
		flags |= D_PAGED;
	printf("flags 0x%08x:\n", flags);
	if (flags & HAS_RELOC)
		printf("HAS_RELOC"), delim = comma;
	if (flags & EXEC_P)
		printf("%sEXEC_P", delim), delim = comma;
	if (flags & HAS_SYMS)
		printf("%sHAS_SYMS", delim), delim = comma;
	if (flags & DYNAMIC)
		printf("%sDYNAMIC", delim), delim = comma;
	if (flags & D_PAGED)
		printf("%sD_PAGED", delim);
	putchar('\n');
}


#define NOPE(Shdr, Ehdr, i) (Shdr.sh_size == 0 ||\
(Shdr.sh_addr == 0 && Shdr.sh_type != SHT_PROGBITS) ||\
Shdr.sh_type == SHT_SYMTAB || Shdr.sh_type == SHT_NOBITS ||\
(Shdr.sh_type == SHT_STRTAB && i >= Ehdr->e_shstrndx) ||\
Shdr.sh_type == SHT_NULL)
/**
 * print_section_stuff_64 - prints the section contents
 * @elf_fd: ELF file descriptor
 * @Ehdr: ELF header of ELF file
 * @string_table: string table
 **/
void print_section_stuff_64(int elf_fd, Elf64_Ehdr *Ehdr, char *string_table)
{
	unsigned char contents[16];
	uint16_t i, k, l, addr_size;
	uint64_t j;
	Elf64_Shdr Shdr;

	for (i = 0; i < Ehdr->e_shnum; i++)
	{
		lseek(elf_fd, Ehdr->e_shoff + (i * sizeof(Shdr)), SEEK_SET);
		read(elf_fd, &Shdr, sizeof(Shdr));
		if (Ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
			bswap_Elf64_Shdr(&Shdr);
		if (NOPE(Shdr, Ehdr, i))
			continue;
		lseek(elf_fd, Shdr.sh_offset, SEEK_SET);
		printf("Contents of section %s:\n", string_table + Shdr.sh_name);
		for (addr_size = 0, j = Shdr.sh_size + Shdr.sh_addr; j; j /= 16)
			addr_size++;
		if (addr_size < 4)
			addr_size = 4;
		for (j = 0; j < Shdr.sh_size; j += 16)
		{
			printf(" %0*lx ", addr_size, Shdr.sh_addr + j);
			read(elf_fd, contents, sizeof(contents));
			for (k = 0, l = 0; k < 16 && j + k < Shdr.sh_size; k++)
			{
				if (k && k % 4 == 0)
					putchar(' '), l++;
				printf("%02x", contents[k]), l += 2;
			}
			printf("%*s", 37 - l, "");
			for (k = 0; k < 16 && j + k < Shdr.sh_size; k++)
				if (contents[k] < 32 || contents[k] > 126)
					putchar('.');
				else
					putchar(contents[k]);
			for (; k < 16; k++)
				putchar(' ');
			putchar('\n');
		}
	}
}


/**
 * hobjdump_64 - hobjdump 64-bit case
 * @file: file to dump
 * @elf_fd: file descriptor
 * Return: 0
 **/
int hobjdump_64(char *file, int elf_fd)
{
	Elf64_Ehdr Ehdr;
	Elf64_Shdr Shdr;
	char *string_table;

	read(elf_fd, &Ehdr, sizeof(Ehdr));
	if (Ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
		bswap_Elf64_Ehdr(&Ehdr);
	printf("\n%s:     ", file);
	printf("file format elf64-x86-64\n");
	printf("architecture: i386:x86-64, ");
	print_flags_64(elf_fd, &Ehdr);
	printf("start address 0x%016lx\n\n", Ehdr.e_entry);

	/* Get string table */
	lseek(elf_fd, Ehdr.e_shoff + (Ehdr.e_shstrndx * sizeof(Shdr)), SEEK_SET);
	read(elf_fd, &Shdr, sizeof(Shdr));
	if (Ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
		bswap_Elf64_Shdr(&Shdr);
	lseek(elf_fd, Shdr.sh_offset, SEEK_SET);
	string_table = malloc(Shdr.sh_size);
	read(elf_fd, string_table, Shdr.sh_size);
	if (Ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
		bswap_string(string_table, Shdr.sh_size);
	print_section_stuff_64(elf_fd, &Ehdr, string_table);
	return (0);
}
