#include "hreadelf.h"


/**
 * read_elf64_header_MSB - fills in an Elf64 header using MSB encoding
 * @h: pointer to header struct
 * @fd: file descriptor
 **/
void read_elf64_header_MSB(Elf64_Ehdr *h, int fd)
{
	unsigned char i, j;

	read(fd, &(h->e_ident), sizeof(h->e_ident));

	for (j = 0; j < sizeof(h->e_type)     ; j++)
		read(fd, &i, 1), h->e_type      = (h->e_type      << 8) + i;
	for (j = 0; j < sizeof(h->e_machine)  ; j++)
		read(fd, &i, 1), h->e_machine   = (h->e_machine   << 8) + i;
	for (j = 0; j < sizeof(h->e_version)  ; j++)
		read(fd, &i, 1), h->e_version   = (h->e_version   << 8) + i;
	for (j = 0; j < sizeof(h->e_entry)    ; j++)
		read(fd, &i, 1), h->e_entry     = (h->e_entry     << 8) + i;
	for (j = 0; j < sizeof(h->e_phoff)    ; j++)
		read(fd, &i, 1), h->e_phoff     = (h->e_phoff     << 8) + i;
	for (j = 0; j < sizeof(h->e_shoff)    ; j++)
		read(fd, &i, 1), h->e_shoff     = (h->e_shoff     << 8) + i;
	for (j = 0; j < sizeof(h->e_flags)    ; j++)
		read(fd, &i, 1), h->e_flags     = (h->e_flags     << 8) + i;
	for (j = 0; j < sizeof(h->e_ehsize)   ; j++)
		read(fd, &i, 1), h->e_ehsize    = (h->e_ehsize    << 8) + i;
	for (j = 0; j < sizeof(h->e_phentsize); j++)
		read(fd, &i, 1), h->e_phentsize = (h->e_phentsize << 8) + i;
	for (j = 0; j < sizeof(h->e_phnum)    ; j++)
		read(fd, &i, 1), h->e_phnum     = (h->e_phnum     << 8) + i;
	for (j = 0; j < sizeof(h->e_shentsize); j++)
		read(fd, &i, 1), h->e_shentsize = (h->e_shentsize << 8) + i;
	for (j = 0; j < sizeof(h->e_shnum)    ; j++)
		read(fd, &i, 1), h->e_shnum     = (h->e_shnum     << 8) + i;
	for (j = 0; j < sizeof(h->e_shstrndx) ; j++)
		read(fd, &i, 1), h->e_shstrndx  = (h->e_shstrndx  << 8) + i;
}

/**
 * read_elf32_header_MSB - fills in an Elf32 header using MSB encoding
 * @h: pointer to header struct
 * @fd: file descriptor
 **/
void read_elf32_header_MSB(Elf32_Ehdr *h, int fd)
{
	unsigned char i, j;

	read(fd, &(h->e_ident), sizeof(h->e_ident));

	for (j = 0; j < sizeof(h->e_type)     ; j++)
		read(fd, &i, 1), h->e_type      = (h->e_type      << 8) + i;
	for (j = 0; j < sizeof(h->e_machine)  ; j++)
		read(fd, &i, 1), h->e_machine   = (h->e_machine   << 8) + i;
	for (j = 0; j < sizeof(h->e_version)  ; j++)
		read(fd, &i, 1), h->e_version   = (h->e_version   << 8) + i;
	for (j = 0; j < sizeof(h->e_entry)    ; j++)
		read(fd, &i, 1), h->e_entry     = (h->e_entry     << 8) + i;
	for (j = 0; j < sizeof(h->e_phoff)    ; j++)
		read(fd, &i, 1), h->e_phoff     = (h->e_phoff     << 8) + i;
	for (j = 0; j < sizeof(h->e_shoff)    ; j++)
		read(fd, &i, 1), h->e_shoff     = (h->e_shoff     << 8) + i;
	for (j = 0; j < sizeof(h->e_flags)    ; j++)
		read(fd, &i, 1), h->e_flags     = (h->e_flags     << 8) + i;
	for (j = 0; j < sizeof(h->e_ehsize)   ; j++)
		read(fd, &i, 1), h->e_ehsize    = (h->e_ehsize    << 8) + i;
	for (j = 0; j < sizeof(h->e_phentsize); j++)
		read(fd, &i, 1), h->e_phentsize = (h->e_phentsize << 8) + i;
	for (j = 0; j < sizeof(h->e_phnum)    ; j++)
		read(fd, &i, 1), h->e_phnum     = (h->e_phnum     << 8) + i;
	for (j = 0; j < sizeof(h->e_shentsize); j++)
		read(fd, &i, 1), h->e_shentsize = (h->e_shentsize << 8) + i;
	for (j = 0; j < sizeof(h->e_shnum)    ; j++)
		read(fd, &i, 1), h->e_shnum     = (h->e_shnum     << 8) + i;
	for (j = 0; j < sizeof(h->e_shstrndx) ; j++)
		read(fd, &i, 1), h->e_shstrndx  = (h->e_shstrndx  << 8) + i;
}


