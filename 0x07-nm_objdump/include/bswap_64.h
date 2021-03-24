#ifndef BSWAP32_H
#define BSWAP32_H 1

#include <elf.h>

void bswap_Elf64_Ehdr(Elf64_Ehdr *h);
void bswap_Elf64_Shdr(Elf64_Shdr *h);
void bswap_Elf64_Sym(Elf64_Sym *h);
void bswap_string(char *str, int size);

#endif /* BSWAP32_H */
