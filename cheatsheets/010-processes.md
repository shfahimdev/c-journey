# Processes & Signals Cheatsheet

## Process Basics
```c
#include <unistd.h>     // fork, exec, getpid, getppid, sleep
#include <sys/wait.h>   // waitpid, WIFEXITED, WEXITSTATUS, WIFSIGNALED, WTERMSIG
#include <signal.h>     // signal, SIGINT, SIGTERM, SIGKILL, SIGCHLD, SIGPIPE
```

---

## fork()
```c
pid_t pid = fork();

if (pid < 0)  { perror("fork"); return 1; }  // failed
if (pid == 0) { /* YOU ARE THE CHILD  — pid == 0 */ }
else          { /* YOU ARE THE PARENT — pid == child's PID */ }
```
- Returns twice — once in each process
- Child inherits parent's memory, file descriptors, open files
- Separate memory after fork — changes in child don't affect parent
- Output order between parent and child is unpredictable

---

## exec()
```c
char *args[] = {"ls", "-l", NULL};   // NULL-terminated, args[0] = program name
execvp("ls", args);                  // searches PATH automatically
perror("execvp");                    // only reached if exec failed
```
- Replaces current process image with a new program
- Same PID, same open files — completely new code
- Nothing after execvp runs if it succeeds
- Use the fork→exec window to set up redirections, close fds, set env vars

---

## waitpid()
```c
int status;
waitpid(pid, &status, 0);          // block until child exits
waitpid(-1,  &status, 0);          // wait for any child
waitpid(-1,  NULL,    WNOHANG);    // don't block — reap if ready, return immediately
```

### Unpacking status
```c
if (WIFEXITED(status))    printf("exit code: %d\n", WEXITSTATUS(status));
if (WIFSIGNALED(status))  printf("killed by signal: %d\n", WTERMSIG(status));
```

---

## Signals

### Common signals
| Signal | # | Sent by | Default | Catchable |
|--------|---|---------|---------|-----------|
| `SIGINT` | 2 | Ctrl+C | Kill | ✅ |
| `SIGTERM` | 15 | `kill <pid>` | Kill | ✅ |
| `SIGKILL` | 9 | `kill -9 <pid>` | Kill | ❌ |
| `SIGCHLD` | 17 | kernel, child died | Ignore | ✅ |
| `SIGPIPE` | 13 | write to closed socket | Kill | ✅ |
| `SIGSEGV` | 11 | bad memory access | Kill+core | ✅ |

### Registering a handler
```c
void handler(int sig) {
    // sig = which signal arrived
    _exit(0);    // use _exit, NOT exit() — exit() is not signal-safe
}

signal(SIGINT,  handler);         // catch Ctrl+C
signal(SIGTERM, handler);         // catch kill <pid>
signal(SIGPIPE, SIG_IGN);         // ignore — handle write errors via return values
```

### Reap children on SIGCHLD (server pattern)
```c
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);  // reap all ready children
}
signal(SIGCHLD, handle_sigchld);
```

---

## Zombie Processes
- A child that exited but whose parent hasn't called `waitpid()` yet
- Shows as `Z` in `ps aux`, marked `<defunct>`
- Harmless alone — dangerous in quantity (exhausts process table)

```bash
ps aux | grep Z     # spot zombies
```

Fix: always `waitpid()` your children. In servers, use `SIGCHLD` + `WNOHANG`.

---

## Classic Fork-Per-Connection Server Pattern
```c
signal(SIGCHLD, handle_sigchld);   // reap children automatically
signal(SIGINT,  handle_sigint);    // graceful shutdown on Ctrl+C
signal(SIGPIPE, SIG_IGN);          // don't die on broken client connection

while (1) {
    int client_fd = accept(server_fd, NULL, NULL);

    pid_t pid = fork();
    if (pid == 0) {
        close(server_fd);           // child doesn't need the listening socket
        handle_request(client_fd);  // do the work
        close(client_fd);
        _exit(0);                   // child exits
    }
    close(client_fd);               // parent doesn't handle this one
}
```

---

## Useful Commands
```bash
ps aux              # list all processes
ps aux | grep Z     # find zombies
kill <pid>          # send SIGTERM
kill -9 <pid>       # send SIGKILL (uncatchable)
kill -l             # list all signal names and numbers
```
