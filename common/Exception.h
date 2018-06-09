#pragma once

#include <string>

namespace Core {

    class Exception {
    public:
        Exception(const std::string& msg): msg(msg) {}
        Exception(const char* msg): msg(msg) {}

    private:
        std::string msg;
    };

    class AssertionFailedException: Exception {
    public:
        AssertionFailedException(const std::string& msg): Exception(msg) {}
        AssertionFailedException(const char* msg): Exception(msg) {}
    };

    class AllocationException: Exception {
    public:
        AllocationException(const std::string& msg): Exception(msg) {}
        AllocationException(const char* msg): Exception(msg) {}
    };

    class OutOfRangeException: Exception {
    public:
        OutOfRangeException(const std::string& msg): Exception(msg) {}
        OutOfRangeException(const char* msg): Exception(msg) {}
    };

}