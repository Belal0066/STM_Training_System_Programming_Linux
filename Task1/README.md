This project contains a C program split into two files: `prime.c` and `main.c`. One file contains a function and a  global variables (used in this function), and the second one contains the main function and uses the function defined in the first file.

## Compilation

### Compilation Commands

To compile `prime.c` and `main.c` into object files:

```bash
gcc -c prime.c -o prime.o
gcc -c main.c -o main.o
```

### Linking Object Files into Executable

To link the object files into an executable named `prog`:

```bash
gcc prime.o main.o -o prog
```

## Symbol Table Analysis

### Symbol Table for `prime.o`

To view the symbol table for `prime.o`:

```bash
objdump -t prime.o
```

**Output:**
```
prime.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 prime.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 g     O .data  0000000000000004 mx
0000000000000000 g     F .text  0000000000000074 is_prime
```

### Symbol Table for `main.o`

To view the symbol table for `main.o`:

```bash
objdump -t main.o
```

**Output:**
```
main.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 main.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 l    d  .rodata        0000000000000000 .rodata
0000000000000000 g     F .text  00000000000000a7 main
0000000000000000         *UND*  0000000000000000 printf
0000000000000000         *UND*  0000000000000000 __isoc99_scanf
0000000000000000         *UND*  0000000000000000 is_prime
0000000000000000         *UND*  0000000000000000 __stack_chk_fail
```

### Symbol Table for Executable `prog`

To view the symbol table for the executable `prog`:

```bash
objdump -t prog
```

**Output:**
```
prog:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000              Scrt1.o
000000000000038c l     O .note.ABI-tag  0000000000000020              __abi_tag
0000000000000000 l    df *ABS*  0000000000000000              crtstuff.c
00000000000010d0 l     F .text  0000000000000000              deregister_tm_clones
0000000000001100 l     F .text  0000000000000000              register_tm_clones
0000000000001140 l     F .text  0000000000000000              __do_global_dtors_aux
0000000000004014 l     O .bss   0000000000000001              completed.0
0000000000003db0 l     O .fini_array    0000000000000000              __do_global_dtors_aux_fini_array_entry
0000000000001180 l     F .text  0000000000000000              frame_dummy
0000000000003da8 l     O .init_array    0000000000000000              __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*  0000000000000000              prime.c
0000000000000000 l    df *ABS*  0000000000000000              main.c
0000000000000000 l    df *ABS*  0000000000000000              crtstuff.c
0000000000002150 l     O .eh_frame      0000000000000000              __FRAME_END__
0000000000000000 l    df *ABS*  0000000000000000              
0000000000003db8 l     O .dynamic       0000000000000000              _DYNAMIC
000000000000204c l       .eh_frame_hdr  0000000000000000              __GNU_EH_FRAME_HDR
0000000000003fa8 l     O .got   0000000000000000              _GLOBAL_OFFSET_TABLE_
0000000000000000       F *UND*  0000000000000000              __libc_start_main@GLIBC_2.34
0000000000000000  w      *UND*  0000000000000000              _ITM_deregisterTMCloneTable
0000000000004000  w      .data  0000000000000000              data_start
0000000000004014 g       .data  0000000000000000              _edata
00000000000012a4 g     F .fini  0000000000000000              .hidden _fini
0000000000000000       F *UND*  0000000000000000              __stack_chk_fail@GLIBC_2.4
0000000000000000       F *UND*  0000000000000000              printf@GLIBC_2.2.5
0000000000004000 g       .data  0000000000000000              __data_start
0000000000004010 g     O .data  0000000000000004              mx
0000000000000000  w      *UND*  0000000000000000              __gmon_start__
0000000000004008 g     O .data  0000000000000000              .hidden __dso_handle
0000000000002000 g     O .rodata        0000000000000004              _IO_stdin_used
0000000000001189 g     F .text  0000000000000074              is_prime
0000000000004018 g       .bss   0000000000000000              _end
00000000000010a0 g     F .text  0000000000000026              _start
0000000000004014 g       .bss   0000000000000000              __bss_start
00000000000011fd g     F .text  00000000000000a7              main
0000000000000000       F *UND*  0000000000000000              __isoc99_scanf@GLIBC_2.7
0000000000004018 g     O .data  0000000000000000              .hidden __TMC_END__
0000000000000000  w      *UND*  00000
```

