#pragma once

#include <string>

namespace Core {

    class String {
    public:
        static std::string trimLeft(const std::string& str);
        static std::string trimRight(const std::string& str);
        static std::string trim(const std::string& str);
    };

}