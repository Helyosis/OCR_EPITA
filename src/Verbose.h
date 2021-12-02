#ifndef VERBOSE_H
#define VERBOSE_H

#ifdef DEFINE_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif
#define MAX_VERBOSE_LEVEL 3

// Defines colors
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

EXTERN int VERBOSE_LEVEL;

void error_s(const char* format, ...);
void warn_s(const char* format, ...);
void log_s(const char* format, ...);
void info_s(const char* format, ...);

#endif
