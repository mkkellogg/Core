#pragma once

#include <string>

#include "png.h"

#include "../common/types.h"
#include "../common/Exception.h"

namespace Core {

    //forward declarations
    class RawImage;

    class PNGLoader {
    public:

        class PNGLoaderException: Exception {
        public:
            PNGLoaderException(const std::string& msg): Exception(msg) {}
            PNGLoaderException(const char* msg): Exception(msg) {}
        };

        static RawImage * loadPNG(const std::string& path);

    };

}
