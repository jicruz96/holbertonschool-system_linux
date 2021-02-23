CC       = gcc
CFLAGS   = -g3 -Wall -Werror -Wextra -pedantic
FILENAME = hreadelf
SRC      = open_ELF.c read_elf_header_MSB.c

0_SRC = 0-$(FILENAME).c $(SRC) print_elf_file_type.c
0-$(FILENAME): $(0_SRC:.c=.o)
	$(CC) $(CFLAGS) $(0_SRC) -o 0-$(FILENAME)

1_SRC = 1-$(FILENAME).c $(SRC) get_string_table.c
1-$(FILENAME): $(1_SRC:.c=.o)
	$(CC) $(CFLAGS) $(1_SRC) -o 1-$(FILENAME)

2_SRC = 2-$(FILENAME).c $(SRC) get_string_table.c print_elf_file_type.c 2-hreadelf_helpers.c read_elf_program_MSB.c
2-$(FILENAME): $(2_SRC:.c=.o)
	$(CC) $(CFLAGS) $(2_SRC) -o 2-$(FILENAME)
