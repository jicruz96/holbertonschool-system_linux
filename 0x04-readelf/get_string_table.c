#include "hreadelf.h"

/**
 * get_string_table - returns alloc'd buffer of an ELF string table
 * @ELF_fd: ELF file descriptor
 * @offset: byte offset where string table begins
 * Return: pointer to alloc'd buffer contiaining string table contents
 **/
char *get_string_table(int ELF_fd, int offset)
{
	char *string_table;
	Elf64_Shdr s_h;

	lseek(ELF_fd, offset, 0);
	read(ELF_fd, &s_h, sizeof(s_h));
	string_table = malloc(sizeof(char) * s_h.sh_size);
	lseek(ELF_fd, s_h.sh_offset, 0);
	read(ELF_fd, string_table, s_h.sh_size);
	lseek(ELF_fd, 0, 0);
	return (string_table);
}
