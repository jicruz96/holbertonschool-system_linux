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
		return (hnm("./a.out"));

	/* If one argument given, execute normally */
	if (argc == 2)
		return (hnm(argv[1]));

	/* If several arguments given, print arguments before printint output */
	for (i = 1; i < argc; i++)
	{
		printf("\n%s:\n", argv[i]);
		status |= hnm(argv[i]);
	}

	return (status);
}

/**
 * hnm - replica of the nm program
 * @filename: name of ELF file to inspect
 * Return: status
 **/
static int hnm(char *filename)
{
	int elf_fd;
	unsigned char elf_id[EI_NIDENT];

	/* Open the file */
	elf_fd = open(filename, O_RDONLY);

	/* Error out if file doesn't exist */
	if (elf_fd == -1)
	{
		if (errno == ENOENT)
			fprintf(stderr, "hnm: '%s': No such file\n", filename);
		else
			fprintf(stderr, "hnm: %s: unknown error when opening\n", filename);
		return (1);
	}

	/* Make sure it's an ELF file */
	if (read(elf_fd, elf_id, sizeof(elf_id)) < EI_NIDENT ||
		memcmp(ELFMAG, elf_id, SELFMAG) != 0)
	{
		fprintf(stderr, "hnm: %s: File format not recognized\n", filename);
		return (1);
	}

	/* Reset cursor */
	lseek(elf_fd, 0, SEEK_SET);

	/* If ELF file is 32-bit class, use hnm_32() */
	if (elf_id[EI_CLASS] == ELFCLASS32)
		return (hnm_32(filename, elf_fd, elf_id[EI_DATA]));

	/* otherwise, it's 64-bit. Use hnm64() */
	return (hnm_64(filename, elf_fd, elf_id[EI_DATA]));
}
