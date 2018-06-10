#pragma once

#include <string>

#include "FileSystem.h"

namespace Core {

    class FileSystemIX : public FileSystem {
        friend class FileSystem;

    public:
        ~FileSystemIX();

        Char getPathSeparator() const override;
        std::string concatenatePaths(const std::string& pathA, const std::string& pathB) const;
        std::string getBasePath(const std::string& path) const;
        std::string fixupPathForLocalFilesystem(const std::string& path) const;
        std::string getFileName(const std::string& fullPath) const;
        Bool fileExists(const std::string& fullPath) const;

    protected:
        FileSystemIX();
    };

}
