# 0x00 C. - ls

This is the first project of the Holberton School Linux System section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did

In this project, we built our own custom version of the `ls` command from the ground up. Each file is an iterative improvement on our custom `ls` program. 

## How to use

Download this repository

```
$ git clone https://github.com/jicruz96/holbertonschool-system_linux.git
```

Change into the `0x00-ls` directory
```
$ cd 0x00-ls
```

Execute the hls file:
```
$ ./hls
```

Move the hls file to a folder within your PATH to use as terminal command
```
$ cp hls /usr/bin
$ hls
```

All other files are the program's source code, which you are free to verify or alter.

## Allowed Functions and System Calls

| <div align=center>Prototype</div> | <div align="center">Description</div> |
|-|-|
| `DIR *opendir(const char *name)` | <div align=center>**opens a directory.**</div><br><ul>Takes the name of a directory (as a string) as an argument.</ul><ul>Returns a pointer to a directory stream ( `DIR *` )</ul>|
| `struct dirent *readdir(DIR *dirp)` | <div align="center">**reads a directory.**</div><br><ul>Takes a directory stream as an argument.</ul><ul>Returns a pointer to a **dirent** structure representing the next directory entry in the directory stream.</ul>|
| `int closedir(DIR *dirp)` | <div align="center">**closes a directory**</div> |
| `int lstat(const char *path, struct stat *buf)` | **returns information about a file**<br><ul>`lstat` is identical to `stat` except that if it encounters a symbolic link, it will stat the link itself rather than the file it refers to.</ul> |
| `perror` | |
| `write` | |
| `printf` | |
| `sprintf` | |
| `fprintf` | |
| `readlink` | |
| `ctime` | |
| `getpwuid` | |
| `getgrgid` | |
| `errno` | |
| `exit` | |
| `free` | |
| `malloc` | |

## The `stat` struct and the `dirent` struct

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
     char           d+name[256];    /* filename */
 };
```

Each call on `readdir` returns a pointer to a **dirent** struct, which is a struct that contains information about a directory entry. I use this struct to access the filename and file type.
