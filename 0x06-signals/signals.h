#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

#ifndef sig_t
typedef void (*sig_t)(int);
#endif /* sig_t */

#ifndef NULL
#define NULL ((void *)0)
#endif /* NULL */

#define SUCCESS 0
#define FAILURE -1


/* 0-handle_signal.c */
int handle_signal(void);

/* 1-current_handler_signal.c */
void (*current_handler_signal(void))(int);

/* 2-handle_sigaction.c */
int handle_sigaction(void);

/* 3-current_handler_sigaction.c */
void (*current_handler_sigaction(void))(int);

/* 4-trace_signal_sender.c */
int trace_signal_sender(void);

/* 100-all_in_one.c */
void all_in_one(void);

/* 101-sigset_init.c */
int sigset_init(sigset_t *set, int *signals);

/* 102-signals_block.c */
int signals_block(int *signals);

/* 103-signals_unblock.c */
int signals_unblock(int *signals);

/* 104-handle_pending.c */
int handle_pending(void (*handler)(int));


#endif /* SIGNALS_H */
