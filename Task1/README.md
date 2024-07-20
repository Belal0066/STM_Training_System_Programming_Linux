---

This project consists of a C program split into two files: prime.c and main.c, demonstrating the use of function definition across files and symbol table analysis.

## Compilation Commands



gcc -c prime.c -o prime.o
gcc -c main.c -o main.o


### Linking Object Files into Executable:

gcc prime.o main.o -o prog


## Symbol Table Analysis

### Symbol Table for prime.o:

objdump -t prime.o

Output:
SYMBOL TABLE:
0000000000000000 g     O .data  0000000000000004 mx
0000000000000000 g     F .text  0000000000000074 is_prime


### Symbol Table for main.o:

objdump -t main.o

Output:
SYMBOL TABLE:
0000000000000000 g     F .text  00000000000000a7 main
0000000000000000         *UND*  0000000000000000 printf
0000000000000000         *UND*  0000000000000000 __isoc99_scanf
0000000000000000         *UND*  0000000000000000 is_prime
0000000000000000         *UND*  0000000000000000 __stack_chk_fail


### Symbol Table for Executable prog:

objdump -t prog

Output:
SYMBOL TABLE:
0000000000000000 g     O .data  0000000000000000 .hidden __data_start
0000000000000000 g     O .data  0000000000000004 mx
0000000000000000 g     F .text  0000000000000074 is_prime
0000000000000000 g     F .text  00000000000000a7 main


## Section Sizes

### Size of .text and .data Sections

#### For prime.o:

size prime.o

Output:
   text    data     bss     dec     hex filename
    204       4       0     208      d0 prime.o


#### For main.o:

size main.o

Output:
   text    data     bss     dec     hex filename
    325       0       0     325     145 main.o


#### For Executable prog:

size prog

Output:
   text    data     bss     dec     hex filename
   1975     620       4    2599     a27 prog


---
