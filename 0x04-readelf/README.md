# 0x04 C - ELF: readelf

This is the fourth project of the Holberton School Linux System section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did 🤔

In this project, we:
* learned about the anatomy of an Executable and Linkable Format (ELF) file
* wrote 3 programs that detail information about an ELF file. These program mimic a type of output of the Linux program `readelf` with the `-W` flag (for fixed width)

1. `0-hreadelf`: mimics `readelf -W -h`, which prints the header of an ELF file.
2. `1-hreadelf`: mimics `readelf -W -S`, which prints the section header table of an ELF file.
3. `2-hreadelf`: mimics `readelf -W -l`, which prints the program header table of an ELF file.

### Usage:

Requirements:
* GNU make >= 3.81
* GCC >= 4.8.4

Install and `make` the programs like so:

```bash
$ git clone https://github.com/jicruz96/holbertonschool-system_linux
...
$ cd 0x04-readelf
$ make 0-hreadelf # makes 0-hreadelf 
$ make 1-hreadelf # makes 1-hreadelf 
$ make 2-hreadelf # makes 2-hreadelf 
```


> Written by J. to the is I.
