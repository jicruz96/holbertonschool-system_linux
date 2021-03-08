#include "signals.h"
#include <string.h>
#include <unistd.h>

/**
 * gotcha - prints gotcha message when called on by SIGQUIT
 * @signum: signal number
 * @info: information about signal
 * @vp: void pointer, irrelevant, only here to satisfy function prototype
 **/
void gotcha(int signum, siginfo_t *info, void *vp)
{
	char pid_str[8] = {'0', 0, 0, 0, 0, 0, 0, 0};
	char *gotcha_message = "SIGQUIT sent by ";
	unsigned int i;
	pid_t pid;

	(void)vp;

	if (info == NULL || signum != SIGQUIT)
		return;

	/* Calculate pid size */
	pid = info->si_pid;
	i = 0;
	while (pid)
	{
		i++;
		pid /= 10;
	}

	/* Stringify pid */
	pid = info->si_pid;
	while (i--)
	{
		pid_str[i] = (pid % 10) + '0';
		pid /= 10;
	}

	write(STDOUT_FILENO, gotcha_message, strlen(gotcha_message));
	write(STDOUT_FILENO, pid_str, strlen(pid_str));
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * trace_signal_sender - determines process that sent a SIGQUIT signal
 * Return: 0 on success, -1 on error
 **/
int trace_signal_sender(void)
{
	struct sigaction sa;

	sa.sa_sigaction = gotcha;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	return (sigaction(SIGQUIT, &sa, NULL));
}
