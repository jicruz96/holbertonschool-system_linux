# Notes
## The gyst

`nm` list symbols from object files. For each symbol, nm shows:
* The symbol value in hexadecimal (i.e. the offset from the beginning of the `.text.` section)[source](https://github.com/okayzed/queens-repair)
* The symbol type. See [symbol types](#symbol-types) for a list of types.
* The symbol name. 

Like this:
```
<symbol value in hexadecimal> <symbol type (a letter)> <symbol name>
```

## Symbol Types

| Symbol | Description |
|-|-|
| **A** | Symbol's value is absolute and will not be changed by further linking |
| **B, b** | Symbol is in the uninitialized data section (known as Block Starting Symbol) |
| **C** | Common.<br>Common symbols are uninitialzed data. When linking, multiple common symbols may appear with the same name. If the symbols is defined anywhere, the common symbols are treated as undefined references. |
| **D, d** | Symbol is in an _initialized_ **data** section |
| **G, g** | Symbol is in an initialized data section _for small objects_.<br>Some object file formats permit more efficient access to small data objects, such as a global int variable as opposed to a large flobal array |
| **i** | For `ELF` format files, this indicates that the symbol is an indirect function. I read a few blogs ([here](https://jasoncc.github.io/gnu_gcc_glibc/gnu-ifunc.html), [here](https://willnewton.name/2013/07/02/using-gnu-indirect-functions/#:~:text=GNU%20indirect%20functions%20are%20an,shipping%20your%20code%20as%20binaries.) and [here](https://www.programmersought.com/article/45465726623/)) but I don't think I followed. |
| **N** | Debugging symbol |
| **p** | Stack unwind section. [Click here for explanation](https://stackoverflow.com/questions/2331316/what-is-stack-unwinding) |
| **R, r** | Read-only data section |
| **S, s** | Symbol is in an uninitialized data section _for small objects_. S is to B as G is to D |
| **T, t** | **Text** . This is the source code |
| **U** | **Undefined** |
| **u** | _Unique_ global symbol. The dynamic linker will make sure that in the entire process there is just one symbol with this name and type in use. |
| **V, v** | Weak object. Still not sure how this relates to a weak symbol... | 
| **W, w** | [Weak symbol.](https://en.wikipedia.org/wiki/Weak_symbol) |
| **-** | A "stabs" (symbol table string) symbol. The next values printed will be the stabs `other` field, the stabs `desc` field, and the stab `type`.<br>_Note: These are used for debugging, but have largely been replaced by the [`DWARF` debugging format](https://en.wikipedia.org/wiki/Weak_symbol)_ |
| **?** | Unknown, or object file format specific |



## Sample output

```bash
$ nm   # if no argument is given, it checks for a file named a.out by default
0000000000601040 B __bss_start
0000000000601040 b completed.6982
0000000000601030 D __data_start
0000000000601030 W data_start
0000000000400470 t deregister_tm_clones
00000000004004e0 t __do_global_dtors_aux
0000000000600e18 t __do_global_dtors_aux_fini_array_entry
0000000000601038 D __dso_handle
0000000000600e28 d _DYNAMIC
0000000000601040 D _edata
0000000000601048 B _end
00000000004005c4 T _fini
0000000000400500 t frame_dummy
0000000000600e10 t __frame_dummy_init_array_entry
0000000000400708 r __FRAME_END__
0000000000601000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
00000000004003e0 T _init
0000000000600e18 t __init_array_end
0000000000600e10 t __init_array_start
00000000004005d0 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000600e20 d __JCR_END__
0000000000600e20 d __JCR_LIST__
                 w _Jv_RegisterClasses
00000000004005c0 T __libc_csu_fini
0000000000400550 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
000000000040052d T main
                 U puts@@GLIBC_2.2.5
00000000004004a0 t register_tm_clones
0000000000400440 T _start
0000000000601040 D __TMC_END__
```
