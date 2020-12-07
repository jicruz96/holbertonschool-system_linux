#include "header.h"
#define IS_PARENT_DIR(x) (len(x) == 2 && x[0] == '.' && x[1] == '.')
#define IS_CWD(x) (len(x) == 1 && x[0] == '.')
#define IS_PATH(x) (find_char(x, '/') != NULL)
#define IS_HIDDEN(x) (x[0] == '.')
#define PRINT_CHECK(x) (!IS_HIDDEN(x) || IS_PATH(x) || flags->dot || \
						(flags->dot_alt && !IS_CWD(x) && !IS_PARENT_DIR(x)))
/**
 * print_list_long - prints file lists in long format (ls -l)
 * @file_list: list to print
 * @flags: ls flags struct (determines printing behavior)
 **/
void print_list_long(file_node_t *file_list, ls_config_t *flags)
{
	char perms[11], time[14], user[256], group[256], name[256], buf[256];
	char sym_link_path[256];
	char *str = "%s %u %s %s %u %s %s";
	unsigned long num_links, size, i;

	if (file_list == NULL)
		return;

	for (; file_list != NULL; file_list = file_list->next)
		if (PRINT_CHECK(file_list->name) == true)
		{
			get_permissions(perms, file_list->info->st_mode);
			get_time(time, file_list->info->st_mtime);
			get_user(user, file_list->info->st_uid);
			get_group(group, file_list->info->st_gid);
			copy_string(name, file_list->name);
			num_links = file_list->info->st_nlink;
			size = file_list->info->st_size;
			printf(str, perms, num_links, user, group, size, time, name);
			if (S_ISLNK(file_list->info->st_mode) == true)
			{
				for (i = 0; i < 256; i++)
					sym_link_path[i] = '\0';
				sprintf(buf, "%s/%s", file_list->dir_name, file_list->name);
				readlink(buf, sym_link_path, 256);
				printf(" -> %s", sym_link_path);
			}
			putchar('\n');
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

	if (file_list == NULL)
		return;

	for (; file_list != NULL; file_list = file_list->next)
		if (PRINT_CHECK(file_list->name) == true)
			printf("%s%s", file_list->name, delimiter);

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
