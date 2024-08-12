# LQ_SEHLL

This is a basic shell implementation in C with memory optimization designed to execute both internal and external commands. It supports several built-in commands and can handle executing external programs.

## Internal Commands

- **pwdlq**: Print the current working directory.
- **echolq <message>**: Print the specified message to the terminal.
- **cplq <source> <destination>**: Copy a file from the source to the destination.
- **mvlq <source> <destination>**: Move a file from the source to the destination.
- **cd <directory>**: Change the current working directory.
- **type <command>**: Determine if a command is internal, external, or unsupported.
- **envir**: Print all environment variables.
- **exitlq**: Exit the shell.
- **help**: Display a list of supported commands.

## External Commands

- Supports executing any command not recognized as an internal command using its name directly (no need for the full path) if found in the system's PATH.

## Dynamic Allocation and Resource Management

- **Arguments Array**: The shell dynamically allocates memory for the argument array based on the number of arguments parsed from user input.
- **Input Buffer**: The input buffer is dynamically resized, handling memory allocation automatically.
- The shell uses `malloc` and `realloc` to allocate memory for argument arrays and user input dynamically.
- It frees allocated memory when it is no longer needed to prevent memory leaks.

### Waiting for Child Processes (external commands)

- The parent process waits for the child process to complete using `waitpid`. This ensures that the shell does not exit or prompt for new commands until the current command finishes execution.
- Handling child processes prevents zombie processes and ensures that system resources are released once the child process has terminated.

## Usage

#### Compilation

```bash
belal@Ubuntu:~/Desktop/st/lqshell$ gcc -o lq *.c
belal@Ubuntu:~/Desktop/st/lqshell$ ./lq
