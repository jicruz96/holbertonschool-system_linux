# -*- MakeFile -*-
CC    = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,PyList -o libPyList.so -fPIC -I /usr/include/python3.4

1_SRC = 1-python.c
1_OBJ = $(1_SRC:.c=.o)
1: $(1_OBJ)
	$(CC) $(CFLAGS) $(1_OBJ)
