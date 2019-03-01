#pragma once

#include <string>

#include "../common/types.h"

namespace Core {

    class String {
    public:
        static std::string trimLeft(std::string& str);
        static std::string trimRight(std::string& str);
        static std::string trim(std::string& str);
        static std::string addLineNumbers(const std::string& src);
        static std::string toString(UInt32 number, UInt32 padTo = 0);
    };

}