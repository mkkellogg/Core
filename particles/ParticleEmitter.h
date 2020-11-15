#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../util/Time.h"
#include "../math/Math.h"

namespace Core {

    enum class ParticleEmitterType {
        Instantaneous = 0,
        Constant = 1,
        Noise = 2
    };

    class ParticleEmitter {

        friend class ParticleSystem;

    public:

        ParticleEmitter() {
            this->emissionRelativeStartTime = 0.0f;
            this->emissionDuration = 0.0f;
            this->emitCount = 0;
            this->age = 0.0f;
            this->timeActive = 0.0f;
            this->activeCount = 0;
        }
        virtual ~ParticleEmitter();

        virtual UInt32 update(Real timeDelta) {
            this->internalUpdate(timeDelta);
            return 0;
        }

        Real emissionRelativeStartTime;
        Real emissionDuration;

    protected:

        Bool internalUpdate (Real timeDelta) {
            this->age += timeDelta;
            this->timeActive = Math::max(this->age - this->emissionRelativeStartTime, 0.0f);
            return this->age >= this->emissionRelativeStartTime &&
                    (this->emissionDuration == 0.0f || (this->timeActive <= emissionDuration));
        }

        void updateEmitCount(UInt32 count) {
            this->emitCount += count;
        }

        Real age;
        Real timeActive;
        UInt32 emitCount;
        UInt32 activeCount;
        UInt32 maxActiveParticles;
    };

    class InstantaneousParticleEmitter: public ParticleEmitter {
    public:

        InstantaneousParticleEmitter (UInt32 burstSize, UInt32 maxBursts, Real waitTimetBetweenBursts) {
            this->burstSize = burstSize;
            this->maxBursts = maxBursts;
            this->waitTimetBetweenBursts = waitTimetBetweenBursts;
            this->burstCount = 0;
            this->timeSinceLastBurst = 0.0f;
        }

        virtual UInt32 update(Real timeDelta) override {
            if (this->internalUpdate(timeDelta)) {
                this->timeSinceLastBurst += timeDelta;
                if (this->burstCount < this->maxBursts || this->maxBursts == 0) {
                    if (this->timeSinceLastBurst >= this->waitTimetBetweenBursts) {
                        this->timeSinceLastBurst -= this->waitTimetBetweenBursts;
                        this->burstCount++;
                        UInt32 toEmitThisFrame = this->burstSize;
                        this->updateEmitCount(toEmitThisFrame);
                        return toEmitThisFrame;
                    }
                }
            }
            return 0;
        }

        UInt32 burstSize;
        UInt32 maxBursts;
        Real waitTimetBetweenBursts;
        UInt32 maximumEmissionPerFrame;

    protected:

        UInt32 burstCount;
        Real timeSinceLastBurst;
    };

    class ContinuousParticleEmitter: public ParticleEmitter {
    public:
        UInt32 emissionMinimum;
        UInt32 emissionMaximum;
    };

    class ConstantParticleEmitter: public ContinuousParticleEmitter {
    public:

        ConstantParticleEmitter() {}

        virtual UInt32 update(Real timeDelta) override {
            if (this->internalUpdate(timeDelta)) {
                Real effectiveEmissionRate = 0.0f;
                if (this->timeActive > 0.0f) effectiveEmissionRate = (Real)this->emitCount / this->timeActive;
                Real emissionRateDelta = this->emissionRate - effectiveEmissionRate;
                if (emissionRateDelta > 1.0f) {
                    UInt32 toEmitThisFrame = (UInt32) emissionRateDelta;
                    this->updateEmitCount(toEmitThisFrame);
                    return toEmitThisFrame;
                }
            }
            return 0;
        }

        UInt32 emissionRate;
    };

    class NoiseParticleEmitter: public ContinuousParticleEmitter {
    public:
        UInt32 scaleEmissionToUsedControlPoints;
        Real timeNoiseCoordinateScale;
        Real timeCoordinateOffset;
        Bool absoluteValue;
        Bool invertAbsoluteValue;
        Real worldTimeNoiseCoordinateScale;
    };

}