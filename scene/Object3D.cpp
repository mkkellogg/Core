#include "Object3D.h"

namespace Core {

  Object3D::Object3D() {

  }

  Object3D::~Object3D() {

  }

  Transform& Object3D::getLocalTransform() {
    return this->localTransform;
  }

  Transform& Object3D::getWorldTransform() {
    return this->worldTransform;
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
    this->children.push_back(object);
  }
}