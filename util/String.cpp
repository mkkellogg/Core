#include "String.h"

#include <vector>

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

    std::string String::addLineNumbers(const std::string& src) {
        const UInt32 MAX_LINE_SIZE = 1024;
        std::vector<std::string> numberedLines;
        const char * cStr = src.c_str();
        UInt32 curStringPos = 0;
        UInt32 curLinePos = 0;
        UInt32 curLineStart = 0;
        UInt32 lineNo = 0;
        while (curStringPos < src.length()) {
            char curChar = cStr[curStringPos];
            if (curChar == '\n' || curLinePos >= MAX_LINE_SIZE - 1) {
                std::string line = toString(lineNo, 4) + std::string(": ") + src.substr(curLineStart, curLinePos - 1);
                numberedLines.push_back(line);
                curLinePos = 0;
                curLineStart = curStringPos + 1;
                lineNo++;
            }
            curLinePos++;
            curStringPos++;
        }

        std::string result;
        for (UInt32 i = 0; i < numberedLines.size(); i++) {
            result += numberedLines[i] + std::string("\n");
        }

        return result;
    }

    std::string String::toString(UInt32 number, UInt32 padTo) {
        UInt32 copy = number;
        UInt32 digits = 0;
        while (copy > 0) {
            digits++;
            copy /= 10;
        }

        if (number == 0) digits = 1;

        std::string result;
        while(digits < padTo) {
            result += "0";
            digits++;
        }
        result += std::to_string(number);
        return result;
    }
}
