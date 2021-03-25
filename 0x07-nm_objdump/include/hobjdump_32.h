#ifndef HOBJDUMP32_H
#define HOBJDUMP32_H 1

#include <elf.h>

int hobjdump_32(char *file, int elf_fd);
void print_section_stuff_32(int elf_fd, Elf32_Ehdr *Ehdr, char *string_table);
void print_flags_32(int elf_fd, Elf32_Ehdr *elf_header);




#endif /* HOBJDUMP32_H */
