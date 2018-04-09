#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1 /* XPG 4.2 - needed for WCOREDUMP() */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


void printStartInfo(char *label) {
    printf("%s identification: \n", label); /*grandparent/parent/child */
    printf("	pid = %d,	ppid = %d,	pgrp = %d\n", getpid(), getppid(), getpgrp());
    printf("	uid = %d,	gid = %d\n", getuid(), getgid());
    printf("	euid = %d,	egid = %d\n", geteuid(), getegid());
}

void printFinishInfo(char *label, pid_t id, int status) {
    printf("%s exit (pid = %d):", label, id);
    if (WIFEXITED(status)) {
        printf("	normal termination (exit code = %d)\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
#ifdef WCOREDUMP
        printf("	signal termination %s(signal = %d)\n", WCOREDUMP(status) ? "with core dump" : "",
               WTERMSIG(status));
#else
        printf("	signal termination %s(signal = %d)\n", "", WTERMSIG(status));
#endif
    } else {
        printf("	unknown type of termination\n");
    }
}

int main(int arc, char **argv) {
    if (arc < 2) {
        fprintf(stderr, "At least one paremeter needed - program to execute in the child process\n");
        return 1;
    }
    printStartInfo("grandparent");
    int status;
    pid_t id = fork();
    if (id == -1) {
        perror("fork");
        return 1;
    } else if (id != 0) {
        // grandparent
        signal(SIGINT, SIG_IGN);
        waitpid(id, &status, 0);
        printFinishInfo("parent", id, status);
        return 0;
    } else {
        printStartInfo("parent");
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid != 0) {
            signal(SIGINT, SIG_IGN);
            waitpid(id,
                    &status, 0);
            printFinishInfo("child", pid, status
            );
            return 0;
        } else {
            printStartInfo("child");
            if(execv(argv[1], argv + 1) != 0){
                perror("execv");
                return 1;
            }
            return 0;
        };
    }
}