## Section Sizes

### Size of `.text` and `.data` Sections

#### For `prime.o`

To check the size of `.text` and `.data` sections in `prime.o`:

```bash
size prime.o
```

**Output:**
```
   text    data     bss     dec     hex filename
    204       4       0     208      d0 prime.o
```


**2ed Command:**
```bash
readelf -S prime.o
```

**Output:**
```
There are 13 section headers, starting at offset 0x270:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       0000000000000074  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  000001d0
       0000000000000018  0000000000000018   I      10     1     8
  [ 3] .data             PROGBITS         0000000000000000  000000b4
       0000000000000004  0000000000000000  WA       0     0     4
  [ 4] .bss              NOBITS           0000000000000000  000000b8
       0000000000000000  0000000000000000  WA       0     0     1
  [ 5] .comment          PROGBITS         0000000000000000  000000b8
       000000000000002c  0000000000000001  MS       0     0     1
  [ 6] .note.GNU-stack   PROGBITS         0000000000000000  000000e4
       0000000000000000  0000000000000000           0     0     1
  [ 7] .note.gnu.pr[...] NOTE             0000000000000000  000000e8
       0000000000000020  0000000000000000   A       0     0     8
  [ 8] .eh_frame         PROGBITS         0000000000000000  00000108
       0000000000000038  0000000000000000   A       0     0     8
  [ 9] .rela.eh_frame    RELA             0000000000000000  000001e8
       0000000000000018  0000000000000018   I      10     8     8
  [10] .symtab           SYMTAB           0000000000000000  00000140
       0000000000000078  0000000000000018          11     3     8
  [11] .strtab           STRTAB           0000000000000000  000001b8
       0000000000000015  0000000000000000           0     0     1
  [12] .shstrtab         STRTAB           0000000000000000  00000200
       000000000000006c  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), l (large), p (processor specific)
```

#### For `main.o`

To check the size of `.text` and `.data` sections in `main.o`:

```bash
size main.o
```

**Output:**
```
   text    data     bss     dec     hex filename
    325       0       0     325     145 main.o
```

**2ed Command:**
```bash
 readelf -S main.o
```

**Output:**
```
There are 14 section headers, starting at offset 0x450:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       00000000000000a7  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  000002d0
       00000000000000f0  0000000000000018   I      11     1     8
  [ 3] .data             PROGBITS         0000000000000000  000000e7
       0000000000000000  0000000000000000  WA       0     0     1
  [ 4] .bss              NOBITS           0000000000000000  000000e7
       0000000000000000  0000000000000000  WA       0     0     1
  [ 5] .rodata           PROGBITS         0000000000000000  000000e7
       0000000000000046  0000000000000000   A       0     0     1
  [ 6] .comment          PROGBITS         0000000000000000  0000012d
       000000000000002c  0000000000000001  MS       0     0     1
  [ 7] .note.GNU-stack   PROGBITS         0000000000000000  00000159
       0000000000000000  0000000000000000           0     0     1
  [ 8] .note.gnu.pr[...] NOTE             0000000000000000  00000160
       0000000000000020  0000000000000000   A       0     0     8
  [ 9] .eh_frame         PROGBITS         0000000000000000  00000180
       0000000000000038  0000000000000000   A       0     0     8
  [10] .rela.eh_frame    RELA             0000000000000000  000003c0
       0000000000000018  0000000000000018   I      11     9     8
  [11] .symtab           SYMTAB           0000000000000000  000001b8
       00000000000000d8  0000000000000
```

#### For Executable `prog`

To check the size of `.text`, `.data`, and `.bss` sections in `prog`:

