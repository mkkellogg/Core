#pragma once

#include <string>
#include <memory>

#include "png.h"

#include "../common/types.h"
#include "../common/Exception.h"
#include "RawImage.h"

namespace Core {

    class PNGLoader {
    public:

        class PNGLoaderException: Exception {
        public:
            PNGLoaderException(const std::string& msg): Exception(msg) {}
            PNGLoaderException(const char* msg): Exception(msg) {}
        };

        static std::shared_ptr<StandardImage> loadPNG(const std::string& path);

    };

}
