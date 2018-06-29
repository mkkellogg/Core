#include "String.h"

static const std::string& TRIM_CHARS = "\t\n\v\f\r ";

namespace Core {

    std::string String::trimLeft(std::string& str) {
        str.erase(0, str.find_first_not_of(TRIM_CHARS));
        return str;
    }

    std::string String::trimRight(std::string& str) {
        str.erase(str.find_last_not_of(TRIM_CHARS) + 1);
        return str;
    }

    std::string String::trim(std::string& str) {
        String::trimRight(str);
        String::trimLeft(str);
        return str;
    }

}
