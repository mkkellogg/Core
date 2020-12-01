#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../geometry/Vector3.h"
#include "../color/Color.h"
#include "../geometry/AttributeArray.h"

namespace Core {

    class ParticleState {
    public:
        Real lifetime;
        Real age;
        UInt32 sequenceNumber;
        Point3r position;
        Vector3r velocity;
        Vector3r normal;
        Real rotation;
        Real rotationalSpeed;
        Real radius;
        Color color;
    };

    class ParticleStatePtr {
    public:
        Real* lifetime;
        Real* age;
        UInt32* sequenceNumber;
        Point3rs* position;
        Vector3rs* velocity;
        Vector3rs* normal;
        Real* rotation;
        Real* rotationalSpeed;
        Real* radius;
        ColorS* color;
    };

    class ParticleStateArrayBase {
    public:
        ParticleStateArrayBase() {
            this->particleCount = 0;
        }

        virtual ~ParticleStateArrayBase(){
        }
    
        void setParticleCount(UInt32 particleCount) {
            if (this->particleCount != particleCount) {
                deallocate();
                allocate(particleCount);
            }
            this->particleCount = particleCount;
        }

        UInt32 getParticleCount() {
            return this->particleCount;
        }
    protected:
        virtual void allocate(UInt32 particleCount) = 0;
        virtual void deallocate() = 0;

        UInt32 particleCount;
    };

    class ParticleStateAttributeArray final: public ParticleStateArrayBase {
    public:
        ParticleStateAttributeArray() {
        }

        void copyParticleState(UInt32 srcIndex, UInt32 destIndex) {
            if (srcIndex >= this->particleCount) {
                throw OutOfRangeException("ParticleStateAttributeArray::copyParticleState() -> 'srcIndex' is out of range.");
            }
            if (destIndex >= this->particleCount) {
                throw OutOfRangeException("ParticleStateAttributeArray::copyParticleState() -> 'destIndex' is out of range.");
            }

            this->lifetimes->copyAttribute(srcIndex, destIndex);
            this->ages->copyAttribute(srcIndex, destIndex);
            this->sequenceNumbers->copyAttribute(srcIndex, destIndex);
            this->positions->copyAttribute(srcIndex, destIndex);
            this->velocities->copyAttribute(srcIndex, destIndex);
            this->normals->copyAttribute(srcIndex, destIndex);
            this->rotations->copyAttribute(srcIndex, destIndex);
            this->rotationalSpeeds->copyAttribute(srcIndex, destIndex);
            this->radiuses->copyAttribute(srcIndex, destIndex);
            this->colors->copyAttribute(srcIndex, destIndex);
        }

        ParticleStatePtr& getParticleStatePtr(UInt32 index) {
            if (index >= this->particleCount) {
                throw OutOfRangeException("ParticleStateAttributeArray::getParticleStatePtr() -> 'index' is out of range.");
            }
            return this->particleStatePointers.get()[index];
        }

        std::shared_ptr<AttributeArray<Point3rs>> getPositions() {return this->positions;}
        std::shared_ptr<ScalarAttributeArray<Real>> getRadiuses() {return this->radiuses;}
        std::shared_ptr<ScalarAttributeArray<Real>> getRotations() {return this->rotations;}

    protected:

        void allocate(UInt32 particleCount) override {
            this->lifetimes = std::make_shared<ScalarAttributeArray<Real>>(particleCount);
            this->ages = std::make_shared<ScalarAttributeArray<Real>>(particleCount);
            this->sequenceNumbers = std::make_shared<ScalarAttributeArray<UInt32>>(particleCount);
            this->positions = std::make_shared<AttributeArray<Point3rs>>(particleCount);
            this->velocities = std::make_shared<AttributeArray<Vector3rs>>(particleCount);
            this->normals = std::make_shared<AttributeArray<Vector3rs>>(particleCount);
            this->rotations = std::make_shared<ScalarAttributeArray<Real>>(particleCount);
            this->rotationalSpeeds = std::make_shared<ScalarAttributeArray<Real>>(particleCount);
            this->radiuses = std::make_shared<ScalarAttributeArray<Real>>(particleCount);
            this->colors = std::make_shared<AttributeArray<ColorS>>(particleCount);

            ParticleStatePtr* particleStatePointers = new(std::nothrow) ParticleStatePtr[particleCount];
            if (particleStatePointers == nullptr) {
                throw AllocationException("ParticleStateAttributeArray::allocate -> Unable to allocate particle state pointer array");
            }
            this->particleStatePointers = std::shared_ptr<ParticleStatePtr>(particleStatePointers);
            for (UInt32 i = 0; i < particleCount; i++) {
                this->bindParticleStatePtr(i, this->particleStatePointers.get()[i]);
            }
        }

        void deallocate() override {
        }

        void bindParticleStatePtr(UInt32 index, ParticleStatePtr& ptr) {
            ptr.lifetime = &this->lifetimes->getAttribute(index);
            ptr.age = &this->ages->getAttribute(index);
            ptr.sequenceNumber = &this->sequenceNumbers->getAttribute(index);
            ptr.position = &this->positions->getAttribute(index);
            ptr.velocity = &this->velocities->getAttribute(index);
            ptr.normal = &this->normals->getAttribute(index);
            ptr.rotation = &this->rotations->getAttribute(index);
            ptr.rotationalSpeed = &this->rotationalSpeeds->getAttribute(index);
            ptr.radius = &this->radiuses->getAttribute(index);
            ptr.color = &this->colors->getAttribute(index);
        }

        std::shared_ptr<ParticleStatePtr> particleStatePointers;
        std::shared_ptr<ScalarAttributeArray<Real>> lifetimes;
        std::shared_ptr<ScalarAttributeArray<Real>> ages;
        std::shared_ptr<ScalarAttributeArray<UInt32>> sequenceNumbers;
        std::shared_ptr<AttributeArray<Point3rs>> positions;
        std::shared_ptr<AttributeArray<Vector3rs>> velocities;
        std::shared_ptr<AttributeArray<Vector3rs>> normals;
        std::shared_ptr<ScalarAttributeArray<Real>> rotations;
        std::shared_ptr<ScalarAttributeArray<Real>> rotationalSpeeds;
        std::shared_ptr<ScalarAttributeArray<Real>> radiuses;
        std::shared_ptr<AttributeArray<ColorS>> colors;
    };
}