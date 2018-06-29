#pragma once

#include <unordered_map>

#include "../common/types.h"
#include "../common/Exception.h"
#include "Shader.h"

namespace Core {

    class ShaderDirectory {
    public:

        class ShaderDirectoryException: Exception {
        public:
            ShaderDirectoryException(const std::string& msg): Exception(msg) {}
            ShaderDirectoryException(const char* msg): Exception(msg) {}
        };

        class Entry {
        public:
            Entry() {
                
            }

            Entry(const std::string& vertexSrc, const std::string& fragmentSrc) {
                this->vertexSource = vertexSrc;
                this->fragmentSource = fragmentSrc;
            }

            Entry(const char vertexSrc[], const char fragmentSrc[]) {
                this->vertexSource = vertexSrc;
                this->fragmentSource = fragmentSrc;
            }

            std::string vertexSource;
            std::string fragmentSource;
        };

        virtual ~ShaderDirectory();
        void addEntry(const std::string& name, const Entry& entry);
        void setShader(Shader::ShaderType type, const std::string& name, const std::string& shaderSrc);
        void setShader(Shader::ShaderType type, const std::string& name, const char shaderSrc[]);

    protected:
        void checkShaderExists(const std::string& name);

        std::unordered_map<std::string, Entry> entries;
    };

}