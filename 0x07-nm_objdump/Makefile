CC               = gcc
INCLUDE          = include/
CFLAGS           = -g3 -Wall -Werror -Wextra -pedantic -I $(INCLUDE)
SRC              = src/
BSWAP            = $(addprefix $(SRC), bswap_32.c bswap_64.c bswap_string.c)
HNM_SRC_DIR      = $(SRC)hnm/
HNM_SRC          = $(addprefix $(HNM_SRC_DIR), main.c hnm_32.c hnm_64.c) $(BSWAP)
HOBJDUMP_SRC_DIR = $(SRC)hobjdump/
HOBJDUMP_SRC     = $(addprefix $(HOBJDUMP_SRC_DIR), main.c hobjdump_32.c hobjdump_64.c) $(BSWAP)


hnm: $(HNM_SRC)
	$(CC) $(CFLAGS) $(HNM_SRC) -o hnm

hobjdump: $(HOBJDUMP_SRC)
	$(CC) $(CFLAGS) $(HOBJDUMP_SRC) -o hobjdump
