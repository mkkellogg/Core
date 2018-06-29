#include <fstream>

#include "FileSystem.h"
#include "FileSystemIX.h"
//#include "FileSystemWin.h"
#include "../util/String.h"

namespace Core {

    FileSystem::FileSystem() {

    }

    FileSystem::~FileSystem() {

    }

    std::shared_ptr<FileSystem> FileSystem::getInstance() {

#ifdef _WIN32
        //return std::shared_ptr<FileSystem>(new FileSystemWin());
        return std::shared_ptr<FileSystem>(new FileSystemIX());
#else		
        return std::shared_ptr<FileSystem>(new FileSystemIX());
#endif
    }

    std::string FileSystem::concatenatePaths(const std::string& pathA, const std::string& pathB) const {
        std::string pathATrimmed = pathA;
        std::string pathBTrimmed = pathB;
        String::trim(pathATrimmed);
        String::trim(pathBTrimmed);

        Char separator = this->getPathSeparator();

        UInt32 iB = 0;
        while (iB < pathBTrimmed.size() && pathBTrimmed.c_str()[iB] != separator) iB++;

        Int32 iA = pathATrimmed.size() - 1;
        while (iA >= 0) {
            iA--;
            if (pathATrimmed.c_str()[iA + 1] == separator) break;
        }

        if (iA >= 0) {
            pathATrimmed = pathATrimmed.substr(0, iA + 1);
        }
        if (iB + 1 < pathBTrimmed.size()) {
            pathBTrimmed = pathBTrimmed.substr(iB + 1);
        }

        pathATrimmed.append(1, separator);
        return pathATrimmed + pathBTrimmed;

    }

    Bool FileSystem::fileExists(const std::string& fullPath) const {
        std::ifstream f(fullPath.c_str());
        Bool isGood = f.good();
        f.close();
        return isGood;
    }

    std::string FileSystem::getBasePath(const std::string& path) const {
        Char separator = this->getPathSeparator();
        size_t pos = path.find_last_of(separator);
        return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
    }

    std::string FileSystem::getFileName(const std::string& fullPath) const {
        Char separator = this->getPathSeparator();
        size_t pos = fullPath.find_last_of(separator);
        return (std::string::npos == pos) ? std::string() : fullPath.substr(pos + 1);
    }

}