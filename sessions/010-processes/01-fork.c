#include <stdio.h>
#include <unistd.h>

int main() {
    int x = 5;
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        x = 99;
        printf("CHILD sees x = %d\n", x);
        printf("CHILD - my PID: %d, Parent's PID: %d\n", getpid(), getppid());
    } else {
        sleep(1);
        printf("PARENT sees x = %d\n", x);
        printf("PARENT - my PID: %d\n", getpid());
    }

    return 0;
}
