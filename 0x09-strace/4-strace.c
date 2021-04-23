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
	int i, status;
	struct user_regs_struct regs;
	pid_t pid;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <full_path> [path_args]\n", argv[0]);
		return (1);
	}
	setbuf(stdout, NULL);
	pid = fork();
	if (pid == 0)
	{
		ptrace(PTRACE_TRACEME, pid, NULL, NULL);
		execve(argv[1], argv + 1, envp);
	}
	else
	{
		/* execve print_execve_line */
		status = print_execve_line(argc, argv, envp, pid);
		for (i = 1; !WIFEXITED(status); i ^= 1)
		{
			ptrace(PT_SYSCALL, pid, NULL, NULL);
			wait(&status);
			ptrace(PT_GETREGS, pid, NULL, &regs);
			if (i)
			{
				printf("\n%s", syscalls_64_g[regs.orig_rax].name);
				print_args(&syscalls_64_g[regs.orig_rax], &regs, pid);
			}
			else if (WIFEXITED(status))
				printf(") = ?\n");
			else
				printf(") = %#lx", (size_t)regs.rax);
		}
	}

	return (0);
}


/**
 * print_execve_line - prints first line of strace output (execve)
 * @argc: argument count
 * @argv: arguments
 * @envp: environtment array
 * @pid: pid
 * Return: 0 on failure | 1 on success
 **/
int print_execve_line(int argc, char *argv[], char *envp[], pid_t pid)
{
	struct user_regs_struct regs;
	int i, status;
	char msg[212];

	ptrace(PT_SYSCALL, pid, NULL, NULL);
	wait(&status);
	ptrace(PT_GETREGS, pid, NULL, &regs);
	if (WIFEXITED(status))
	{
		sprintf(msg, "%s: Can't stat '%s'", argv[0], argv[1]);
		perror(msg);
		return (0);
	}
	printf("execve(\"%s\", [", argv[1]);
	for (i = 1; i < argc; i++)
		printf("%s\"%s\"", i == 1 ? "" : ", ", argv[i]);
	for (i = 0; envp[i]; i++)
		;
	printf("], [/* %d vars */]) = %#lx", i, (size_t)regs.rax);
	return (1);
}


/**
 * print_args - print system call arguments
 * @sc: syscall info
 * @regs: registers (struct user_regs_struct)
 * @pid: pid
 **/
void print_args(const syscall_t *sc, struct user_regs_struct *regs, pid_t pid)
{
	size_t i, params[MAX_PARAMS];
	char *str = malloc(4096);
	unsigned long bytes, buf;

	params[0] = regs->rdi, params[1] = regs->rsi, params[2] = regs->rdx;
	params[3] = regs->r10, params[4] = regs->r8, params[5] = regs->r9;
	putchar('(');
	for (i = 0; sc->params[0] != VOID && i < sc->nb_params; i++)
	{
		printf(i ? ", " : "");
		if (sc->params[i] == CHAR_P)
		{
			if (params[i])
			{
				for (bytes = 0; 1; bytes += sizeof(buf))
				{
					buf = ptrace(PTRACE_PEEKDATA, pid, params[i] + bytes);
					if (errno)
					{
						str[bytes] = '\0';
						break;
					}
					memcpy(str + bytes, &buf, sizeof(buf));
					if (memchr(&buf, '\0', sizeof(buf)))
						break;
				}
				printf("\"%s\"", str);
			}
			else
				printf("0");
		}
		else if (sc->params[i] == VARARGS)
			printf("...");
		else
			printf("%#lx", (size_t)params[i]);
	}
	free(str);
}
