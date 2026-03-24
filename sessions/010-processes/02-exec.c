#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        char *args[] = {"ls", "-l", NULL};
        execvp("ls", args);

        // only reaches if exec fails
        perror("execvp");
        return 1;
    } else {
        printf("PARENT - Spawned child %d, continuing...\n", pid);
    }

    return 0;
}
