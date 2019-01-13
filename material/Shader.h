#pragma once

#include <string>
#include "../common/types.h"
#include "../common/Exception.h"
#include "../math/Matrix4x4.h"
#include "ShaderType.h"
#include "StandardUniforms.h"
#include "StandardAttributes.h"

namespace Core {

    class Shader {
    public:

        class ShaderVariableException: Exception {
        public:
            ShaderVariableException(const std::string& msg): Exception(msg) {}
            ShaderVariableException(const char* msg): Exception(msg) {}
        };

        class ShaderCompilationException: Exception {
        public:
            ShaderCompilationException(const std::string& msg): Exception(msg) {}
            ShaderCompilationException(const char* msg): Exception(msg) {}
        };

        Shader();
        Shader(const std::string& vertex, const std::string& fragment);
        Shader(const std::string& vertex, const std::string& geometry, const std::string& fragment);
        Shader(const char vertex[], const char fragment[]);
        Shader(const char vertex[], const char geometry[], const char fragment[]);
        virtual ~Shader();

        Bool isReady() const;

        virtual Bool build() = 0;
        virtual UInt32 getProgram() const = 0;
        virtual Int32 getUniformLocation(const std::string& var) const = 0;
        virtual Int32 getAttributeLocation(const std::string& var) const = 0;
        virtual Int32 getUniformLocation(const char var[]) const = 0;
        virtual Int32 getAttributeLocation(const char var[]) const = 0;
        virtual Int32 getUniformLocation(StandardUniform uniform) const = 0;
        virtual Int32 getUniformLocation(StandardUniform uniform, UInt32 index) const = 0;
        virtual Int32 getAttributeLocation(StandardAttribute attribute) const = 0;
        virtual Int32 getAttributeLocation(StandardAttribute attribute, UInt32 index) const = 0;

        virtual void setTexture2D(UInt32 slot, UInt32 textureID) = 0;
        virtual void setTextureCube(UInt32 slot, UInt32 textureID) = 0;
        virtual void setUniform1i(UInt32 location, Int32 val) = 0;
        virtual void setUniform1f(UInt32 location, Real val) = 0;
        virtual void setUniform4f(UInt32 location, Real x, Real y, Real z, Real w) = 0;
        virtual void setUniformMatrix4(UInt32 location, const Real * data) = 0;
        virtual void setUniformMatrix4(UInt32 location, const Matrix4x4& data) = 0;

    protected:
        Bool ready;
        Bool hasGeometryShader;
        std::string vertexSource;
        std::string fragmentSource;
        std::string geometrySource;

        virtual UInt32 createShader(ShaderType shaderType, const std::string& src) = 0;
        virtual UInt32 createProgram(const std::string& vertex, const std::string& fragment) = 0;
        virtual UInt32 createProgram(const std::string& vertex, const std::string& geometry, const std::string& fragment) = 0;
    };
}
