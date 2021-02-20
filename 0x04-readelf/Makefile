FILENAME = hreadelf
SRC = $(FILENAME).c
HLP = print_file_type.c open_ELF.c get_string_table.c
CC = gcc
CFLAGS = -g3 -Wall -Werror -Wextra -pedantic


0 = 0
$(0)-$(FILENAME): $(0)-$(SRC:.c=.o) $(HLP:.c=.o)
	$(CC) $(CFLAGS) $(0)-$(SRC) $(HLP) -o $(0)-$(FILENAME)

1 = 1
$(1)-$(FILENAME): $(1)-$(SRC:.c=.o) $(HLP:.c=.o)
	$(CC) $(CFLAGS) $(1)-$(SRC) $(HLP) -o $(1)-$(FILENAME)

2 = 2
$(2)-$(FILENAME): $(2)-$(SRC:.c=.o) $(HLP:.c=.o)
	$(CC) $(CFLAGS) $(2)-$(SRC) $(HLP) -o $(2)-$(FILENAME)
