#pragma once

#include <string>
#include "../common/types.h"
#include "../common/Exception.h"

namespace Core {

    class Shader {
    public:
        enum class ShaderType { 
            Vertex = 1, 
            Fragment = 2 
        };

        class ShaderVariableException: Exception {
        public:
            ShaderVariableException(const std::string& msg): Exception(msg) {}
            ShaderVariableException(const char* msg): Exception(msg) {}
        };

        Shader();
        Shader(const std::string& vertex, const std::string& fragment);
        Shader(const char vertex[], const char fragment[]);
        virtual ~Shader();

        Bool isReady() const;

        virtual Bool build() = 0;
        virtual UInt32 getProgram() const = 0;
        virtual Int32 getUniformLocation(const std::string& var) const = 0;
        virtual Int32 getAttributeLocation(const std::string& var) const = 0;
        virtual Int32 getUniformLocation(const char var[]) const = 0;
        virtual Int32 getAttributeLocation(const char var[]) const = 0;

        virtual void setTexture2D(UInt32 slot, UInt32 textureID) = 0;
        virtual void setTextureCube(UInt32 slot, UInt32 textureID) = 0;
        virtual void setUniform1i(UInt32 location, Int32 val) = 0;

    protected:
        Bool ready;
        std::string vertexSource;
        std::string fragmentSource;

        virtual UInt32 createShader(ShaderType shaderType, const std::string& src) = 0;
        virtual UInt32 createProgram(const std::string& vertex, const std::string& fragment) = 0;
    };
}
