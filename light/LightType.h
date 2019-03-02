#pragma once

namespace Core {

    enum class LightType {
        Ambient = 0,
        AmbientIBL = 1,
        Directional = 2,
        Point = 3,
        Spot = 4,
        Planar = 5
    };

}
