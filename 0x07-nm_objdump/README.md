# 0x07 C - ELF: `nm`/`objdump`

This is the eighth project of the Holberton School Linux System section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did 🤔

In this project, we built our own **custom version of the `nm` and `objdump` commands** from scratch.


## Why we did it: our learning objectives 💡

Building our own custom commands helps us take a deep dive into:

* The ELF format and what information can be extracted from an ELF file
* How to parse the content of an ELF file
* How to use `nm` and `objdump` them properly and how they differ

## Repo Summary

* `src` - source files
    * `src/hnm` - source files for `hnm` program
    * `src/hobjdump` - source files for `hobjdump` program
    * `src/bswap_XXX.c` - byte swap helper functions. used by both programs
* `include` - header files and documentation
* `nm_check.sh` - a simple `bash` script to test my program against `nm -p`


## Usage

### `hnm` - A `nm` replica

```bash
$ make hnm
...
$ ./hnm YOUR_EXECUTABLE_FILE [MORE_EXECUTABLE_FILES]
```

### `hobjdump` - An `objdump` replica

```bash
$ make hobjdump
...
$ ./hobjdump YOUR_EXECUTABLE_FILE [MORE_EXECUTABLE_FILES]
```

## Notes

Check out my notes in the [notes.md](./notes.md) file


> Written by J. to the is I.
