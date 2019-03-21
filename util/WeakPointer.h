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

        template<typename U>
        friend class WeakPointer;

        WeakPointer(): std::weak_ptr<T>(), _ptr(nullptr), _cacheShared(true), _cachedSharedSet(false)  {
        }

        WeakPointer(const std::weak_ptr<T>& ptr) : std::weak_ptr<T>(ptr), _ptr(nullptr), _cacheShared(true), _cachedSharedSet(false) {
        }

        WeakPointer(const std::shared_ptr<T>& ptr) : std::weak_ptr<T>(ptr), _ptr(nullptr), _cacheShared(true), _cachedSharedSet(false) {
        }

        template <typename U>
        WeakPointer(const WeakPointer<U>& ptr) : std::weak_ptr<T>(ptr),  _cacheShared(true), _cachedSharedSet(false) {
            this->_ptr = ptr._ptr;
        }

        template <typename U>
        WeakPointer<typename std::enable_if<std::is_base_of<T, U>::value, T>::type>& operator =(const WeakPointer<U>& other) {
            if ((void *)&other == (void *)this) return *this;
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

        const T* operator ->() const {
            return this->tryGetPtr();
        }

        // TODO: Make this thread-safe. The following code is only valid in a SINGLE THREADED context!
        T* operator ->() {
            return this->tryGetPtr();
        }

        T *get() {
            return this->tryGetPtr();
        }

        const T *get() const {
            return this->tryGetPtr();
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

        template<typename U> 
        static WeakPointer<U> dynamicPointerCast(const WeakPointer<T>& src) {
            std::shared_ptr<T> _src_shared = src.lock();
            if (_src_shared) {
                std::shared_ptr<U> u_shared = std::dynamic_pointer_cast<U>(_src_shared);
                if (u_shared) {
                    WeakPointer<U> t(u_shared);
                    t._ptr = u_shared.get();
                    t._cacheShared = src._cacheShared;
                    t._cachedSharedSet = src._cachedSharedSet;
                    if (t._cachedSharedSet) t._cachedShared = u_shared;
                    return t;
                }
            }
            return WeakPointer<U>::nullPtr();
        }

        static WeakPointer nullPtr() {
            return WeakPointer();
        }

    protected:

        WeakPointer(const std::weak_ptr<T>& ptr, Bool cacheShared) : std::weak_ptr<T>(ptr), _ptr(nullptr), _cacheShared(cacheShared), _cachedSharedSet(false) {
        }

        WeakPointer(const std::shared_ptr<T>& ptr, Bool cacheShared) : std::weak_ptr<T>(ptr), _ptr(nullptr), _cacheShared(cacheShared), _cachedSharedSet(false) {
        }

        template <typename U>
        WeakPointer(const WeakPointer<U>& ptr, Bool cacheShared) : std::weak_ptr<T>(ptr),  _cacheShared(cacheShared), _cachedSharedSet(false) {
            this->_ptr = ptr._ptr;
        }

        WeakPointer(Bool cacheShared): std::weak_ptr<T>(), _ptr(nullptr), _cacheShared(cacheShared), _cachedSharedSet(false)  {
        }

        T * _getPtr() {
            return this->_ptr;
        }

        T* tryGetPtr() const {

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
                this->tryUpdatePtr();
            }
            if (!this->_ptr) {
                throw WeakPointerAssertionFailure("Tried to use null weak pointer (2nd try).");
            }
            if (!this->isValid()) {
                throw WeakPointerAssertionFailure("Tried to use invalid weak pointer.");
            }
            return this->_ptr;
        }

        void tryUpdatePtr() const {
            std::shared_ptr<T> temp = this->lock();
            if (!temp) {
                throw WeakPointerAssertionFailure("Tried to use null weak pointer (1st try).");
            }
            this->_ptr = temp.get();
        }

        mutable T * _ptr;
        Bool _cacheShared;
        mutable Bool _cachedSharedSet;
        mutable std::shared_ptr<T> _cachedShared;
    };
}
