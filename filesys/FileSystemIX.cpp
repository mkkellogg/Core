#include <memory.h>
#include <iostream>
#include <fstream>

#include "FileSystemIX.h"

namespace Core {

    FileSystemIX::FileSystemIX() {

    }

    FileSystemIX::~FileSystemIX() {

    }

    Char FileSystemIX::getPathSeparator() const {
        return '/';
    }
  
    std::string FileSystemIX::fixupPathForLocalFilesystem(const std::string& path) const {
        const UInt32 size = (UInt32)path.size() + 1;
        std::unique_ptr<Char> chars = std::unique_ptr<Char>((Char *):: operator new (size * sizeof(Char), std::nothrow));
        if (chars.get() == nullptr) {
            throw FileIOException("FileSystemIX::fixupPathForLocalFilesystem -> Could not allocate path array.");
        }

        strcpy(chars.get(), path.c_str());

        Char sep = getPathSeparator();
        for (UInt32 i = 0; i < path.size(); i++) {
            if (chars.get()[i] == '\\') chars.get()[i] = sep;
        }

        std::string newPath = std::string(chars.get());

        return newPath;
    }


}
