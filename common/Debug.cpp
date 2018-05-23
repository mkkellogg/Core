#include "debug.h"
#include "stdio.h"
#include <stdarg.h>

void Debug::PrintMessage(const char* fmt...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void Debug::PrintError(const char* fmt...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}