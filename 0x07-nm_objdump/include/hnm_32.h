#ifndef HNM32_H
#define HNM32_H 1

#include <elf.h>

int hnm_32(char *program_name, char *file, int elf_fd, int encoding);

void print_symbol_table_32(int elf_fd, int encoding, uint32_t sh_offset,
	uint32_t symbol_table_offset, uint32_t num_rows);

void print_symbol_table_row_32(int elf_fd, int encoding,
	uint32_t sh_offset, char *string_table);

char get_symbol_type_32(int elf_fd, int encoding,
	uint32_t sh_offset, Elf32_Sym *symbol);

char get_symbol_type_from_section_32(int elf_fd, int encoding,
	uint32_t sh_offset, Elf32_Sym *symbol);

#endif /* HNM32_H */
