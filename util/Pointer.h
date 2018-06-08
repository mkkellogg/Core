#pragma once

#include <memory>

#include "Exception.h"

namespace Core {

    template <typename T>
    class ValidWeakPointer {
    public:

        ValidWeakPointer(std::weak_ptr<T> ptr): pointer(ptr) {
            this->lockedPointer = expectValidWeakPointer(this->pointer); 
        }

        static std::shared_ptr<T> expectValidWeakPointer(std::weak_ptr<T> ptr) {
            std::shared_ptr<T> sharedPtr = ptr.lock();
            if (!sharedPtr) {
                throw new AssertionFailedException("Expected valid weak pointer.");
            }
            return sharedPtr;
        }

        T* operator->() {
            return this->get();
        }

        T* get() {
            return this->lockedPointer.get();
        }

    private:
        std::weak_ptr<T> pointer;
        std::shared_ptr<T> lockedPointer;
    };

}
