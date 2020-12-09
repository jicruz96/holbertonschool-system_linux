# 0x00 C. - getline

This is the second project of the Holberton School Linux System section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did 🤔

In this project, we built our own **custom version of the `getline` command** from scratch.

**My version is called `_getline` and the source code is in the [`_getline.c`](./_getline.c) file**

## Why we did it: our learning objectives 💡 

### Static variables! What are they and how do we use them?

Well, instead of reading about it and claiming to know what they are, we programmed it 💪🏽. `_getline` relies on a static array of file descriptors to do all its dirty work. If you want to learn more about static variables, Google it.

## Allowed Functions and System Calls

We were only allowed to use the folllowing C Standard Library functions and system calls. Everything else was custom-made.

| <div align="center">Prototype</div> | <div align="center">Description</div> |
|-|-|
| `ssize_t read(int fd, void *buf, size_t count)` | <div align=center>**read `count` bytes from a file descriptor**</div><br><ul>`fd` is a file descriptor. It reads the file associated with `fd`.</ul><ul>The read bytes are stored in buf and the function returns the number of bytes it read</ul>|
| `ssize_t write(int fd, const void *buf, size_t count)` | <div align="center">**writes `count` bytes from `buf` into file referred to by `fd`**</div><br><ul>Returns number of bytes written.</ul>|
| `void exit(int status)`     | <div align="center">**Terminate program with status number**</div> |
| `void free(void *ptr)`     | <div align="center">**Free allocated memory pointed to by pointer**</div> |
| `void *malloc(size_t size)`   | <div align="center">**Allocate `size` bytes of dynamic memory**</div> |

We were also allowed to use the following functions, but I chose to rewrite these from scratch:

* `strcpy`
* `strncpy`
* `strdup`
* `strcat`
* `memset`
* `memcpy`

> Written by J. to the is I.
