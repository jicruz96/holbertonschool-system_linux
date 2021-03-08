#include "signals.h"
#include <string.h>
#include <unistd.h>

/**
 * gotcha - prints "Gotcha! [<signum]" when called on by a signal handler
 * @signum: signal number
 **/
void gotcha(int signum)
{
	char signum_str[3] = {'\0', '\0', '\0'};
	char *gotcha_message = "Gotcha! ";

	/**
	 * This signum-to-signum_str section assumes there are fewer than 100
	 * distinct signals (which, there are, so I'm good)
	 **/
	if (signum > 9)
	{
		signum_str[0] = (signum / 10) + '0';
		signum_str[1] = (signum % 10) + '0';
	}
	else
	{
		signum_str[0] = signum + '0';
	}

	/**
	 * "Why not just use printf?", I hear you ask.
	 *
	 * printf is not async-signal-safe and, therefore, it's bad practice to use
	 * it in signal handlers. See `man 7 signal-safety` for a list of
	 * async-signal-safe functions.
	 **/
	write(STDOUT_FILENO, gotcha_message, strlen(gotcha_message));
	write(STDOUT_FILENO, "[", 1);
	write(STDOUT_FILENO, signum_str, strlen(signum_str));
	write(STDOUT_FILENO, "]\n", 2);
}

/**
 * handle_sigaction - handles the SIGINT signal using sigaction
 * Return: 0 on success, -1 on error
 **/
int handle_sigaction(void)
{
	struct sigaction sa;

	sa.sa_handler = gotcha;

	return (sigaction(SIGINT, &sa, NULL));
}
