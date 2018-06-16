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

        WeakPointer(): std::weak_ptr<T>()  {
            
        }

        WeakPointer(std::weak_ptr<T> ptr) : std::weak_ptr<T>(ptr) {
            this->lockedPointer = expectWeakPointer(*this);
        }

        WeakPointer(std::shared_ptr<T> ptr) : std::weak_ptr<T>(ptr) {
            this->lockedPointer = expectWeakPointer(*this);
        }

        template <typename U>
        WeakPointer(const WeakPointer<U>& ptr) : std::weak_ptr<T>(ptr) {
            this->lockedPointer = expectWeakPointer(*this);
        }

        WeakPointer& operator =(const WeakPointer other) {
            if (&other == this) return *this;
            std::weak_ptr<T>::operator=(other);
            this->lockedPointer = other.lockedPointer;
            return *this;
        }

        Bool operator ==(const WeakPointer<T> other) {
            return this.lockedPointer == other.lockedPointer;
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

        Bool isValid() const {
            return isValid(*this);
        }

        operator bool() const { 
            return this->isValid();
        }

        static std::shared_ptr<T> expectWeakPointer(std::weak_ptr<T> ptr) {
            std::shared_ptr<T> sharedPtr = ptr.lock();
            if (!sharedPtr) {
                throw WeakPointerAssertionFailure("Expected valid weak pointer.");
            }
            return sharedPtr;
        }

        static Bool isInvalid(std::weak_ptr<T> const &weak) {
            return !isValid(weak);
        }

        static Bool isValid(std::weak_ptr<T> const &weak) {
            using wt = std::weak_ptr<T>;
            return weak.owner_before(wt{}) || wt{}.owner_before(weak);
        }

    private:
        std::shared_ptr<T> lockedPointer;
    };
}
