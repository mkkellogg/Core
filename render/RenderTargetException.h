#pragma once

#include "../common/Exception.h"

namespace Core {

    class RenderTargetException: Exception {
    public:
        RenderTargetException(const std::string& msg): Exception(msg) {}
        RenderTargetException(const char* msg): Exception(msg) {}
    };

}