#include <elf.h>
#include <hnm_32.h>
#include <bswap_32.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


#define ADDR_SIZE (32 / 4)

/**
 * hnm_32 - hnm 32
 * @program_name: program name
 * @file: file name
 * @elf_fd: ELF file descriptor
 * @encoding: file encoding
 * Return: 1 if no symbols found | 0 on success
 **/
int hnm_32(char *program_name, char *file, int elf_fd, int encoding)
{
	uint16_t i;
	Elf32_Ehdr h;
	Elf32_Shdr section;
	uint32_t symbol_table_offset = 0, num_rows = 0;

	/* Read ELF file header */
	lseek(elf_fd, 0, SEEK_SET);
	read(elf_fd, &h, sizeof(h));
	if (encoding == ELFDATA2MSB)
		bswap_Elf32_Ehdr(&h);

	/* Go to beginning of section header segment */
	lseek(elf_fd, h.e_shoff, SEEK_SET);

	/* Read section headers until SYMbol TABle section header found */
	for (i = 0; i < h.e_shnum; i++)
	{
		read(elf_fd, &section, sizeof(section));
		if (encoding == ELFDATA2MSB)
			bswap_Elf32_Shdr(&section);

		if (section.sh_type == SHT_SYMTAB)
		{
			/* We'll skip the first symbol, which is always empty */
			symbol_table_offset = section.sh_offset + sizeof(Elf32_Sym);
			num_rows = (section.sh_size / sizeof(Elf32_Sym)) - 1;
			break;
		}
	}

	/* Print error message and return if no symbols in ELF file */
	if (num_rows == 0)
	{
		fprintf(stderr, "%s: %s: no symbols\n", program_name, file);
		return (1);
	}

	print_symbol_table_32(elf_fd, encoding,
		h.e_shoff, symbol_table_offset, num_rows);
	return (0);
}


/**
 * print_symbol_table_32 - prints a symbol table for a 32-bit ELF file
 * @elf_fd: ELF file descriptor
 * @encoding: ELF identity details
 * @sh_offset: section headers section offset from start of file
 * @symbol_table_offset: symbol table offset from start of ELF file
 * @num_rows: number of rows in symbol table
 **/
void print_symbol_table_32(int elf_fd, int encoding, uint32_t sh_offset,
	uint32_t symbol_table_offset, uint32_t num_rows)
{
	Elf32_Shdr string_table_Shdr;
	char *string_table;
	unsigned int i;


	/* Read symbol string table *section header* (to find location) */
	read(elf_fd, &string_table_Shdr, sizeof(string_table_Shdr));
	if (encoding == ELFDATA2MSB)
		bswap_Elf32_Shdr(&string_table_Shdr);

	/* Allocate memory for string table */
	string_table = malloc(string_table_Shdr.sh_size);

	/* Find and read symbol string table */
	lseek(elf_fd, string_table_Shdr.sh_offset, SEEK_SET);
	read(elf_fd, string_table, string_table_Shdr.sh_size);
	if (encoding == ELFDATA2MSB)
		bswap_string(string_table, string_table_Shdr.sh_size);

	/* Go to symbol table */
	lseek(elf_fd, symbol_table_offset, SEEK_SET);

	/* Print contents */
	for (i = 0; i < num_rows; i++)
		print_symbol_table_row_32(elf_fd, encoding,
			sh_offset, string_table);

	/* Free string table */
	free(string_table);
}

/**
 * print_symbol_table_row_32 - prints a row of a 32-bit ELF symbol table
 * @elf_fd: ELF file descriptor
 * @encoding: file encoding
 * @sh_offset: section header section offset from start of ELF file
 * @string_table: symbol string table
 **/
