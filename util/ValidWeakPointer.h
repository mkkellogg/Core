#pragma once

#include <memory>

#include "../common/Exception.h"

namespace Core {

    class ValidWeakPointerAssertionFailure: AssertionFailedException {
    public:
        ValidWeakPointerAssertionFailure(const std::string& msg): AssertionFailedException(msg) {}
        ValidWeakPointerAssertionFailure(const char* msg): AssertionFailedException(msg) {}
    };

    template <typename T>
    class ValidWeakPointer {
    public:

        ValidWeakPointer(std::weak_ptr<T> ptr): pointer(ptr) {
            this->lockedPointer = expectValidWeakPointer(this->pointer); 
        }

        static std::shared_ptr<T> expectValidWeakPointer(std::weak_ptr<T> ptr) {
            std::shared_ptr<T> sharedPtr = ptr.lock();
            if (!sharedPtr) {
                throw new ValidWeakPointerAssertionFailure("Expected valid weak pointer.");
            }
            return sharedPtr;
        }

        static bool isUninitialized(std::weak_ptr<T> const& weak) {
            using wt = std::weak_ptr<T>;
            return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
        }

        T* operator->() {
            return this->get();
        }

        T* get() {
            return this->lockedPointer.get();
        }

        std::shared_ptr<T> getLockedPointer() {
            return this->lockedPointer;
        }

    private:
        std::weak_ptr<T> pointer;
        std::shared_ptr<T> lockedPointer;
    };

}
