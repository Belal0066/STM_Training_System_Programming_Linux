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
SYMBOL TABLE:
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
SYMBOL TABLE:
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
SYMBOL TABLE:
0000000000000000 g     O .data  0000000000000000 .hidden __data_start
0000000000000000 g     O .data  0000000000000004 mx
0000000000000000 g     F .text  0000000000000074 is_prime
0000000000000000 g     F .text  00000000000000a7 main
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

---

