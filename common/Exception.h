#pragma once

#include <string>
#include <iostream>

namespace Core {

    class Exception {
    public:
        Exception(const std::string& msg): msg(msg) {std::cerr << msg << std::endl;}
        Exception(const char* msg): msg(msg) {std::cerr << msg << std::endl;}

    private:
        std::string msg;
    };

    class NullPointerException: Exception {
    public:
        NullPointerException(const std::string& msg): Exception(msg) {}
        NullPointerException(const char* msg): Exception(msg) {}
    };

    class InvalidArgumentException: Exception {
    public:
        InvalidArgumentException(const std::string& msg): Exception(msg) {}
        InvalidArgumentException(const char* msg): Exception(msg) {}
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