#pragma once

enum class EngineRenderQueue {
    Geometry = 1000,
    AlphaClippedGeometry = 2000,
    Transparent = 3000
};