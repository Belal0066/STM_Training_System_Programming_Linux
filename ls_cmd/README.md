# Dynamic ls Implementation

## Overview

This project is a custom implementation of the `ls` command. Made with dynamic memory allocation, allowing it to handle directories of any size efficiently. This implementation supports many of the common flags used with the standard `ls` command and provides formatted output for both long listing and column-based display.

## Features

- Dynamic memory allocation for handling directories of any size
- Support for the following flags:
  - `-l`: Long listing format
  - `-a`: Show hidden files (starting with .)
  - `-t`: Sort by modification time
  - `-u`: Sort by access time
  - `-c`: Sort by change time
  - `-i`: Show inode numbers
  - `-f`: Do not sort
  - `-d`: List directory entries instead of contents
  - `-1`: List one file per line
- Formatted output for both long listing and column-based display
- Support for processing multiple directories


## Usage

Compile the program:

   ```
   gcc -o myls my_ls.c
   ```

Basic usage:

```
./myls [OPTIONS] [FILE/DIRECTORY...]
```

Examples:

1. List contents of the current directory:
   ```
   ./myls
   ```

2. Long listing of a specific directory:
   ```
   ./myls -l /path/to/directory
   ```

3. Show hidden files and sort by modification time:
   ```
   ./myls -at
   ```

4. Display inode numbers and use one file per line:
   ```
   ./myls -i1
   ```

5. List multiple directories:
   ```
   ./myls /home /usr /etc
   ```

## Limitations

It dosen't not cover all the options of the system's built-in `ls` command. 

## Upcomming
coloring for all Linux files.
