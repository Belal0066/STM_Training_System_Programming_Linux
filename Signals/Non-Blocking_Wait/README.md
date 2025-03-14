# **Non-Blocking Wait for Child Processes**

Implementing a **non-blocking wait** alternative to the `wait` system call. It handles terminated child processes asynchronously, prints their PID, exit status, and termination reason (signal or normal exit), and ensures that no zombie processes are created.

---

## **Compilation**

To compile the program, use the following command:

```bash
gcc -o non_blocking_wait non_blocking_wait.c
```

---

## **Usage**

Run the compiled program:

```bash
./non_blocking_wait
```

---

## **Program Description**

1. **Sets up a `SIGCHLD` handler**:
   - The handler uses `waitpid` with the `WNOHANG` flag to reap terminated child processes without blocking the parent.
   - It prints the PID, exit status, and termination reason (signal or normal exit) for each child.

2. **Launches child processes**:
   - Three child processes are created using `fork`.
   - Each child sleeps for a different duration and then exits:
     - The first child raises a segmentation fault (`SIGSEGV`) to simulate a signal-induced termination.
     - The other two children exit normally with different exit codes.

3. **Parent process continues execution**:
   - The parent process continues executing its main loop (`while (1) { sleep(1); }`) while handling child terminations asynchronously.

---

## **Example Output**


```bash
Parenting...
Child 11532 terminated by signal 11 (Segmentation fault)
Child 11533 terminated normally with exit status 11
Child 11534 terminated normally with exit status 12
```

### **Explanation of the Output**
- **Parenting...**: The parent process continues executing its main loop after launching the child processes.
- **Child 11532 terminated by signal 11 (Segmentation fault)**: The first child process was terminated by a segmentation fault (signal 11, `SIGSEGV`).
- **Child 11533 terminated normally with exit status 11**: The second child process exited normally with an exit status of 11.
- **Child 11534 terminated normally with exit status 12**: The third child process exited normally with an exit status of 12.

---

## **Key Features**
- **Non-blocking wait**: Uses `WNOHANG` to avoid blocking the parent process.
- **Zombie prevention**: All terminated children are reaped promptly.
- **Async-signal safety**: Uses `write` for printing in the signal handler to avoid unsafe functions.
- **Race condition avoidance**: Blocks `SIGCHLD` during setup to prevent missed signals.

---

## **Reqs**
- A C compiler (eg. `gcc`).
- A POSIX-compliant operating system (Linux, macOS, etc..).