/**
 * read_elf32_section_MSB - fills in an Elf32 header using MSB encoding
 * @h: pointer to header struct
 * @fd: file descriptor
 **/
void read_elf32_section_MSB(Elf32_Shdr *h, int fd)
{
	unsigned char i, j;

	for (j = 0; j < sizeof(h->sh_name)     ; j++)
		read(fd, &i, 1), h->sh_name        = (h->sh_name      << 8) + i;
	for (j = 0; j < sizeof(h->sh_type)     ; j++)
		read(fd, &i, 1), h->sh_type        = (h->sh_type      << 8) + i;
	for (j = 0; j < sizeof(h->sh_flags)    ; j++)
		read(fd, &i, 1), h->sh_flags       = (h->sh_flags     << 8) + i;
	for (j = 0; j < sizeof(h->sh_addr)     ; j++)
		read(fd, &i, 1), h->sh_addr        = (h->sh_addr      << 8) + i;
	for (j = 0; j < sizeof(h->sh_offset)   ; j++)
		read(fd, &i, 1), h->sh_offset      = (h->sh_offset    << 8) + i;
	for (j = 0; j < sizeof(h->sh_size)     ; j++)
		read(fd, &i, 1), h->sh_size        = (h->sh_size      << 8) + i;
	for (j = 0; j < sizeof(h->sh_link)     ; j++)
		read(fd, &i, 1), h->sh_link        = (h->sh_link      << 8) + i;
	for (j = 0; j < sizeof(h->sh_info)     ; j++)
		read(fd, &i, 1), h->sh_info        = (h->sh_info      << 8) + i;
	for (j = 0; j < sizeof(h->sh_addralign); j++)
		read(fd, &i, 1), h->sh_addralign   = (h->sh_addralign << 8) + i;
	for (j = 0; j < sizeof(h->sh_entsize)  ; j++)
		read(fd, &i, 1), h->sh_entsize     = (h->sh_entsize   << 8) + i;
}

/**
 * read_elf64_section_MSB - fills in an Elf32 header using MSB encoding
 * @h: pointer to header struct
 * @fd: file descriptor
 **/
void read_elf64_section_MSB(Elf64_Shdr *h, int fd)
{
	unsigned char i, j;

	for (j = 0; j < sizeof(h->sh_name)     ; j++)
		read(fd, &i, 1), h->sh_name        = (h->sh_name      << 8) + i;
	for (j = 0; j < sizeof(h->sh_type)     ; j++)
		read(fd, &i, 1), h->sh_type        = (h->sh_type      << 8) + i;
	for (j = 0; j < sizeof(h->sh_flags)    ; j++)
		read(fd, &i, 1), h->sh_flags       = (h->sh_flags     << 8) + i;
	for (j = 0; j < sizeof(h->sh_addr)     ; j++)
		read(fd, &i, 1), h->sh_addr        = (h->sh_addr      << 8) + i;
	for (j = 0; j < sizeof(h->sh_offset)   ; j++)
		read(fd, &i, 1), h->sh_offset      = (h->sh_offset    << 8) + i;
	for (j = 0; j < sizeof(h->sh_size)     ; j++)
		read(fd, &i, 1), h->sh_size        = (h->sh_size      << 8) + i;
	for (j = 0; j < sizeof(h->sh_link)     ; j++)
		read(fd, &i, 1), h->sh_link        = (h->sh_link      << 8) + i;
	for (j = 0; j < sizeof(h->sh_info)     ; j++)
		read(fd, &i, 1), h->sh_info        = (h->sh_info      << 8) + i;
	for (j = 0; j < sizeof(h->sh_addralign); j++)
		read(fd, &i, 1), h->sh_addralign   = (h->sh_addralign << 8) + i;
	for (j = 0; j < sizeof(h->sh_entsize)  ; j++)
		read(fd, &i, 1), h->sh_entsize     = (h->sh_entsize   << 8) + i;
}
