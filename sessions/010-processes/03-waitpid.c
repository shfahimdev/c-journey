#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("child  — working...\n");
        sleep(10);
        printf("child  — done, exiting with 42\n");
        return 42;
    } else {
        printf("Parent - waiting for child %d\n", pid);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("parent — child exited normally, status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("parent — child killed by signal: %d\n", WTERMSIG(status));
        }
    }

    return 0;
}
