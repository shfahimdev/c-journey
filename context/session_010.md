# Session 010 — Processes & Signals

**Date:** 2026-03-24
**Duration:** ~1 session
**Folder:** `10-processes/`

---

## What We Covered

### What is a Process?
- A process is the OS's unit of isolation — a running program with its own memory, file descriptors, and PID
- Every process has a PID (process ID) and a parent PID
- `ps aux` lists all running processes — PID 1 is `init`/`systemd`, ancestor of everything

### fork()
- Duplicates the current process — same memory, same code, same open files
- Returns twice: `0` in the child, child's PID in the parent, `-1` on failure
- From the return of `fork()`, two independent processes run simultaneously
- The child inherits the result of everything before `fork()` — it doesn't re-execute it
- Modifying a variable in the child does NOT affect the parent — they have separate memory copies
- Output order between parent and child is unpredictable — the OS scheduler decides

```c
pid_t pid = fork();
if (pid < 0)  { perror("fork"); return 1; }  // error
if (pid == 0) { /* child */ }
else          { /* parent — pid holds child's PID */ }
```

### exec()
- Replaces the current process's memory with a completely different program
- Same PID, same open files — but entirely new code
- If `execvp` succeeds, nothing after it ever runs — the process is gone
- `args` array must be NULL-terminated, `args[0]` is the program name by convention
- The window between `fork()` and `exec()` is where you set up redirections, close fds, set env vars

```c
char *args[] = {"ls", "-l", NULL};
execvp("ls", args);
perror("execvp");   // only reached if exec failed
```

### waitpid()
- Parent blocks until the specified child exits, then reaps it
- Without `waitpid()`, dead children become zombies — entries stuck in the process table
- `status` is a packed integer — use macros to unpack it

```c
int status;
waitpid(pid, &status, 0);

if (WIFEXITED(status))   printf("exited: %d\n",  WEXITSTATUS(status));
if (WIFSIGNALED(status)) printf("killed by: %d\n", WTERMSIG(status));
```

### Signals
- Async interrupts delivered to a process from outside its normal flow
- Default for most signals is death — you can override handlers for all except SIGKILL and SIGSTOP
- `signal(SIGINT, handler)` registers a function to call when the signal arrives
- Inside signal handlers: use `_exit()` not `exit()` — `exit()` is not signal-safe

| Signal | Number | Sent by | Default |
|--------|--------|---------|---------|
| `SIGINT` | 2 | Ctrl+C | Kill |
| `SIGTERM` | 15 | `kill <pid>` | Kill |
| `SIGKILL` | 9 | `kill -9 <pid>` | Kill — uncatchable |
| `SIGCHLD` | 17 | kernel, child died | Ignore |
| `SIGPIPE` | 13 | kernel, write to closed socket | Kill |

```c
void handle_sigint(int sig) {
    printf("caught %d\n", sig);
    _exit(0);
}
signal(SIGINT, handle_sigint);
```

### Zombie Processes
- When a child exits, the kernel keeps a tiny process table entry until the parent calls `waitpid()`
- State shown as `Z` in `ps aux`, marked `<defunct>`
- Not dangerous alone — dangerous in quantity (exhausts the process table)
- Fix in a server: catch `SIGCHLD`, call `waitpid(-1, NULL, WNOHANG)` in a loop

```c
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
signal(SIGCHLD, handle_sigchld);
```

---

## Gotchas
- The clone starts at the return of `fork()` — not from the top of `main()`
- `exec` args array must end with `NULL` — otherwise exec reads garbage off the end
- `_exit()` inside signal handlers, never `exit()` — flush/cleanup functions aren't signal-safe
- `SIGKILL` cannot be caught — your handler will never fire for it
- After `free(p)` the address is still in `p` — same principle applies to process state after exit

---

## Files Written
- `01-fork.c` — fork, print PID in parent and child, run multiple times to see order change
- `02-exec.c` — fork + exec, child becomes `ls -l`
- `03-waitpid.c` — parent waits, prints exit status; tested both normal exit and kill -9
- `04-signals.c` — catch SIGINT, SIGTERM; confirmed SIGKILL cannot be caught
- `05-zombie.c` — created zombie, observed `Z+` `<defunct>` in `ps aux`, fixed with waitpid

---

## Observed in Terminal
- Output order between parent and child changed between runs — real scheduler non-determinism
- `kill -9` showed `Killed` printed by the shell, not by any handler — confirmed uncatchable
- `ps aux | grep zombie` showed PID with `Z+` state and `<defunct>` tag during the window
- Second `ps aux` after `waitpid()` — zombie entry completely gone

---

## Why This Matters for the HTTP Server
- Classic Unix server: parent forks a child per connection, child handles request and exits
- Parent catches `SIGCHLD` and reaps children with `WNOHANG` — no zombies accumulate
- `SIGTERM`/`SIGINT` handlers close sockets and free memory before shutdown
- `SIGPIPE` ignored with `SIG_IGN` — write errors handled via return values instead
- This entire pattern is Sessions 016–021

---

## Where We Stopped
Finished `10-processes`. All 5 exercises done. Zombie confirmed in `ps aux`.

## What's Next
- **Sockets I** — `socket()`, `bind()`, `listen()`, `accept()`, `read()`, `write()`
- The literal foundation of the HTTP server
