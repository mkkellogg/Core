#pragma once

#include <memory>

#include "../common/gl.h"
#include "../image/Texture2D.h"

namespace Core {

    // forward declaration
    class GraphicsGL;
    class RawImage;

    class Texture2DGL final : public Texture2D {
        friend class GraphicsGL;

    public:
        ~Texture2DGL();

        void build(RawImage* imageData) override;

    protected:
        Texture2DGL(const TextureAttributes& attributes);
    };
}
