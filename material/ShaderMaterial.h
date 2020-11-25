#pragma once

#include "../util/WeakPointer.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "BaseLitMaterial.h"
#include "ShaderManager.h"

namespace Core {

    template <class T=Material>
    class ShaderMaterial : public T {
        friend class Engine;

    public:

        virtual Bool build() override {
            WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
            ShaderManager& shaderManager = graphics->getShaderManager();
            Bool ready = false;
            if (this->useBuiltInShader) {
            this->shader = shaderManager.getShader(this->builtInShaderName);
            if (this->shader.isValid()) ready = true;
            } else {
                const std::string& vertexSrc = shaderManager.getShaderSource(ShaderType::Vertex, this->vertexShaderName);
                const std::string& fragmentSrc = shaderManager.getShaderSource(ShaderType::Fragment, this->fragmentShaderName);
                ready = this->buildFromSource(vertexSrc, fragmentSrc);
            }
            if (!ready) {
                return false;
            }
            graphics->activateShader(this->shader);
            this->bindShaderVarLocations();
            return true;
        }
    
    protected:
        ShaderMaterial(const std::string& vertexShaderName, const std::string& fragmentShaderName) {
            this->vertexShaderName = vertexShaderName;
            this->fragmentShaderName = fragmentShaderName;
            this->useBuiltInShader = false;
        }

        ShaderMaterial(const std::string& builtInShaderName) {
            this->builtInShaderName = builtInShaderName;
            this->useBuiltInShader = true;
        }


        std::string builtInShaderName;
        Bool useBuiltInShader;

        std::string vertexShaderName;
        std::string fragmentShaderName;
    };
}
