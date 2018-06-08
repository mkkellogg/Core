#pragma once

#include <string>

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
