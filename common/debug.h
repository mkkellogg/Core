#pragma once

// #define DebugPrintError(msg) printf("%s\n", (msg));
// #define DebugPrintMessage(msg) printf("%s\n", (msg));

class Debug {
    public:

    static void PrintMessage(const char* fmt...);
    static void PrintError(const char* fmt...);
};
