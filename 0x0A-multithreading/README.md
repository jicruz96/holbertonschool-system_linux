# 0x02 - Huffman Coding

This is the third project of the Algorithms section of the Low Level Systems, Algorithms, and Blockchain curriculum.

## What we did 🤔

In this project, we made some basic C functions and programs that incorporate
multithreading techniques.

## Learning Objectives

By the end of this project, we should be able to clearly explain:
* What is a thread
* What are the differences between a thread and a process
* What is the difference between Concurrency and Parallelism
* How to create a thread
* How to properly exit a thread
* How to handle mutual execution
* What is a deadlock
* What is a race condition

Check out [notes.md](./notes.md) if you want to see my own notes on these
subjects. 


## The Breakdown:

| File | Purpose | Usage |
|-|-|-|
| [multithreading.h](./multithreading.h) | Header file for all other files | |
| [0-thread_entry.c](./0-thread_entry.c) | Defines function that serves as entry point to a new thread | |
| [1-tprintf.c](./1-tprintf.c) | Defines function that uses the `printf` family to print out a given formatted string | |
| [10-blur_portion.c](./10-blur_portion.c) | Defines a function that blurs a portion of an image using a [Gaussian Blur](https://www.youtube.com/watch?v=C_zFhWdM4ic) | |
| [11-blur_image.c](./11-blur_image.c) | Defines a function that blurs the entirety of an image using a Gaussian Blur | |
| [20-tprintf.c](./20-tprintf.c) | Same as [1-tprintf.c](./1-tprintf.c), but uses `mutex` to avoid race conditions. | |
| [21-prime_factors.c](./21-prime_factors.c) | Defines a function that factors a number into a list of prime factors. No multithreading. | |
| [22-prime_factors.c](./22-prime_factors.c) | Same as previous file, but using a thread pool. | |


> Written by J.I.
