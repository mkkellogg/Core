#pragma once

#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../util/PersistentWeakPointer.h"
#include "../util/ObjectPool.h"
#include "RenderItem.h"

namespace Core {

     // forward declarations
    class Object3D;
    class BaseObjectRenderer;
    class BaseRenderable;

    class RenderList {
    public:

        RenderList();

        UInt32 getItemCount() const;
        void clear();
        void addItem(WeakPointer<BaseObjectRenderer> objectRenderer, WeakPointer<BaseRenderable> renderable, Bool isStatic);
        RenderItem& getRenderItem(UInt32 index);

    protected:
        ObjectPool<RenderItem> renderItemPool;
        std::vector<RenderItem*> renderItems;

    private:
    };
}