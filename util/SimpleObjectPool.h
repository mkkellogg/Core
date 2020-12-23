#pragma once

#include <vector>
#include <unordered_map>

#include "../common/types.h"

namespace Core {

    template <typename T> class SimpleObjectPool final {
    public:
        SimpleObjectPool(UInt32 initialCapacity) {
            this->capacity = 0;
            this->size = 0;
            this->increaseCapacity(initialCapacity);
        }

        SimpleObjectPool(): SimpleObjectPool(512) {
            
        }

        ~SimpleObjectPool() {
            this->deallocateObjects();
        }

        UInt32 getCapacity() {
            return this->capacity;
        }

        T& acquireObject() {
            if (this->size >= this->capacity) throw "Error!";
            T& object = this->objects[this->size];
            this->size++;
            return object;
        }

        void returnAll() {
            this->size = 0;
        }

    private:

        void deallocateObjects () {
            this->objects.erase(this->objects.begin(), this->objects.end());
        }
        
        void allocate(UInt32 oldCapacity, UInt32 newCapacity) {
            for (UInt32 i = oldCapacity; i < newCapacity; i++) {
                this->objects.push_back(T());
            }
        }

        void increaseCapacity() {
            increaseCapacity(this->capacity * 2);
        }

        void increaseCapacity(UInt32 newCapacity) {
            allocate(0, newCapacity);
            this->capacity = newCapacity;
        }

        UInt32 size;
        std::vector<T> objects;
        UInt32 capacity;
    };

}