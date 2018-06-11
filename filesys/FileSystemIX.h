#pragma once

#include <string>

#include "FileSystem.h"

namespace Core {

    class FileSystemIX : public FileSystem {
        friend class FileSystem;

    public:
        ~FileSystemIX();

        Char getPathSeparator() const override;
        std::string fixupPathForLocalFilesystem(const std::string& path) const override;

    protected:
        FileSystemIX();
    };

}
