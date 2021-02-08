# The Linux `/proc` folder

## Contents

1. [The Gist](#the-gist)
2. [/proc/PID/mem](#/proc/mem)
## The Gist

_(basically wikipedia cliff notes)_

### What is `/proc`?

It's a special filesystem in Unix-like OS's that presents process information.

### What's it for?

* Acts as an interface to internal data structures in the kernal.
* Can be used to obtain information about the system and to change certain kernel parameters at runtime.

### Why use it?

You can access key information about the system and your processes without using any specialized system calls.

### What's inside `/proc`?

Linux `/proc` has two types of items:
* process folders
* non-process-related system info

Below is a brief explainer for both.

### What's inside a `/proc` process folder?

Each running process will have its own proc folder. The folder name is the
process's process ID. This process folder contains the following "files":
| file | description |
|-|-|
`./PID/cmdline`| command line that originally started process |
`./PID/cwd`    | link to current working directory of process|
`./PID/environ`| process's environment variables |
`./PID/exe`    | link to original executable file if it still exists|
`./PID/fd`     | *directory* with links for each open file descriptor |
`./PID/fdinfo` | *directory* with info about each open file descriptor |
`./PID/maps`   | info about mapped files & blocks (like heap & stack) |
`./PID/mem`    | binary image representing process's virtual memory. can only be accessed by a ptrace'ing process|
`./PID/root`   | link to root path as seen by process. Usually `/` unless process is running in a chroot jail|
`./PID/status` | info such as run state and memory usage |
`./PID/task`   | *directory* containing *hard* links to any tasks started by process |

### What non-process-related info is in `/proc`?

There are _tons and tons_ of other files and folders in the `/proc` folder that provide non-process-related information, although in the 2.6 Linux kernel much of that information moved to a separate pseudo-file system, `sysfs`, mounted under `/sys`.

Below is a list of some of the more basic files. For a full comprehensive list, check the [Wikipedia page](https://en.wikipedia.org/wiki/Procfs).

| file | description |
|-|-|
| `./cmdline`         | boot options passed to the kernel                 |
| `./cpuinfo`         | contains CPU vendor, family, model, model names, clockspeed, cache size, # sibligins, cores, and CPU flags. |
| `./devices`         | list of character and block devices |
| `./meminfo` | summary of how the kernel is managing its memory
| `./modules` | list of kernel modules currently loaded. Gives some indication(not always entirely correct) of dependencies.
| `./net/`    | *directory* containing useful information about the network stack |
| `./self` | link to current traversing process |
| `./sys` | access to dynamically-configurable kernel options |
| `./sysvipc` | info about memory-sharing and inter-process communication |
| `./tty`     | info about current terminals
| `./uptime`  | length of time the kernel has been running since boot and spent in idle mode (in seconds) |
| `./version` | info about Kernel version, distribution number, gcc version number used to build the kernel, and any other pertinent information relating to the version of the kernel currently running |
