#include "multithreading.h"
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;

/**
 * tprintf - uses printf family to print out a given formatted string
 *			 uses mutex to prevent race conditions
 * @format: formatted string
 * Return: number of characters printed
 **/
int tprintf(char const *format, ...)
{
	pthread_t self = pthread_self();

	pthread_mutex_lock(&lock);
	printf("[%lu] %s", (unsigned long)self, format);
	pthread_mutex_unlock(&lock);
	return (0);
}

__attribute__((constructor)) void begin(void)
{
	pthread_mutex_init(&lock, NULL);
}

__attribute__((destructor)) void end(void)
{
	pthread_mutex_destroy(&lock);
}

void end(void) __attribute__((destructor));
