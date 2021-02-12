#ifndef READELF_H
#define READELF_H

#include <elf.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int read_bytes(char *buffer, int n, int encoding);
#endif /* READELF_H */
