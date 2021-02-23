#include "hreadelf.h"

/**
 * read_elf32_program_MSB - fills in an Elf32 header using MSB encoding
 * @h: pointer to header struct
 * @fd: file descriptor
 **/
void read_elf32_program_MSB(Elf32_Phdr *h, int fd)
{
	unsigned char i, j;

	for (j = 0; j < sizeof(h->p_type); j++)
		read(fd, &i, 1), h->p_type   = (h->p_type   << 8) + i;

	for (j = 0; j < sizeof(h->p_offset); j++)
		read(fd, &i, 1), h->p_offset = (h->p_offset << 8) + i;

	for (j = 0; j < sizeof(h->p_vaddr) ; j++)
		read(fd, &i, 1), h->p_vaddr  = (h->p_vaddr  << 8) + i;

	for (j = 0; j < sizeof(h->p_paddr) ; j++)
		read(fd, &i, 1), h->p_paddr  = (h->p_paddr  << 8) + i;

	for (j = 0; j < sizeof(h->p_filesz); j++)
		read(fd, &i, 1), h->p_filesz = (h->p_filesz << 8) + i;

	for (j = 0; j < sizeof(h->p_memsz) ; j++)
		read(fd, &i, 1), h->p_memsz  = (h->p_memsz  << 8) + i;

	for (j = 0; j < sizeof(h->p_flags) ; j++)
		read(fd, &i, 1), h->p_flags  = (h->p_flags  << 8) + i;

	for (j = 0; j < sizeof(h->p_align) ; j++)
		read(fd, &i, 1), h->p_align  = (h->p_align  << 8) + i;

}

/**
 * read_elf64_program_MSB - fills in an Elf32 header using MSB encoding
 * @h: pointer to header struct
 * @fd: file descriptor
 **/
void read_elf64_program_MSB(Elf64_Phdr *h, int fd)
{
	unsigned char i, j;

	for (j = 0; j < sizeof(h->p_type)  ; j++)
		read(fd, &i, 1), h->p_type   = (h->p_type   << 8) + i;

	for (j = 0; j < sizeof(h->p_offset); j++)
		read(fd, &i, 1), h->p_offset = (h->p_offset << 8) + i;

	for (j = 0; j < sizeof(h->p_vaddr) ; j++)
		read(fd, &i, 1), h->p_vaddr  = (h->p_vaddr     << 8) + i;

	for (j = 0; j < sizeof(h->p_paddr) ; j++)
		read(fd, &i, 1), h->p_paddr  = (h->p_paddr     << 8) + i;

	for (j = 0; j < sizeof(h->p_filesz); j++)
		read(fd, &i, 1), h->p_filesz = (h->p_filesz    << 8) + i;

	for (j = 0; j < sizeof(h->p_memsz) ; j++)
		read(fd, &i, 1), h->p_memsz  = (h->p_memsz     << 8) + i;

	for (j = 0; j < sizeof(h->p_flags) ; j++)
		read(fd, &i, 1), h->p_flags  = (h->p_flags     << 8) + i;

	for (j = 0; j < sizeof(h->p_align) ; j++)
		read(fd, &i, 1), h->p_align  = (h->p_align     << 8) + i;

}
