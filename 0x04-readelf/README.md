# 0x04 C - ELF: readelf

This is the fourth project of the Holberton School Linux System section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did 🤔

In this project, we:
* learned about the `/proc` filesystem and all its intricacies. Check out notes in the [proc_notes.md](./proc_notes.md) file.
* learned about virtual memory mapping
* wrote a script that can find and replace a string in a process's heap.

## The `read_write_heap.py` Python script

[This script](./read_write_heap.py) finds a string in the heap of a running process, and replaces it with a user-defined replacement.

### Usage:

```bash
read_write_heap.py pid search_string replace_string
```
where:
* `pid` is the process ID of the running process.
* `search_string` and `replace_string` are ASCII-encoded strings

[Here's a video](youtube.com/watch?v=xcpXT4Bukgk) demonstrating the script in action.

> Written by J. to the is I.
