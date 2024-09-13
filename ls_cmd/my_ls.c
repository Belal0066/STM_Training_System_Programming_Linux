#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define MAX_PATH 4096
#define INITIAL_CAPACITY 64

typedef struct
{
    char *name;
    struct stat st;
    char *linkname;
} file_info;

char **currPath = NULL;
int currDir = 0;
int pathCapacity = 0;
int color = 0;

void parse_input(int argc, char *argv[], int *flags, char *path);
int process_and_sort(const char *path, int flags, file_info **files, int *capacity);
void print_output(file_info *files, int count, int flags);
char get_file_type(mode_t mode);
void get_permissions(mode_t mode, char *perms);

// Comp functions for sorting
int compare_name(const void *a, const void *b)
{
    return strcmp(((file_info *)a)->name, ((file_info *)b)->name);
}

int compare_mtime(const void *a, const void *b)
{
    return ((file_info *)b)->st.st_mtime - ((file_info *)a)->st.st_mtime;
}

int compare_atime(const void *a, const void *b)
{
    return ((file_info *)b)->st.st_atime - ((file_info *)a)->st.st_atime;
}

int compare_ctime(const void *a, const void *b)
{
    return ((file_info *)b)->st.st_ctime - ((file_info *)a)->st.st_ctime;
}

void parse_input(int argc, char *argv[], int *flags, char *path)
{
    pathCapacity = INITIAL_CAPACITY;
    currPath = malloc(pathCapacity * sizeof(char *));
    if (!currPath)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(path, "/");
    while (token != NULL)
    {
        if (currDir >= pathCapacity)
        {
            pathCapacity *= 2;
            char **temp = realloc(currPath, pathCapacity * sizeof(char *));
            if (!temp)
            {
                perror("realloc");
                free(currPath);
                exit(EXIT_FAILURE);
            }
            currPath = temp;
        }
        currPath[currDir] = strdup(token);
        if (!currPath[currDir])
        {
            perror("strdup");
            for (int i = 0; i < currDir; i++)
            {
                free(currPath[i]);
            }
            free(currPath);
            exit(EXIT_FAILURE);
        }
        currDir++;
        token = strtok(NULL, "/");
    }

    int opt;
    int option_index = 0;
    *flags = 0;

    static struct option long_options[] = {
        {"color", no_argument, 0, 0},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "latucifd1", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 0:
            if (strcmp(long_options[option_index].name, "color") == 0)
            {
                color = 1;
            }
            break;
        case 'l':
            *flags |= 0x001;
            break;
        case 'a':
            *flags |= 0x002;
            break;
        case 't':
            *flags |= 0x004;
            break;
        case 'u':
            *flags |= 0x008;
            break;
        case 'c':
            *flags |= 0x010;
            break;
        case 'i':
            *flags |= 0x020;
            break;
        case 'f':
            *flags |= 0x040;
            color = 1 ^ color;
            break;
        case 'd':
            *flags |= 0x080;
            break;
        case '1':
            *flags |= 0x100;
            break;
        case 'C':
            color = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-latucifd1] [path]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind < argc)
    {
        strncpy(path, argv[optind], MAX_PATH - 1);
        path[MAX_PATH - 1] = '\0';
    }
    else
    {
        strcpy(path, ".");
    }
}

