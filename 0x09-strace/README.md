# 0x09 C - `strace`

This is the tenth project of the Holberton School Linux System section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did 🤔

**We built a rough replica of the Linux command `strace`**

## Why we did it: our learning objectives 💡

* Learn what is `strace`
* Learn what is `ptrace` and what possibilities it offers
* Learn how to read from and write to the memory of a process being traced.

## Repo Summary

There are **7 versions of the `strace` replica**. Each version is slightly more complex than the previous.

Use the command `make strace_<version_number>` to build the program. The `<version_number>` can be any number from `0` to `6`.

| Program | File(s) | Functionality |
|-|-|-|
| `strace_0` | [0-strace.c](./0-strace.c) | Prints system call numbers. |
| `strace_1` | [1-strace.c](./1-strace.c) | Prints system call names. |
| `strace_2` | [2-strace.c](./2-strace.c) | Same as `strace_1` + return values in hexadecimal. |
| `strace_3` | [3-strace.c](./3-strace.c) | Same as `strace_2` + raw parameter values. |
| `strace_4` | [4-strace.c](./4-strace.c) | Same as `strace_3` + string parameters. |
| `strace_5` | [5-strace.c](./5-strace.c) | Same as `strace_4` + integer paramaters + return values in the correct format. |
| `strace_6` | [6-strace.c](./6-strace.c) and [6-strace_2.c](./6-strace_2.c) | Same as `strace_5` + macro values for `open(2)`, `access(2)` and `mmap(2)` |
| `strace_7` | [7-strace.c](./7-strace.c) and [7-strace_2.c](./7-strace_2.c) | Same as `strace_6` + buffer contents for `read` and `write` system calls |

## Notes

Check out more of my notes in the [notes.md](./notes.md) file


> Written by J.I.
