#include "hreadelf.h"

/**
 * get_string_table64 - returns alloc'd buffer of an ELF string table
 * @ELF_fd: ELF file descriptor
 * @offset: byte offset where string table begins
 * @encoding: data encoding type (either ELFDATA2MSB or ELFDATA2LSB)
 * Return: pointer to alloc'd buffer contiaining string table contents
 **/
char *get_string_table64(int ELF_fd, int offset, int encoding)
{
	char *string_table;
	Elf64_Shdr s_h;

	lseek(ELF_fd, offset, 0);
	if (encoding == ELFDATA2MSB)
		read_elf64_section_MSB(&s_h, ELF_fd);
	else
		read(ELF_fd, &s_h, sizeof(s_h));
	string_table = malloc(sizeof(char) * s_h.sh_size);
	lseek(ELF_fd, s_h.sh_offset, 0);
	read(ELF_fd, string_table, s_h.sh_size);
	lseek(ELF_fd, 0, 0);
	return (string_table);
}

/**
 * get_string_table32 - returns alloc'd buffer of an ELF string table
 * @ELF_fd: ELF file descriptor
 * @offset: byte offset where string table begins
 * @encoding: data encoding type (either ELFDATA2MSB or ELFDATA2LSB)
 * Return: pointer to alloc'd buffer contiaining string table contents
 **/
char *get_string_table32(int ELF_fd, int offset, int encoding)
{
	char *string_table;
	Elf32_Shdr s_h;

	lseek(ELF_fd, offset, 0);
	if (encoding == ELFDATA2MSB)
		read_elf32_section_MSB(&s_h, ELF_fd);
	else
		read(ELF_fd, &s_h, sizeof(s_h));
	string_table = malloc(sizeof(char) * s_h.sh_size);
	lseek(ELF_fd, s_h.sh_offset, 0);
	read(ELF_fd, string_table, s_h.sh_size);
	lseek(ELF_fd, 0, 0);
	return (string_table);
}
