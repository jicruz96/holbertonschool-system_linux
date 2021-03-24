#include <elf.h>


/**
 * bswap_Elf32_Ehdr - MSB to LSB encoding for Elf32_Ehdr
 * @h: header
 **/
void bswap_Elf32_Ehdr(Elf32_Ehdr *h)
{
	h->e_type       = __builtin_bswap16(h->e_type);
	h->e_machine    = __builtin_bswap16(h->e_machine);
	h->e_version    = __builtin_bswap32(h->e_version);
	h->e_entry      = __builtin_bswap32(h->e_entry);
	h->e_phoff      = __builtin_bswap32(h->e_phoff);
	h->e_shoff      = __builtin_bswap32(h->e_shoff);
	h->e_flags      = __builtin_bswap32(h->e_flags);
	h->e_ehsize     = __builtin_bswap16(h->e_ehsize);
	h->e_phentsize  = __builtin_bswap16(h->e_phentsize);
	h->e_phnum      = __builtin_bswap16(h->e_phnum);
	h->e_shentsize  = __builtin_bswap16(h->e_shentsize);
	h->e_shnum      = __builtin_bswap16(h->e_shnum);
	h->e_shstrndx   = __builtin_bswap16(h->e_shstrndx);
}

/**
 * bswap_Elf32_Shdr - MSB to LSB encoding for Elf32_Shdr
 * @h: header
 **/
void bswap_Elf32_Shdr(Elf32_Shdr *h)
{
	h->sh_name      = __builtin_bswap32(h->sh_name);
	h->sh_type      = __builtin_bswap32(h->sh_type);
	h->sh_flags     = __builtin_bswap32(h->sh_flags);
	h->sh_addr      = __builtin_bswap32(h->sh_addr);
	h->sh_offset    = __builtin_bswap32(h->sh_offset);
	h->sh_size      = __builtin_bswap32(h->sh_size);
	h->sh_link      = __builtin_bswap32(h->sh_link);
	h->sh_info      = __builtin_bswap32(h->sh_info);
	h->sh_addralign = __builtin_bswap32(h->sh_addralign);
	h->sh_entsize   = __builtin_bswap32(h->sh_entsize);
}

/**
 * bswap_Elf32_Sym - MSB to LSB encoding for Elf32_Sym
 * @h: header
 **/
void bswap_Elf32_Sym(Elf32_Sym *h)
{
	h->st_name  = __builtin_bswap32(h->st_name);
	h->st_value = __builtin_bswap32(h->st_value);
	h->st_size  = __builtin_bswap32(h->st_size);
	h->st_shndx = __builtin_bswap16(h->st_shndx);
}
