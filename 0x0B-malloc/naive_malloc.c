#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "malloc.h"

#define shift_address(addr, size) \
	((void *)((char *)(addr) + (size)))

/**
 * align - returns the page-aligned value of a quantity
 * @size: size to align
 * Return: aligned size
 **/
size_t align(size_t size)
{
	if (!size)
		return (0);

	if (size < 8)
		return (8);

	if (size % 8 == 0)
		return (size);

	return (size + (8 - (size % 8)));
}

/**
 * naive_malloc - naive version of malloc
 * @size: number of bytes to allocate
 *
 * Return: the memory address newly allocated, or NUll on error
 *
 * Note: for learning purposes only
 **/
void *naive_malloc(size_t size)
{
	size_t chunk_size = align(size + sizeof(size_t));
	static void *next_spot;
	static size_t allocated;
	void *tmp;
	static long pagesize;

	if (size == 0)
		return (NULL);

	for (; allocated < chunk_size; allocated += pagesize)
		if (!next_spot)
		{
			pagesize = sysconf(_SC_PAGESIZE);
			next_spot = sbrk(pagesize);
		}
		else
		{
			sbrk(pagesize);
		}

	tmp = next_spot;
	memcpy(tmp, &chunk_size, sizeof(chunk_size));
	next_spot = shift_address(tmp, chunk_size);

	return (shift_address(tmp, sizeof(chunk_size)));
}
