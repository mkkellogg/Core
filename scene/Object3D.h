#pragma once

#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "../common/assert.h"
#include "../common/complextypes.h"
#include "../base/CoreObject.h"
#include "../render/Object3DRenderer.h"
#include "../util/PersistentWeakPointer.h"
#include "../util/ValueIterator.h"
#include "Transform.h"

namespace Core {

    // forward declarations
    class Mesh;
    class MeshRenderer;
    class Engine;
    class Object3DComponent;
    class BaseObject3DRenderer;
    class BaseRenderableContainer;
    class MeshContainer;
    class MeshRenderer;
    class Light;
    class DirectionalLight;
    class PointLight;
    class AmbientLight;
    class AmbientIBLLight;
    class ReflectionProbe;
    class Camera;
    class ParticleSystem;
    class ParticleSystemRenderer;

    class Object3D: public CoreObject {

        friend class Engine;

    public:
        virtual ~Object3D() override;

        UInt64 getID() const;
        Transform& getTransform();
        SceneObjectIterator<Object3D> beginIterateChildren();
        SceneObjectIterator<Object3D> endIterateChildren();
        SceneObjectIterator<Object3DComponent> beginIterateComponents();
        SceneObjectIterator<Object3DComponent> endIterateComponents();
        UInt32 childCount() const;
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

        WeakPointer<BaseObject3DRenderer> getBaseRenderer();
        WeakPointer<MeshRenderer> getMeshRenderer();
        WeakPointer<ParticleSystemRenderer> getParticleSystemRenderer();

        WeakPointer<BaseRenderableContainer> getBaseRenderableContainer();
        WeakPointer<MeshContainer> getMeshContainer();

        WeakPointer<Light> getLight();
        WeakPointer<DirectionalLight> getDirectionalLight();
        WeakPointer<PointLight> getPointLight();
        WeakPointer<AmbientLight> getAmbientLight();
        WeakPointer<AmbientIBLLight> getAmbientIBLLight();

        WeakPointer<ReflectionProbe> getReflectionProbe();
        WeakPointer<Camera> getCamera();

        WeakPointer<ParticleSystem> getParticleSystem();

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

        WeakPointer<BaseObject3DRenderer> baseRenderer;
        WeakPointer<MeshRenderer> meshRenderer;
        WeakPointer<ParticleSystemRenderer> particleSystemRenderer;

        WeakPointer<BaseRenderableContainer> baseRenderableContainer;
        WeakPointer<MeshContainer> meshContainer;

        WeakPointer<ReflectionProbe> reflectionProbe;
        WeakPointer<Camera> camera;

        WeakPointer<Light> light;
        WeakPointer<DirectionalLight> directionalLight;
        WeakPointer<PointLight> pointLight;
        WeakPointer<AmbientLight> ambientLight;
        WeakPointer<AmbientIBLLight> ambientIBLLight;

        WeakPointer<ParticleSystem> particleSystem;

    private:
        static UInt64 getNextID();
        static UInt64 _nextID;

        template <typename T>
        Bool testAndSetComponentMemberVar(WeakPointer<Object3DComponent> component, WeakPointer<T>& memberVar, const std::string& errComponentName) {
            WeakPointer<T> derivedComponent = WeakPointer<Object3DComponent>::dynamicPointerCast<T>(component);
            if (derivedComponent.isValid()) {
                if(memberVar.isValid()) {
                    throw Exception(std::string("Object3D::addComponent() -> Trying to add more than one ") + errComponentName + ".");
                } else {
                    memberVar = derivedComponent;
                    return true;
                }
            }
            return false;
        }
        
    };
}
