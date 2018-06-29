#include <regex>
#include <sstream>

#include "ShaderDirectory.h"

namespace Core {

    ShaderDirectory::~ShaderDirectory() {
    }

    void ShaderDirectory::setShader(Shader::ShaderType type, const std::string& name, const std::string& shaderSrc) {
        this->setShader(type, name, shaderSrc.c_str());
    }

    void ShaderDirectory::setShader(Shader::ShaderType type, const std::string& name, const char shaderSrc[]) {
        Entry& entry = this->entries[name];
        std::string processedSource= processShaderSource(type, shaderSrc);
        switch (type) {
            case Shader::ShaderType::Vertex:
                entry.vertexSource = processedSource;
                break;
            case Shader::ShaderType::Fragment:
                entry.fragmentSource = processedSource;
                break;
        }
    }

    const std::string& ShaderDirectory::getShader(Shader::ShaderType type, const std::string& name) {
        if (this->entries.find(name) != this->entries.end()) {
            Entry& entry = this->entries[name];
            switch (type) {
                case Shader::ShaderType::Vertex:
                    return entry.vertexSource;
                    break;
                case Shader::ShaderType::Fragment:
                    return entry.fragmentSource;
                    break;
            }
        }
        throw ShaderDirectoryException(std::string("Could not locate requested shader ") + name);
    }

    std::string ShaderDirectory::processShaderSource(Shader::ShaderType type, const std::string& src) {
        std::istringstream iss(src);
        std::string result;
        std::regex includeRegex("^#include \"[0-9a-zA-Z]*\"$");
        std::regex nameRegex("\"[0-9a-zA-Z]*\"");
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