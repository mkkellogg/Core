#pragma once

#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "../common/assert.h"
#include "../common/complextypes.h"
#include "../render/BaseRenderable.h"
#include "../render/ObjectRenderer.h"
#include "../render/ObjectRenderers.h"
#include "../render/Renderable.h"
#include "../util/PersistentWeakPointer.h"
#include "../util/ValueIterator.h"
#include "Object3DComponent.h"
#include "Transform.h"

namespace Core {

    // forward declarations
    class Mesh;
    class MeshRenderer;
    class Engine;

    class Object3D {
        friend class Engine;

    public:
        virtual ~Object3D();

        Transform& getTransform();
        GameObjectIterator<Object3D> beginIterateChildren();
        GameObjectIterator<Object3D> endIterateChildren();
        GameObjectIterator<Object3DComponent> beginIterateComponents();
        GameObjectIterator<Object3DComponent> endIterateComponents();
        UInt32 size() const;
        void addChild(WeakPointer<Object3D> object);
        void removeChild(WeakPointer<Object3D> object);
        WeakPointer<Object3D> getParent();
        Bool addComponent(WeakPointer<Object3DComponent> component);

    protected:
        Object3D();

        Transform transform;
        std::vector<PersistentWeakPointer<Object3D>> children;
        PersistentWeakPointer<Object3D> parent;
        std::vector<PersistentWeakPointer<Object3DComponent>> components;
    };
}
