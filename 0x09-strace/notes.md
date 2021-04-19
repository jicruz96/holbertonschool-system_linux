# Notes

## `ptrace` summary

```C++
long int ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data)
```
* `ptrace` - allows observation and execution control of another process
    * `request`: determines what action needs to perform
    * `pid`: PID of the process to be traced
    * `addr`: The address in the user space of the traced process...
        1. ...  to where `data` may be written,
        2. ...  or from where a word is read and returned
    * `data`: data to write at `addr`
 
 ## Return value:
 On success, it depends on the value of `request`:
    1. `PTRACE_PEEK*` type: the requested data
    2. `PTRACE_SECCOMP_GET_FILTER`: number of instructions in the BPF program
    3. All others return 0
 
 On error, -1 is returned and errno is set appropriately
  
 Note: `PTRACE_PEEK` values could be -1 on success. So make sure you clear errno before the ptrace call

## `request` values

| request macro | request macro v2 | purpose |
|-|-|-|
| `PTRACE_TRACEME` | `PT_TRACEME` | Indicate that the _process making this request should be traced._ All signals received by this process can be intercepted by its parent, and its parent can use the other `ptrace` requests |
| `PTRACE_PEEKTEXT` | `PT_READ_I` | Return the word in the process's text space at address `addr` |
| `PTRACE_PEEKDATA` | `PT_READ_D` | Return the word in the process's data space at address `addr` |
| `PTRACE_PEEKUSER` | `PT_READ_U` | Return the word in the process's user space at address `addr` |
| `PTRACE_POKETEXT` | `PT_WRITE_I` | Write the word `data` into the proccess's text space at address `addr` |
| `PTRACE_POKEDATA` | `PT_WRITE_D` | Write the word `data` into the proccess's data space at address `addr` |
| `PTRACE_POKEUSER` | `PT_WRITE_U` | Write the word `data` into the proccess's user space at address `addr` |
| `PTRACE_CONT` | `PT_CONTINUE` | Continue the process |
| `PTRACE_KILL` | `PT_KILL` | Kill the process |
| `PTRACE_SINGLESTEP` | `PT_STEP` | Single step the process.* |
| `PTRACE_GETREGS` | `PT_GETREGS` | Get all general purpose registers used by a process.* |
| `PTRACE_SETREGS` | `PT_SETREGS` | Set all general purpose registers used by a process.* |
| `PTRACE_GETFPREGS` | `PT_GETFPREGS` | Get all floating point registers used by a process.* |
| `PTRACE_SETFPREGS` | `PT_SETFPREGS` | Set all floating point registers used by a process.* |
| `PTRACE_ATTACH` | `PT_ATTACH` | Attach to a process that is already running. |
| `PTRACE_DETACH` | `PT_DETACH` | Detach to a process that is already running. |
| `PTRACE_GETFPXREGS` | `PT_GETFPXREGS` | Get all extended floating point registers used by a process. |
| `PTRACE_SETFPXREGS` | `PT_SETFPXREGS` | Set all extended floating point registers used by a process. |
| `PTRACE_SYSCALL` | `PT_SYSCALL` | Continue and stop at the next (return from) syscall |
| `PTRACE_SETOPTIONS` | `PT_SETOPTIONS` | Set ptrace filter options |
| `PTRACE_GETEVENTMSG` | `PT_GETEVENTMSG` | Get last ptrace message |
| `PTRACE_GETSIGINFO` | `PT_GETSIGINFO` | Get siginfo for process |
| `PTRACE_SETSIGINFO` | `PT_SETSIGINFO` | Set new siginfo for process |
| `PTRACE_GETREGSET` | | Get register content |
| `PTRACE_SETREGSET` | | Set register content |
| `PTRACE_SEIZE` | | Like `PTRACE_ATTACH`, but do not force tracee to trap and do not affect signal or group stop state |
| `PTRACE_INTERRUPT` | | Trap seized tracee. |
| `PTRACE_LISTEN` | | Wait for next group event. |
| `PTRACE_PEEKSIGINFO` |  | Return sig info at `addr` |
