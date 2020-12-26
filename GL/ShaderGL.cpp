#include "ShaderGL.h"

#include <stdlib.h>
#include <string.h>

#include "../common/debug.h"
#include "../util/String.h"

namespace Core {

    ShaderGL::ShaderGL() : glProgram(0) {
    }

    ShaderGL::ShaderGL(const std::string &vertex, const std::string &fragment) : Shader(vertex, fragment) {
    }

    ShaderGL::ShaderGL(const char vertex[], const char fragment[]) : Shader(vertex, fragment) {
    }

    ShaderGL::ShaderGL(const std::string &vertex, const std::string &geometry, const std::string &fragment) : Shader(vertex, geometry, fragment) {
    }

    ShaderGL::ShaderGL(const char vertex[], const char geometry[], const char fragment[]) : Shader(vertex, geometry, fragment) {
    }

    ShaderGL::~ShaderGL() {
        if (this->glProgram) {
            glDeleteProgram(this->glProgram);
            this->glProgram = 0;
        }
    }

    Bool ShaderGL::build() {
        GLuint program;
        if (this->hasGeometryShader) {
            program = this->createProgram(this->vertexSource, this->geometrySource, this->fragmentSource);
        }
        else {
            program = this->createProgram(this->vertexSource, this->fragmentSource);
        }
        return program ? true : false;
    }

    Int32 ShaderGL::getUniformLocation(const std::string &var) const {
        return (Int32)glGetUniformLocation(this->glProgram, var.c_str());
    }

    Int32 ShaderGL::getUniformLocation(const std::string &var, UInt32 index) const {
        return this->getUniformLocation(var + "[" + std::to_string(index) + "]");
    }

    Int32 ShaderGL::getAttributeLocation(const std::string &var) const {
        return (Int32)glGetAttribLocation(this->glProgram, var.c_str());
    }

    Int32 ShaderGL::getAttributeLocation(const std::string &var, UInt32 index) const {
        return this->getAttributeLocation(var + "[" + std::to_string(index) + "]");
    }

    Int32 ShaderGL::getUniformLocation(const char var[]) const {
        return (Int32)glGetUniformLocation(this->glProgram, var);
    }

    Int32 ShaderGL::getUniformLocation(const char var[], UInt32 index) const {
        return this->getUniformLocation(std::string(var), index);
    }

    Int32 ShaderGL::getAttributeLocation(const char var[]) const {
        return (Int32)glGetAttribLocation(this->glProgram, var);
    }

    Int32 ShaderGL::getAttributeLocation(const char var[], UInt32 index) const {
        return this->getAttributeLocation(std::string(var), index);
    }

    Int32 ShaderGL::getUniformLocation(StandardUniform uniform) const {
        return this->getUniformLocation(StandardUniforms::getUniformName(uniform));
    }

    Int32 ShaderGL::getUniformLocation(StandardUniform uniform, UInt32 index) const {
        return this->getUniformLocation(StandardUniforms::getUniformName(uniform), index);
    }

    Int32 ShaderGL::getAttributeLocation(StandardAttribute attribute) const {
        return this->getAttributeLocation(StandardAttributes::getAttributeName(attribute));
    }

    Int32 ShaderGL::getAttributeLocation(StandardAttribute attribute, UInt32 index) const {
        return this->getAttributeLocation(StandardAttributes::getAttributeName(attribute), index);
    }


