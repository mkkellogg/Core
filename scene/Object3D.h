#pragma once

#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "../common/assert.h"
#include "../common/complextypes.h"
#include "../render/ObjectRenderer.h"
#include "../util/PersistentWeakPointer.h"
#include "../util/ValueIterator.h"
#include "Transform.h"

namespace Core {

    // forward declarations
    class Mesh;
    class MeshRenderer;
    class Engine;
    class Object3DComponent;

    class Object3D {
        friend class Engine;

    public:
        virtual ~Object3D();

        UInt32 getID();
        Transform& getTransform();
        SceneObjectIterator<Object3D> beginIterateChildren();
        SceneObjectIterator<Object3D> endIterateChildren();
        SceneObjectIterator<Object3DComponent> beginIterateComponents();
        SceneObjectIterator<Object3DComponent> endIterateComponents();
        UInt32 size() const;
        void addChild(WeakPointer<Object3D> object);
        void removeChild(WeakPointer<Object3D> object);
        WeakPointer<Object3D> getParent();
        Bool addComponent(WeakPointer<Object3DComponent> component);
        void setActive(Bool active);

    protected:
        Object3D();

        Transform transform;
        std::vector<PersistentWeakPointer<Object3D>> children;
        PersistentWeakPointer<Object3D> parent;
        PersistentWeakPointer<Object3D> _self;
        std::vector<PersistentWeakPointer<Object3DComponent>> components;
        Bool active;
        UInt32 id;

        static UInt32 _nextID;
    };
}
