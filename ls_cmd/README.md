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
- Options parsing with different styles (as shown in examples)


## Usage

Compile the program:

   ```
   gcc -o myls my_ls.c
   ```

Basic usage:

```
./myls [OPTIONS] [FILE/DIRECTORY...]
```

Examples (test is a folder with all file types of linux): 

1. List contents of the current directory:
   ``` 
   ❯ ./myls test
      setuid_file   symlink.txt   reg.txt       exec          
      my_fifo       dir           socketfile    block_device  
      setgid_file   stkDir        char_device  
   ```

2. Long listing of a specific directory:

   ``` 
    ❯ ./myls /home/belal/Desktop/STM/ls/test -l
      total 32
      brw-r--r--  1 root  root      0 Sep 12 13:06 block_device
      crw-r--r--  1 root  root      0 Sep 12 13:05 char_device
      drwxrwxr-x  2 belal belal  4096 Sep 12 13:20 dir
      -rwxrwxr-x  1 belal belal 21560 Sep 12 12:54 exec
      prw-rw-r--  1 belal belal     0 Sep 12 13:06 my_fifo
      -rw-rw-r--  1 belal belal     0 Sep 12 12:53 reg.txt
      -rw-rwSr--  1 belal belal     0 Sep 12 13:21 setgid_file
      -rwSrw-r--  1 belal belal     0 Sep 12 13:21 setuid_file
      srwxrwxr-x  1 belal belal     0 Sep 12 13:11 socketfile
      drwxrwxr-t  2 belal belal  4096 Sep 12 12:54 stkDir
      lrwxrwxrwx  1 belal belal     7 Sep 12 12:56 symlink.txt -> reg.txt
   ```


3. Show hidden files and sort by modification time:
   ```
    ❯ ./myls -at test                          
      ..            setuid_file   my_fifo       symlink.txt   reg.txt       
      .             dir           block_device  stkDir        
      setgid_file   socketfile    char_device   exec   
   ```

4. Display inode numbers and use one file per line:
   ```
   ❯ ./myls -i -1 test  
      3924 setuid_file
      3921 my_fifo
      3926 setgid_file
      3918 symlink.txt
      3923 dir
      3917 stkDir
      3915 reg.txt
      3922 socketfile
      3919 char_device
      3916 exec
      3920 block_device
   ```

5. List multiple directories:
   ```
   ❯ ./myls test -u / -t ~ -i 
    3924 setuid_file   3923 dir           3921 my_fifo       3918 symlink.txt   
    3926 setgid_file   3916 exec          3920 block_device  3915 reg.txt       
    3917 stkDir        3922 socketfile    3919 char_device   
    
    /:
     786433 tmp          524289 opt          655361 etc              17 libx32      1572865 boot        
         18 sbin        2097153 usr         2621441 var              15 lib32            12 swapfile    
         13 bin          262145 snap        3276801 mnt         2883585 media             1 proc        
         14 lib         1048577 cdrom             1 run               1 dev          131073 root        
         16 lib64        393217 srv               1 sys         3014657 home             11 lost+found  
    
    /home/belal:
    3014670 Public        3014669 Templates     3014668 Downloads     3014674 Videos        3014673 Pictures      
    3186955 STM32CubeIDE  3014671 Documents     3014667 Desktop       3014901 snap   
   ```

## Limitations

It dosen't not cover all the options of the system's built-in `ls` command. 

## Upcoming
coloring for all Linux files.
