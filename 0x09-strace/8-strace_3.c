#include "syscalls.h"


/**
 * print_mode - print mode
 * @mode: mode
 **/
void print_mode(mode_t mode)
{
	if (S_ISREG(mode))
		printf("S_IFREG");
	else if (S_ISDIR(mode))
		printf("S_IFDIR");
	else if (S_ISCHR(mode))
		printf("S_IFCHR");
	else if (S_ISBLK(mode))
		printf("S_IFBLK");
	else if (S_ISFIFO(mode))
		printf("S_IFIFO");
	else if (S_ISLNK(mode))
		printf("S_IFLNK");
	else if (S_ISSOCK(mode))
		printf("S_IFSOCK");
}

/**
 * print_stat_struct - prints stat struct info for fstat syscall
 * @pid: pid of running process to check
 * @addr: address of struct
 **/
void print_stat_struct(pid_t pid, unsigned long addr)
{
	unsigned long bytes, tmp;
	struct stat info;
	char arr[sizeof(info)];

	for (bytes = 0; bytes < sizeof(info); bytes++)
	{
		tmp = ptrace(PTRACE_PEEKDATA, pid, addr + bytes, NULL);
		memcpy(arr + bytes, &tmp, sizeof(tmp));
	}
	memcpy((void *)&info, arr, sizeof(info));
	printf("{st_mode=");
	print_mode(info.st_mode);
	printf("|0%o, ", info.st_mode & 0777);
	printf("st_size=%lu, ...}", info.st_size);
}
