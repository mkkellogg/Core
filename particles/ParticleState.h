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
        }

        void deallocate() override {
        }

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