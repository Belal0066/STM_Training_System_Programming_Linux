#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h>
// #include <limits.h>

#include "commands.h"

#define PATH_MAX 2048

void pwd_command()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd");
    }
}

void echo_command(char **argv)
{
    for (int i = 1; argv[i] != NULL; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
}

void cp_command(char **argv)
{
    char buf[1024];
    int count;
    char *source_path, *target_path;
    char *source_filename;
    char full_target_path[PATH_MAX];

    if (argv[1] == NULL || argv[2] == NULL)
    {
        fprintf(stderr, "Usage: cp <source> <destination>\n");
        return;
    }

    source_path = argv[1];
    target_path = argv[2];

    if (access(source_path, F_OK) == -1)
    {
        perror("src does not exist");
        return;
    }

    source_filename = basename(source_path);

    struct stat st = {0};
    if (stat(target_path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        snprintf(full_target_path, sizeof(full_target_path), "%s/%s", target_path, source_filename);
    }
    else
    {
        strncpy(full_target_path, target_path, sizeof(full_target_path) - 1);
        full_target_path[sizeof(full_target_path) - 1] = '\0';
    }

    if (access(full_target_path, F_OK) != -1)
    {
        fprintf(stderr, "Error: Target file '%s' already exists\n", full_target_path);
        return;
    }

    int fd1 = open(source_path, O_RDONLY);
    if (fd1 == -1)
    {
        perror("err opening src file");
        return;
    }

    int fd2 = open(full_target_path, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fd2 == -1)
    {
        perror("err opening dest file");
        close(fd1);
        return;
    }

    while ((count = read(fd1, buf, 1024)) > 0)
    {
        if (write(fd2, buf, count) != count)
        {
            perror("W err");
            close(fd1);
            close(fd2);
            return;
        }
    }

    if (count == -1)
    {
        perror("R err");
    }

    close(fd1);
    close(fd2);
    printf("File copied successfully to: %s\n", full_target_path);
}

int mv_command(char **argv)
{
    if (argv[1] == NULL || argv[2] == NULL)
    {
        fprintf(stderr, "Usage: mv <source> <destination>\n");
        return 1;
    }

    const char *source_path = argv[1];
    const char *target_path = argv[2];
    char full_target_path[PATH_MAX];
    struct stat st = {0};

    if (access(source_path, F_OK) == -1)
    {
        fprintf(stderr, "err: src file '%s' does not exist\n", source_path);
        return 1;
    }

    char *source_filename = basename((char *)source_path);

    if (stat(target_path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        snprintf(full_target_path, sizeof(full_target_path), "%s/%s", target_path, source_filename);
    }
    else
    {
        strncpy(full_target_path, target_path, sizeof(full_target_path) - 1);
        full_target_path[sizeof(full_target_path) - 1] = '\0';
    }

    if (access(full_target_path, F_OK) != -1)
    {
        fprintf(stderr, "err: Target file '%s' already exists\n", full_target_path);
        return 1;
    }

    if (rename(source_path, full_target_path) != 0)
    {
        perror("err moving file");
        return 1;
    }

    printf("File moved successfully to: %s\n", full_target_path);
    return 0;
}

int is_builtin(const char *command)
{
    const char *builtins[] = {
        "envir", "cd", "exitlq", "type", "help", "echolq", "pwdlq", "cplq", "mvlq"};
    const int num_builtins = sizeof(builtins) / sizeof(char *);

    for (int i = 0; i < num_builtins; i++)
    {
        if (strcmp(command, builtins[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_executable(const char *command)
{
    char *path = getenv("PATH");
    if (!path)
        return 0;

    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    char full_path[1024];

    while (dir)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return 1;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return 0;
}

void type_command(const char *command)
{
    if (command == NULL)
    {
        fprintf(stderr, "Usage: typelq <command>\n");
        return;
    }

    if (is_builtin(command))
    {
        printf("%s is an internal command (builtin)\n", command);
    }
    else if (is_executable(command))
    {
        printf("%s is an external command (executable)\n", command);
    }
    else
    {
        printf("%s is an unsupported command\n", command);
    }
}

extern char **environ;

void env_command()
{
    char **env = environ;
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
}

void cd_command(char **argv)
{
    if (argv[1] != NULL)
    {
        if (chdir(argv[1]) == 0)
        {
            printf("Directory changed to %s\n", argv[1]);
        }
        else
        {
            perror("cd");
        }
    }
    else
    {
        printf("cd: missing argument\n");
    }
}
