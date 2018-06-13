#include "Object3D.h"
#include "Transform.h"
#include "../util/WeakPointer.h"

namespace Core {

  Object3D::Object3D(): transform(*this) {

  }

  Object3D::~Object3D() {

  }

  Transform& Object3D::getTransform() {
    return this->transform;
  }

  std::vector<std::shared_ptr<Object3D>>::const_iterator Object3D::beginIterateChildren() {
    return this->children.begin();
  }

  std::vector<std::shared_ptr<Object3D>>::const_iterator Object3D::endIterateChildren() {
    return this->children.end();
  }

  UInt32 Object3D::size() const {
    return this->children.size();
  }

  void Object3D::addObject(std::weak_ptr<Object3D> object) {
    if (WeakPointer<Object3D>::isInitialized(object)) {
      WeakPointer<Object3D> objPtr(object);
      if(WeakPointer<Object3D>::isInitialized(objPtr->parent)) {
        WeakPointer<Object3D> parentPtr(objPtr->parent);
        parentPtr->removeObject(object);
        Transform& parentTransform = parentPtr->getTransform();
        parentTransform.updateWorldMatrix();
        Matrix4x4 inverse(parentTransform.getWorldMatrix());
        inverse.invert();
        objPtr->getTransform().getLocalMatrix().preMultiply(inverse);
      }
      this->children.push_back(objPtr.getLockedPointer());
    }
  }

  void Object3D::removeObject(std::weak_ptr<Object3D> object) {
    std::vector<std::shared_ptr<Object3D>>::iterator result = this->children.end();
    WeakPointer<Object3D> objectPtr(object);
    for (auto itr = this->children.begin(); itr != this->children.end(); ++itr) {
      if (*itr == objectPtr.getLockedPointer()) {
        result = itr;
        break;
      }
    }
    if (result != this->children.end()) {
      Transform& transform = objectPtr->getTransform();
      transform.updateWorldMatrix();
      transform.getLocalMatrix().copy(transform.getWorldMatrix());
      this->children.erase(result);
    }
  }

  std::weak_ptr<Object3D> Object3D::getParent() {
    return this->parent;
  }
}