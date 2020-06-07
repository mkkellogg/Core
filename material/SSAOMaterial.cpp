#include "SSAOMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"
#include "../image/Texture.h"

namespace Core {

    SSAOMaterial::SSAOMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
    }

    SSAOMaterial::~SSAOMaterial() {
    }
    
    Bool SSAOMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("ScreenSpaceAmbientOcclusion");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    Int32 SSAOMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        Int32 attributeLocation = BaseMaterial::getShaderLocation(attribute, offset);
        if (attributeLocation >= 0) return attributeLocation;
        switch (attribute) {
            case StandardAttribute::AlbedoUV:
                return this->albedoUVLocation;
            default:
                return -1;
        }
    }

    void SSAOMaterial::sendCustomUniformsToShader() {
        UInt32 textureLoc = 0;
        this->shader->setTexture2D(textureLoc, this->viewPositions->getTextureID());
        this->shader->setUniform1i(this->viewPositionsLocation, textureLoc);
        textureLoc++;
        this->shader->setTexture2D(textureLoc, this->viewNormals->getTextureID());
        this->shader->setUniform1i(this->viewNormalsLocation, textureLoc);
        textureLoc++;
        this->shader->setTexture2D(textureLoc, this->noise->getTextureID());
        this->shader->setUniform1i(this->noiseLocation, textureLoc);
        textureLoc++;
        for (UInt32 i = 0; i < Constants::SSAOSamples; i++) {
            const Vector3r& sample = this->samples[i];
            this->shader->setUniform3f(this->samplesLocation[i], sample.x, sample.y, sample.z);
        }
        this->shader->setUniformMatrix4(this->projectionLocation, this->projection);
    }

    WeakPointer<Material> SSAOMaterial::clone() {
        WeakPointer<SSAOMaterial> newMaterial = Engine::instance()->createMaterial<SSAOMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void SSAOMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->viewPositionsLocation = this->shader->getUniformLocation("viewPositions");
        this->viewNormalsLocation = this->shader->getUniformLocation("viewNormals");
        this->noiseLocation = this->shader->getUniformLocation("noise");
        for (UInt32 i = 0; i < Constants::SSAOSamples; i++) {
            this->samplesLocation[i] = this->shader->getUniformLocation("samples", i);
        }
        this->projectionLocation = this->shader->getUniformLocation("projection");
        this->albedoUVLocation = this->shader->getAttributeLocation(StandardAttribute::AlbedoUV);
    }

    void SSAOMaterial::setViewPositions(WeakPointer<Texture> positions) {
        this->viewPositions = positions;
    }

    void SSAOMaterial::setViewNormals(WeakPointer<Texture> normals) {
        this->viewNormals = normals;
    }

    void SSAOMaterial::setNoise(WeakPointer<Texture> noise) {
        this->noise = noise;
    }

    void SSAOMaterial::setSamples(const std::vector<Vector3r>& samples) {
        this->samples = samples;
    }

    void SSAOMaterial::setProjection(const Matrix4x4& projection) {
        this->projection.copy(projection);
    }
}
