CC        = gcc
CFLAGS    = -g3 -Wall -Werror -Wextra -pedantic -I./src
MAIN_FILE = src/hreadelf.c

0_DIR = 0-readelf_elf_header
0_SRC = $(addprefix src/, 0-main.o $(0_DIR)/print_ELF_header.c read_attr.c get_ELF_file_type.c)
0-hreadelf: $(0_SRC:.c=.o)
	$(CC) $(CFLAGS) $(0_SRC) -o 0-hreadelf

1_DIR = 1-readelf_section_header
1_SRC = $(addprefix src/, 1-main.o $(1_DIR)/print_ELF_section_header_table.c read_attr.c get_string_table.c)
1-hreadelf: $(1_SRC:.c=.o)
	$(CC) $(CFLAGS) $(1_SRC) -o 1-hreadelf

2_SRC = print_ELF_program_header_table.c
2-hreadelf: $(2_SRC:.c=.o)
	$(CC) $(CFLAGS) $(2_SRC) -o 2-hreadelf

src/0-main.o: $(MAIN_FILE)
	$(CC) $(CFLAGS) $(MAIN_FILE) -D TASK_0 -c -o src/0-main.o

src/1-main.o: $(MAIN_FILE)
	$(CC) $(CFLAGS) $(MAIN_FILE) -D TASK_1 -c -o src/1-main.o

src/2-main.o: $(MAIN_FILE)
	$(CC) $(CFLAGS) $(MAIN_FILE) -D TASK_2 -c -o src/2-main.o
