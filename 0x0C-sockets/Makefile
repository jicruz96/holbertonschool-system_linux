CC       = gcc
CFLAGS   = -g3 -Wall -Werror -Wextra -pedantic


0-server: 0-server.o
	$(CC) $(CFLAGS) 0-server.c -o 00-server

1-server: 1-server.o
	$(CC) $(CFLAGS) 1-server.c -o 11-server

2-client: 2-client.o
	$(CC) $(CFLAGS) 2-client.c -o 22-client

3-server: 3-server.o
	$(CC) $(CFLAGS) 3-server.c -o 33-server

todo_api_0: 4-todo_api.o
	$(CC) $(CFLAGS) 4-todo_api.o -o todo_api_0

todo_api_1: 5-todo_api.o
	$(CC) $(CFLAGS) 5-todo_api.o -o todo_api_1

todo_api_2: 6-todo_api.o
	$(CC) $(CFLAGS) 6-todo_api.o -o todo_api_2

todo_api_3: 7-todo_api.o
	$(CC) $(CFLAGS) 7-todo_api.o -o todo_api_3

todo_api_4: 8-todo_api.o 8-helpers.o
	$(CC) $(CFLAGS) 8-todo_api.o 8-helpers.o -o todo_api_4

todo_api_5: 9-todo_api.o 9-helpers.o
	$(CC) $(CFLAGS) 9-todo_api.o 9-helpers.o -o todo_api_5

todo_api_6: 10-todo_api.o 10-helpers.o
	$(CC) $(CFLAGS) 10-todo_api.o 10-helpers.o -o todo_api_6

todo_api_7: 11-todo_api.o
	$(CC) $(CFLAGS) 11-todo_api.o -o todo_api_7
