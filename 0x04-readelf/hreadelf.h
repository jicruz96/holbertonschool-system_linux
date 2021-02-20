#ifndef HREADELF_H
#define HREADELF_H

#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define PH_SUMMARY "There are %d program headers, starting at offset %d\n\n"
#define PH_INTRO "Program Headers:"
#define PH_FIRST_ROW \
"  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align"
#define PH_FIRST_ROW32 \
"  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align"
#define IS_INTERP_ROW 1


/**
 * struct macro_matcher_s - matches a macro with a string description
 * @macro: macro
 * @macro_string: macro string
 **/
typedef struct macro_matcher_s
{
	int macro;
	char *macro_string;
} macro_matcher_t;


void print_elf64_header(int ELF_fd);
void print_elf32_header(int ELF_fd);
void print_elf64_section_header_table(int ELF_fd);
void print_elf32_section_header_table(int ELF_fd);
void print_elf64_program_header_table(int ELF_fd);
void print_elf32_program_header_table(int ELF_fd);
void print_segment64(int ELF_fd, Elf64_Addr paddr, uint64_t filesz, char *st);
void print_segment32(int ELF_fd, Elf32_Addr paddr, uint32_t filesz, char *st);
int print_ELF64_program_header_row(Elf64_Phdr ph);
int print_ELF32_program_header_row(Elf32_Phdr ph);


void print_sh_flags(uint32_t sh_flags);
void print_sh_type(uint32_t sh_type);
void print_p_flags(uint32_t p_flags);
int print_p_type(uint32_t p_type);
void print_ELF_file_type(uint16_t e_type);

void print_os_abi(unsigned char os_abi);
void print_machine(uint16_t e_machine);

char *get_string_table(int ELF_fd, int offset);
char *get_string_table32(int ELF_fd, int offset);


unsigned char open_ELF_file(int *ELF_fd, char *ELF_name, char *program);


#endif /* HREADELF_H */
