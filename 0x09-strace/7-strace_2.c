#include "syscalls.h"

#define STR(x) (#x)
#define FLAG(p, x) {STR(p##_##x), p##_##x}


/**
 * print_read_write_buffer - prints string from the memory of a running process
 * @pid: pid of running process to check
 * @addr: address of string
 * @buf_size: size of string to print
 **/
void print_read_write_buffer(pid_t pid, unsigned long addr, size_t buf_size)
{
	unsigned long offset, tmp;
	char buf[sizeof(tmp) + 1];
	size_t i, count;

	buf[sizeof(tmp)] = '\0';
	putchar('"');
	for (count = 0, offset = 0; 1; offset += sizeof(tmp))
	{
		tmp = ptrace(PTRACE_PEEKDATA, pid, addr + offset);
		memcpy(buf, &tmp, sizeof(tmp));
		for (i = 0; i < (sizeof(buf) / sizeof(buf[0])) - 1; count++, i++)
		{
			if (count == buf_size || count == 32)
			{
				putchar('"');
				if (count != buf_size)
					printf("...");
				return;
			}
			if (buf[i] >= 32 && buf[i] <= 126)
				putchar(buf[i]);
			else if (buf[i] == '\a')
				printf("\\a");
			else if (buf[i] == '\b')
				printf("\\b");
			else if (buf[i] == '\t')
				printf("\\t");
			else if (buf[i] == '\n')
				printf("\\n");
			else if (buf[i] == '\v')
				printf("\\v");
			else if (buf[i] == '\f')
				printf("\\f");
			else if (buf[i] == '\r')
				printf("\\r");
			else
				printf("\\%o", buf[i]);
		}
	}
}

/**
 * print_mmap_prot_flags - prints a number in terms of mmap prot flag values
 * @prot: prot value
 **/
void print_mmap_prot_flags(int prot)
{
	char *delim = "";
#define PROT(x) FLAG(PROT, x)
	flag_t flags[] = {PROT(READ), PROT(WRITE), PROT(EXEC),
		PROT(GROWSDOWN), PROT(GROWSUP)};
	unsigned int i;

	if (prot == PROT_NONE)
	{
		printf(STR(PROT_NONE));
		return;
	}

	for (i = 0; i < (sizeof(flags) / sizeof(flags[0])); i++)
	{
		if ((prot & flags[i].value) == flags[i].value)
		{
			printf("%s%s", delim, flags[i].name);
			delim = "|";
		}
	}
}

/**
 * print_mmap_flags - prints a number in terms of mmap MAP_* flag values
 * @mmap_flags: value
 **/
void print_mmap_flags(int mmap_flags)
{
	char *delim = "";
#define MAP(x) FLAG(MAP, x)
	flag_t flags[] = {MAP(SHARED), MAP(PRIVATE), MAP(FIXED), MAP(ANONYMOUS),
		MAP(DENYWRITE)};
	size_t i;

	for (i = 0; i < (sizeof(flags) / sizeof(flags[0])); i++)
	{
		if ((mmap_flags & flags[i].value) == flags[i].value)
		{
			printf("%s%s", delim, flags[i].name);
			delim = "|";
		}
	}
}


/**
 * print_open_flags - prints a number in terms of open(2) flags (O_*)
 * @open_flags: value
 **/
void print_open_flags(int open_flags)
{
	char *delim = "|";
#define O(x) FLAG(O, x)
	flag_t flags[] = {O(CREAT), O(EXCL), O(NOCTTY), O(TRUNC), O(APPEND),
		O(NONBLOCK), O(NDELAY), O(SYNC), O(ASYNC), O(DIRECTORY), O(NOFOLLOW),
		O(CLOEXEC), O(DSYNC)
	};
	size_t i;

	if ((open_flags & O_ACCMODE) == O_ACCMODE)
		printf("O_ACCMODE");
	else if (open_flags & O_WRONLY)
		printf("O_WRONLY");
	else if (open_flags & O_RDWR)
		printf("O_RDWR");
	else
		printf("O_RDONLY");

	for (i = 0; i < (sizeof(flags) / sizeof(flags[0])); i++)
		if ((open_flags & flags[i].value) == flags[i].value)
			printf("%s%s", delim, flags[i].name);
}

/**
 * print_access_flags - print a number in terms of access() syscall flag values
 * @access_flags: values
 **/
void print_access_flags(int access_flags)
{
	char *delim = "";


	if (access_flags == F_OK)
		printf("F_OK");
	else
	{
		if ((access_flags & R_OK) == R_OK)
			printf("R_OK"), delim = "|";
		if ((access_flags & W_OK) == W_OK)
			printf("%sW_OK", delim), delim = "|";
		if ((access_flags & X_OK) == X_OK)
			printf("%sX_OK", delim);
	}
}
