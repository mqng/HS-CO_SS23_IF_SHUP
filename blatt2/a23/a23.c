#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int i = 0;

void handler(int signal) {
	printf("Signal %d received\n", signal);
	if (++i == 3) {
		printf("Exiting with code 5\n");
		exit(5);
	}
}
int main() {
    printf("This solution has been created by Minh Nguyen\n");

	printf("Waiting for signals\n");
	signal(SIGINT, handler);
	signal(SIGTERM, handler);
	while(1);
	return 0;
}
