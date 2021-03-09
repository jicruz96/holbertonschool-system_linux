#include "signals.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/**
 * main - entry point
 * @argc: argument count
 * @argv: arguments
 * Return: EXIT_SUCCESS on success | EXIT_FAILURE on failure
 **/
int main(int argc, char *argv[])
{
	pid_t pid;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	pid = atoi(argv[1]);

	if (kill(pid, SIGINT) == -1)
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
