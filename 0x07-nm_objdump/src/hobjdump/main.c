#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <elf.h>
#include <string.h>
#include <unistd.h>
#include <hobjdump.h>

/**
 * main - entry point for hobjdump
 * @argc: argument count
 * @argv: arguments
 * Return: status
 **/
int main(int argc, char *argv[])
{
	int i, status;

	/* If no arguments given, assume "./a.out" */
	if (argc == 1)
		return (hobjdump(argv[0], "a.out"));

	/* Otherwise, print normally */
	for (i = 1; i < argc; i++)
		status |= hobjdump(argv[0], argv[i]);

	return (status);
}

/**
 * hobjdump - replica of the nm program
 * @program_name: program name
 * @filename: name of ELF file to inspect
 * Return: status
 **/
static int hobjdump(char *program_name, char *filename)
{
	int elf_fd;
	unsigned char elf_id[EI_NIDENT];
	char *error_message;

	/* Open the file */
	elf_fd = open(filename, O_RDONLY);

	/* Error out if file doesn't exist */
	if (elf_fd == -1)
	{
		if (errno == ENOENT)
			error_message = "No such file\n";
		else
			error_message = "unknown error when opening\n";
		fprintf(stderr, "%s: '%s': %s", program_name, filename, error_message);
		return (1);
	}

	/* Make sure it's an ELF file */
	if (read(elf_fd, elf_id, sizeof(elf_id)) < EI_NIDENT ||
		memcmp(ELFMAG, elf_id, SELFMAG) != 0)
	{
		fprintf(stderr, "%s: %s: ", program_name, filename);
		fprintf(stderr, "File format not recognized\n");
		return (1);
	}

	/* Reset cursor */
	lseek(elf_fd, 0, SEEK_SET);

	/* If ELF file is 32-bit class, use hobjdump_32() */
	if (elf_id[EI_CLASS] == ELFCLASS32)
		return (hobjdump_32(filename, elf_fd));

	/* otherwise, it's 64-bit. Use hobjdump64() */
	return (hobjdump_64(filename, elf_fd));
}
