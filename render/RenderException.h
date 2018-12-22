#pragma once

#include "../common/Exception.h"

namespace Core {

    class RenderException: Exception {
    public:
        RenderException(const std::string& msg): Exception(msg) {}
        RenderException(const char* msg): Exception(msg) {}
    };

}