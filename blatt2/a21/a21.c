#include <stdio.h>
#include <unistd.h>

int main() {

	printf("This solution has been created by Minh Nguyen\n");

	int pid;
	pid = fork();

	if (pid == 0) {
		printf("Child PID: %d\n", getpid());
		printf("Parent PID: %d\n", getppid());
	}
	else {
		printf("Parent PID: %d\n", getpid());
		printf("Parent Parent PID: %d\n", getppid());
	}

	while(1);

	return 0;
}
