#pragma once

#include <string>
#include <memory>

#include "../common/types.h"
#include "../common/Exception.h"

namespace Core {

    class FileSystem {        
    public:

        class FileIOException: Exception {
        public:
            FileIOException(const std::string& msg): Exception(msg) {}
            FileIOException(const char* msg): Exception(msg) {}
        };

        virtual ~FileSystem();

        static std::shared_ptr<FileSystem> getInstance();
        std::string concatenatePaths(const std::string& pathA, const std::string& pathB) const;
        Bool fileExists(const std::string& fullPath) const;
        std::string getBasePath(const std::string& path) const;
        std::string getFileName(const std::string& fullPath) const;

        virtual Char getPathSeparator() const = 0;
        virtual std::string fixupPathForLocalFilesystem(const std::string& path) const = 0;
        
    protected:
        FileSystem();
    };
}
