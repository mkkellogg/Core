#pragma once

namespace Core {

    enum class RenderPath {
        Forward = 0,
        Deferred = 1,
        SinglePassMultiLight = 2
    };

}