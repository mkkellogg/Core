#include <regex>
#include <sstream>

#include "ShaderManager.h"
#include "../util/String.h"

static const Core::UInt32 ASCII_ZERO = 48;
static const Core::UInt32 ASCII_NINE = 57;
static const Core::UInt32 ASCII_A = 65;
static const Core::UInt32 ASCII_Z = 90;
static const Core::UInt32 ASCII_a = 97;
static const Core::UInt32 ASCII_z = 122;

namespace Core {

    const std::string ShaderManager::INCLUDE_PARAMS_PATTERN(".*");
    const std::string ShaderManager::INCLUDE_VAL_PATTERN(std::string("\"[0-9a-zA-Z]+(\\(") + INCLUDE_PARAMS_PATTERN + std::string("\\)){0,1}\""));

    ShaderManager::~ShaderManager() {
    }

    void ShaderManager::setShader(ShaderType type, const std::string& name, const std::string& shaderSrc) {
        this->setShader(type, name, shaderSrc.c_str());
    }

    void ShaderManager::setShader(ShaderType type, const std::string& name, const char shaderSrc[]) {
        Entry& entry = this->entries[name];
        switch (type) {
            case ShaderType::Vertex:
                entry.vertexSource = shaderSrc;
                break;
            case ShaderType::Geometry:
                entry.geometrySource = shaderSrc;
                break;
            case ShaderType::Fragment:
                entry.fragmentSource = shaderSrc;
                break;
            case ShaderType::Base:
                entry.baseSource = shaderSrc;
                break;
        }
    }

    std::string ShaderManager::getShader(ShaderType type, const std::string& name) {
        IncludeParameterCollection params;
        return this->getShader(type, name, params);
    }

    std::string ShaderManager::getShader(ShaderType type, const std::string& name, const IncludeParameterCollection& params) {
        if (this->entries.find(name) != this->entries.end()) {
            Entry& entry = this->entries[name];
            switch (type) {
                case ShaderType::Vertex:
                    if (entry.vertexSource.size() > 0)
                        return processShaderSource(type, entry.vertexSource, params);
                case ShaderType::Geometry:
                    if (entry.geometrySource.size() > 0)
                        return processShaderSource(type, entry.geometrySource, params);
                case ShaderType::Fragment:
                    if (entry.fragmentSource.size() > 0)
                        return processShaderSource(type, entry.fragmentSource, params);          
            }
            if (entry.baseSource.size() > 0)
                return processShaderSource(type, entry.baseSource, params);
        }

        throw ShaderManagerException(std::string("Could not locate requested shader ") + name);
    }

    std::string ShaderManager::processShaderSource(ShaderType type, const std::string& src, const IncludeParameterCollection& params) {
        std::istringstream iss(src);
        std::string result;
        std::regex includeLineRegex(std::string("^( )*#include( )+") + INCLUDE_VAL_PATTERN + std::string("( )*$"));
        std::regex variableLineRegex("^.*@.*$");
        for (std::string line; std::getline(iss, line);) {
            if (std::regex_match(line, variableLineRegex)) {
                line = this->processLineWithVariables(type, line, params);
            }
            if (std::regex_match(line, includeLineRegex)) {
                result += processIncludeDirective(type, line, params);
            } else {
                result += line + "\n";
            }
        }
        return result;
    }

    std::string ShaderManager::processIncludeDirective(ShaderType type, const std::string& includeLine, const IncludeParameterCollection& params) {
        std::regex includeValRegex(INCLUDE_VAL_PATTERN);
        std::regex paramsContainerRegex(std::string("^( )*\\(") + INCLUDE_PARAMS_PATTERN + std::string("\\)( )*$"));
        Bool matchFound = false;
        std::string match;
        std::string name;

        IncludeParameterCollection includeParams;
        for (auto i = std::sregex_iterator(includeLine.begin(), includeLine.end(), includeValRegex); i != std::sregex_iterator(); ++i) {
            match = i->str();
            match = match.substr(1, match.size() - 2);

            size_t firstParensPos = match.find_first_of('(');
            size_t lastParensPos = match.find_last_of(')');

            if (firstParensPos != std::string::npos) {
                name = match.substr(0, firstParensPos);
            } else {
                name = match;
            }

            if (firstParensPos != std::string::npos && lastParensPos != std::string::npos) {
                std::string paramsStr = match.substr(firstParensPos + 1, lastParensPos - firstParensPos - 1);
                std::vector<std::string> paramTokens = this->splitStr(paramsStr, ',');
                for (size_t i = 0; i < paramTokens.size(); i++) {
                    std::string& token = paramTokens[i];
                    std::vector<std::string> nameVal = splitStr(token, '=');
                    if (nameVal.size() == 2) {
                        std::string paramName = nameVal[0];
                        std::string paramValue = nameVal[1];
                        Core::String::trim(paramName);
                        Core::String::trim(paramValue);
                        includeParams.setValue(paramName, paramValue);
                    }
                }
            }

            matchFound = true;
            break;
        }
        if (matchFound) {
            return this->getShader(type, name, includeParams);
        }
        return std::string("");
    }

    std::string ShaderManager::processLineWithVariables(ShaderType type, const std::string& varLine, const IncludeParameterCollection& params) {
        size_t s = 0;
        std::stringstream newStringStream;
        while (s < varLine.size()) {
            char curChar = varLine[s];
            if (varLine[s] == '@') {
                size_t e = s + 1;
                std::stringstream paramNameStream;
                while (e < varLine.size()) {
                    char testChar = varLine[e];
                    int asc = (int)testChar;
                    if (asc >= ASCII_ZERO && asc <= ASCII_NINE || asc >= ASCII_A && asc <= ASCII_Z || asc >= ASCII_a && asc <= ASCII_z) {
                        paramNameStream << testChar;
                        e++;
                    } else {
                        break;
                    }
                }
                const std::string paramName = paramNameStream.str();
                if (params.containsParameter(paramName)) {
                    newStringStream << params.getValue(paramName);
                } else {
                    newStringStream << "!!" << paramName;
                }
                s = e;
            } else {
                newStringStream << curChar;
                s++;
            }
        }
        return newStringStream.str();
    }

    std::vector<std::string> ShaderManager::splitStr(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
}