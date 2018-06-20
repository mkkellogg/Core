#include "Object3D.h"
#include "Transform.h"

namespace Core {

    Object3D::Object3D() : transform(*this), active(true) {
    }

    Object3D::~Object3D() {
    }

    Transform& Object3D::getTransform() {
        return this->transform;
    }

    GameObjectIterator<Object3D> Object3D::beginIterateChildren() {
        return  GameObjectIterator<Object3D>(this->children.begin());
    }

    GameObjectIterator<Object3D> Object3D::endIterateChildren() {
        return GameObjectIterator<Object3D>(this->children.end());
    }

    GameObjectIterator<Object3DComponent> Object3D::beginIterateComponents() {
        return GameObjectIterator<Object3DComponent>(this->components.begin());
    }

    GameObjectIterator<Object3DComponent> Object3D::endIterateComponents() {
        return GameObjectIterator<Object3DComponent>(this->components.end());
    }

    UInt32 Object3D::size() const {
        return this->children.size();
    }

    void Object3D::addChild(WeakPointer<Object3D> object) {
        if (object->parent.isValid()) {
            object->parent->removeChild(object);
            Transform& parentTransform = object->parent->getTransform();
            parentTransform.updateWorldMatrix();
            Matrix4x4 inverse(parentTransform.getWorldMatrix());
            inverse.invert();
            object->getTransform().getLocalMatrix().preMultiply(inverse);
        }
        this->children.push_back(object);
    }

    void Object3D::removeChild(WeakPointer<Object3D> object) {
        auto end = this->endIterateChildren();
        auto result = end;
        for (GameObjectIterator<Object3D> itr = this->beginIterateChildren(); itr != end; ++itr) {
            if (*itr == object) {
                result = itr;
                break;
            }
        }
        if (result != end) {
            Transform& transform = object->getTransform();
            transform.updateWorldMatrix();
            transform.getLocalMatrix().copy(transform.getWorldMatrix());
            this->children.erase(result.getSrc());
        }
    }

    WeakPointer<Object3D> Object3D::getParent() {
        return this->parent;
    }

    Bool Object3D::addComponent(WeakPointer<Object3DComponent> component) {
        GameObjectIterator<Object3DComponent> end = this->endIterateComponents();
        GameObjectIterator<Object3DComponent> result = end;
        for(GameObjectIterator<Object3DComponent> itr = this->beginIterateComponents(); itr != end; ++itr) {
            // don't add component if it already is present in list
            if (component == *itr) {
                return false;
            }
        }
        this->components.push_back(component);
        return true;
    }

    void Object3D::setActive(Bool active) {
        this->active = active;
    }
}