int process_and_sort(const char *path, int flags, file_info **files, int *capacity)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        return -1;
    }

    *capacity = INITIAL_CAPACITY;
    *files = malloc(*capacity * sizeof(file_info));
    if (*files == NULL)
    {
        perror("malloc");
        closedir(dir);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (flags & 0x040) // -f
        {
            flags |= 0x002;
        }

        if (!(flags & 0x002) && entry->d_name[0] == '.')
            continue;

        if (count >= *capacity)
        {
            *capacity *= 2;
            file_info *temp = realloc(*files, *capacity * sizeof(file_info));
            if (temp == NULL)
            {
                perror("realloc");
                for (int i = 0; i < count; i++)
                {
                    free((*files)[i].name);
                    free((*files)[i].linkname);
                }
                free(*files);
                closedir(dir);
                return -1;
            }
            *files = temp;
        }

        (*files)[count].name = strdup(entry->d_name);
        if ((*files)[count].name == NULL)
        {
            perror("strdup");
            for (int i = 0; i < count; i++)
            {
                free((*files)[i].name);
                free((*files)[i].linkname);
            }
            free(*files);
            closedir(dir);
            return -1;
        }

        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", path, entry->d_name);

        if (lstat(full_path, &(*files)[count].st) == -1)
        {
            perror("lstat");
            free((*files)[count].name);
            continue;
        }

        if (S_ISLNK((*files)[count].st.st_mode))
        {
            (*files)[count].linkname = malloc(MAX_PATH);
            if ((*files)[count].linkname == NULL)
            {
                perror("malloc");
                free((*files)[count].name);
                for (int i = 0; i < count; i++)
                {
                    free((*files)[i].name);
                    free((*files)[i].linkname);
                }
                free(*files);
                closedir(dir);
                return -1;
            }
            ssize_t len = readlink(full_path, (*files)[count].linkname, MAX_PATH - 1);
            if (len != -1)
            {
                (*files)[count].linkname[len] = '\0';
            }
            else
            {
                (*files)[count].linkname[0] = '\0';
            }
        }
        else
        {
            (*files)[count].linkname = NULL;
        }

        count++;
    }

    closedir(dir);

    if (flags & 0x080)
    { // -d option
        *files = realloc(*files, sizeof(file_info));
        if (*files == NULL)
        {
            perror("realloc");
            return -1;
        }

        (*files)[0].name = strdup(path);
        if ((*files)[0].name == NULL)
        {
            perror("strdup");
            free(*files);
            return -1;
        }

        if (lstat(path, &(*files)[0].st) == -1)
        {
            perror("lstat");
            free((*files)[0].name);
            free(*files);
            return -1;
        }

        (*files)[0].linkname = NULL;
        return 1;
    }

    if (!(flags & 0x040)) // If -f is not set
    {
        if (flags & 0x004) // -t option
        {
            if (flags & 0x008) // -u option
                qsort(*files, count, sizeof(file_info), compare_atime);
            else if (flags & 0x010) // -c option
                qsort(*files, count, sizeof(file_info), compare_ctime);
            else
                qsort(*files, count, sizeof(file_info), compare_mtime);
        }
        else if (flags & 0x008 && !(flags & 0x001)) // -u option without -t
        {
            qsort(*files, count, sizeof(file_info), compare_atime);
        }
        else if (flags & 0x010 && !(flags & 0x001)) // -c option without -t
        {
            qsort(*files, count, sizeof(file_info), compare_ctime);
        }
        else if (flags & 0x001) // -l option without -t, -u, or -c
        {
            qsort(*files, count, sizeof(file_info), compare_name);
        }
    }

    return count;
}

char get_file_type(mode_t mode)
{
    if (S_ISREG(mode))
        return '-';
    if (S_ISDIR(mode))
        return 'd';
    if (S_ISLNK(mode))
        return 'l';
    if (S_ISCHR(mode))
        return 'c';
    if (S_ISBLK(mode))
        return 'b';
    if (S_ISFIFO(mode))
        return 'p';
    if (S_ISSOCK(mode))
        return 's';
    return '?';
}

void get_permissions(mode_t mode, char *perms)
{
    static const char *rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    strcpy(&perms[0], rwx[(mode >> 6) & 7]);
    strcpy(&perms[3], rwx[(mode >> 3) & 7]);
    strcpy(&perms[6], rwx[(mode & 7)]);
    if (mode & S_ISUID)
        perms[2] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID)
        perms[5] = (mode & S_IXGRP) ? 's' : 'S';
    if (mode & 01000) // sticky bit
        perms[8] = (mode & S_IXOTH) ? 't' : 'T';
}

void colorize_file(file_info file)
{
    mode_t mode = file.st.st_mode;

    if (S_ISDIR(mode) && (mode & 01000))
    {
        // Sticky directory: white text on blue background
        printf("\033[37;44m");
    }
    else if (S_ISDIR(mode))
    {
        // Directory: blue
        printf("\033[34m");
    }
    else if (S_ISREG(mode) && (mode & S_IXUSR))
    {
        // Executable: green
        printf("\033[32m");
    }
    else if (S_ISREG(mode) && (mode & S_ISUID))
    {
        // Setuid: white text on red background
        printf("\033[37;41m");
    }
    else if (S_ISREG(mode) && (mode & S_ISGID))
    {
        // Setgid: black text on yellow background
        printf("\033[30;43m");
    }
    else if (S_ISCHR(mode) || S_ISBLK(mode))
    {
        // Block/Char device: bold yellow
        printf("\033[1;33m");
    }
    else if (S_ISFIFO(mode))
    {
        // FIFO: yellow
        printf("\033[33m");
    }
    else if (S_ISSOCK(mode))
    {
        // Socket: magenta
        printf("\033[35m");
    }
    else if (S_ISLNK(mode))
    {
        // Symlink: cyan
        printf("\033[36m");
    }
}

