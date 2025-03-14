# [STMicroelectronics](https://github.com/STMicroelectronics) Training: System Programming Under Linux

Trainee in System Programming under Linux (Ubuntu)
July 2024 – February 2025

## Phase 1 (July – September 2024)
- **Linux/Unix History, POSIX and SUSv3**
- **ELF File and Build Process**
- **System Calls and Processes in Linux**
- **Memory Allocation**
- **Users and Groups Management**
- **Partition Table Parsing and File System**
- **GDB and Makefile**

## Phase 2 (September 2024 – February 2025)
- **Compiling Minimal Linux Distro & Yocto Build System**
- **Socket Programming in Unix and Networking**
- **Signals: Standard and real-time signals**
- **Inter-Process Communication (IPC) and filelocks**
- **Multi-Threading & Synchronization**
- **Valgrind, Unity C testing and GCOV**

# Tasks

## [C Compilation and Symbol Analysis](C_compilation_and_symbol_analysis)
This project contains a C program split into two files: `prime.c` and `main.c`. One file contains a function and a global variable (used in this function), and the second one contains the main function and uses the function defined in the first file.

## [ELF Parser](Elf_parser)
This C program parses an ELF file and prints its header information similar to the `readelf -h` command. It reads the ELF header and displays various fields such as type, machine, version, entry point, and more.

## [LQ_SHELL](LQ_SHELL)

This is a basic shell implementation in C with memory optimization designed to execute internal and external commands. It supports several built-in commands and can handle executing external programs.

## [Heap Memory Manager Projects](https://github.com/Belal0066/Heap-Memory-Manager/blob/main/README.md)

Two projects focused on managing heap memory in C: a simulation of a Heap Memory Manager (HMM) and a custom HMM Library. These projects demonstrate low-level memory management concepts, including dynamic memory allocation, deallocation, and fragmentation management.

## [fdskL](fdisk_l)

A command-line utility for displaying partition information on disk devices. This tool supports both MBR (Master Boot Record) and GPT (GUID Partition Table) partition schemes and provides detailed information about each partition.

## [ls Command](ls_cmd)
This project is a custom implementation of the ls command. Made with dynamic memory allocation, allowing it to handle directories of any size efficiently. This implementation supports many of the common flags used with the standard ls command and provides formatted output.

## [HTTP Server in C](https://github.com/Belal0066/HTTP_server_C)
A lightweight HTTP server implementation in C that handles basic web requests, serves static files, generates directory listings, and supports CGI script execution.

## [Signals](Signals)
- #### [Non-Blocking Wait](Signals/task_1): Implements a mechanism to wait for signals without blocking the main process, allowing for more responsive applications.
- #### [Real-Time Logger](Signals/task_3): A logging system that utilizes real-time signals to log events and messages efficiently.


## [IPC: Shop Simulation](IPC)
A multi-process shop simulation implemented in C, demonstrating efficient use of Linux IPC mechanisms. The system consists of three main processes: Shop Process, Customer Process, and Inventory Manager, managing inventory and processing orders.
