#pragma once

#include <vector>
#include "../util/WeakPointer.h"
#include "../light/Light.h"
#include "../light/ShadowLight.h"
#include "../light/DirectionalLight.h"
#include "../light/PointLight.h"
#include "../light/AmbientLight.h"
#include "../light/AmbientIBLLight.h"

namespace Core {

    class Light;
    class DirectionalLight;
    class PointLight;
    class AmbientLight;
    class AmbientIBLLight;
    class ShadowLight;

    class LightPack {

    public:

        void clear() {
            this->directionalLights.resize(0);
            this->pointLights.resize(0);
            this->ambientLights.resize(0);
            this->ambientIBLLights.resize(0);
            this->shadowLights.resize(0);
            this->lights.resize(0);
        }

        UInt32 lightCount() const {
            return this->lights.size();
        }

        WeakPointer<Light> getLight(UInt32 index) const {
            if (index >= this->lights.size()) {
                throw OutOfRangeException("LightPack::getLight() -> 'index' is out of range.");
            }
            return this->lights[index];
        }

        std::vector<WeakPointer<Light>>& getLights() {
            return this->lights;
        }

        WeakPointer<ShadowLight> getShadowLight(UInt32 index) const {
            if (index >= this->shadowLights.size()) {
                throw OutOfRangeException("LightPack::getShadowLight() -> 'index' is out of range.");
            }
            return this->shadowLights[index];
        }

        std::vector<WeakPointer<Light>>& getNonIBLLights() {
            return this->nonIBLLights;
        }

        WeakPointer<DirectionalLight> getDirectionalLight(UInt32 index) const {
            if (index >= this->directionalLights.size()) {
                throw OutOfRangeException("LightPack::getDirectionalLight() -> 'index' is out of range.");
            }
            return this->directionalLights[index];
        }

        std::vector<WeakPointer<DirectionalLight>>& getDirectionalLights() {
            return this->directionalLights;
        }

        void addDirectionalLight(WeakPointer<DirectionalLight> light) {
            this->directionalLights.push_back(light);
            this->shadowLights.push_back(light);
            this->lights.push_back(light);
            this->nonIBLLights.push_back(light);
        }

        std::vector<WeakPointer<PointLight>>& getPointLights() {
            return this->pointLights;
        }

        WeakPointer<PointLight> getPointLight(UInt32 index) const {
            if (index >= this->pointLights.size()) {
                throw OutOfRangeException("LightPack::getPointLight() -> 'index' is out of range.");
            }
            return this->pointLights[index];
        }

        void addPointLight(WeakPointer<PointLight> light) {
            this->pointLights.push_back(light);
            this->shadowLights.push_back(light);
            this->lights.push_back(light);
            this->nonIBLLights.push_back(light);
        }

        WeakPointer<AmbientLight> getAmbientLight(UInt32 index) const {
            if (index >= this->ambientLights.size()) {
                throw OutOfRangeException("LightPack::getAmbientLight() -> 'index' is out of range.");
            }
            return this->ambientLights[index];
        }

        void addAmbientLight(WeakPointer<AmbientLight> light) {
            this->ambientLights.push_back(light);
            this->lights.push_back(light);
            this->nonIBLLights.push_back(light);
        }

        WeakPointer<AmbientIBLLight> getAmbientIBLLight(UInt32 index) const {
            if (index >= this->ambientIBLLights.size()) {
                throw OutOfRangeException("LightPack::getAmbientIBLLight() -> 'index' is out of range.");
            }
            return this->ambientIBLLights[index];
        }

        void addAmbientIBLLight(WeakPointer<AmbientIBLLight> light) {
            this->ambientIBLLights.push_back(light);
            this->lights.push_back(light);
        }

    private:
        std::vector<WeakPointer<DirectionalLight>> directionalLights;
        std::vector<WeakPointer<PointLight>> pointLights;
        std::vector<WeakPointer<AmbientLight>> ambientLights;
        std::vector<WeakPointer<AmbientIBLLight>> ambientIBLLights;
        std::vector<WeakPointer<ShadowLight>> shadowLights;
        std::vector<WeakPointer<Light>> nonIBLLights;
        std::vector<WeakPointer<Light>> lights;
    };

}