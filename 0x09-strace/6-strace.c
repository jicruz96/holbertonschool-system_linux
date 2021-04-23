#include "syscalls.h"

/**
 * print_string - prints string from the memory of a running process
 * @pid: pid of running process to check
 * @addr: address of string
 **/
void print_string(pid_t pid, unsigned long addr)
{
	unsigned long bytes, tmp;
	size_t buf_size = 4096;
	char *buf = malloc(sizeof(char) * buf_size);

	for (bytes = 0; 1; bytes += sizeof(tmp))
	{
		if (buf_size < bytes + sizeof(tmp))
		{
			buf_size *= 2;
			buf = realloc(buf, buf_size);
		}

		tmp = ptrace(PTRACE_PEEKDATA, pid, addr + bytes);
		if (errno)
		{
			buf[bytes] = '\0';
			break;
		}

		memcpy(buf + bytes, &tmp, sizeof(tmp));
		if (memchr(&tmp, '\0', sizeof(tmp)))
			break;
	}
	printf("\"%s\"", buf);
	free(buf);
}

/**
 * print_arg - prints an argument, depending on its type
 * @type: type
 * @arg: arg
 **/
void print_arg(type_t type, unsigned long int arg)
{
	if (type == INT)
		printf("%d", (int)arg);
	else if (IS_LONG(type))
		printf("%ld", (long)arg);
	else if (IS_UINT(type))
		printf("%lu", arg);
	else if (type == VARARGS)
		printf("...");
	else
		printf("%#lx", arg);
}

/**
 * print_args - print system call arguments
 * @sc: syscall info
 * @regs: registers (struct user_regs_struct)
 * @pid: pid
 **/
void print_args(const syscall_t *sc, struct user_regs_struct *regs, pid_t pid)
{
	size_t i, args[MAX_PARAMS];

	args[0] = regs->rdi, args[1] = regs->rsi, args[2] = regs->rdx;
	args[3] = regs->r10, args[4] = regs->r8,  args[5] = regs->r9;

	putchar('(');

	for (i = 0; sc->params[0] != VOID && i < sc->nb_params; i++)
	{
		if (i)
			printf(", ");

		if (sc->params[i] == CHAR_P && args[i])
			print_string(pid, args[i]);
		else if (i == 1 && !strcmp(sc->name, "open"))
			print_open_flags(args[i]);
		else if (i == 1 && !strcmp(sc->name, "access"))
			print_access_flags(args[i]);
		else if (IS_POINTER(sc->params[i]) && !args[i])
			printf("NULL");
		else if (strcmp(sc->name, "mmap") || (i != 2 && i != 3))
			print_arg(sc->params[i], args[i]);
		else if (i == 2)
			print_mmap_prot_flags(args[i]);
		else
			print_mmap_flags(args[i]);
	}
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
	char msg[256];

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
			else if (!WIFEXITED(status))
			{
				printf(") = ");
				print_arg(syscalls_64_g[regs.orig_rax].ret, regs.rax);
			}
		}
	}
	printf(") = ?\n");
	return (0);
}
