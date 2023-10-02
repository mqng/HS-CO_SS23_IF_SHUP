#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main(void) {
    printf("This solution has been created by Minh Nguyen\n");
    char command[100];
    char *arguments[100];
    char *token;

    char *cwd = getcwd(NULL,0);

    while (1) {

        printf("sh %s $ ",cwd);
        fgets(command, 100, stdin);
        // Remove newline
        size_t length = strlen(command);
            if (command[length - 1] == '\n') {
            command[length - 1] = '\0';
        }
        int is_whitespace = 1;
        for (int i = 0; i < length; i++) {
            if (!isspace(command[i])) {
                is_whitespace = 0;
                break;
            }
        }

        int argcount = 0;
        token = strtok(command, " ");
        while (token != NULL && argcount < 100) {
            arguments[argcount++] = token;
            token = strtok(NULL, " ");
        }
        arguments[argcount] = NULL;

        if (argcount == 0 || is_whitespace) {
            continue;
        }
        if (strcmp(arguments[0], "exit") == 0) {
            break;
        }

        int pid = fork();
        if (pid==0) {
            char *path = getenv("PATH");
            char *colon = strchr(path, ':');

            while (colon != NULL) {
                // Construct the full path to the executable
                char full_path[100];
                strncpy(full_path, path, colon - path);
                full_path[colon - path] = '/';
                strncpy(full_path + (colon - path) + 1, arguments[0], 100 - (colon - path) - 1);

                // Attempt to execute the command
                execv(full_path, arguments);

                // Move on to the next PATH component
                path = colon + 1;
                colon = strchr(path, ':');
            }

            // Try one last time with the last PATH component
            char full_path[100];
            strncpy(full_path, path, 100 - 1);
            full_path[100 - 1] = '\0';
            strcat(full_path, "/");
            strcat(full_path, arguments[0]);
            execv(full_path, arguments);

            // If execv() returns, the command was not found
            fprintf(stderr, "%s: command not found\n", arguments[0]);
            exit(EXIT_FAILURE);
        }
        else {
            int status;
            waitpid(pid, &status,0);
        }
    }
    free(cwd);
    return 0;
}
