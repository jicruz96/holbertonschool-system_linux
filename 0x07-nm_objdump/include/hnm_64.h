#ifndef HNM64_H
#define HNM64_H 1

#include <elf.h>

int hnm_64(char *program_name, char *file, int elf_fd, int encoding);

void print_symbol_table_64(int elf_fd, int encoding, uint64_t sh_offset,
	uint64_t symbol_table_offset, uint64_t num_rows);

void print_symbol_table_row_64(int elf_fd, int encoding,
	uint64_t sh_offset, char *string_table);

char get_symbol_type_64(int elf_fd, int encoding,
	uint64_t sh_offset, Elf64_Sym *symbol);

char get_symbol_type_from_section_64(int elf_fd, int encoding,
	uint64_t sh_offset, Elf64_Sym *symbol);

#endif /* HNM64_H */
