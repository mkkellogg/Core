#include "ShaderDirectoryGL.h"

namespace Core {

    ShaderDirectoryGL::~ShaderDirectoryGL() {
        
    }

    void ShaderDirectoryGL::init() {
        this->setShader(Shader::ShaderType::Vertex, "BasicTextured", ShaderDirectoryGL::BasicTextured_vertex);
        this->setShader(Shader::ShaderType::Fragment, "BasicTextured", ShaderDirectoryGL::BasicTextured_fragment);
    }

    const char ShaderDirectoryGL::BasicTextured_vertex[] =  
        "#version 100\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "attribute vec2 uv;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "varying vec4 vColor;\n"
        "varying vec2 vUV;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * modelMatrix * pos;\n"
        "    vUV = uv;\n"
        "    vColor = color;\n"
        "}\n";

    const char ShaderDirectoryGL::BasicTextured_fragment[] =   
        "#version 100\n"
        "precision mediump float;\n"
        "uniform sampler2D textureA;\n"
        "varying vec4 vColor;\n"
        "varying vec2 vUV;\n"
        "void main() {\n"
        "    vec4 textureColor = texture2D(textureA, vUV);\n"
        "    gl_FragColor = textureColor;\n"
        "}\n";

}