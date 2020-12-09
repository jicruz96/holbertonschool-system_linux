# 0x00 C. - ls

This is the first project of the Holberton School Linux System section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did and how to use it

In this project, we built our own **custom version of the `ls` command** from the ground up.

**Our version is called `hls` and is available to download and use**

### Linux / MacOS download
```
$ wget https://github.com/jicruz96/holbertonschool-system_linux/blob/main/0x00-ls/hls
```

### Windows download
```
$ curl https://github.com/jicruz96/holbertonschool-system_linux/blob/main/0x00-ls/hls -O hls
```

Execute the hls file:
```
$ ./hls
```

🎉🎉🎉 *Ta-dah!* Output! Looks like `ls`, don't it? Check the next section to see what our program can and cannot do.

Move the hls file to a folder within your PATH to use as a terminal command, if you're so inclined.
```
$ cp hls /usr/bin
$ hls
```

All other files in the repository are the program's source code, which you are free to verify or alter.

## What `hls` is capable of

```
hls [-1aAlrSt] [FILE] ...
```

This `hls` works with the following options. Call these options all together or separately, before or after the file names-- it doesn't matter; it'll work.

| Option | Description |
|-|-|
| `-1` | Print one file per line |
| `-a` | Print hidden files (files that start with a '.') |
| `-A` | Print hidden files, excluding the current and parent directories ('.' and '..') |
| `-l` | Print in long format. Long format prints additional information for each file as described in this image which I stole from the internet (thanks CloudXLab.com!)<br><br><div align="center">![](https://cloudxlab.com/blog/wp-content/uploads/2017/12/ls-al.png)</div> |
| `-r` | Print contents in reverse order |
| `-S` | Sort by file size |
| `-t` | Sort by modification time |

## What `hls` is *not* capable of

Our `hls` program was not designed to have adaptive spacing for the output, so `hls` output does not look as clean as `ls` output. Honestly, I wanted to add some pretty-printing functionality but my school specifically asked me not to. 🤷🏽‍♂️


## Allowed Functions and System Calls

We were only allowed to use the folllowing C Standard Library functions and system calls. Everything else was custom-made.

| <div align="center">Prototype</div> | <div align="center">Description</div> |
|-|-|
| `DIR *opendir(const char *name)` | <div align=center>**opens a directory.**</div><br><ul>Takes the name of a directory (as a string) as an argument.</ul><ul>Returns a pointer to a directory stream ( `DIR *` )</ul>|
| `struct dirent *readdir(DIR *dirp)` | <div align="center">**reads a directory.**</div><br><ul>Takes a directory stream as an argument.</ul><ul>Returns a pointer to a **dirent** structure representing the next directory entry in the directory stream.</ul>|
| `int closedir(DIR *dirp)` | <div align="center">**closes a directory**</div> |
| `int lstat(const char *path, struct stat *buf)` | <div align="center">**returns information about a file**</div><br><ul>`lstat` is identical to `stat` except that if it encounters a symbolic link, it will stat the link itself rather than the file it refers to.</ul> |
| `void perror(const char *s)`   | <div align="center">**prints a system error message**</div><br><ul>Prints system error message associated with most recently encountered error.</ul><br><ul>If the string `s` is not null, then this string is printed first, followed by a colon, space, and the error message. |
| `int printf(const char *s, ...)`   | <div align="center">**prints a formatted string (and its arguments) to stdout**</div> |
| `int sprintf(char *buffer, const char *s, ...)`  | <div align="center">**prints a formatted string (and its arguments) to a buffer**</div> |
| `int fprintf(int file_descriptor, const char *s, ...)`  | <div align="center">**writes a formatted string (and its arguments) onto a file**</div> |
| `ssize_t readlink(const char *path, char *buf, size_t bufsiz)` | <div align="center">**read value of a symbolic link**</div> |
| `char *ctime(time_t *timer)`    | <div align="center">**Returns a string representation of the timer value**</div><br><ul>Example output string: `Tue Dec  8 20:25:47`</ul> |
| `struct passwd *getpwuid(uid_t uid)` | <div align="center">**Returns user information for a specific user id (uid)**</div> |
| `struct group *getgrgid(gid_t gid)` | <div align="center">**Returns group information for a specific group id (gid)**</div> |
| `void exit(int status)`     | <div align="center">**Terminate program with status number**</div> |
| `void free(void *ptr)`     | <div align="center">**Free allocated memory pointed to by pointer**</div> |
| `void *malloc(size_t size)`   | <div align="center">**Allocate `size` bytes of dynamic memory**</div> |


## The `stat` struct and the `dirent` struct

We used these structs extensively, so I thought I'd just leave a brief description here as a notetaking exercise.

### The `stat` struct

```
struct stat {
    dev_t       st_dev;     /* ID of device containing file */
    ino_t       st_ino;     /* inode number */
    mode_t      st_mode;    /* file permissions */
    nlink_t     st_nlink;   /* number of hard links */
    uid_t       st_uid;     /* file owner's user ID */
    gid_t       st_gid;     /* file owner's group ID */
    dev_t       st_rdev;    /* device ID (if special file) */
    off_t       st_size;    /* total size, in bytes */
    blksize_t   st_blksize; /* blocksize for filesystem I/O */
    blkcnt_t    st_blocks;  /* number of 512B blocks allocated */
    time_t  st_atime;       /* time of last access */
    time_t  st_mtime;       /* time of last modification */
    time_t  st_ctime;       /* time of last status change */
};
```

When `lstat()` is called on a file path, `lstat()` will return a `stat` struct with all the above information about the file. By accessing the values in this struct, I can easily find the file information I'd need to display if a `-l` ( or `--long`) option is used with my command.

### The `dirent` struct

```
 struct dirent {
     ino_t          d_ino;          /* inode number */
     off_t          d_off;          /* we'll ifnore this one for now... */
     unsigned short d_reclen;       /* record length */
     unsigned char  d_type;         /* type of file; not supported by all filesystem types */
     char           d_name[256];    /* filename */
 };
```

Each call on `readdir` returns a pointer to a **dirent** struct, which is a struct that contains information about a directory entry. I use this struct to access the filename.
