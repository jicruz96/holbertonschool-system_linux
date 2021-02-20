#include "hreadelf.h"

/**
 * open_ELF_file - opens ELF file
 * @ELF_fd: pointer where ELF file descriptor will be stored
 * @ELF_name: ELF file name
 * @program: program attempting to open ELF file
 * Return: ELF file class
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
		fprintf(stderr, "%s: Error: %s: Failed to read file's magic number\n",
				program, ELF_name);
		close(*ELF_fd);
		exit(EXIT_FAILURE);
	}

	if (memcmp(ELFMAG, elf_identity, SELFMAG))
	{
		dprintf(STDERR_FILENO, "%s: Error: Not an ELF file -", program);
		dprintf(STDERR_FILENO, " it has the wrong magic bytes at the start\n");
		close(*ELF_fd);
		exit(EXIT_FAILURE);
	}

	lseek(*ELF_fd, 0, 0);
	return (elf_identity[EI_CLASS]);

}
