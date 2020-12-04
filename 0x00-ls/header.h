#ifndef HLS_HEADER
#define HLS_HEADER

/* Header for opendir, readdir, closedir, lstat */
#include <sys/types.h>

/* Header for opendir, readdir, closedir */
#include <dirent.h>

/* Header for printf, sprintf, fprintf */
#include <stdio.h>

/* Headers for lstat */
#include <sys/stat.h>
#include <unistd.h>

/* Header for errno */
#include <errno.h>

/* Header for ctime */
#include <time.h>

/* For booleans */
#include <stdbool.h>

/* For getpwuid */
#include <pwd.h>

/* For getgrgid */
#include <grp.h>

/* For malloc, free */
#include <stdlib.h>

/**
 * struct ls_settings_s - config struct
 * @program_name: program name
 * @long_format: long format
 * @one_per_line: -1
 * @show_hiddens: -a
 * @show_hiddens_alt: -A ('.' and '..' ommitted)
 * @reversed: -r
 * @sort_by_size: -S
 * @sort_by_time: -t
 * @recursive: -R
 * @max_hard_links: max hard links
 * @max_size: max size
 * @max_strlen: max strlen
 **/
typedef struct ls_settings_s
{
	char *program_name;
	bool long_format;
	bool one_per_line;
	bool show_hiddens;
	bool show_hiddens_alt;
	bool reversed;
	bool sort_by_size;
	bool sort_by_time;
	bool recursive;
	int max_hard_links;
	int max_size;
	int max_strlen;
} ls_config_t;

/**
 * struct file_link_s - file linked list node
 * @mode: mode
 * @num_links: num links
 * @user: user
 * @group: group
 * @size: size
 * @mod_date: string of date of last modification of file
 * @file_name: file name
 * @next: next
 * @prev: prev
 **/
typedef struct file_link_s
{
	mode_t mode;
	nlink_t num_links;
	char *user;
	char *group;
	off_t size;
	char *mod_date;
	char *file_name;
	struct file_link_s *next;
	struct file_link_s *prev;
} file_node_t;

/**
 * struct dir_node_s - directory node
 * @dir_name: dir name
 * @list: list
 * @error_code: error_code
 * @next: next
 * @prev: prev
 **/
typedef struct dir_node_s
{
	char *dir_name;
	file_node_t *list;
	int error_code;
	struct dir_node_s *next;
	struct dir_node_s *prev;
} dir_node_t;

/* Function Prototypes */
int generate_list(ls_config_t settings, char *dir_name);
int print_error_message(dir_node_t *dir_node, char *program_name);
void print_file_info(char *filename, ls_config_t settings);
int get_config(int arg_count, char **args, ls_config_t *config);
file_node_t *read_dir(char *dir_name, DIR *dir_stream, ls_config_t *config);
int ls(char *dir_name, ls_config_t *config, dir_node_t **head);
int make_lists(ls_config_t *, char **, int, int, dir_node_t **);
void print_list(file_node_t *file_list, ls_config_t config);
char *get_program_name(char *program_path);
int _strlen(char *str);
char *get_group(gid_t group_id);
char *get_user(uid_t user_id);
void free_everything(dir_node_t *head);

char get_type(mode_t mode);
void get_permissions(char *buffer, mode_t mode);
void print_list_long(file_node_t *file_list, ls_config_t config);
void get_long_stats(file_node_t *new, struct stat file_info);

#define DEFAULT_LS                            \
	{                                         \
		NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
	}
#endif /* HLS_HEADER */
