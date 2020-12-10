#ifndef GETLINE_H
#define GETLINE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_SIZE 1

/**
 * struct reader_s -    associates a file descriptor to its file's contents.
 *                      The i allows the struct to track what's been read.
 * @fd:			file descriptor
 * @buf:		file's text
 * @num_bytes:	number of bytes copied into buffer (needed to differentiate
 *				between copied null bytes and null bytes from initialization)
 * @buf_size:	size of buffer allocated to memory
 * @i:			index where _getline left off reading the buffer
 * @next:		pointer to next reader struct
 **/
typedef struct reader_s
{
	int fd;
	char *buf;
	int num_bytes;
	int buf_size;
	int i;
	struct reader_s *next;
} reader_t;

char *_getline(const int fd);
char *free_readers(reader_t **readers);
char *_memcpy(char *dest, char *src, size_t size);
reader_t *reader_init(int fd);
char *return_line(reader_t *rd);

#endif /* GETLINE_H */
