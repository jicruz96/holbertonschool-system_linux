#ifndef BSWAP32_H
#define BSWAP32_H 1

#include <elf.h>

void bswap_Elf32_Ehdr(Elf32_Ehdr *h);
void bswap_Elf32_Shdr(Elf32_Shdr *h);
void bswap_Elf32_Sym(Elf32_Sym *h);
void bswap_string(char *str, int size);

#endif /* BSWAP32_H */
