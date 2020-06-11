#include "SSAOMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"
#include "../image/Texture.h"

namespace Core {

    SSAOMaterial::SSAOMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
        this->radius = 2.0f;
    }

    SSAOMaterial::~SSAOMaterial() {
    }
    
    Bool SSAOMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("SSAO");
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
        this->shader->setUniform1f(this->radiusLocation, this->radius);
        this->shader->setUniform1f(this->biasLocation, this->bias);
        this->shader->setUniform1f(this->screenWidthLocation, this->screenWidth);
        this->shader->setUniform1f(this->screenHeightLocation, this->screenHeight);
    }

    WeakPointer<Material> SSAOMaterial::clone() {
        WeakPointer<SSAOMaterial> newMaterial = Engine::instance()->createMaterial<SSAOMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

     void SSAOMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<SSAOMaterial> ssaoMaterial = WeakPointer<Material>::dynamicPointerCast<SSAOMaterial>(target);
        if (ssaoMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            ssaoMaterial->viewPositionsLocation = this->viewPositionsLocation;
            ssaoMaterial->viewNormalsLocation = this->viewNormalsLocation;
            ssaoMaterial->noiseLocation = this->noiseLocation;
            for (UInt32 i = 0; i < Constants::SSAOSamples; i++) {
                ssaoMaterial->samplesLocation[i] = this->samplesLocation[i];
                ssaoMaterial->samples[i] = this->samples[i];
            }
            ssaoMaterial->projectionLocation = this->projectionLocation;
            ssaoMaterial->radiusLocation = this->radiusLocation;
            ssaoMaterial->biasLocation = this->biasLocation;
            ssaoMaterial->screenWidthLocation = this->screenWidthLocation;
            ssaoMaterial->screenHeightLocation = this->screenHeightLocation;
            ssaoMaterial->albedoUVLocation = this->albedoUVLocation;

            ssaoMaterial->viewPositions = this->viewPositions;
            ssaoMaterial->viewNormals = this->viewNormals;
            ssaoMaterial->noise = this->noise;
            ssaoMaterial->projection = this->projection;
            ssaoMaterial->radius = this->radius;
            ssaoMaterial->screenWidth = this->screenWidth;
            ssaoMaterial->screenHeight = this->screenHeight;
            throw InvalidArgumentException("SSAOMaterial::copyTo() -> 'target must be same material.");
        }
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
        this->radiusLocation = this->shader->getUniformLocation("radius");
        this->biasLocation = this->shader->getUniformLocation("bias");
        this->screenWidthLocation = this->shader->getUniformLocation("screenWidth");
        this->screenHeightLocation = this->shader->getUniformLocation("screenHeight");
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

    void SSAOMaterial::setRadius(Real radius) {
        this->radius = radius;
    }

    void SSAOMaterial::setBias(Real bias) {
        this->bias = bias;
    }

    void SSAOMaterial::setScreenWidth(Real screenWidth) {
        this->screenWidth = screenWidth;
    }

    void SSAOMaterial::setScreenHeight(Real screenHeight) {
        this->screenHeight = screenHeight;
    }

}
