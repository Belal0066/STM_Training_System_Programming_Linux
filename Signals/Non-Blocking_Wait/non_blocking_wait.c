#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> 


void safe_print(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
}


void sigchld_handler(int sig) {
    int status, saved_errno;
    pid_t child_pid;

    saved_errno = errno; 

    
    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) {
        char msg_buffer[256];

        if (WIFEXITED(status)) {
            snprintf(msg_buffer, sizeof(msg_buffer), "Child %ld terminated normally with exit status %d\n", (long)child_pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            snprintf(msg_buffer, sizeof(msg_buffer), "Child %ld terminated by signal %d (%s)\n", (long)child_pid, WTERMSIG(status), strsignal(WTERMSIG(status)));
        } else {
            snprintf(msg_buffer, sizeof(msg_buffer), "Child %ld terminated with unknown reason\n", (long)child_pid);
        }

        safe_print(msg_buffer); 
    }

    if (child_pid == -1 && errno != ECHILD) {
        safe_print("waitpid_err\n");
    }

    errno = saved_errno; 
}

int main() {
    struct sigaction sa;
    sigset_t mask;

    
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask_err");
        exit(EXIT_FAILURE);
    }

    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; 
    sa.sa_handler = sigchld_handler;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction_err");
        exit(EXIT_FAILURE);
    }

    
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
        perror("sigprocmask_err");
        exit(EXIT_FAILURE);
    }

    
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork_err");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            
            sleep(2 + i);  
            if (i == 0) {
                
                raise(SIGSEGV); 
            }
            exit(10 + i);   
        }
    }

    
    printf("Parenting...\n");
    while (1) {
        sleep(1); 
    }

    return 0;
}