#include "syscalls.h"

/**
 * main - traces a process and prints system call numbers as they're called
 * @argc: argument count
 * @argv: argument array
 * @envp: environment parameters
 * Return: 0 on success | 1 on failure (not enough arguments)
 **/
int main(int argc, char *argv[], char *envp[])
{
	int i, skip, status, num_calls = sizeof(syscalls_64_g) / sizeof(syscall_t);
	struct user_regs_struct regs;
	pid_t pid;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <full_path> [path_args]\n", argv[0]);
		return (1);
	}

	pid = fork();

	if (pid == 0)
	{
		ptrace(PTRACE_TRACEME, pid, NULL, NULL);
		execve(argv[1], argv + 1, envp);
	}
	else
	{
		for (status = 1, skip = 0; !WIFEXITED(status); skip ^= 1)
		{
			ptrace(PT_SYSCALL, pid, NULL, NULL);
			wait(&status);
			ptrace(PT_GETREGS, pid, NULL, &regs);
			if (skip)
				continue;
			for (i = 0; i < num_calls; i++)
			{
				if (syscalls_64_g[i].nr == (size_t)regs.orig_rax)
				{
					puts(syscalls_64_g[i].name);
					break;
				}
			}
		}
	}

	return (0);
}
