#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_iterations> <seed>\n", argv[0]);
        return 1;
    }
    printf("This solution has been created by Minh Nguyen\n");
    int num_iterations = atoi(argv[1]);
    int seed = atoi(argv[2]);

    // Create shared memory space
    int *shared_mem = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Initialize RNG with seed
    srand(seed);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process reads from shared memory
        for (int i = 0; i < num_iterations; i++) {
            int num = shared_mem[i];
            printf("Child reads: %d\n", num);
        }
    } else {
        // Parent process writes to shared memory
        for (int i = 0; i < num_iterations; i++) {
            int num = rand();
            shared_mem[i] = num;
            printf("Parent wrote: %d\n", num);
        }
    }

    return 0;
}
