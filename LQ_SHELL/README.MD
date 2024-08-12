# LQ_SHELL

This is a basic shell implementation in C with memory optimization designed to execute internal and external commands. It supports several built-in commands and can handle executing external programs.

## Internal Commands

- **pwdlq**: Print the current working directory.
- **echolq <message>**: Print the specified message to the terminal.
- **cplq <source> <destination>**: Copy a file from the source to the destination.
- **mvlq <source> <destination>**: Move a file from the source to the destination.
- **cd <directory>**: Change the current working directory.
- **type <command>**: Determine if a command is internal, external, or unsupported.
- **envir**: Print all environment variables.
- **freelq**: information about the system's memory usage.
- **uptimelq**:shows how long the system has been running and the time spent in the idle process.
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
#### compilation
```bash
belal@Ubuntu:~/Desktop/st/lqshell$ gcc -o lq *.c
belal@Ubuntu:~/Desktop/st/lqshell$ ./lq
```
#### some commands and features 
```bash
THE SHELL IS ALIVE! Type 'help' for a list of commands.
eb3t ya ghaly >>> help
Supported commands are:
1- pwdlq = print working directory
2- echolq = print a user input string
3- cplq = copy a file to another file
4- mvlq = move a file to another dir
5- cd = change the current working dir
6- type = print the type of the command (internal, external, unsupported)
7- envir = print all the enviroment variable
8- freelq = information about the system's memory usage
9- uptimelq = shows how long the system has been running and the time spent in the idle process.  
10- exitlq = quit the shell
11- help = print all supported commands
12- external commands = type names directly
```
##### freelq Command
```bash
eb3t ya ghaly >>> freelq
              total        used        free      shared  buff/cache   available
Mem:        3354364     1721020      267152      108012     1366192     1349756
Swap:       2097148      398172     1698976
eb3t ya ghaly >>> 
```

##### uptimelq Command
```bash
eb3t ya ghaly >>> uptimelq
Uptime: 00:29:03
Time Idle: 01:16:31
eb3t ya ghaly >>> 
```


##### Input Redirection
```bash
eb3t ya ghaly >>> cat testInput
2
4
5
2
1
eb3t ya ghaly >>> sort < testInput
1
2
2
4
5
eb3t ya ghaly >>> 
```

##### Output Redirection
```bash
eb3t ya ghaly >>> pwd
/home/belal/Desktop/st/lqshell
eb3t ya ghaly >>> pwd > testOutput 
eb3t ya ghaly >>> cat testOutput
/home/belal/Desktop/st/lqshell
eb3t ya ghaly >>> 
```

##### Error Redirection
```bash
eb3t ya ghaly >>> ls lol
ls: cannot access 'lol': No such file or directory
eb3t ya ghaly >>> ls lol 2> testError
eb3t ya ghaly >>> cat testError
ls: cannot access 'lol': No such file or directory
eb3t ya ghaly >>> 
```

##### Piping `|`
```bash
eb3t ya ghaly >>> ls
commands.c  commands.h	lq  lqshell_optm.c  notNamed  README.md  testError  testInput  testOutput
eb3t ya ghaly >>> ls | grep .c
commands.c
lqshell_optm.c
eb3t ya ghaly >>> 
```
### Upcoming Updates

#### `todo`:
- [ ] **Multi-Piping**: Implement multi-piping functionality to allow multiple commands to be piped together seamlessly.
- [ ] **Redirection with Piping**: Add support for input and output redirection in conjunction with piping.

#### `fix`:
- [ ] **Echo Errors**: Address and resolve any existing errors related to the echo command.
- [ ] **Segmentation Fault Errors**: Correct segmentation fault errors to enhance stability.

#### `Improvements`:
- [ ] **Optimization**: Improve performance by using more system calls instead of the standard C library (libc) functions.



