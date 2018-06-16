#pragma once

#include <memory>

namespace Core {

    // forward declaration
    class Object3D;

    class Object3DComponent {
    public:
        Object3DComponent(std::weak_ptr<Object3D> owner);
        virtual ~Object3DComponent();

        std::weak_ptr<Object3D> getOwner() {
            return this->owner;
        }

    protected:
        std::weak_ptr<Object3D> owner;
    };
}
