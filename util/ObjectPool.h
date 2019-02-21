#pragma once

#include <vector>
#include <unordered_map>

#include "../common/types.h"

namespace Core {

    template <typename T> class ObjectPool {
    public:
        ObjectPool(UInt32 initialCapacity): size(0) {
            this->increaseCapacity(initialCapacity);
        }

        ObjectPool(): ObjectPool(32) {
            
        }

        UInt32 capacity() {
            return this->objects.capacity();
        }

        T& acquireObject() {
            if (this->size >= capacity()) increaseCapacity();
            UInt32 nextFreeLocation = this->nextFreeLocations.back();
            this->nextFreeLocations.pop_back();
            T& targetObj = this->objects[nextFreeLocation];
            this->occupied[nextFreeLocation] = true;
            this->size++;
            return targetObj;
        }

        void returnObject(const T& object) {
            if (this->objectLocations.find(&object) != this->objectLocations.end()) {
                UInt32 returnLocation = this->objectLocations[&object];
                this->occupied[returnLocation] = false;
                this->nextFreeLocations.push_back(returnLocation);
                this->size--;
            }
        }   

        void returnAll() {
            this->occupied.clear();
            this->objectLocations.clear();
            this->size = 0;
            this->nextFreeLocations.clear();
            this->setupNextFreeLocations(0, this->capacity());
        }

    private:
        
        void setupNextFreeLocations(UInt32 oldCapacity, UInt32 newCapacity) {
            for (UInt32 i = oldCapacity; i < newCapacity; i++) {
                this->nextFreeLocations.push_back(newCapacity - 1 - i + oldCapacity);
            }
        }

        void increaseCapacity() {
            increaseCapacity(this->capacity() * 2);
        }

        void increaseCapacity(UInt32 newCapacity) {
            UInt32 oldCapacity = objects.capacity();
            this->objects.reserve(newCapacity);
            for (unsigned int i = oldCapacity; i < newCapacity; i++) {
                this->objects.emplace_back();
                this->objectLocations[&this->objects[i]] = i;
            }
            setupNextFreeLocations(oldCapacity, newCapacity);
        }

        UInt32 size;
        std::vector<T> objects;
        std::unordered_map<UInt32, bool> occupied;
        std::unordered_map<T*, UInt32> objectLocations;
        std::vector<int> nextFreeLocations;
    };

}