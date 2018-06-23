#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "../common/types.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class Engine;
    class Object3D;

    class Scene {
        friend class Engine;

    public:
        WeakPointer<Object3D> getRoot();

    private:
        Scene(WeakPointer<Object3D> root);

        PersistentWeakPointer<Object3D> root;
    };
}
