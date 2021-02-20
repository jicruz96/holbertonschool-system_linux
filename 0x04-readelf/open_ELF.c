#include "hreadelf.h"

/**
 * open_ELf_file - opens ELF file
 * @ELF_fd: pointer where ELF file descriptor will be stored
 * @ElF_name: ELF file name
 * @program: program attempting to open ELF file
 * Return: 
 **/
unsigned char open_ELF_file(int *ELF_fd, char *ELF_name, char *program)
{
    unsigned char elf_identity[EI_NIDENT];

    *ELF_fd = open(ELF_name, O_RDONLY);
    if (*ELF_fd <= 0)
    {
        fprintf(stderr, "%s: Error: '%s': No such file\n", program, ELF_name);
        exit(EXIT_FAILURE);
    }

    if (read(*ELF_fd, &elf_identity, EI_NIDENT) < EI_NIDENT)
    {
        fprintf(stderr, "%s: Error: %s: Failed to read file's magic number\n", program, ELF_name);
        close(*ELF_fd);
        exit(EXIT_FAILURE);
    }

    if (memcmp(ELFMAG, elf_identity, SELFMAG))
    {
        fprintf(stderr, "%s: Error: Not an ELF file - it has the wrong magic bytes at the start\n", program);
        close(*ELF_fd);
        exit(EXIT_FAILURE);
    }

    lseek(*ELF_fd, 0, 0);
    return (elf_identity[EI_CLASS]);

}
