#include "Object3D.h"
#include "Transform.h"

namespace Core {

  Object3D::Object3D(): transform(*this), parent(nullptr) {

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

  void Object3D::addObject(std::shared_ptr<Object3D> object) {
    if (object->parent) {
      object->parent->removeObject(object);
      Transform& parentTransform = object->parent->getTransform();
      parentTransform.updateWorldMatrix();
      Matrix4x4 inverse(parentTransform.getWorldMatrix());
      inverse.invert();
      object->getTransform().getLocalMatrix().preMultiply(inverse);
    }
    this->children.push_back(object);
  }

  void Object3D::removeObject(std::shared_ptr<Object3D> object) {
    std::vector<std::shared_ptr<Object3D>>::iterator result = this->children.end();
    for (auto itr = this->children.begin(); itr != this->children.end(); ++itr) {
      if (*itr == object) {
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

  std::shared_ptr<Object3D> Object3D::getParent() {
    return this->parent;
  }
}