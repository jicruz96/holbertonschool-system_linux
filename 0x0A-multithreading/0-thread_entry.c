#include "multithreading.h"
#include <pthread.h>
#include <stdio.h>

/**
 * thread_entry - entry point to a new thread
 * @arg: address of a string to be printed, followed by a new line
 * Return: ???
 **/
void *thread_entry(void *arg)
{
	if (arg)
		puts((char *)arg);
	pthread_exit(NULL);
}
