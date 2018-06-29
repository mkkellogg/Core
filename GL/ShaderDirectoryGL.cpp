#include "ShaderDirectoryGL.h"

namespace Core {

    ShaderDirectoryGL::~ShaderDirectoryGL() {
        
    }

    void ShaderDirectoryGL::init() {

        this->setShader(Shader::ShaderType::Vertex, "Test", ShaderDirectoryGL::Test_vertex);
        this->setShader(Shader::ShaderType::Fragment, "Test", ShaderDirectoryGL::Test_fragment);

        this->setShader(Shader::ShaderType::Vertex, "Basic", ShaderDirectoryGL::Basic_vertex);
        this->setShader(Shader::ShaderType::Fragment, "Basic", ShaderDirectoryGL::Basic_fragment);

        this->setShader(Shader::ShaderType::Vertex, "BasicTextured", ShaderDirectoryGL::BasicTextured_vertex);
        this->setShader(Shader::ShaderType::Fragment, "BasicTextured", ShaderDirectoryGL::BasicTextured_fragment);

        this->setShader(Shader::ShaderType::Vertex, "BasicCube", ShaderDirectoryGL::BasicCube_vertex);
        this->setShader(Shader::ShaderType::Fragment, "BasicCube", ShaderDirectoryGL::BasicCube_fragment);
    }

    const char ShaderDirectoryGL::Test_vertex[] =
        "// some comments\n"
        "// some more comments\n";

    const char ShaderDirectoryGL::Test_fragment[] =
        "// some fragment comments\n"
        "// some morefragment comments\n";

    const char ShaderDirectoryGL::Basic_vertex[] =
        "#include \"Test\"\n"
        "#version 100\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "attribute vec2 uv;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * modelMatrix * pos;\n"
        "    vColor = color;\n"
        "}\n";

    const char ShaderDirectoryGL::Basic_fragment[] =   
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_FragColor = vColor;\n"
        "}\n";

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

    const char ShaderDirectoryGL::BasicCube_vertex[] =  
        "#version 100\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "mat4 scale = mat4(1.0, 0.0, 0.0, 0.0,\n"
        "                  0.0, 1.0, 0.0, 0.0,\n"
        "                  0.0, 0.0, 1.0, -20.0,\n"
        "                  0.0, 0.0, 0.0, 1.0);\n"
        "varying vec4 vColor;\n"
        "varying vec3 vUV;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * pos;\n"
        "    vUV = normalize(pos.xyz);\n"
        "    vColor = color;\n"
        "}\n";

    const char ShaderDirectoryGL::BasicCube_fragment[] =  
        "#version 100\n"
        "precision mediump float;\n"
        "uniform samplerCube skybox;\n"
        "varying vec4 vColor;\n"
        "varying vec3 vUV;\n"
        "void main() {\n"
        "    vec4 textureColor = textureCube(skybox, vUV);\n"
        "    gl_FragColor = textureColor;\n"
        "}\n";

}