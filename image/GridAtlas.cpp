
#include "GridAtlas.h"

namespace Core {

    GridAtlas::GridAtlas() {
        this->HorizontalSections = 0;
        this->VerticalSections = 0;
    }

    GridAtlas::GridAtlas(WeakPointer<Texture2D> texture, UInt32 horizontalSections, UInt32 verticalSections) {
        this->AtlasTexture = texture;
        this->HorizontalSections = horizontalSections;
        this->VerticalSections = verticalSections;
    }

    GridAtlas::~GridAtlas() {

    }
}