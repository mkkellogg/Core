#pragma once

#include <unordered_map>

#include "../common/types.h"
#include "../common/Exception.h"
#include "Shader.h"

namespace Core {

    class ShaderManager {
    public:

        class ShaderManagerException: Exception {
        public:
            ShaderManagerException(const std::string& msg): Exception(msg) {}
            ShaderManagerException(const char* msg): Exception(msg) {}
        };

        class Entry {
        public:
            Entry() {
                
            }

            std::string baseSource;
            std::string vertexSource;
            std::string fragmentSource;
        };

        virtual ~ShaderManager();
        void setShader(ShaderType type, const std::string& name, const std::string& shaderSrc);
        void setShader(ShaderType type, const std::string& name, const char shaderSrc[]);
        std::string getShader(ShaderType type, const std::string& name);


    protected:
        std::string processShaderSource(ShaderType type, const std::string& src);

        std::unordered_map<std::string, Entry> entries;
    };

}