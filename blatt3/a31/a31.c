#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    printf("This solution has been created by Minh Nguyen\n");
    // Create semaphores
    int semid1 = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int semid2 = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);

    if (semid1 == -1 || semid2 == -1) {
        perror("semget");
        exit(1);
    }

    // Set semaphore initial values
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } semun1, semun2;
    semun1.val = 0;
    semun2.val = 0;

    if (semctl(semid1, 0, SETVAL, semun1) == -1 || semctl(semid2, 0, SETVAL, semun2) == -1) {
        perror("semctl");
        exit(1);
    }

    printf("Parent process created semaphores.\n");

    // Fork child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process

        printf("Child process waiting for input...\n");

        // Wait for parent to signal
        struct sembuf sembuf1 = {0, -1, 0};
        if (semop(semid1, &sembuf1, 1) == -1) {
            perror("semop");
            exit(1);
        }

        printf("Child process received input.\n");

        // Signal parent
        struct sembuf sembuf2 = {0, 1, 0};
        if (semop(semid2, &sembuf2, 1) == -1) {
            perror("semop");
            exit(1);
        }

        printf("Child process signaled parent.\n");

        exit(0);
    } else {
        // Parent process

        printf("Parent process created child process.\n");
        printf("Parent process waiting for input...\n");

        // Prompt for input
        char input[256];
        fgets(input, sizeof(input), stdin);

        // Signal child
        struct sembuf sembuf1 = {0, 1, 0};
        if (semop(semid1, &sembuf1, 1) == -1) {
            perror("semop");
            exit(1);
        }

        printf("Parent process signaled child.\n");

        // Wait for child to respond
        struct sembuf sembuf2 = {0, -1, 0};
        if (semop(semid2, &sembuf2, 1) == -1) {
            perror("semop");
            exit(1);
        }

        printf("Parent process received notification from child.\n");

        // Cleanup semaphores
        if (semctl(semid1, 0, IPC_RMID, semun1) == -1 || semctl(semid2, 0, IPC_RMID, semun2) == -1) {
            perror("semctl");
            exit(1);
        }

        printf("Parent process cleaned up semaphores.\n");

        exit(0);
    }
}
