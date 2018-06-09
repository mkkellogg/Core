#pragma once

#include <IL/il.h>
#include <string>

#include "../common/types.h"
#include "../common/debug.h"
#include "../common/Exception.h"

namespace Core {

    //forward declarations
    class RawImage;

    enum ImageLoaderError {
        GeneralLoadError = 1
    };

    class ImageLoaderException: Exception {
    public:
        ImageLoaderException(const std::string& msg): Exception(msg) {}
        ImageLoaderException(const char* msg): Exception(msg) {}
    };

    class ImageLoader {
        static Bool ilInitialized;
        static Bool initialize();

    public:

        static RawImage * loadImageU(const std::string& fullPath);
        static RawImage * loadImageU(const std::string& fullPath, Bool reverseOrigin);
        static RawImage * getRawImageFromILData(const ILubyte * data, UInt32 width, UInt32 height);
        static void destroyRawImage(RawImage * image);
        static std::string getFileExtension(const std::string& filePath);
    };

}
