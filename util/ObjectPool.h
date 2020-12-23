#pragma once

#include <vector>
#include <unordered_map>

#include "../common/types.h"

namespace Core {

    template <typename T> class ObjectPool final {
    public:
        ObjectPool(UInt32 initialCapacity) {
            this->capacity = 0;
            this->size = 0;
            this->increaseCapacity(initialCapacity);
        }

        ObjectPool(): ObjectPool(32) {
            
        }

        ~ObjectPool() {
            this->deallocateObjects();
        }

        UInt32 getCapacity() {
            return this->capacity;
        }

        T& acquireObject() {
            if (this->size >= this->capacity) this->increaseCapacity();
            UInt32 nextFreeLocation = this->nextFreeLocations.back();
            StorageDesc storageDesc = this->storageMap[nextFreeLocation];
            this->nextFreeLocations.pop_back();
            T& targetObj = this->objects[storageDesc.array][storageDesc.index];
            this->size++;
            return targetObj;
        }

        void returnObject(const T& object) {
            if (this->objectLocations.find(&object) != this->objectLocations.end()) {
                UInt32 returnLocation = this->objectLocations[&object];
                this->nextFreeLocations.push_back(returnLocation);
                this->size--;
            }
        }   

        void returnAll() {
            this->objectLocations.clear();
            this->size = 0;
            this->nextFreeLocations.clear();
            this->setupNextFreeLocations(0, this->capacity);
        }

    private:

        class StorageDesc {
            public:
            StorageDesc() {}
            StorageDesc(UInt32 array, UInt32 index) {
                this->array = array;
                this->index = index;
            }
            UInt32 array;
            UInt32 index;
        };

        void deallocateObjects () {
            for (UInt32 i = 0; i < this->objects.size(); i++) {
                delete[] this->objects[i];
            }
            this->objects.erase(this->objects.begin(), this->objects.end());
        }
        
        void setupNextFreeLocations(UInt32 oldCapacity, UInt32 newCapacity) {
            for (UInt32 i = oldCapacity; i < newCapacity; i++) {
                this->nextFreeLocations.push_back(i);
            }
        }

        void increaseCapacity() {
            increaseCapacity(this->capacity * 2);
        }

        void increaseCapacity(UInt32 newCapacity) {
            UInt32 capacityDelta = newCapacity - this->capacity;
            T* temp = new T[capacityDelta];
            UInt32 nextArray = this->objects.size();
            this->objects.push_back(temp);
            for (unsigned int i = 0; i < capacityDelta; i++) {
                this->storageMap[i + this->capacity] = StorageDesc(nextArray, i);
            }
            for (unsigned int i = capacity; i < newCapacity; i++) {
                StorageDesc storageDesc = this->storageMap[i];
                T& object = this->objects[storageDesc.array][storageDesc.index];
                this->objectLocations[&object] = i;
            }
            setupNextFreeLocations(capacity, newCapacity);
            this->capacity = newCapacity;
        }

        UInt32 size;
        std::vector<T*> objects;
        UInt32 capacity;
        std::unordered_map<UInt32, StorageDesc> storageMap;
        std::unordered_map<T*, UInt32> objectLocations;
        std::vector<UInt32> nextFreeLocations;
    };

}