### ELF Parser

#### Description
This C program parses an ELF file and prints its header information similar to the `readelf -h` command. It reads the ELF header and displays various fields such as type, machine, version, entry point, and more.

#### Compilation

```
gcc elf.c -o elfprog -static
```

#### Usage
Run the compiled executable with an ELF file as an argument to parse its header:
```
./elfparser <ELF_FILE>
```

#### Example Output
Example of running the program with an ELF file named `main.o`:
```
$ ./elfprog main.o
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:   ELF64
  Data:    2's complement, little endian
  Version: 1 (current)
  OS/ABI:  UNIX System V
  ABI Version: 0
  Type:    REL (Relocatable file)
  Machine: Advanced Micro Devices X86-64
  Version: 0x1
  Entry point address: 0x0
  Start of program headers: 0 (bytes into file)
  Start of section headers: 1104 (bytes into file)
  Flags:   0x0
  Size of this header: 64 (bytes)
  Size of program headers: 0 (bytes)
  Number of program headers: 0
  Size of section headers: 64 (bytes)
  Number of section headers: 14
  Section header string table index: 13
```

