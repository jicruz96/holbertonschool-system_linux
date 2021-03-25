#ifndef HOBJDUMP64_H
#define HOBJDUMP64_H 1

#include <elf.h>

int hobjdump_64(char *file, int elf_fd);
void print_section_stuff_64(int elf_fd, Elf64_Ehdr *Ehdr, char *string_table);
void print_flags_64(int elf_fd, Elf64_Ehdr *elf_header);




#endif /* HOBJDUMP64_H */