    void ShaderGL::setTexture2D(UInt32 slot, UInt32 textureID) {
        static UInt32 slots[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5,
                                 GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8, GL_TEXTURE9, GL_TEXTURE10, GL_TEXTURE11,
                                 GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17,
                                 GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23,
                                 GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29,
                                 GL_TEXTURE30, GL_TEXTURE31};
        if (slot >= 31) {
            std::cerr << "slot: " << slot << std::endl;
            throw Shader::ShaderVariableException("ShaderGL::setTexture2D() value for [slot] is too high.");
        }
        glActiveTexture(slots[slot]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    void ShaderGL::setTexture2D(UInt32 samplerSlot, UInt32 uniformLocation, UInt32 textureID) {
        this->setTexture2D(samplerSlot, textureID);
        this->setUniform1i(uniformLocation, samplerSlot);
    }

    void ShaderGL::setTextureCube(UInt32 slot, UInt32 textureID) {
        static UInt32 slots[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5,
                                 GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8, GL_TEXTURE9, GL_TEXTURE10, GL_TEXTURE11,
                                 GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17,
                                 GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23,
                                 GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29,
                                 GL_TEXTURE30, GL_TEXTURE31};

        if (slot >= 31) {
            std::cerr << "slot: " << slot << std::endl;
            throw Shader::ShaderVariableException("ShaderGL::setTextureCube() value for [slot] is too high.");
        }
        glActiveTexture(slots[slot]);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    }

    void ShaderGL::setTextureCube(UInt32 samplerSlot, UInt32 uniformLocation, UInt32 textureID) {
        this->setTextureCube(samplerSlot, textureID);
        this->setUniform1i(uniformLocation, samplerSlot);
    }

    void ShaderGL::setUniform1i(UInt32 location, Int32 val) {
        glUniform1i(location, val);
    }

    void ShaderGL::setUniform1f(UInt32 location, Real val) {
        glUniform1f(location, val);
    }

    void ShaderGL::setUniform2f(UInt32 location, Real x, Real y) {
        glUniform2f(location, x, y);
    }

    void ShaderGL::setUniform3f(UInt32 location, Real x, Real y, Real z) {
        glUniform3f(location, x, y, z);
    }

    void ShaderGL::setUniform4f(UInt32 location, Real x, Real y, Real z, Real w) {
        glUniform4f(location, x, y, z, w);
    }

    void ShaderGL::setUniformMatrix4(UInt32 location, const Real *data) {
        glUniformMatrix4fv(location, 1, GL_FALSE, data);
    }

    void ShaderGL::setUniformMatrix4(UInt32 location, const Matrix4x4 &matrix) {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.getConstData());
    }

    std::string ShaderGL::shaderTypeString(ShaderType shaderType) {
        switch (shaderType) {
            case ShaderType::Vertex:
                return "vertex";
            case ShaderType::Fragment:
                return "fragment";
            case ShaderType::Geometry:
                return "geometry";
        }
        return "";
    }

    Bool ShaderGL::checkGlError(const char *funcName) {
        GLint err = glGetError();
        if (err != GL_NO_ERROR) {
            std::string msg("GL error after ");
            msg += funcName;
            msg += "0x";
            msg += err;
            throw ShaderCompilationException(msg);
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
            std::string msg("Could not compile ");
            msg += shaderTypeString(shaderType);
            msg += " shader\n\n";
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
            if (infoLogLen > 0) {
                GLchar *infoLog = (GLchar *)malloc(infoLogLen);
                if (infoLog) {
                    glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                    msg += infoLog;
                    msg += "\n\n";        
                    free(infoLog);
                }
            }
            glDeleteShader(shader);
            msg += String::addLineNumbers(src);
            msg += "\n\n";            
            throw ShaderCompilationException(msg);
            return 0;
        }
        return shader;
    }

    UInt32 ShaderGL::createProgram(const std::string &vertex, const std::string &fragment) {
        return this->createProgramInternal(vertex, fragment, nullptr);
    }

    UInt32 ShaderGL::createProgram(const std::string &vertex, const std::string &geometry, const std::string &fragment) {
        return this->createProgramInternal(vertex, fragment, &geometry);
    }

    UInt32 ShaderGL::createProgramInternal(const std::string &vertex, const std::string &fragment, const std::string* geometry) {
        static char errorString[128];
        this->ready = false;
        GLuint vtxShader = 0;
        GLuint geoShader = 0;
        GLuint fragShader = 0;
        GLuint program = 0;
        GLint linked = GL_FALSE;

        vtxShader = createShader(ShaderType::Vertex, vertex);
        if (!vtxShader) goto exit;

        if (geometry != nullptr) {
            geoShader = createShader(ShaderType::Geometry, *geometry);
            if (!geoShader) goto exit;
        }

        fragShader = createShader(ShaderType::Fragment, fragment);
        if (!fragShader) goto exit;

        program = glCreateProgram();
        if (!program) {
            checkGlError("glCreateProgram");
            goto exit;
        }

        /*if (geoShader) {
            glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
            glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);
            glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, 6);
        }*/

        glAttachShader(program, vtxShader);
        if (geoShader) glAttachShader(program, geoShader);
        glAttachShader(program, fragShader);

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint infoLogLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
            if (infoLogLen) {
                GLchar *infoLog = (GLchar *)malloc(infoLogLen);
                if (infoLog) {
                    glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                    sprintf(errorString, "Could not link program:\n%s\n", infoLog);
                    free(infoLog);
                }
            }
            else {
                sprintf(errorString, "Could not link program");
            }
            glDeleteProgram(program);
            program = 0;
            throw ShaderCompilationException(errorString);
        }

        this->ready = true;
        this->glProgram = program;
    exit:
        if (vtxShader) glDeleteShader(vtxShader);
        if (geoShader) glDeleteShader(geoShader);
        if (fragShader) glDeleteShader(fragShader);
        return program;
    }

    GLenum ShaderGL::convertShaderType(ShaderType shaderType) {
        switch (shaderType) {
            case ShaderType::Vertex:
                return GL_VERTEX_SHADER;
            case ShaderType::Fragment:
                return GL_FRAGMENT_SHADER;
            case ShaderType::Geometry:
#ifdef GL_GEOMETRY_SHADER
                return GL_GEOMETRY_SHADER;
#else 
                return GL_GEOMETRY_SHADER_EXT;
#endif
        }
        return 0;
    }

    UInt32 ShaderGL::getProgram() const {
        return (UInt32)this->glProgram;
    }
}