```bash
size prog
```

**Output:**
```
   text    data     bss     dec     hex filename
   1975     620       4    2599     a27 prog
```

**2ed Command:**
```bash
readelf -S prog
```

**Output:**
```
There are 31 section headers, starting at offset 0x3758:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .interp           PROGBITS         0000000000000318  00000318
       000000000000001c  0000000000000000   A       0     0     1
  [ 2] .note.gnu.pr[...] NOTE             0000000000000338  00000338
       0000000000000030  0000000000000000   A       0     0     8
  [ 3] .note.gnu.bu[...] NOTE             0000000000000368  00000368
       0000000000000024  0000000000000000   A       0     0     4
  [ 4] .note.ABI-tag     NOTE             000000000000038c  0000038c
       0000000000000020  0000000000000000   A       0     0     4
  [ 5] .gnu.hash         GNU_HASH         00000000000003b0  000003b0
       0000000000000024  0000000000000000   A       6     0     8
  [ 6] .dynsym           DYNSYM           00000000000003d8  000003d8
       00000000000000d8  0000000000000018   A       7     1     8
  [ 7] .dynstr           STRTAB           00000000000004b0  000004b0
       00000000000000c3  0000000000000000   A       0     0     1
  [ 8] .gnu.version      VERSYM           0000000000000574  00000574
       0000000000000012  0000000000000002   A       6     0     2
  [ 9] .gnu.version_r    VERNEED          0000000000000588  00000588
       0000000000000050  0000000000000000   A       7     1     8
  [10] .rela.dyn         RELA             00000000000005d8  000005d8
       00000000000000c0  0000000000000018   A       6     0     8
  [11] .rela.plt         RELA             0000000000000698  00000698
       0000000000000048  0000000000000018  AI       6    24     8
  [12] .init             PROGBITS         0000000000001000  00001000
       000000000000001b  0000000000000000  AX       0     0     4
  [13] .plt              PROGBITS         0000000000001020  00001020
       0000000000000040  0000000000000010  AX       0     0     16
  [14] .plt.got          PROGBITS         0000000000001060  00001060
       0000000000000010  0000000000000010  AX       0     0     16
  [15] .plt.sec          PROGBITS         0000000000001070  00001070
       0000000000000030  0000000000000010  AX       0     0     16
  [16] .text             PROGBITS         00000000000010a0  000010a0
       0000000000000204  0000000000000000  AX       0     0     16
  [17] .fini             PROGBITS         00000000000012a4  000012a4
       000000000000000d  0000000000000000  AX       0     0     4
  [18] .rodata           PROGBITS         0000000000002000  00002000
       000000000000004a  0000000000000000   A       0     0     4
  [19] .eh_frame_hdr     PROGBITS         000000000000204c  0000204c
       000000000000003c  0000000000000000   A       0     0     4
  [20] .eh_frame         PROGBITS         0000000000002088  00002088
       00000000000000cc  0000000000000000   A       0     0     8
  [21] .init_array       INIT_ARRAY       0000000000003da8  00002da8
       0000000000000008  0000000000000008  WA       0     0     8
  [22] .fini_array       FINI_ARRAY       0000000000003db0  00002db0
       0000000000000008  0000000000000008  WA       0     0     8
  [23] .dynamic          DYNAMIC          0000000000003db8  00002db8
       00000000000001f0  0000000000000010  WA       7     0     8
  [24] .got              PROGBITS         0000000000003fa8  00002fa8
       0000000000000058  0000000000000008  WA       0     0     8
  [25] .data             PROGBITS         0000000000004000  00003000
       0000000000000014  0000000000000000  WA       0     0     8
  [26] .bss              NOBITS           0000000000004014  00003014
       0000000000000004  0000000000000000  WA       0     0     1
  [27] .comment          PROGBITS         0000000000000000  00003014
       000000000000002b  0000000000000001  MS       0     0     1
  [28] .symtab           SYMTAB           0000000000000000  00003040
       00000000000
```

---

