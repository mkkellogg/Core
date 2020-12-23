#pragma once

#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../util/PersistentWeakPointer.h"
#include "../util/ObjectPool.h"
#include "../util/SimpleObjectPool.h"
#include "RenderItem.h"

namespace Core {

     // forward declarations
    class Object3D;
    class BaseObject3DRenderer;
    class BaseRenderable;
    class MeshRenderer;
    class Mesh;
    class ParticleSystemRenderer;
    class ParticleSystem;

    class RenderList {
    public:

        RenderList();

        UInt32 getItemCount() const;
        void clear();
        void addItem(WeakPointer<BaseObject3DRenderer> renderer, WeakPointer<BaseRenderable> renderable, Bool isStatic, Bool isActive, Int32 layer);
        void addMesh(WeakPointer<MeshRenderer> meshRenderer, WeakPointer<Mesh> mesh, Bool isStatic, Bool isActive, Int32 layer);
        void addParticleSystem(WeakPointer<ParticleSystemRenderer> particleSystemRenderer, WeakPointer<ParticleSystem> particleSystem, Bool isStatic, Bool isActive, Int32 layer);
        RenderItem& getRenderItem(UInt32 index);
        void setAllActive();

    protected:
        void initRenderItem(RenderItem& renderItem, Bool isStatic, Bool isActive, Int32 layer);
        SimpleObjectPool<RenderItem> renderItemPool;
        std::vector<RenderItem*> renderItems;
        UInt32 itemCount;

    private:
    };
}