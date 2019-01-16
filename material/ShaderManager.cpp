#include <regex>
#include <sstream>

#include "ShaderManager.h"

namespace Core {

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
        if (this->entries.find(name) != this->entries.end()) {
            Entry& entry = this->entries[name];
            switch (type) {
                case ShaderType::Vertex:
                    if (entry.vertexSource.size() > 0)
                        return processShaderSource(type, entry.vertexSource);
                case ShaderType::Geometry:
                    if (entry.geometrySource.size() > 0)
                        return processShaderSource(type, entry.geometrySource);
                case ShaderType::Fragment:
                    if (entry.fragmentSource.size() > 0)
                        return processShaderSource(type, entry.fragmentSource);          
            }
            if (entry.baseSource.size() > 0)
                return processShaderSource(type, entry.baseSource);
        }

        throw ShaderManagerException(std::string("Could not locate requested shader ") + name);
    }

    std::string ShaderManager::processShaderSource(ShaderType type, const std::string& src) {
        std::istringstream iss(src);
        std::string result;
        std::string includeName("\"[0-9a-zA-Z]*\"");
        std::regex includeRegex(std::string("^( )*#include( )+") + includeName + std::string("( )*$"));
        std::regex nameRegex(includeName);
        for (std::string line; std::getline(iss, line);) {
            if (std::regex_match(line, includeRegex)) {
                Bool matchFound = false;
                std::string match;
                for (auto i = std::sregex_iterator(line.begin(), line.end(), nameRegex); i != std::sregex_iterator(); ++i) {
                match = i->str();
                match = match.substr(1, match.size() - 2);
                matchFound = true;
                break;
                }
                if (matchFound) {
                    std::string nextShader = this->getShader(type, match);
                    result += nextShader;
                }
            }
            else {
                result += line + "\n";
            }
        }
        return result;
    }
}