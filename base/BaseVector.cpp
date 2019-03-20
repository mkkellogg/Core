#include "BaseVector.h"

namespace Core {

    template <> 
    void BaseVector<Real, 2>::copy(const BaseVector<Real, 2>& other) {
        this->data[0] = other.data[0];
        this->data[1] = other.data[1];
    }

    template <>
    void BaseVector<Real, 3>::copy(const BaseVector<Real, 3>& other) {
        this->data[0] = other.data[0];
        this->data[1] = other.data[1];
        this->data[2] = other.data[2];
    }

    template <>
    void BaseVector<Real, 4>::copy(const BaseVector<Real, 4>& other) {
        this->data[0] = other.data[0];
        this->data[1] = other.data[1];
        this->data[2] = other.data[2];
        this->data[3] = other.data[3];
    }
    
}