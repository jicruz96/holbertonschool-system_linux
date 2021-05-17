#include "multithreading.h"
#include <stdio.h>
#include <pthread.h>

/**
 * tprintf - uses printf family to print out a given formatted string
 * @format: formatted string
 * Return: number of characters printed
 **/
int tprintf(char const *format, ...)
{
	pthread_t self = pthread_self();

	printf("[%lu] %s", self, format);
	return (0);
}
