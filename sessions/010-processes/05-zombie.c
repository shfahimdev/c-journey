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
        printf("child  — exiting now, watch me become a zombie\n");
        return 0; // child exits immediately
    }

    // parent does NOT call waitpid yet
    // child is now a zombie — dead but not reaped
    printf("parent — child %d is now a zombie. Sleeping 30s...\n", pid);
    printf("parent — run this in another terminal: ps aux | grep zombie\n");
    sleep(30);

    // now fix it
    printf("parent — reaping the child now\n");
    int status;
    waitpid(pid, &status, 0);
    printf("parent — child reaped. Sleeping 30s — check ps again, zombie is gone\n");
    sleep(30);

    printf("parent — done\n");
    return 0;
}
