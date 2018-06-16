#include "Object3D.h"
#include "Transform.h"

namespace Core {

    Object3D::Object3D() : transform(*this) {
    }

    Object3D::~Object3D() {
    }

    Transform& Object3D::getTransform() {
        return this->transform;
    }

    std::vector<std::shared_ptr<Object3D>>::iterator Object3D::beginIterateChildren() {
        return this->children.begin();
    }

    std::vector<std::shared_ptr<Object3D>>::iterator Object3D::endIterateChildren() {
        return this->children.end();
    }

    std::vector<WeakPointer<Object3DComponent>>::iterator Object3D::beginIterateComponents() {
        return this->components.begin();
    }

    std::vector<WeakPointer<Object3DComponent>>::iterator Object3D::endIterateComponents() {
        return this->components.end();
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
        this->children.push_back(object.getLockedPointer());
    }

    void Object3D::removeChild(WeakPointer<Object3D> object) {
        std::vector<std::shared_ptr<Object3D>>::iterator result = this->children.end();
        for (auto itr = this->children.begin(); itr != this->children.end(); ++itr) {
            if (*itr == object.getLockedPointer()) {
                result = itr;
                break;
            }
        }
        if (result != this->children.end()) {
            Transform& transform = object->getTransform();
            transform.updateWorldMatrix();
            transform.getLocalMatrix().copy(transform.getWorldMatrix());
            this->children.erase(result);
        }
    }

    WeakPointer<Object3D> Object3D::getParent() {
        return this->parent;
    }

    Bool Object3D::addComponent(WeakPointer<Object3DComponent> component) {
        for(auto itr = this->components.begin(); itr != this->components.end(); ++itr) {
            // don't add component if it already is present in list
            if (component == *itr) {
                return false;
            }
        }
        this->components.push_back(component);
        return true;
    }
}