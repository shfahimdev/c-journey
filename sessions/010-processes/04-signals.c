#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handle_sigint(int sig) {
    printf("\ncaught signal %d (SIGINT) — cleaning up...\n", sig);
    _exit(0);
}

void handle_sigterm(int sig) {
    printf("\ncaught signal %d (SIGTERM) — shutting down...\n", sig);
    _exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigterm);

    printf("running — try Ctrl+C or: kill %d\n", getpid());

    while (1) {
        printf("tick\n");
        sleep(1);
    }

    return 0;
}
