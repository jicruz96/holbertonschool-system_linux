#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <hnm.h>


/**
 * main - entry point for hnm
 * @argc: argument count
 * @argv: arguments
 * Return: status
 **/
int main(int argc, char *argv[])
{
	int i, status;

	/* If no arguments given, assume "./a.out" */
	if (argc == 1)
		return (hnm(argv[0], "a.out"));

	/* If one argument given, execute normally */
	if (argc == 2)
		return (hnm(argv[0], argv[1]));

	/* If several arguments given, print arguments before printing output */
	for (i = 1; i < argc; i++)
	{
		printf("\n%s:\n", argv[i]);
		status |= hnm(argv[0], argv[i]);
	}

	return (status);
}

/**
 * hnm - replica of the nm program
 * @program_name: program name
 * @filename: name of ELF file to inspect
 * Return: status
 **/
static int hnm(char *program_name, char *filename)
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

	/* If ELF file is 32-bit class, use hnm_32() */
	if (elf_id[EI_CLASS] == ELFCLASS32)
		return (hnm_32(program_name, filename, elf_fd, elf_id[EI_DATA]));

	/* otherwise, it's 64-bit. Use hnm64() */
	return (hnm_64(program_name, filename, elf_fd, elf_id[EI_DATA]));
}
