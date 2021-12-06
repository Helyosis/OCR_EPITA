#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#pragma GCC diagnostic pop

#include "Verbose.h"

void error_s(const char* format, ...) {
    va_list args;
    va_start(args, format);

    printf(RED "[-] " RESET);
    vprintf(format, args);
    printf("\n");

    va_end(args);

    exit(1);
}

void warn_s(const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (VERBOSE_LEVEL >= 1) {
        printf(YEL "[-] " RESET);
        vprintf(format, args);
        printf("\n");
    }

    va_end(args);
}

void log_s(const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (VERBOSE_LEVEL >= 2) {
        printf(GRN "[+] " RESET);
        vprintf(format, args);
        printf(RESET "\n");
    }

    va_end(args);
}

void info_s(const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (VERBOSE_LEVEL >= 3) {
        printf(CYN "[*] " RESET);
        vprintf(format, args);
        printf( "\n");
    }

    va_end(args);
}
