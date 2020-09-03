#pragma once

#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../util/PersistentWeakPointer.h"
#include "../util/ObjectPool.h"

namespace Core {

     // forward declarations
    class Object3D;
    class BaseObjectRenderer;
    class BaseRenderable;

    class RenderQueue {
    public:

        class RenderItem {
        public:
            RenderItem(){}
            RenderItem(WeakPointer<BaseObjectRenderer> objectRenderer) {
                this->ObjectRenderer = objectRenderer;
            }
            PersistentWeakPointer<BaseObjectRenderer> ObjectRenderer;
            PersistentWeakPointer<BaseRenderable> Renderable;
        };

        RenderQueue(UInt32 id);

        UInt32 getID() const;
        UInt32 getItemCount() const;
        void clear();
        void addItem(WeakPointer<BaseObjectRenderer> objectRenderer, WeakPointer<BaseRenderable> renderable);
        RenderItem& getRenderItem(UInt32 index);

    protected:
        UInt32 id;
        ObjectPool<RenderItem> renderItemPool;
        std::vector<RenderItem> renderItems;

    private:
    };

}