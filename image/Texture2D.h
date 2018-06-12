#pragma once

#include <memory>
#include "../common/gl.h"
#include "RawImage.h"
#include "Texture.h"

namespace Core {

    class Texture2D: public Texture {
    public:
        virtual ~Texture2D();
        virtual void build(RawImage *imageData) = 0;

    protected:
        Texture2D();
    };
}
