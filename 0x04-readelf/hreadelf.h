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

#define is_printable(x) ((x) >= 32 && (x) <= 126)
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


/* 0-hreadelf.c prototypes */
void print_elf64_header(int ELF_fd);
void print_elf32_header(int ELF_fd);
void print_os_abi(unsigned char os_abi);
void print_machine(uint16_t e_machine);


/* 1-hreadelf.c prototypes */
void print_elf64_section_header_table(int ELF_fd);
void print_elf32_section_header_table(int ELF_fd);
void print_sh_flags(uint32_t sh_flags);
void print_sh_type(uint32_t sh_type);

/* 2-hreadelf.c prototypes */
int print_elf64_program_header_table(int ELF_fd);
int print_elf32_program_header_table(int ELF_fd);
int print_ELF64_program_header_row(Elf64_Phdr ph);
int print_ELF32_program_header_row(Elf32_Phdr ph);

/* 2-hreadelf_helpers.c prototypes */
void print_segment64(int ELF_fd, Elf64_Addr paddr, uint64_t filesz, char *st);
void print_segment32(int ELF_fd, Elf32_Addr paddr, uint32_t filesz, char *st);
int print_p_type(uint32_t p_type);
void print_p_flags(uint32_t p_flags);

/* read_elf_header_MSB.c prototypes */
void read_elf32_header_MSB(Elf32_Ehdr *h, int fd);
void read_elf64_header_MSB(Elf64_Ehdr *h, int fd);
void read_elf64_section_MSB(Elf64_Shdr *h, int fd);
void read_elf32_section_MSB(Elf32_Shdr *h, int fd);
void read_elf64_program_MSB(Elf64_Phdr *h, int fd);
void read_elf32_program_MSB(Elf32_Phdr *h, int fd);

/* print_elf_file_type.c prototypes */
void print_elf_file_type(uint16_t e_type);

/* get_string_table64.c prototypes */
char *get_string_table64(int ELF_fd, int offset, int encoding);
char *get_string_table32(int ELF_fd, int offset, int encoding);

/* open_ELF.c prototypes */
unsigned char open_ELF(int *ELF_fd, char *ELF_name, char *program);


#endif /* HREADELF_H */