void print_output(file_info *files, int count, int flags)
{
    size_t max_pw_len = 0;
    size_t max_gr_len = 0;
    size_t max_size_len = 0;
    size_t max_inode_len = 0;
    size_t max_filename_len = 0;
    long long max_size = -1;

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminal_width = w.ws_col;

    for (int i = 0; i < count; i++)
    {
        struct passwd *pw = getpwuid(files[i].st.st_uid);
        struct group *gr = getgrgid(files[i].st.st_gid);
        if (pw && strlen(pw->pw_name) > max_pw_len)
            max_pw_len = strlen(pw->pw_name);
        if (gr && strlen(gr->gr_name) > max_gr_len)
            max_gr_len = strlen(gr->gr_name);
        if (files[i].st.st_size > max_size)
            max_size = files[i].st.st_size;

        size_t inode_len = snprintf(NULL, 0, "%lu", files[i].st.st_ino);
        if (inode_len > max_inode_len)
            max_inode_len = inode_len;

        size_t filename_len = strlen(files[i].name);
        if (filename_len > max_filename_len)
            max_filename_len = filename_len;
    }

    max_size_len = snprintf(NULL, 0, "%lld", max_size);

    if (flags & 0x001)
    {
        long long total_blocks = 0;
        for (int i = 0; i < count; i++)
            total_blocks += files[i].st.st_blocks;
        printf("total %lld\n", total_blocks / 2); //ls block size is 1024 
    }

    if (!(flags & 0x001) && !(flags & 0x100))
    {

        int col_width = max_filename_len + 2;
        int num_cols = terminal_width / col_width;
        if (num_cols == 0)
            num_cols = 1;
        int num_rows = (count + num_cols - 1) / num_cols;

        for (int row = 0; row < num_rows; row++)
        {
            for (int col = 0; col < num_cols; col++)
            {
                int index = col * num_rows + row;
                if (index < count)
                {
                    if (flags & 0x020)
                    { // -i option (inode)
                        printf("%*lu ", (int)max_inode_len, files[index].st.st_ino);
                    }

                    if (color)
                        colorize_file(files[index]);
                    printf("%-*s", col_width, files[index].name);
                    printf("\033[0m");
                }
            }
            printf("\n");
        }
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            if (flags & 0x020)
            { // -i option (inode)
                printf("%*lu ", (int)max_inode_len, files[i].st.st_ino);
            }

            if (flags & 0x001)
            { // Long listing format
                char perms[11] = {0};
                perms[0] = get_file_type(files[i].st.st_mode);
                get_permissions(files[i].st.st_mode, perms + 1);

                struct passwd *pw = getpwuid(files[i].st.st_uid);
                struct group *gr = getgrgid(files[i].st.st_gid);
                char time_str[20];
                time_t time_to_use;

                if (flags & 0x004) // -t option
                {
                    if (flags & 0x008) // -u option
                        time_to_use = files[i].st.st_atime;
                    else if (flags & 0x010) // -c option
                        time_to_use = files[i].st.st_ctime;
                    else
                        time_to_use = files[i].st.st_mtime;
                }
                else if (flags & 0x008) // -u option without -t
                    time_to_use = files[i].st.st_atime;
                else if (flags & 0x010) // -c option without -t
                    time_to_use = files[i].st.st_ctime;
                else
                    time_to_use = files[i].st.st_mtime;

                strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&time_to_use));

                printf("%s %2lu %-*s %-*s %*lld %s ",
                       perms,
                       files[i].st.st_nlink,
                       (int)max_pw_len, pw ? pw->pw_name : "",
                       (int)max_gr_len, gr ? gr->gr_name : "",
                       (int)max_size_len, (long long)files[i].st.st_size,
                       time_str);

                if (color)
                    colorize_file(files[i]);
                printf("%s", files[i].name);
                printf("\033[0m");

                if (S_ISLNK(files[i].st.st_mode) && files[i].linkname)
                {
                    printf(" -> %s", files[i].linkname);
                }
                printf("\n");
            }
            else
            {
                if (color)
                    colorize_file(files[i]);
                printf("%s%s", files[i].name, (flags & 0x100) ? "\n" : "  ");
                printf("\033[0m");
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int flags;
    char path[MAX_PATH];
    file_info *files = NULL;
    int capacity = 0;

    parse_input(argc, argv, &flags, path);

    int count = process_and_sort(path, flags, &files, &capacity);
    if (count < 0)
    {
        return EXIT_FAILURE;
    }

    print_output(files, count, flags);

    // Free allocated memory
    for (int i = 0; i < count; i++)
    {
        free(files[i].name);
        free(files[i].linkname);
    }
    free(files);

    for (int i = 0; i < currDir; i++)
    {
        free(currPath[i]);
    }
    free(currPath);

    if (!(flags & 0x080) && optind < argc - 1)
    {
        for (int i = optind + 1; i < argc; i++)
        {
            printf("\n%s:\n", argv[i]);
            count = process_and_sort(argv[i], flags, &files, &capacity);
            if (count > 0)
            {
                print_output(files, count, flags);
                for (int j = 0; j < count; j++)
                {
                    free(files[j].name);
                    free(files[j].linkname);
                }
                free(files);
            }
        }
    }

    return EXIT_SUCCESS;
}
