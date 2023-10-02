#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

	printf("This solution has been created by Minh Nguyen\n");

	if (argc !=5) {
		printf("Please use 4 arguments\n");
		return 1;
	}

	int pid_child1, pid_child2, pid_child3, status;

	pid_child1 = fork();

	if (pid_child1 == 0) { printf("Child 1 PID: %d | Argument: %s\n", getpid(), argv[2]); while(1); }

	else {
		printf("Parent PID: %d | Argument: %s\n", getpid(), argv[1]);

		pid_child3 = fork();
		if(pid_child3 == 0){ printf("Child 3 PID: %d | Argument: %s\n", getpid(), argv[4]); sleep(1); return 2;}

		else {
			pid_child2 = fork();
			if(pid_child2 == 0) {printf("Child 2 PID: %d | Argument: %s\n", getpid(), argv[3]); while(1);}
		}

		sleep(2);
		kill(pid_child1, 15);
		kill(pid_child2, 9);
		kill(pid_child3, 9);

		waitpid(pid_child1, &status,0);
		if(WIFSIGNALED(status)) { printf("Child 1 exited with signal: %d\n", WTERMSIG(status)); }
		else { printf("Child 1 exited with status: %d\n", WEXITSTATUS(status)); }

		waitpid(pid_child2, &status,0);
		if(WIFSIGNALED(status)) { printf("Child 2 exited with signal: %d\n", WTERMSIG(status)); }
		else { printf("Child 2 exited with status: %d\n", WEXITSTATUS(status)); }

		waitpid(pid_child3, &status,0);
		if(WIFSIGNALED(status)) { printf("Child 3 exited with signal: %d\n", WTERMSIG(status)); }
		else { printf("Child 3 exited with status: %d\n", WEXITSTATUS(status)); }
	}
	return 0;
}
