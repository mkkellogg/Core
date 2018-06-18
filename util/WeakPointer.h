#pragma once

#include <memory>

#include "../common/Exception.h"
#include "../common/types.h"


//#define __CORE_WEAK_POINTER_CACHE_SHARED true

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

        WeakPointer(Bool cacheShared = true): std::weak_ptr<T>(), _ptr(nullptr), _cacheShared(cacheShared), _cachedSharedSet(false)  {
        }

        WeakPointer(std::weak_ptr<T> ptr, Bool cacheShared = true) : std::weak_ptr<T>(ptr), _ptr(nullptr), _cacheShared(cacheShared), _cachedSharedSet(false) {
        }

        WeakPointer(std::shared_ptr<T> ptr, Bool cacheShared = true) : std::weak_ptr<T>(ptr), _cacheShared(cacheShared), _cachedSharedSet(false) {
            this->_ptr = ptr.get();
        }

        template <typename U>
        WeakPointer(WeakPointer<U>& ptr, Bool cacheShared = true) : std::weak_ptr<T>(ptr),  _cacheShared(cacheShared), _cachedSharedSet(false) {
            this->_ptr = static_cast<T*>(ptr.get());
        }

        WeakPointer& operator =(const WeakPointer& other) {
            if (&other == this) return *this;
            std::weak_ptr<T>::operator=(other);
            this->_ptr = other._ptr;
            return *this;
        }

        WeakPointer& operator =(std::shared_ptr<T>& other) {
            std::weak_ptr<T>::operator=(other);
            this->_ptr = other.get();
            return *this;
        }

        Bool operator ==(const WeakPointer<T>& other) {
            if (!this->_ptr || !other._ptr)return false;
            return this->_ptr == other._ptr;
        }

        // TODO: Make this thread-safe. The following code is only valid in a SINGLE THREADED context!
        T *operator->() {

#ifdef __CORE_WEAK_POINTER_CACHE_SHARED
            if (this->_cacheShared) {
                if (!this->_cachedSharedSet) {
                    this->_cachedShared = this->lock();
                    if (!this->_cachedShared) {
                        throw WeakPointerAssertionFailure("Shared pointer is invalid.");
                    }
                    this->_cachedSharedSet = true;
                }
                return this->_cachedShared.get();
            }
#endif

            if (!this->_ptr) {
                std::shared_ptr<T> temp = this->lock();
                if (!temp) {
                    throw WeakPointerAssertionFailure("Tried to use null weak pointer (1st try).");
                }
                this->_ptr = temp.get();
            }
            if (!this->_ptr) {
                throw WeakPointerAssertionFailure("Tried to use null weak pointer (2nd try).");
            }
            if (!this->isValid()) {
                throw WeakPointerAssertionFailure("Tried to use invalid weak pointer.");
            }
            return this->_ptr;

        }

        T *get() {
            return this->_ptr;
        }

        Bool isValid() const {
            return isValid(*this);
        }

        operator bool() const { 
            return this->isValid();
        }

        static std::shared_ptr<T> expectValidWeakPointer(std::weak_ptr<T> ptr) {
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

    protected:
        T * _ptr;

        std::shared_ptr<T> _cachedShared;
        Bool _cacheShared;
        Bool _cachedSharedSet;
    };
}
