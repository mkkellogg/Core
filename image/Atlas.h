#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "Texture2D.h"

namespace Core {

    class Atlas {
    public:

        class FrameSetDescriptor {
        public:
            FrameSetDescriptor(UInt32 length, Real x, Real y, Real width, Real height) {
                this->length = length;
                this->x = x;
                this->y = y;
                this->width = width;
                this->height = height;
            }

            UInt32 length;
            Real x;
            Real y;
            Real width;
            Real height;
        };     

        Atlas();
        Atlas(WeakPointer<Texture2D> texture);
        virtual ~Atlas();

        WeakPointer<Texture2D> getTexture();
        UInt32 getFrameSetCount () const;
        void addFrameSet(UInt32 length, Real x, Real y, Real width, Real height);
        FrameSetDescriptor& getFrameSet(UInt32 index);

    private:
        PersistentWeakPointer<Texture2D> texture;
        std::vector<FrameSetDescriptor> frameSets;
    };
}
