#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../geometry/Vector2.h"
#include "../geometry/Vector3.h"
#include "../color/Color.h"
#include "../geometry/AttributeArray.h"
#include "../geometry/AttributeType.h"

namespace Core {

    enum class ParticleStateProgressType {
        Time = 0,
        Sequence = 1
    };

    class ParticleState {
    public:
        Real progressType;
        Real lifetime;
        Real age;
        Vector4r sequenceElement;
        Point3r position;
        Vector3r velocity;
        Vector3r acceleration;
        Vector3r normal;
        Real rotation;
        Real rotationalSpeed;
        Vector2r size;
        Color color;

        Vector2r initialSize;
        Color initialColor;
    };

    class ParticleStatePtr {
    public:
        Real* progressType;
        Real* lifetime;
        Real* age;
        Vector4rs* sequenceElement;
        Point3rs* position;
        Vector3rs* velocity;
        Vector3rs* acceleration;
        Vector3rs* normal;
        Real* rotation;
        Real* rotationalSpeed;
        Vector2rs* size;
        ColorS* color;

        Vector2rs* initialSize;
        ColorS* initialColor;
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

        void copyState(UInt32 srcIndex, UInt32 destIndex) {
            if (srcIndex >= this->particleCount) {
                throw OutOfRangeException("ParticleStateAttributeArray::copyState() -> 'srcIndex' is out of range.");
            }
            if (destIndex >= this->particleCount) {
                throw OutOfRangeException("ParticleStateAttributeArray::copyState() -> 'destIndex' is out of range.");
            }

            this->progressTypes->copyAttribute(srcIndex, destIndex);
            this->lifetimes->copyAttribute(srcIndex, destIndex);
            this->ages->copyAttribute(srcIndex, destIndex);
            this->sequenceElements->copyAttribute(srcIndex, destIndex);
            this->positions->copyAttribute(srcIndex, destIndex);
            this->velocities->copyAttribute(srcIndex, destIndex);
            this->accelerations->copyAttribute(srcIndex, destIndex);
            this->normals->copyAttribute(srcIndex, destIndex);
            this->rotations->copyAttribute(srcIndex, destIndex);
            this->rotationalSpeeds->copyAttribute(srcIndex, destIndex);
            this->sizes->copyAttribute(srcIndex, destIndex);
            this->colors->copyAttribute(srcIndex, destIndex);

            this->initialSizes->copyAttribute(srcIndex, destIndex);
            this->initialColors->copyAttribute(srcIndex, destIndex);
        }

        ParticleStatePtr& getStatePtr(UInt32 index) {
            if (index >= this->particleCount) {
                throw OutOfRangeException("ParticleStateAttributeArray::getStatePtr() -> 'index' is out of range.");
            }
            return this->particleStatePointers.get()[index];
        }

        std::shared_ptr<AttributeArray<Point3rs>> getPositions() {return this->positions;}
        std::shared_ptr<AttributeArray<Vector2rs>> getSizes() {return this->sizes;}
        std::shared_ptr<ScalarAttributeArray<Real>> getRotations() {return this->rotations;}
        std::shared_ptr<AttributeArray<Vector4rs>> getSequenceElements() {return this->sequenceElements;}
        std::shared_ptr<AttributeArray<ColorS>> getColors() {return this->colors;}

    protected:

