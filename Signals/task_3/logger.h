#ifndef LOGGER_H
#define LOGGER_H

#include <signal.h>

// Enum representing log levels
typedef enum {
    LOG_LEVEL_DISABLED = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

// Initialize logger with a base signal and an optional log file
void init_logger(int base_signal, const char *log_file);

// Logging functions
void LOG_ERROR(const char *format, ...);
void LOG_WARNING(const char *format, ...);
void LOG_INFO(const char *format, ...);
void LOG_DEBUG(const char *format, ...);

// Utility functions
LogLevel get_log_level(void);
void set_log_level(LogLevel level);

#endif // LOGGER_H
