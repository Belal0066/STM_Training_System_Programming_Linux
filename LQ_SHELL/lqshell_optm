#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h>

#include "commands.h"

#define INITIAL_ARG_SIZE 10
#define ARG_INCREMENT 10
#define INITIAL_INPUT_SIZE 100
#define INPUT_INCREMENT 100
#define MAX_ARGS 64

void parse_input(char *input, char ***args, int *arg_count)
{
    int size = INITIAL_ARG_SIZE;
    *args = malloc(size * sizeof(char *));
    if (*args == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(input, " \n");
    *arg_count = 0;

    while (token != NULL)
    {
        if (*arg_count >= size)
        {
            size += ARG_INCREMENT;
            *args = realloc(*args, size * sizeof(char *));
            if (*args == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        (*args)[(*arg_count)++] = token;
        token = strtok(NULL, " \n");
    }
    (*args)[*arg_count] = NULL;
}

void execute_command_main(char **argv)
{
    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;
    int error_fd = STDERR_FILENO;
    int i, j;
    char *args[MAX_ARGS];

    for (i = 0, j = 0; argv[i] != NULL && j < MAX_ARGS - 1; i++)
    {
        if (strcmp(argv[i], "<") == 0)
        {
            i++;
            if (argv[i] == NULL)
            {
                fprintf(stderr, "Error: Missing input file\n");
                return;
            }
            input_fd = open(argv[i], O_RDONLY);
            if (input_fd == -1)
            {
                perror("Error opening input file");
                return;
            }
        }
        else if (strcmp(argv[i], ">") == 0)
        {
            i++;
            if (argv[i] == NULL)
            {
                fprintf(stderr, "Error: Missing output file\n");
                return;
            }
            output_fd = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1)
            {
                perror("Error opening output file");
                return;
            }
        }
        else if (strcmp(argv[i], ">>") == 0)
        {
            i++;
            if (argv[i] == NULL)
            {
                fprintf(stderr, "Error: Missing output file\n");
                return;
            }
            output_fd = open(argv[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd == -1)
            {
                perror("Error opening output file");
                return;
            }
        }
        else if (strcmp(argv[i], "2>") == 0)
        {
            i++;
            if (argv[i] == NULL)
            {
                fprintf(stderr, "Error: Missing error file\n");
                return;
            }
            error_fd = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (error_fd == -1)
            {
                perror("Error opening error file");
                return;
            }
        }
        else
        {
            args[j++] = argv[i];
        }
    }
    args[j] = NULL;

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        if (error_fd != STDERR_FILENO)
        {
            dup2(error_fd, STDERR_FILENO);
            close(error_fd);
        }

        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);

        if (input_fd != STDIN_FILENO)
            close(input_fd);
        if (output_fd != STDOUT_FILENO)
            close(output_fd);
        if (error_fd != STDERR_FILENO)
            close(error_fd);
    }
}

void pipCmd(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: [command1] x '|' [command2] x \n", argv[0]);

        return;
    }

    int pipe_index = -1;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            pipe_index = i;
            break;
        }
    }

    if (pipe_index == -1 || pipe_index == 0 || pipe_index == argc - 1)
    {
        fprintf(stderr, "Error: Pipe symbol '|' must between two commands.\n");

        return;
    }

    argv[pipe_index] = NULL;
    char **cmd1 = &argv[0];

    char **cmd2 = &argv[pipe_index + 1];

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)
    {

        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execute_command_main(cmd1);
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0)
    {

        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        execute_command_main(cmd2);
        exit(EXIT_FAILURE);
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void print_help()
{
    printf("Supported commands are:\n");
    printf("1- pwdlq = print working directory\n");
    printf("2- echolq = print a user input string\n");
    printf("3- cplq = copy a file to another file\n");
    printf("4- mvlq = move a file to another dir\n");
    printf("5- cd = change the current working dir\n");
    printf("6- type = print the type of the command (internal, external, unsupported)\n");
    printf("7- envir = print all the enviroment variable\n");

    printf("8- freelq = information about the system's memory usage\n");
    printf("9- uptimelq = shows how long the system has been running and the time spent in the idle process.\n");

    printf("10- exitlq = quit the shell\n");
    printf("11- help = print all supported commands\n");
    printf("12- external commands = type names directly\n");
}

int main()
{
    printf("THE SHELL IS ALIVE! Type 'help' for a list of commands.\n");

    char *input = malloc(INITIAL_INPUT_SIZE);
    if (input == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int input_size = INITIAL_INPUT_SIZE;

    char **args = NULL;
    int arg_count = 0;

    while (1)
    {
        printf("\033[38;5;190meb3t ya ghaly >>> \033[0m");

        ssize_t nread = getline(&input, (size_t *)&input_size, stdin);
        if (nread == -1)
        {
            if (feof(stdin))
            {
                break;
            }
            perror("getline");
            break;
        }
        if (args != NULL)
        {
            free(args);
            args = NULL;
        }

        if (input[0] == '\n')
            continue;

        parse_input(input, &args, &arg_count);

        if (strcmp(args[0], "pwdlq") == 0)
        {
            pwd_command();
        }
        else if (strcmp(args[0], "echolq") == 0)
        {
            echo_command(args);
        }
        else if (strcmp(args[0], "cplq") == 0)
        {
            cp_command(args);
        }
        else if (strcmp(args[0], "mvlq") == 0)
        {
            mv_command(args);
        }
        else if (strcmp(args[0], "exitlq") == 0)
        {
            printf("goodbye\n");
            break;
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            cd_command(args);
        }
        else if (strcmp(args[0], "help") == 0)
        {
            print_help();
        }
        else if (strcmp(args[0], "type") == 0)
        {
            type_command(args[1]);
        }
        else if (strcmp(args[0], "envir") == 0)
        {
            env_command();
        }
        else if (strcmp(args[0], "uptimelq") == 0)
        {
            uptime_command();
        }
        else if (strcmp(args[0], "freelq") == 0)
        {
            free_command();
        }
        else
        {

            int pipe_present = 0;
            for (int i = 0; i < arg_count; i++)
            {
                if (strcmp(args[i], "|") == 0)
                {
                    pipe_present = 1;
                    break;
                }
            }

            if (pipe_present)
            {
                pipCmd(arg_count, args);
            }
            else
            {

                execute_command_main(args);
            }
        }
    }

    free(input);
    if (args != NULL)
    {
        free(args);
    }
    return 0;
}
