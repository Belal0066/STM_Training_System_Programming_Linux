# **Real-Time Signals Communication**

## **Overview**
This project demonstrates inter-process communication (IPC) in Linux using **real-time signals** to simulate terminate and abort functionality. It consists of two files: 

1. **Receiver (rcv.c):** Listens for real-time signals and performs specific actions based on the received signal's data.
2. **Sender (snd.c):** Sends real-time signals with associated data to the receiver process.

---

## **Features**
- Uses **real-time signals (`SIGRTMIN`)** for reliable communication.
- **Signal Data Handling:**
  - **0:** Receiver terminates gracefully.
  - **1:** Receiver aborts with a core dump.
- The use of Signals:
  - `sigaction` for setting up advanced signal handlers.
  - `sigqueue` for sending signals with additional data.

---



## **How It Works**
### Receiver (`rcv.c`)
- Registers a signal handler for `SIGRTMIN` using `sigaction`.
- Processes signal data received via the `si_value.sival_int` field of `siginfo_t`.
- Performs the following:
  - **Data `0`:** Logs a termination message and exits.
  - **Data `1`:** Logs an abort message and triggers a core dump.
  - Logs a message for any other data.

### Sender (`snd.c`)
- Accepts the following command-line arguments:
  - **PID:** Process ID of the receiver.
  - **Data:** Integer value (`0` or `1`).
- Uses `sigqueue` to send `SIGRTMIN` with the specified data to the receiver.

---

## **Setup and Usage**

### **Compilation**

```bash
gcc -o r rcv.c
gcc -o s snd.c
```

### **Running the Receiver**
Start the receiver application to listen for real-time signals:

```bash
./r
```

- The receiver will print its **PID** (Process ID) upon launch.

### **Sending Signals**
Use the sender application to send a signal to the receiver:

```bash
./s <pid> <0 or 1>
```

#### Arguments:
- **`<pid>`**: Process ID of the running receiver (printed by the receiver).
- **`<0 or 1>`**: Integer data to send:
  - **0**: Terminates the receiver.
  - **1**: Aborts the receiver with a core dump.

#### Example:
![image](https://github.com/user-attachments/assets/b5910d94-a5c1-43a8-857b-0dc6e59f6cb5)


---

## **Technical Highlights**
- **Real-Time Signals:** Uses `SIGRTMIN` for queuing and passing additional data.
- **`sigaction`:** Allows access to extended signal data using the `SA_SIGINFO` flag.
- **`sigqueue`:** Sends signals with payload data encapsulated in a `union sigval`.

