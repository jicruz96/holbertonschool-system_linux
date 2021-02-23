#include "hreadelf.h"

/**
 * open_ELF - opens ELF file
 * @ELF_fd: pointer where ELF file descriptor will be stored
 * @ELF_name: ELF file name
 * @program: program attempting to open ELF file
 * Return: ELF file class
 **/
unsigned char open_ELF(int *ELF_fd, char *ELF_name, char *program)
{
	unsigned char elf_identity[EI_NIDENT];
	Elf32_Ehdr h32;
	Elf64_Ehdr h64;

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
		close(*ELF_fd), exit(EXIT_FAILURE);
	}
	if (memcmp(ELFMAG, elf_identity, SELFMAG))
	{
		fprintf(stderr, "%s: Error: Not an ELF file -", program);
		fprintf(stderr, " it has the wrong magic bytes at the start\n");
		close(*ELF_fd), exit(EXIT_FAILURE);
	}
	lseek(*ELF_fd, 0, 0);
	if (elf_identity[EI_CLASS] == ELFCLASS32)
	{
		if (read(*ELF_fd, &h32, sizeof(h32) != sizeof(h32)))
		{
			fprintf(stderr, "%s: Error: %s: ", program, ELF_name);
			fprintf(stderr, "Failed to read file header\n");
			close(*ELF_fd), exit(EXIT_FAILURE);
		}
	}
	else if (read(*ELF_fd, &h64, sizeof(h64)) != sizeof(h64))
	{
		fprintf(stderr, "%s: Error: %s: ", program, ELF_name);
		fprintf(stderr, "Failed to read file header\n");
		close(*ELF_fd), exit(EXIT_FAILURE);
	}
	lseek(*ELF_fd, 0, 0);
	return (elf_identity[EI_CLASS]);
}
