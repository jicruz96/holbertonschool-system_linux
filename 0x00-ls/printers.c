#include "header.h"

/**
 * print_list_long - prints file lists in long format (ls -l)
 * @file_list: list to print
 * @flags: ls flags struct (determines printing behavior)
 **/
void print_list_long(file_node_t *file_list, ls_config_t *flags)
{
	char perms[11], time[14], user[256], group[256], *f;
	char *str = "%s %u %s %s %u %s %s\n";
	struct stat *info;
	bool print_this;
	unsigned long num_links;
	off_t size;
	int i;

	if (file_list == NULL)
		return;

	for (; file_list != NULL; file_list = file_list->next)
	{
		info = file_list->info;
		get_permissions(perms, info->st_mode);
		get_time(time, info->st_mtime);
		get_user(user, info->st_uid);
		get_group(group, info->st_gid);
		f = file_list->name;
		num_links = info->st_nlink;
		size = info->st_size;
		if (f[0] == '.')
		{
			for (i = 1; f[i] == '.'; i++)
				;
			if (f[i] == '/' || flags->dot || (f[i] && flags->dot_alt))
				printf(str, perms, num_links, user, group, size, time, f);
		}
		else
		{
			printf(str, perms, num_links, user, group, size, time, f);
		}
	}
}

/**
 * print_list - prints lists
 * @file_list: list of files to print
 * @flags: flags
 **/
void print_list(file_node_t *file_list, ls_config_t *flags)
{
	char *delimiter = flags->one_per_line ? "\n" : "  ";
	char *f;
	int i;

	if (file_list == NULL)
		return;

	for (; file_list != NULL; file_list = file_list->next)
	{
		f = file_list->name;
		if (f[0] == '.')
		{
			for (i = 1; f[i] == '.'; i++)
				;
			if (f[i] == '/' || flags->dot || (f[i] && flags->dot_alt))
				printf("%s%s", f, delimiter);
		}
		else
		{
			printf("%s%s", f, delimiter);
		}
	}
	if (flags->one_per_line == false)
		putchar('\n');
}

/**
 * print_error_message - does what it says, doc
 * @f: f to place on error message
 * Return: error code
 **/
int print_error_message(char *f)
{
	char buffer[256];
	char *error_message;
	int status = errno;

	if (errno == 13)
		error_message = "hls: cannot open directory %s";
	else
		error_message = "hls: cannot access %s";

	sprintf(buffer, error_message, f);
	perror(buffer);
	return (status);
}

/**
 * print_dirs - print dirs
 * @head: head
 * @flags: flags
 * @printer: printer function
 * Return: status
 **/
int print_dirs(dir_node_t *head, ls_config_t *flags, print_t printer)
{
	dir_node_t *tmp;
	int status = 0;

	for (tmp = head; tmp != NULL; tmp = tmp->next)
		if (tmp->error_code)
		{
			status = print_error_message(tmp->dir_name);
		}
		else
		{
			if (flags->print_dir_name)
				printf("%s:\n", tmp->dir_name);
			printer(tmp->list, flags);
			if (tmp->next != NULL)
				putchar('\n');
		}
	return (status);
}
