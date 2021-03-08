#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * main - entry point
 * @argc: argument count
 * @argv: arguments
 * Return: EXIT_SUCCESS on success | EXIT_FAILURE on failure
 **/
int main(int argc, char *argv[])
{
	unsigned long int signum;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <signum>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	signum = atoi(argv[1]);

	if (signum > (sizeof(sys_siglist) / sizeof(char *)) - 1)
		printf("%lu: Unknown signal %lu\n", signum, signum);
	else
		printf("%lu: %s\n", signum, sys_siglist[signum]);

	return (EXIT_SUCCESS);
}
