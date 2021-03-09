#include "signals.h"
#include <errno.h>

/**
 * pid_exist - checks if a given pid represents an existing process
 * @pid: process id to check
 * Return: 1 if process exists | 0 if not
 **/
int pid_exist(pid_t pid)
{
    return (kill(pid, 0) < 1 && errno != ESRCH);
}
