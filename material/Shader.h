#pragma once

#include <string>
#include "../common/types.h"

namespace Core {

    class Shader {
    public:
        enum class ShaderType { 
            Vertex = 1, 
            Fragment = 2 
        };

        Shader();
        Shader(const std::string& vertex, const std::string& fragment);
        Shader(const char vertex[], const char fragment[]);
        virtual ~Shader();

        Bool isReady() const;

        virtual Bool build() = 0;
        virtual UInt32 getProgram() const = 0;

    protected:
        Bool ready;
        std::string vertexSource;
        std::string fragmentSource;

        virtual UInt32 createShader(ShaderType shaderType, const std::string& src) = 0;
        virtual UInt32 createProgram(const std::string& vertex, const std::string& fragment) = 0;
    };
}
