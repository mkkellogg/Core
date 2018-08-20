#pragma once

#include <memory>
#include <type_traits>
#include <vector>
#include <functional>

#include "../common/types.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class Engine;
    class Object3D;

    class Scene {
        friend class Engine;

    public:
        using VisitorCallback  = std::function<void(WeakPointer<Object3D> curObject)>;
        
        WeakPointer<Object3D> getRoot();
        void visitScene(WeakPointer<Object3D> object, VisitorCallback callback);
        
    private:
        Scene(WeakPointer<Object3D> root);

        PersistentWeakPointer<Object3D> root;
    };
}
