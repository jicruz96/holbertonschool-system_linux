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



int handle_signal(void);
void (*current_handler_signal(void))(int);

#endif /* SIGNALS_H */
