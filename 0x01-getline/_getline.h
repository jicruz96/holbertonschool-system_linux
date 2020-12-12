#ifndef GETLINE_H
#define GETLINE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_SIZE 10

/**
 * struct reader_s -    associates a file descriptor to its file's contents.
 *                      The i allows the struct to track what's been read.
 * @fd:			file descriptor
 * @buf:		file's text
 * @bytes:	number of bytes copied into buffer (needed to differentiate
 *				between copied null bytes and null bytes from initialization)
 * @buf_size:	size of buffer allocated to memory
 * @i:			index where _getline left off reading the buffer
 * @next:		pointer to next reader struct
 **/
typedef struct reader_s
{
	int fd;
	char *buf;
	int bytes;
	int buf_size;
	int i;
	struct reader_s *next;
} reader_t;

char *_getline(const int fd);
char *return_line(char *line, reader_t *reader, int buf_len, int offset);
char *free_readers(reader_t **readers);
char *_memcpy(char *dest, char *src, size_t size);
reader_t *reader_init(int fd);
char *find_line(reader_t *rd);

#endif /* GETLINE_H */
