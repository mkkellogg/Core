#pragma once

#include "../util/PersistentWeakPointer.h"
#include "Texture2D.h"

namespace Core {

    class GridAtlas {
    public:
        GridAtlas();
        GridAtlas(WeakPointer<Texture2D> texture, UInt32 horizontalSections, UInt32 verticalSections);
        virtual ~GridAtlas();

        PersistentWeakPointer<Texture2D> AtlasTexture;
        UInt32 HorizontalSections;
        UInt32 VerticalSections;
    };
}
