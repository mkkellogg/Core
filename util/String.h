#pragma once

#include <string>

namespace Core {

    class String {
    public:
        static std::string trimLeft(std::string& str);
        static std::string trimRight(std::string& str);
        static std::string trim(std::string& str);
    };

}