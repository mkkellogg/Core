#include "String.h"

namespace Core {

    std::string String::trimLeft(const std::string& str) {
        std::size_t first = str.find_first_not_of(' ');
        return str.substr(first, str.size());
    }

    std::string String::trimRight(const std::string& str) {
        std::size_t last = str.find_last_not_of(' ');
        return str.substr(0, last + 1);
    }

    std::string String::trim(const std::string& str) {
        std::size_t first = str.find_first_not_of(' ');
        std::size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }

}
