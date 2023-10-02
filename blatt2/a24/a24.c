#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("This solution has been created by Minh Nguyen\n");
    int pid, status, exitcode;
    pid = fork();
    if (pid == 0) {
        char *execute[] = {"./P-new", NULL};
        execvp(execute[0], &argv[0]);
        printf("Error executing\n");
        exit(1);
    }
    else {
        printf("Waiting for child to exit\n");
        waitpid(pid, &status, 0);
        exitcode = status >> 8;
        printf("Process ended with PID %d and status %d or %d\n", pid, status, exitcode);
    }
    return 0;
}
