# 0x00 Python - /proc filesystem

This is the fourth project of the Holberton School Linux System section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did 🤔

In this project, we learned about the `/proc` filesystem and all its intricacies. We wrote two blog posts and wrote one python script as proof of work. I've also included my own personal notes.

* [Click here](https://google.com) to read my blog post on the /proc filesystem
* [Click here](https://google.com) to read my blog post on mapping virtual memory to RAM
* Check out my detailed notes in the [proc_notes.md](./proc_notes.md) file.
* Continue reading to learn more about the Python script.

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
