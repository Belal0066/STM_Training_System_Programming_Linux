# lq Shell

This project is a simple shell program implemented in C that supports basic commands.

## Files

- `shell.c`: Contains the main loop and command parsing.
- `commands.c`: Contains the implementations of the commands.
- `commands.h`: Header file for the commands.

## Commands

   -  pwd: Print the current working directory.
   -  echo <text>: Print the specified text.
   -  cp <source> <destination>: Copy a file from source to destination.
   -  mv <source> <destination>: Move a file from source to destination.
   -  exit: Exit the shell.
   -  help: Print a list of supported commands.

## TEST
```
belal@Ubuntu:~/Desktop/st/3/task_3$ gcc -o lq *.c
belal@Ubuntu:~/Desktop/st/3/task_3$ sudo ./lq
THE SHELL IS ALIVE! Type 'help' for a list of commands.
eb3t ya ghaly >>>
eb3t ya ghaly >>> help
Supported commands are:
1- pwd = print working directory
2- echo = print a user input string
3- cp = copy a file to another file
4- mv = move a file to another dir
5- exit = quit the shell
6- help = print all supported commands
eb3t ya ghaly >>>
eb3t ya ghaly >>> pwd
/home/belal/Desktop/st/3/task_3
eb3t ya ghaly >>>
eb3t ya ghaly >>> echo hello ya world!
hello ya world!
eb3t ya ghaly >>>
eb3t ya ghaly >>> cp lqShell.c lqShellcp.c
File copied successfully to: lqShellcp.c
eb3t ya ghaly >>>
eb3t ya ghaly >>> mv lqShellcp.c /elfolder
File moved successfully to: /elfolder
eb3t ya ghaly >>>
eb3t ya ghaly >>> exit
goodbye
belal@Ubuntu:~/Desktop/st/3/task_3$
```
   

