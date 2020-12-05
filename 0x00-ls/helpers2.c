#include "header.h"

/**
 * get_user - gets user id in string format
 * @user_id: user id
 * Return: user id as string
 **/
char *get_user(uid_t user_id)
{
	struct passwd *user_info = getpwuid(user_id);

	return (user_info->pw_name);
}

/**
 * get_group - gets group name
 * @group_id: group id
 * Return: group name
 **/
char *get_group(gid_t group_id)
{
	struct group *group_info = getgrgid(group_id);

	return (group_info->gr_name);
}

/**
 * get_config - gets configurations for ls command
 * @arg_count: also known as argc. the number of arguments in args.
 * @args: arguments
 * @config: pointer to config struct
 * Return: index of args where we left off
 **/
void get_config(int arg_count, char **args, ls_config_t *config)
{
	int i, j;

	/* Fix for options at the end rather than at the beginning */

	for (i = 1; i < arg_count; i++)
		if (args[i][0] == '-')
			for (j = 1; args[i][j] != '\0'; j++)
				if (args[i][j] == '1')
					config->one_per_line = true;
				else if (args[i][j] == 'a')
					config->show_hiddens = true;
				else if (args[i][j] == 'A')
					config->show_hiddens_alt = true;
				else if (args[i][j] == 'l')
					config->long_format = true;
				else if (args[i][j] == 'r')
					config->reversed = true;
				else if (args[i][j] == 'R')
					config->recursive = true;
				else if (args[i][j] == 't')
					config->sort_by_time = true;
				else if (args[i][j] == 'S')
					config->sort_by_size = true;

	return;
}

/**
 * print_error_message - does what it says, doc
 * @dir_node: a directory struct
 * Return: errno
 **/
int print_error_message(dir_node_t *dir_node)
{
	char buffer[256];
	char *error_message;
	int error_code = dir_node->error_code;

	if (error_code == 2)
	{
		error_message = "hls: cannot access %s";
		errno = error_code;
	}
	else if (error_code == 13)
	{
		error_message = "hls: cannot open directory %s";
		errno = error_code;
	}
	sprintf(buffer, error_message, dir_node->dir_name);
	perror(buffer);
	return (error_code);
}

/**
 * get_program_name - helper to previous function, also does what it says
 * @program_path: used to extract program name
 * Return: program name
 **/
char *get_program_name(char *program_path)
{
	int i, last_slash = 0, len;
	char *program_name;

	for (i = 0; program_path[i]; i++)
	{
		if (program_path[i] == '/')
			last_slash = i;
	}

	len = i - last_slash;
	program_path = program_path + last_slash + 1;
	program_name = malloc(sizeof(char) * len + 2);
	for (i = 0; i < len; i++)
		program_name[i] = program_path[i];
	program_name[i] = '\0';
	return (program_name);
}
