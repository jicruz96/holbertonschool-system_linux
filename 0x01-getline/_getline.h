#ifndef GETLINE_H
#define GETLINE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_SIZE 1024

/**
 * struct reader_s -    associates a file descriptor to its file's contents.
 *                      The i allows the struct to track what's been read.
 * @fd:			file descriptor
 * @buf:		file's text
 * @bytes:	number of bytes copied into buffer (needed to differentiate
 *				between copied null bytes and null bytes from initialization)
 * @next:		pointer to next reader struct
 **/
typedef struct reader_s
{
	int fd;
	char *buf;
	int bytes;
	struct reader_s *next;
} reader_t;

char *_getline(const int fd);
char *find_line(reader_t *rd);

#endif /* GETLINE_H */