        void allocate(UInt32 particleCount) override {
            this->progressTypes = std::make_shared<ScalarAttributeArray<Real>>(particleCount, AttributeType::Float, false);
            this->lifetimes = std::make_shared<ScalarAttributeArray<Real>>(particleCount, AttributeType::Float, false);
            this->ages = std::make_shared<ScalarAttributeArray<Real>>(particleCount, AttributeType::Float, false);
            this->sequenceElements = std::make_shared<AttributeArray<Vector4rs>>(particleCount, AttributeType::Float, false);
            this->positions = std::make_shared<AttributeArray<Point3rs>>(particleCount, AttributeType::Float, false);
            this->velocities = std::make_shared<AttributeArray<Vector3rs>>(particleCount, AttributeType::Float, false);
            this->accelerations = std::make_shared<AttributeArray<Vector3rs>>(particleCount, AttributeType::Float, false);
            this->normals = std::make_shared<AttributeArray<Vector3rs>>(particleCount, AttributeType::Float, false);
            this->rotations = std::make_shared<ScalarAttributeArray<Real>>(particleCount, AttributeType::Float, false);
            this->rotationalSpeeds = std::make_shared<ScalarAttributeArray<Real>>(particleCount, AttributeType::Float, false);
            this->sizes = std::make_shared<AttributeArray<Vector2rs>>(particleCount, AttributeType::Float, false);
            this->colors = std::make_shared<AttributeArray<ColorS>>(particleCount, AttributeType::Float, false);

            this->initialSizes = std::make_shared<AttributeArray<Vector2rs>>(particleCount, AttributeType::Float, false);
            this->initialColors = std::make_shared<AttributeArray<ColorS>>(particleCount, AttributeType::Float, false);

            ParticleStatePtr* particleStatePointers = new(std::nothrow) ParticleStatePtr[particleCount];
            if (particleStatePointers == nullptr) {
                throw AllocationException("ParticleStateAttributeArray::allocate -> Unable to allocate particle state pointer array");
            }
            this->particleStatePointers = std::shared_ptr<ParticleStatePtr>(particleStatePointers);
            for (UInt32 i = 0; i < particleCount; i++) {
                this->bindStatePtr(i, this->particleStatePointers.get()[i]);
            }
        }

        void deallocate() override {
        }

        void bindStatePtr(UInt32 index, ParticleStatePtr& ptr) {
            ptr.progressType = &this->progressTypes->getAttribute(index);
            ptr.lifetime = &this->lifetimes->getAttribute(index);
            ptr.age = &this->ages->getAttribute(index);
            ptr.sequenceElement = &this->sequenceElements->getAttribute(index);
            ptr.position = &this->positions->getAttribute(index);
            ptr.velocity = &this->velocities->getAttribute(index);
            ptr.acceleration = &this->accelerations->getAttribute(index);
            ptr.normal = &this->normals->getAttribute(index);
            ptr.rotation = &this->rotations->getAttribute(index);
            ptr.rotationalSpeed = &this->rotationalSpeeds->getAttribute(index);
            ptr.size = &this->sizes->getAttribute(index);
            ptr.color = &this->colors->getAttribute(index);

            ptr.initialSize = &this->initialSizes->getAttribute(index);
            ptr.initialColor = &this->initialColors->getAttribute(index);
        }

        std::shared_ptr<ParticleStatePtr> particleStatePointers;
        std::shared_ptr<ScalarAttributeArray<Real>> progressTypes;
        std::shared_ptr<ScalarAttributeArray<Real>> lifetimes;
        std::shared_ptr<ScalarAttributeArray<Real>> ages;
        std::shared_ptr<AttributeArray<Vector4rs>> sequenceElements;
        std::shared_ptr<AttributeArray<Point3rs>> positions;
        std::shared_ptr<AttributeArray<Vector3rs>> velocities;
        std::shared_ptr<AttributeArray<Vector3rs>> accelerations;
        std::shared_ptr<AttributeArray<Vector3rs>> normals;
        std::shared_ptr<ScalarAttributeArray<Real>> rotations;
        std::shared_ptr<ScalarAttributeArray<Real>> rotationalSpeeds;
        std::shared_ptr<AttributeArray<Vector2rs>> sizes;
        std::shared_ptr<AttributeArray<ColorS>> colors;

        std::shared_ptr<AttributeArray<Vector2rs>> initialSizes;
        std::shared_ptr<AttributeArray<ColorS>> initialColors;
    };
}