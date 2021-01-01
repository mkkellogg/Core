#include "OutlineMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"
#include "../image/Texture.h"
#include "../image/Texture2D.h"

namespace Core {

    OutlineMaterial::OutlineMaterial() {
        this->outlineColorLocation = -1;
        this->edgeWidthLocation = -1;
        this->pctExtendLocation = -1;
        this->absExtendLocation = -1;
        this->opacityMapLocation = -1;

        this->outlineColor.set(1.0, 0.0, 1.0, 1.0);
        this->edgeWidth = 0.005;
        this->pctExtend = 0.0;
        this->absExtend = 0.0025;
        this->hasOpacityMap = false;
    }

    void OutlineMaterial::setOutlineColor(Color color) {
        this->outlineColor = color;
    }

    void OutlineMaterial::setEdgeWidth(Real width) {
        this->edgeWidth = width;
    }

    void OutlineMaterial::setPctExtend(Real extend) {
        this->pctExtend = extend;
    }

    void OutlineMaterial::setAbsExtend(Real extend) {
        this->absExtend = extend;
    }

    void OutlineMaterial::setHasOpacityMap(Bool hasOpacityMap) {
        this->hasOpacityMap = hasOpacityMap;
    }

    void OutlineMaterial::setOpacityMap(WeakPointer<Texture> opacityMap) {
        this->opacityMap = opacityMap;
    }

    Bool OutlineMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Outline");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    void OutlineMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->outlineColorLocation, this->outlineColor.r, this->outlineColor.g, this->outlineColor.b, this->outlineColor.a);
        this->shader->setUniform1f(this->edgeWidthLocation, this->edgeWidth);
        this->shader->setUniform1f(this->pctExtendLocation, this->pctExtend);
        this->shader->setUniform1f(this->absExtendLocation, this->absExtend);
        if (this->hasOpacityMap) {
            this->shader->setUniform1f(this->opacityMapEnabledLocation, 1.0);
            this->shader->setTexture2D(0, this->opacityMap->getTextureID());
            this->shader->setUniform1i(this->opacityMapLocation, 0);
        } else {
            this->shader->setUniform1f(this->opacityMapEnabledLocation, 0.0);
            this->shader->setTexture2D(0, Engine::instance()->getGraphicsSystem()->getPlaceHolderTexture2D()->getTextureID());
            this->shader->setUniform1i(this->opacityMapLocation, 0);
        }
    }

    void OutlineMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<OutlineMaterial> outlineMaterial = WeakPointer<Material>::dynamicPointerCast<OutlineMaterial>(target);
        if (outlineMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            outlineMaterial->outlineColorLocation = this->outlineColorLocation;
            outlineMaterial->edgeWidthLocation = this->edgeWidthLocation;
            outlineMaterial->pctExtendLocation = this->pctExtendLocation;
            outlineMaterial->absExtendLocation = this->absExtendLocation;

            outlineMaterial->outlineColor = this->outlineColor;
            outlineMaterial->edgeWidth = this->edgeWidth;
            outlineMaterial->pctExtend = this->pctExtend;
            outlineMaterial->absExtend = this->absExtend;
        } else {
            throw InvalidArgumentException("OutlineMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> OutlineMaterial::clone() {
        WeakPointer<OutlineMaterial> newMaterial = Engine::instance()->createMaterial<OutlineMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void OutlineMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->outlineColorLocation = this->shader->getUniformLocation("color");
        this->edgeWidthLocation = this->shader->getUniformLocation("edgeWidth");
        this->pctExtendLocation = this->shader->getUniformLocation("pctExtend");
        this->absExtendLocation = this->shader->getUniformLocation("absExtend");
        this->opacityMapEnabledLocation = this->shader->getUniformLocation("opacityMapEnabled");
        this->opacityMapLocation = this->shader->getUniformLocation("opacityMap");
    }
}