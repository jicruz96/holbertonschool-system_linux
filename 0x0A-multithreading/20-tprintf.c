#include "multithreading.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>

/**
 * tprintf - uses printf family to print out a given formatted string
 *			 uses mutex to prevent race conditions
 * @format: formatted string
 * Return: number of characters printed
 **/
int tprintf(char const *format, ...)
{
	pthread_t self = pthread_self();
	va_list args;
	int chars_printed;

	va_start(args, format);
	pthread_mutex_lock(&tprintf_mutex);
	chars_printed = printf("[%lu] ", (unsigned long)self);
	chars_printed += vprintf(format, args);
	pthread_mutex_unlock(&tprintf_mutex);
	va_end(args);
	return (chars_printed);
}

__attribute__((constructor)) void tprintf_mutex_init(void)
{
	pthread_mutex_init(&tprintf_mutex, NULL);
}

__attribute__((destructor)) void tprintf_mutex_destroy(void)
{
	pthread_mutex_destroy(&tprintf_mutex);
}

void end(void) __attribute__((destructor));
