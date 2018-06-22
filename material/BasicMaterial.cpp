#include "BasicMaterial.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"

static const char vertexShader[] =
    "#version 100\n"
    "attribute vec4 pos;\n"
    "attribute vec4 color;\n"
    "attribute vec2 uv;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 modelMatrix;\n"
    "varying vec4 vColor;\n"
    "void main() {\n"
    "    gl_Position = projection * viewMatrix * modelMatrix * pos;\n"
    "    vColor = color;\n"
    "}\n";

static const char fragmentShader[] =
    "#version 100\n"
    "precision mediump float;\n"
    "varying vec4 vColor;\n"
    "void main() {\n"
    "    gl_FragColor = vColor;\n"
    "}\n";

namespace Core {

    BasicMaterial::BasicMaterial(Engine& engine, WeakPointer<Graphics> graphics) : Material(engine, graphics) {
    }

    Bool BasicMaterial::build() {
        std::string vertexSrc = vertexShader;
        std::string fragmentSrc = fragmentShader;
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }

        this->positionLocation = this->shader->getAttributeLocation("pos");
        this->colorLocation = this->shader->getAttributeLocation("color");
        this->projectionMatrixLocation = this->shader->getUniformLocation("projection");
        this->viewMatrixLocation = this->shader->getUniformLocation("viewMatrix");
        this->modelMatrixLocation = this->shader->getUniformLocation("modelMatrix");
        return true;
    }

    Int32 BasicMaterial::getShaderLocation(StandardAttribute attribute) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            case StandardAttribute::UV0:
                return this->uvLocation;
            default:
                return -1;
        }
    }

    Int32 BasicMaterial::getShaderLocation(StandardUniform uniform) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
            default:
                return -1;
        }
    }

    void BasicMaterial::sendCustomUniformsToShader() {
    }

    WeakPointer<Material> BasicMaterial::clone() {
        WeakPointer<BasicMaterial> newMaterial = this->engine.createMaterial<BasicMaterial>(false);
        newMaterial->setShader(this->getShader());
   
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        return newMaterial;
    }
}