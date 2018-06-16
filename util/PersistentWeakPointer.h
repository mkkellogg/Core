#pragma once

#include <memory>

#include "WeakPointer.h"
#include "../common/Exception.h"
#include "../common/types.h"

namespace Core {

    template <typename T>
    class PersistentWeakPointer : public WeakPointer<T> {
    public:

        PersistentWeakPointer(): WeakPointer<T>(false) {
            
        }

        PersistentWeakPointer(std::shared_ptr<T> ptr) : WeakPointer<T>(ptr, false) {

        }

        template <typename U>
        PersistentWeakPointer(WeakPointer<U>& ptr) : WeakPointer<T>(ptr, false) {
        }

        PersistentWeakPointer& operator =(const WeakPointer<T>& other) {
            if (&other == this) return *this;
            WeakPointer<T>::operator=(other);
            return *this;
        }


    };
}
