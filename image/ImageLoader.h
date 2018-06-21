#pragma once

#ifdef CORE_USE_PRIVATE_INCLUDES
#include "png.h"
#endif

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


    class ImageLoader {
    public:

        class ImageLoaderException: Exception {
        public:
            ImageLoaderException(const std::string& msg): Exception(msg) {}
            ImageLoaderException(const char* msg): Exception(msg) {}
        };

        static RawImage * loadImageU(const std::string& fullPath);
        static RawImage * loadImageU(const std::string& fullPath, Bool reverseOrigin);

        static void destroyRawImage(RawImage * image);
        static std::string getFileExtension(const std::string& filePath);
    
    private:
        static Bool initialized;
        static Bool initialize();
    };

}
