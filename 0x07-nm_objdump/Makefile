CC           = gcc
INCLUDE      = include/
CFLAGS       = -g3 -Wall -Werror -Wextra -pedantic -I $(INCLUDE)
SRC          = src/
HNM_SRC      = $(addprefix $(SRC), test.c) 
HOBJDUMP_SRC = 


hnm: $(HNM_SRC)
	$(CC) $(CFLAGS) $(HNM_SRC) -o hnm

hobjdump: $(HOBJDUMP_SRC)
	$(CC) $(CFLAGS) $(HNM_SRC) -o hobjdump
