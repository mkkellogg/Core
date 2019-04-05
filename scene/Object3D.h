#pragma once

#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "../common/assert.h"
#include "../common/complextypes.h"
#include "../base/CoreObject.h"
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

    class Object3D: public CoreObject {

        friend class Engine;

    public:
        virtual ~Object3D();

        UInt64 getID() const;
        Transform& getTransform();
        SceneObjectIterator<Object3D> beginIterateChildren();
        SceneObjectIterator<Object3D> endIterateChildren();
        SceneObjectIterator<Object3DComponent> beginIterateComponents();
        SceneObjectIterator<Object3DComponent> endIterateComponents();
        UInt32 size() const;
        void addChild(WeakPointer<Object3D> object);
        void removeChild(WeakPointer<Object3D> object);
        WeakPointer<Object3D> getParent() const;
        Bool addComponent(WeakPointer<Object3DComponent> component);
        void setActive(Bool active);
        Bool isActive() const;
        void setStatic(Bool objStatic);
        Bool isStatic() const;
        void setName(const std::string& name);
        const std::string& getName() const;
        UInt32 childCount();
        WeakPointer<Object3D> getChild(UInt32 index);

    protected:
        Object3D();

        Transform transform;
        std::vector<PersistentWeakPointer<Object3D>> children;
        PersistentWeakPointer<Object3D> parent;
        PersistentWeakPointer<Object3D> _self;
        std::vector<PersistentWeakPointer<Object3DComponent>> components;
        Bool active;
        Bool objStatic;
        UInt64 id;
        std::string name;

    private:
        static UInt64 getNextID();
        static UInt64 _nextID;
        
    };
}
