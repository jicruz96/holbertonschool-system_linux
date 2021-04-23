CC       = gcc
CFLAGS   = -g3 -Wall -Werror -Wextra -pedantic
FILENAME = strace

0_SRC = 0-$(FILENAME).c
$(FILENAME)_0: $(0_SRC:.c=.o)
	$(CC) $(CFLAGS) $(0_SRC) -o $(FILENAME)_0

1_SRC = 1-$(FILENAME).c
$(FILENAME)_1: $(1_SRC:.c=.o)
	$(CC) $(CFLAGS) $(1_SRC) -o $(FILENAME)_1

2_SRC = 2-$(FILENAME).c
$(FILENAME)_2: $(2_SRC:.c=.o)
	$(CC) $(CFLAGS) $(2_SRC) -o $(FILENAME)_2

3_SRC = 3-$(FILENAME).c
$(FILENAME)_3: $(3_SRC:.c=.o)
	$(CC) $(CFLAGS) $(3_SRC) -o $(FILENAME)_3

4_SRC = 4-$(FILENAME).c
$(FILENAME)_4: $(4_SRC:.c=.o)
	$(CC) $(CFLAGS) $(4_SRC) -o $(FILENAME)_4

5_SRC = 5-$(FILENAME).c
$(FILENAME)_5: $(5_SRC:.c=.o)
	$(CC) $(CFLAGS) $(5_SRC) -o $(FILENAME)_5

6_SRC = 6-$(FILENAME).c 6-$(FILENAME)_2.c
$(FILENAME)_6: $(6_SRC:.c=.o)
	$(CC) $(CFLAGS) $(6_SRC) -o $(FILENAME)_6

7_SRC = 7-$(FILENAME).c 7-$(FILENAME)_2.c
$(FILENAME)_7: $(7_SRC:.c=.o)
	$(CC) $(CFLAGS) $(7_SRC) -o $(FILENAME)_7

8_SRC = 8-$(FILENAME).c 8-$(FILENAME)_2.c 8-$(FILENAME)_3.c
$(FILENAME)_8: $(8_SRC:.c=.o)
	$(CC) $(CFLAGS) $(8_SRC) -o $(FILENAME)_8
