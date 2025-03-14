# **Dynamically Configurable Logger with Signal Handling**

This project implements a **dynamically configurable logging system** that allows real-time log level changes using **signals**. The logger supports the following log levels:  

- **DISABLED**  
- **ERROR**  
- **WARNING**  
- **INFO**  
- **DEBUG**  

## **Compilation**  

```bash
gcc -o app main.c logger.c -lrt
gcc -o set_log_level set_log_level.c -lrt
```

## **Usage**  

### **1. Start the Application**  
Run the application in the background:  
```bash
./app &
```
This will print a **process ID (PID)**. You can also find the PID using:  
```bash
pgrep app
```

### **2. Change the Log Level**  
```bash
./set_log_level <PID> <log_level>
```
<log_level>:
- `disable`  
- `error`  
- `warning`  
- `info`  
- `debug`  


### **3. View Log Output in application.log**  
```bash
cat application.log
```

```bash
[INFO] Info message.
[WARNING] Warning message.
[ERROR] Error message.
```

### **4. Stop the Application**  
Bring the process back to the foreground:  
```bash
fg
```
Then, **terminate it** using:  
```bash
Ctrl+C
```

## **Example Run**  

```bash
❯ gcc -o app main.c logger.c -lrt
❯ gcc -o set_log_level set_log_level.c -lrt

❯ ./app & ./set_log_level $! info
[3] 29597
Log level set to info for process 29597

❯ cat application.log            
[INFO] Info message.
[WARNING] Warning message.
[ERROR] Error message.

❯ ./app & ./set_log_level $! error
[4] 29665
Log level set to error for process 29665

❯ cat application.log             
[INFO] Info message.
[WARNING] Warning message.
[ERROR] Error message.
[INFO] Info message.
[WARNING] Warning message.
[ERROR] Error message.

❯ fg
[4]  - 29665 running    ./app
^C

```

---
### **System Architecture**

1. **Main Application (main.c)**  
2. **Logger Module (logger.c & logger.h)**  
3. **Log Level Setter (set_log_level.c)**  

---

#### **System Components**  

1. **Main Application (main.c)**  
   - **Purpose**: This is the core part of the application. It starts the logging process and continuously logs messages based on the current log level.
   - **Key Tasks**:
     - Initializes the logger using `init_logger()`.
     - Logs messages of different levels (e.g., debug, info, warning, error) continuously in an infinite loop.
     - Responds to signals that adjust the log level (by calling `handle_signal()`).
   - **Interaction**:
     - It includes `logger.h` for function declarations and interacts with the **Logger Module** for logging.
     - Receives signals from the **Log Level Setter** to adjust the log level dynamically.

2. **Logger Module (logger.c & logger.h)**  
   - **Purpose**: The Logger Module is responsible for logging messages to a file based on the current log level. It also handles signal-based log level changes.
   - **Key Tasks**:
     - **`init_logger()`**: Initializes the logger and sets up signal handlers for dynamic log level changes.
     - **`handle_signal()`**: Receives signals (e.g., `SIGUSR1`, `SIGUSR2`) and updates the log level accordingly.
     - **`log_message()`**: Logs a message with a specific log level to a file (`application.log`).
     - **`safe_write()`**: Ensures that log messages are written safely to the log file, even in the case of interruptions.
   - **Interaction**:
     - It provides the logging functionality to the **Main Application** and interacts with it via the `logger.h` header.
     - It listens for signals from the **Log Level Setter** and adjusts its logging behavior in real-time.

3. **Log Level Setter (set_log_level.c)**  
   - **Purpose**: This component allows an external user to change the log level of the running application by sending signals to the process.
   - **Key Tasks**:
     - Accepts the process ID (PID) and the desired log level as command-line arguments.
     - Maps the desired log level (e.g., `debug`, `info`, `warning`) to a specific signal (e.g., `SIGUSR1`, `SIGUSR2`).
     - Sends the corresponding signal to the running process using `kill()` to change the log level.
   - **Interaction**:
     - It communicates with the **Main Application** and **Logger Module** by sending signals to change the log level dynamically.
     - It only sends signals and does not interact with the actual logging behavior beyond that.


