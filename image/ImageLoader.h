#pragma once

#include <string>
#include <memory>

#ifdef CORE_USE_PRIVATE_INCLUDES
#include <IL/il.h>
#endif

#include "../common/types.h"
#include "../common/debug.h"
#include "../common/Exception.h"
#include "../image/RawImage.h"

namespace Core {

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

        static std::shared_ptr<StandardImage> loadImageU(const std::string& fullPath);
        static std::shared_ptr<StandardImage> loadImageU(const std::string& fullPath, Bool reverseOrigin);
        static std::string getFileExtension(const std::string& filePath);
    
    private:
        static Bool initialized;
        static Bool initialize();
#ifdef CORE_USE_PRIVATE_INCLUDES
        static std::shared_ptr<StandardImage> getStandardImageFromILData(const ILubyte * data, UInt32 width, UInt32 height);
#endif
    };

}
