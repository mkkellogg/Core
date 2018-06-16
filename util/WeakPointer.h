#pragma once

#include <memory>

#include "../common/Exception.h"
#include "../common/types.h"

namespace Core {

    class WeakPointerAssertionFailure : AssertionFailedException {
    public:
        WeakPointerAssertionFailure(const std::string &msg) : AssertionFailedException(msg) {
        }
        WeakPointerAssertionFailure(const char *msg) : AssertionFailedException(msg) {
        }
    };

    template <typename T>
    class WeakPointer : public std::weak_ptr<T> {
    public:
        WeakPointer(std::weak_ptr<T> ptr) : std::weak_ptr<T>(ptr) {
            this->lockedPointer = expectWeakPointer(*this);
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

        operator bool() const { 
            return this->isInitialized();
        }

        static std::shared_ptr<T> expectWeakPointer(std::weak_ptr<T> ptr) {
            std::shared_ptr<T> sharedPtr = ptr.lock();
            if (!sharedPtr) {
                throw WeakPointerAssertionFailure("Expected valid weak pointer.");
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
