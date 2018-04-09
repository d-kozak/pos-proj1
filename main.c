#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1 /* XPG 4.2 - needed for WCOREDUMP() */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int arc, char **argv) {
    if(arc == 1){
        printf("%s",argv[0]);
    }


    int status;
    pid_t id = fork();
    if (id == -1) {
        perror("fork");
        return 1;
    } else if (id != 0) {
        // parent
        signal(SIGINT, SIG_IGN);
        waitpid(id, &status, 0);
        if (WIFEXITED(status)) {
            printf("child %d exited with %d\n", (int) id, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("child %d signaled with %d\n", (int) id, WTERMSIG(status));
        }
        return 0;
    } else {
        // child
        sleep(10);
        return 1;
    }
}