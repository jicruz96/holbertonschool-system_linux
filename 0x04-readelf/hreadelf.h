#ifndef HREADELF_H
#define HREADELF_H

#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


/**
 * struct macro_matcher_s
 * @macro: macro
 * @macro_string: macro string
 **/
typedef struct macro_matcher_s
{
    int macro;
    char *macro_string;
} macro_matcher_t;


void print_elf64_header(int elf_file);
void print_elf32_header(int elf_file);
void print_elf64_section_header_table(int elf_file);
void print_elf32_section_header_table(int elf_file);
void print_elf64_program_header_table(int elf_file);
void print_elf32_program_header_table(int elf_file);

void print_sh_flags(uint32_t sh_flags);
void print_sh_type(uint32_t sh_type);
void print_p_flags(uint32_t p_flags);
int print_p_type(uint32_t p_type);
void print_ELF_file_type(uint16_t e_type);

void print_row(char *column1);
void print_os_abi(unsigned char os_abi);
void print_machine(uint16_t e_machine);

char *get_string_table(int elf_file, int offset);

unsigned char open_ELF_file(int *ELF_fd, char *ELF_name, char *program);


#endif /* HREADELF_H */
