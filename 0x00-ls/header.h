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
 * struct ls_settings_s - flags struct
 * @long_format: long format
 * @one_per_line: -1
 * @dot: -a
 * @dot_alt: -A ('.' and '..' ommitted)
 * @reversed: -r
 * @sort_by_size: -S
 * @sort_by_time: -t
 * @recursive: -R
 * @print_dir_name: if true, print directory name before printing list
 * @max_hard_links: max hard links
 * @max_size: max size
 * @max_strlen: max strlen
 **/
typedef struct ls_settings_s
{
	bool long_format;
	bool one_per_line;
	bool dot;
	bool dot_alt;
	bool reversed;
	bool sort_by_size;
	bool sort_by_time;
	bool recursive;
	bool print_dir_name;
	int max_hard_links;
	int max_size;
	int max_strlen;
} ls_config_t;

/**
 * struct file_link_s - file linked list node
 * @name: file name
 * @info: stat struct with file info
 * @next: next
 * @prev: prev
 **/
typedef struct file_link_s
{
	char *name;
	struct stat *info;
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

#define ISLOWER(x) ((x) >= 'a' && (x) <= 'z')
#define ISUPPER(x) ((x) >= 'A' && (x) <= 'Z')

typedef void (*print_t)(file_node_t *, ls_config_t *);
/* Function Prototypes */
void get_permissions(char *buffer, mode_t mode);
void get_group(char *buffer, gid_t group_id);
void get_user(char *buffer, uid_t user_id);
void get_time(char *buffer, time_t time_val);
void free_everything(dir_node_t *d_head, file_node_t *f_head);
char *_strdup(char *str);
char *_strcpy(char *dest, char *src);
int add_dir_node(char *name, DIR *stream, dir_node_t **head);
int add_file_node(char *file_name, char *dir_name, file_node_t **head);
void set_flags(char *arg, ls_config_t *flags);
char *which_goes_first(char *s1, char *s2);
int print_error_message(char *name);
void print_list(file_node_t *file_list, ls_config_t *flags);
int print_dirs(dir_node_t *head, ls_config_t *flags, print_t printer);
void free_file_list(file_node_t *file_list);
file_node_t *file_node_init(char *name, struct stat *info);

char get_type(mode_t mode);
void print_list_long(file_node_t *file_list, ls_config_t *flags);
void get_long_stats(file_node_t *new, struct stat file_info);

#endif /* HLS_HEADER */
