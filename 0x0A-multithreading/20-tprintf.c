#include "multithreading.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>

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
	va_list args;
	size_t i;

	va_start(args, format);
	pthread_mutex_lock(&lock);
	printf("[%lu] ", (unsigned long)self);
	for (i = 0; i < strlen(format); i++)
	{
		if (format[i] == '%' && i + 1 < strlen(format))
		{
			i += 1;
			if (strchr("dic", format[i]))
				printf("%d", va_arg(args, int));
			else if (strchr("u", format[i]))
				printf("%u", va_arg(args, unsigned int));
			else if (format[i] == 's')
				printf("%s", va_arg(args, char *));
		}
		else
		{
			putchar(format[i]);
		}
	}
	pthread_mutex_unlock(&lock);
	va_end(args);
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
