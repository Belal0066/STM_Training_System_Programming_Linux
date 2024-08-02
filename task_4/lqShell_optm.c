
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

void execute_command(char **args)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        execvp(args[0], args);

        printf("Eh da ya ghaly (o_-)? %s\n", args[0]);

        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
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
    printf("8- exitlq = quit the shell\n");
    printf("9- help = print all supported commands\n");
    printf("10- external commands = type names directly\n");
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
        printf("eb3t ya ghaly >>> ");
        
        // Read input dynamically
        ssize_t nread = getline(&input, (size_t*)&input_size, stdin);
        if (nread == -1)
        {
            if (feof(stdin)) {
                break; // End of input (Ctrl+D)
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
        else
        {
            execute_command(args);
        }
    }

    free(input);
    if (args != NULL)
    {
        free(args);
    }
    return 0;
}
