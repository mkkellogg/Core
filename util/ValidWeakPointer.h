#pragma once

#include <memory>

#include "../common/Exception.h"
#include "../common/types.h"

namespace Core {

    class ValidWeakPointerAssertionFailure : AssertionFailedException {
    public:
        ValidWeakPointerAssertionFailure(const std::string &msg) : AssertionFailedException(msg) {
        }
        ValidWeakPointerAssertionFailure(const char *msg) : AssertionFailedException(msg) {
        }
    };

    template <typename T>
    class ValidWeakPointer : public std::weak_ptr<T> {
    public:
        ValidWeakPointer(std::weak_ptr<T> ptr) : std::weak_ptr<T>(ptr) {
            this->lockedPointer = expectValidWeakPointer(*this);
        }

        T *operator->() {
            return this->get();
        }

        T *get() {
            return this->lockedPointer.get();
        }

        std::shared_ptr<T> getLockedPointer() {
            return this->lockedPointer;
        }

        Bool isInitialized() {
            return isInitialized(*this);
        }

        static std::shared_ptr<T> expectValidWeakPointer(std::weak_ptr<T> ptr) {
            std::shared_ptr<T> sharedPtr = ptr.lock();
            if (!sharedPtr) {
                throw ValidWeakPointerAssertionFailure("Expected valid weak pointer.");
            }
            return sharedPtr;
        }

        static Bool isUninitialized(std::weak_ptr<T> const &weak) {
            using wt = std::weak_ptr<T>;
            return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
        }

        static Bool isInitialized(std::weak_ptr<T> const &weak) {
            using wt = std::weak_ptr<T>;
            return weak.owner_before(wt{}) || wt{}.owner_before(weak);
        }

    private:
        std::shared_ptr<T> lockedPointer;
    };
}
