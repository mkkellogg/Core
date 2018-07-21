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
        PersistentWeakPointer(const WeakPointer<U>& ptr) : WeakPointer<T>(ptr, false) {
        }


        template <typename U>
        PersistentWeakPointer<typename std::enable_if<std::is_base_of<T, U>::value, T>::type>& operator =(const WeakPointer<U>& other) {
            if ((void *)&other == (void *)this) return *this;
            WeakPointer<T>::operator=(other);
            return *this;
            
        }

    };
}
