// lqShell.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

void parse_input(char *input, char **args) {
    char *token = strtok(input, " \n");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

void print_help() {
    printf("Supported commands are:\n");
    printf("1- pwd = print working directory\n");
    printf("2- echo = print a user input string\n");
    printf("3- cp = copy a file to another file\n");
    printf("4- mv = move a file to another dir\n");
    printf("5- exit = quit the shell\n");
    printf("6- help = print all supported commands\n");
}

int main() {
    printf("THE SHELL IS ALIVE! Type 'help' for a list of commands.\n");
    
    char input[100];
    char *args[10];

    while (1) {
        printf("eb3t ya ghaly >>> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (input[0] == '\n') continue;
        
        parse_input(input, args);

        if (strcmp(args[0], "pwd") == 0) {
            pwd_command();
        } else if (strcmp(args[0], "echo") == 0) {
            echo_command(args);
        } else if (strcmp(args[0], "cp") == 0) {
            cp_command(args);
        } else if (strcmp(args[0], "mv") == 0) {
            mv_command(args);
        } else if (strcmp(args[0], "exit") == 0) {
            printf("goodbye\n");
            break;
        } else if (strcmp(args[0], "help") == 0) {
            print_help();
        } else {
            printf("Eh da ya ghaly (-_o) ? : %s\n", args[0]);
        }
    }

    return 0;
}

