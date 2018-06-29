#include "ShaderDirectory.h"

namespace Core {

    ShaderDirectory::~ShaderDirectory() {
    }

    void ShaderDirectory::addEntry(const std::string& name, const Entry& entry) {
        if (this->entries.find(name) != this->entries.end()) {
            throw ShaderDirectoryException(std::string("Cannot add shader for ") + name + std::string(" because it already exists."));
        }
        this->entries[name] = entry;
    }

    void ShaderDirectory::setShader(Shader::ShaderType type, const std::string& name, const std::string& shaderSrc) {
        this->setShader(type, name, shaderSrc.c_str());
    }

    void ShaderDirectory::setShader(Shader::ShaderType type, const std::string& name, const char shaderSrc[]) {
        Entry& entry = this->entries[name];
        switch (type) {
            case Shader::ShaderType::Vertex:
                entry.vertexSource = shaderSrc;
                break;
            case Shader::ShaderType::Fragment:
                entry.fragmentSource = shaderSrc;
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
        throw ShaderDirectoryException(std::string("Could not locate request shader ") + name);
    }
}