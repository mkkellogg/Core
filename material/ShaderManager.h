#pragma once

#include <unordered_map>
#include <vector>

#include "../common/types.h"
#include "../common/Exception.h"
#include "Shader.h"

namespace Core {

    class ShaderManager {
    public:

        class IncludeParameterCollection {
            
            std::unordered_map<std::string, std::string> paramNameValueMap;

        public:

            Core::Bool containsParameter(const std::string& paramName) const {
                return this->paramNameValueMap.find(paramName) != this->paramNameValueMap.end();
            }

            std::string getValue(const std::string& paramName) const {
                if (!this->containsParameter(paramName)) {
                    throw KeyNotFoundException(std::string("Include parameter not found: ") + paramName);
                }
                return paramNameValueMap.at(paramName);
            }

            void setValue(const std::string& paramName, const std::string& value) {
                paramNameValueMap[paramName] = value;
            }

            Core::UInt32 parameterCount() const {
                return paramNameValueMap.size();
            }
        };

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
            std::string geometrySource;
            std::string fragmentSource;
        };

        virtual ~ShaderManager();
        void setShader(ShaderType type, const std::string& name, const std::string& shaderSrc);
        void setShader(ShaderType type, const std::string& name, const char shaderSrc[]);
        std::string getShader(ShaderType type, const std::string& name);
        std::string getShader(ShaderType type, const std::string& name, const IncludeParameterCollection& params);

    protected:
        static const std::string INCLUDE_VAL_PATTERN;
        static const std::string INCLUDE_PARAMS_PATTERN;
        std::unordered_map<std::string, Entry> entries;

        std::string processShaderSource(ShaderType type, const std::string& src, const IncludeParameterCollection& params);
        std::string processIncludeDirective(ShaderType type, const std::string& includeLine, const IncludeParameterCollection& params);
        std::string processLineWithVariables(ShaderType type, const std::string& varLine, const IncludeParameterCollection& params);
        std::vector<std::string> splitStr(const std::string& s, char delimiter);
    };
}