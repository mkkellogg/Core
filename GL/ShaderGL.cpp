#include "ShaderGL.h"
#include <stdlib.h>
#include <string.h>
#include "../common/debug.h"

namespace Core {

    ShaderGL::ShaderGL() : glProgram(0) {
    }

    ShaderGL::ShaderGL(const std::string &vertex, const std::string &fragment) : Shader(vertex, fragment) {
    }

    ShaderGL::ShaderGL(const char vertex[], const char fragment[]) : Shader(vertex, fragment) {
    }

    ShaderGL::~ShaderGL() {
        if (this->glProgram) {
            glDeleteProgram(this->glProgram);
            this->glProgram = 0;
        }
    }

    Bool ShaderGL::build() {
        GLuint program = this->createProgram(this->vertexSource, this->fragmentSource);
        return program ? true : false;
    }

    Int32 ShaderGL::getUniformLocation(const std::string &var) const {
        return (Int32)glGetUniformLocation(this->glProgram, var.c_str());
    }

    Int32 ShaderGL::getAttributeLocation(const std::string &var) const {
        return (Int32)glGetAttribLocation(this->glProgram, var.c_str());
    }

    Int32 ShaderGL::getUniformLocation(const char var[]) const {
        return (Int32)glGetUniformLocation(this->glProgram, var);
    }

    Int32 ShaderGL::getAttributeLocation(const char var[]) const {
        return (Int32)glGetAttribLocation(this->glProgram, var);
    }

    void ShaderGL::setTexture2D(UInt32 slot, UInt32 textureID) {
        static UInt32 slots[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2};

        if (slot >= 3) {
            throw Shader::ShaderVariableException("ShaderGL::setTexture2D() value for [slot] is too high.");
        }
        glActiveTexture(slots[slot]);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    void ShaderGL::setTextureCube(UInt32 slot, UInt32 textureID) {
        static UInt32 slots[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2};

        if (slot >= 3) {
            throw Shader::ShaderVariableException("ShaderGL::setTextureCube() value for [slot] is too high.");
        }
        glActiveTexture(slots[slot]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    }

    void ShaderGL::setUniform1i(UInt32 location, Int32 val) {
        glUniform1i(location, 0);
    }

    void ShaderGL::setUniformMatrix4(UInt32 location, const Real * data) {
        glUniformMatrix4fv(location, 1, GL_FALSE, data);
    }

    void ShaderGL::setUniformMatrix4(UInt32 location, const Matrix4x4& matrix) {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.getConstData());
    }

    Bool ShaderGL::checkGlError(const char *funcName) {
        GLint err = glGetError();
        if (err != GL_NO_ERROR) {
            Debug::PrintError("GL error after %s(): 0x%08x\n", funcName, err);
            return true;
        }
        return false;
    }

    UInt32 ShaderGL::createShader(ShaderType shaderType, const std::string &src) {
        GLuint shader = glCreateShader(convertShaderType(shaderType));
        if (!shader) {
            ShaderGL::checkGlError("glCreateShader");
            return 0;
        }
        const char *srcPtr = src.c_str();
        glShaderSource(shader, 1, &srcPtr, NULL);

        GLint compiled = GL_FALSE;
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLogLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
            if (infoLogLen > 0) {
                GLchar *infoLog = (GLchar *)malloc(infoLogLen);
                if (infoLog) {
                    glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                    Debug::PrintError("Could not compile %s shader:\n%s\n", shaderType == ShaderType::Vertex ? "vertex" : "fragment", infoLog);
                    free(infoLog);
                }
            }
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    UInt32 ShaderGL::createProgram(const std::string &vertex, const std::string &fragment) {
        static char errorString[128];
        this->ready = false;
        GLuint vtxShader = 0;
        GLuint fragShader = 0;
        GLuint program = 0;
        GLint linked = GL_FALSE;

        vtxShader = createShader(ShaderType::Vertex, vertex);
        if (!vtxShader) goto exit;

        fragShader = createShader(ShaderType::Fragment, fragment);
        if (!fragShader) goto exit;

        program = glCreateProgram();
        if (!program) {
            checkGlError("glCreateProgram");
            goto exit;
        }
        glAttachShader(program, vtxShader);
        glAttachShader(program, fragShader);

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            Debug::PrintError("Could not link program");
            GLint infoLogLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
            if (infoLogLen) {
                GLchar *infoLog = (GLchar *)malloc(infoLogLen);
                if (infoLog) {
                    glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                    sprintf(errorString, "Could not link program:\n%s\n", infoLog);
                    Debug::PrintError(errorString);
                    free(infoLog);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }

        this->ready = true;
        this->glProgram = program;
    exit:
        if (vtxShader) glDeleteShader(vtxShader);
        if (fragShader) glDeleteShader(fragShader);
        return program;
    }

    GLenum ShaderGL::convertShaderType(ShaderType shaderType) {
        switch (shaderType) {
            case ShaderType::Vertex:
                return GL_VERTEX_SHADER;
            case ShaderType::Fragment:
                return GL_FRAGMENT_SHADER;
        }
        return 0;
    }

    UInt32 ShaderGL::getProgram() const {
        return (UInt32)this->glProgram;
    }
}