void print_symbol_table_row_32(int elf_fd, int encoding,
	uint32_t sh_offset, char *string_table)
{
	Elf32_Sym symbol;

	/* Get symbol struct info */
	read(elf_fd, &symbol, sizeof(symbol));
	if (encoding == ELFDATA2MSB)
		bswap_Elf32_Sym(&symbol);

	/* We don't care about symbols describing SECTION or FILEs */
	if (ELF32_ST_TYPE(symbol.st_info) == STT_SECTION ||
		ELF32_ST_TYPE(symbol.st_info) == STT_FILE)
		return;

	/* Print symbol location offset (from .text section), given by st_value */
	if (symbol.st_shndx == SHN_UNDEF)
		printf("%*s ", ADDR_SIZE, "");
	else
		printf("%0*x ", ADDR_SIZE, symbol.st_value);

	/* Print symbol type code  */
	printf("%c ", get_symbol_type_32(elf_fd, encoding, sh_offset, &symbol));

	/* Print symbol name */
	puts(string_table + symbol.st_name);
}

/**
 * get_symbol_type_32 - returns letter representing a symbol's type
 * @elf_fd: ELF file descriptor
 * @encoding: file encoding
 * @sh_offset: section header segment offset from start of ELF file
 * @symbol: pointer to symbol struct
 * Return: type code
 **/
char get_symbol_type_32(int elf_fd, int encoding,
	uint32_t sh_offset, Elf32_Sym *symbol)
{
	/* Absolute symbol -> 'A' */
	if (symbol->st_shndx == SHN_ABS)
		return ('A');

	/* Common symbol -> 'C' */
	if (symbol->st_shndx == SHN_COMMON)
		return ('C');

	if (symbol->st_shndx == SHN_UNDEF)
	{
		/* Weak object, undef -> 'v' | Weak symbol, undef -> 'w' */
		if (ELF32_ST_BIND(symbol->st_info) == STB_WEAK)
			return (ELF32_ST_TYPE(symbol->st_info) == STT_OBJECT ? 'v' : 'w');

		/* undefined symbol -> 'U' */
		return ('U');
	}

	/* Weak object -> 'V' | Weak symbol -> 'w' */
	if (ELF32_ST_BIND(symbol->st_info) == STB_WEAK)
		return (ELF32_ST_TYPE(symbol->st_info) == STT_OBJECT ? 'V' : 'W');

	/* If type code not found, we must check info in its related section */
	return (get_symbol_type_from_section_32(elf_fd, encoding, sh_offset, symbol));
}

/**
 * get_symbol_type_from_section_32 - finds section referred by a symbol and
 * uses section info to return a letter representing a symbol's type
 * @elf_fd: ELF file descriptor
 * @encoding: file encoding
 * @sh_offset: section header segment offset from start of ELF file
 * @symbol: pointer to symbol struct
 * Return: type code
 **/
char get_symbol_type_from_section_32(
	int elf_fd, int encoding, uint32_t sh_offset, Elf32_Sym *symbol)
{
	off_t previous_offset = lseek(elf_fd, 0, SEEK_CUR); /* save file offset */
	Elf32_Shdr section;

	/* find and read related section */
	lseek(elf_fd, sh_offset + (sizeof(section) * symbol->st_shndx), SEEK_SET);
	read(elf_fd, &section, sizeof(section));
	if (encoding == ELFDATA2MSB)
		bswap_Elf32_Shdr(&section);

	/* reset to previous file offset */
	lseek(elf_fd, previous_offset, SEEK_SET);

	/**
	 * Note:
	 * For all cases, we'll return lower case letter for a LOCAL symbol and
	 * upper case for a GLOBAL symbol
	 **/

	/* If section is an EXECution INSTRuction or an ARRAY, it's text */
	if (section.sh_flags & SHF_EXECINSTR ||
		section.sh_type == SHT_INIT_ARRAY ||
		section.sh_type == SHT_FINI_ARRAY)
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL ? 't' : 'T');


	if (section.sh_flags & SHF_WRITE)
	{
		/* If section is writable with no bits, it's a .bss data segment */
		if (section.sh_type == SHT_NOBITS)
			return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL ? 'b' : 'B');

		/* Otherwise, it's regular data */
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL ? 'd' : 'D');
	}

	/* If ALLOCated but with no write permissions, it's a read-only segment */
	if (section.sh_flags & SHF_ALLOC)
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL ? 'r' : 'R');

	return ('?'); /* If we made it here, we have no idea what it is */
}
