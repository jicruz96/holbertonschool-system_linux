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
    char buffer[64];

    /* Write a program that displays the information contained in the ELF file header of an ELF file. */

    /* This means we have to read and interpret the first 64 bytes of the exe */

    /* if no arg , exit out */
    if (argv[1] == NULL || !argc)
        return (-1);
    
    /* So first we have to first open the file */
    elf = fopen(argv[1], "r");
    
    /* Then we have to read 64 bytes */
    if (fread(buffer, 1, 64, elf) != 64)
        return (-1);
    for (i = 0; i < 64; i++)
        printf("%d. %02d\n", i, buffer[i]);
    putchar('\n');

    /* figure out if 32-bit or 64-bit */

        /* we'll read byte 5 for this */
    elf_size = buffer[EI_CLASS];

    printf("This is a %d-bit executable.\n", elf_size == 1 ? 32 : 64);
    /* figure out data encoding -> LSB | MSB */

        /* use byte 6 */
    encoding = buffer[EI_DATA];

    printf("LSB - %d or MSB- %d\n\t->Encoding=%d\n", ELFDATA2LSB, ELFDATA2MSB, encoding);
    
    /* byte 7 is elf version. only 1 exists. skip this */
    /* bytes 8 through 16 don't matter. skip 'em */ 

    /* byte 17 and 18 give us the file type, use data encoding to determine how to read this */
    file_type = read_bytes(buffer + 16, 2, encoding);
    machine_type = read_bytes(buffer + 18, 2, encoding);
    read_size = ((machine_type == 62) ? 8 : 4);
    /* remember to change static ints here for dynamic values */
    start_address = read_bytes(buffer + 24, read_size, encoding);
    start_program_header = read_bytes(buffer + 32, read_size, encoding);
    start_section_headers = read_bytes(buffer + 40, read_size, encoding);
    printf("file type: %d\nmachine type: %d\nstart address: %d\nstart program header: %x\nstart_section_headers: %x\n",
            file_type, machine_type, start_address, start_program_header, start_section_headers);
    
    /* bytes 32 - 40: where we will find the program header table */
    /* "Somewhat vague "next field" is the section header table offset, seems to be irrelevant */

    /*
    "ELF Header:\n",
    "Magic:\t", // then we'll loop through bytes and print 
    "Class:\tELF%d\n",
    "Data:\t2's complement, %s endian\n",
    "Version:\t1 (current)\n",
    "OS/ABI: %s\n",
    "ABI Version:\t0\n",
    "Type:\t%s\n",
    "Machine:\t%s\n",
    "Version:\t0x1\n",
    "Entry point address: 0x%x\n",
  Start of program headers:          64 (bytes into file)
  Start of section headers:          38256 (bytes into file)
  Flags:                             0x0
  "Size of this header:\t%d (bytes)\n",
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         36
  Section header string table index: 33



    */

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

    if (encoding == ELFDATA2MSB)
        while (i < n)
            val = (val << 8) + buffer[i++];
    else
        while (--n >= 0)
            val = (val << 8) + buffer[n];
            
    return (val);
}
