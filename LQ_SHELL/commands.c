#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h>
#include <regex.h>


#include "commands.h"

#define PATH_MAX 2048
#define UPTIME_FILE "/proc/uptime"
#define MEMINFO_FILE "/proc/meminfo"
#define MAX_LINE 256

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
        "envir", "cd", "exitlq", "type", "help", "echolq", "pwdlq", "cplq", "mvlq", "freelq", "uptimelq"};
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



void format_time(double seconds, char *buffer, size_t size) {
    int total_seconds = (int)seconds;
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int secs = total_seconds % 60;

    snprintf(buffer, size, "%02d:%02d:%02d", hours, minutes, secs);
}

void uptime_command() {
    int fd;
    char buffer[128];
    ssize_t bytes_read;
    double uptime, idle;
    char uptime_s[16], idle_s[16];

    fd = open(UPTIME_FILE, O_RDONLY);
    if (fd == -1) {
        perror("Error opening uptime file");
        return;
    }

    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Error reading uptime file");
        close(fd);
        return;
    }

    buffer[bytes_read] = '\0';
    if (sscanf(buffer, "%lf %lf", &uptime, &idle) != 2) {
        perror("Error parsing uptime data");
        close(fd);
        return;
    }

    close(fd);

    format_time(uptime, uptime_s, sizeof(uptime_s));
    format_time(idle, idle_s, sizeof(idle_s));

    printf("Uptime: %s\n", uptime_s);
    printf("Time Idle: %s\n", idle_s);
}

typedef struct {
    unsigned long MemTotal, SwapTotal, MemFree, SwapFree, Shmem, Buffers, Cached, SReclaimable, MemAvailable;
} MemInfo;

unsigned long read_meminfo_value(const char *pattern) {
    FILE *fp;
    char line[MAX_LINE];
    regex_t regex;
    regmatch_t matches[2];
    unsigned long value = 0;

    fp = fopen(MEMINFO_FILE, "r");
    if (fp == NULL) {
        perror("Error opening /proc/meminfo");
        return 0;
    }

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Error compiling regex\n");
        fclose(fp);
        return 0;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (regexec(&regex, line, 2, matches, 0) == 0) {
            sscanf(line + matches[1].rm_so, "%lu", &value);
            break;
        }
    }

    regfree(&regex);
    fclose(fp);
    return value;
}

void gather_meminfo(MemInfo *info) {
    info->MemTotal = read_meminfo_value("MemTotal:\\s+([0-9]+)");
    info->SwapTotal = read_meminfo_value("SwapTotal:\\s+([0-9]+)");
    info->MemFree = read_meminfo_value("MemFree:\\s+([0-9]+)");
    info->SwapFree = read_meminfo_value("SwapFree:\\s+([0-9]+)");
    info->Shmem = read_meminfo_value("Shmem:\\s+([0-9]+)");
    info->Buffers = read_meminfo_value("Buffers:\\s+([0-9]+)");
    info->Cached = read_meminfo_value("Cached:\\s+([0-9]+)");
    info->SReclaimable = read_meminfo_value("SReclaimable:\\s+([0-9]+)");
    info->MemAvailable = read_meminfo_value("MemAvailable:\\s+([0-9]+)");

    if (info->MemTotal == 0 || info->SwapTotal == 0 || info->MemFree == 0 || 
        info->SwapFree == 0 || info->MemAvailable == 0) {
        fprintf(stderr, "Error: Failed to read some memory values\n");
        exit(1);
    }
}

void free_command() {
    MemInfo info;
    gather_meminfo(&info);

    unsigned long MemUsed = info.MemTotal - info.MemFree - info.Buffers - info.Cached - info.SReclaimable;
    unsigned long SwapUsed = info.SwapTotal - info.SwapFree;
    unsigned long buff_cache = info.Buffers + info.Cached + info.SReclaimable;

    printf("              total        used        free      shared  buff/cache   available\n");
    printf("Mem:    %11lu %11lu %11lu %11lu %11lu %11lu\n",
           info.MemTotal, MemUsed, info.MemFree, info.Shmem, buff_cache, info.MemAvailable);
    printf("Swap:   %11lu %11lu %11lu\n",
           info.SwapTotal, SwapUsed, info.SwapFree);
}
