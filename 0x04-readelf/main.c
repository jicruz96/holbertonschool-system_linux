#include "readelf.h"

/**
 * main - reads elf
 * 
 * Return: 0
 **/
int main(int argc, char *argv[])
{
    FILE* elf;
    int i = 0, elf_size, encoding, file_type, machine_type, start_address = 0;
    int start_program_header, read_size, start_section_headers;
    int num_bytes;
    int offset;
    char buffer[64];

    if (argv[1] == NULL || !argc)
        return (-1);
    
    elf = fopen(argv[1], "r");
    
    /* Then we have to read 64 bytes */
    num_bytes = fread(buffer, 1, 64, elf);
    if (num_bytes <= 0)
        return (-1);
    for (i = 0; i < num_bytes; i++)
        printf("%d. %02d\n", i, buffer[i]);

    /* figure out if 32-bit or 64-bit */
    elf_size = buffer[4];
    printf("This is a %d-bit executable.\n", elf_size == 1 ? 32 : 64);


    /* figure out data encoding -> LSB | MSB */
    encoding = buffer[5];
    printf("Encoding = %s\n", encoding == 2 ? "MSB" : "LSB");
    

    /* byte 17 and 18 give us the file type, use data encoding to determine how to read this */
    file_type = read_bytes(buffer + 16, 2, encoding);
    machine_type = read_bytes(buffer + 18, 2, encoding);
    read_size = ((machine_type == 62) ? 8 : 4);
    /* remember to change static ints here for dynamic values */
    offset = 24;
    start_address = read_bytes(buffer + offset, read_size, encoding);
    offset += read_size;
    start_program_header = read_bytes(buffer + offset, read_size, encoding);
    offset += read_size;
    start_section_headers = read_bytes(buffer + offset, read_size, encoding);
    printf("file type: %d\nmachine type: %d\nstart address: %d\nstart program header: %x\nstart_section_headers: %x\n",
            file_type, machine_type, start_address, start_program_header, start_section_headers);
    return (0);
}

/* read_bytes 
 * n - number of bytes
 * encoding - either LSB or MSB
 * Return: value
 **/
int read_bytes(char *buffer, int n, int encoding)
{
    int i = 0;
    int val = 0;

    if (encoding == 1)
        while (i < n)
            val = (val << 8) + buffer[i++];
    else
        while (--n >= 0)
            val = (val << 8) + buffer[n];
            
    return (val);
}

/*
lines = {
    "ELF Header:",
    "Magic:\t",
    "Class:\tELF%d",
    "Data:\t2's complement, %s endian",
    "Version:\t1 (current)",
    "OS/ABI: %s",
    "ABI Version:\t0",
    "Type:\t%s",
    "Machine:\t%s",
    "Version:\t0x1",
    "Entry point address: 0x%x",
    "Start of program headers:\t%d(bytes into file)",
    "Start of section headers:\t%d(bytes into file)",
    "Flags:\t%d",
    "Size of this header:\t%d (bytes)",
    "Size of program headers:%d (bytes)",
    "Number of program headers:\t%d",
    "Size of section headers:\t%d (bytes)",
    "Number of section headers:\t%d",
    "Section header string table index: %d"
};
*/
