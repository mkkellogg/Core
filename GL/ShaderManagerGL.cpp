#include "ShaderManagerGL.h"

namespace Core {

    ShaderManagerGL::~ShaderManagerGL() {
        
    }

    void ShaderManagerGL::init() {

        this->setShader(ShaderType::Vertex, "Test", ShaderManagerGL::Test_vertex);
        this->setShader(ShaderType::Fragment, "Test", ShaderManagerGL::Test_fragment);

        this->setShader(ShaderType::Vertex, "Depth", ShaderManagerGL::Depth_vertex);
        this->setShader(ShaderType::Fragment, "Depth", ShaderManagerGL::Depth_fragment);

        this->setShader(ShaderType::Vertex, "Distance", ShaderManagerGL::Distance_vertex);
        this->setShader(ShaderType::Fragment, "Distance", ShaderManagerGL::Distance_fragment);

        this->setShader(ShaderType::Vertex, "Basic", ShaderManagerGL::Basic_vertex);
        this->setShader(ShaderType::Fragment, "Basic", ShaderManagerGL::Basic_fragment);

        this->setShader(ShaderType::Vertex, "BasicTextured", ShaderManagerGL::BasicTextured_vertex);
        this->setShader(ShaderType::Fragment, "BasicTextured", ShaderManagerGL::BasicTextured_fragment);

        this->setShader(ShaderType::Vertex, "BasicTexturedLit", ShaderManagerGL::BasicTexturedLit_vertex);
        this->setShader(ShaderType::Fragment, "BasicTexturedLit", ShaderManagerGL::BasicTexturedLit_fragment);

        this->setShader(ShaderType::Vertex, "BasicCube", ShaderManagerGL::BasicCube_vertex);
        this->setShader(ShaderType::Fragment, "BasicCube", ShaderManagerGL::BasicCube_fragment);
    }

    const char ShaderManagerGL::Test_vertex[] =
        "// some comments\n"
        "// some more comments\n";

    const char ShaderManagerGL::Test_fragment[] =
        "// some fragment comments\n"
        "// some morefragment comments\n";

    const char ShaderManagerGL::Depth_vertex[] =
        "#include \"Test\"\n"
        "#version 100\n"
        "attribute vec4 pos;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * modelMatrix * pos;\n"
        "}\n";

    const char ShaderManagerGL::Depth_fragment[] =   
        "#version 100\n"
        "precision mediump float;\n"
        "void main() {\n"
        "    gl_FragColor = vec4(gl_FragCoord.z, 0.0, 0.0, 0.0);\n"
        "}\n";

    const char ShaderManagerGL::Distance_vertex[] =
        "#include \"Test\"\n"
        "#version 100\n"
        "attribute vec4 pos;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "varying vec4 vPos;\n"
        "void main() {\n"
        "    vPos = modelMatrix * pos;\n"
        "    gl_Position = projection * viewMatrix * vPos;\n"
        "}\n";

    const char ShaderManagerGL::Distance_fragment[] =   
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec4 vPos;\n"
        "void main() {\n"
        "    gl_FragColor = vec4(length(vPos.xyz), 0.0, 0.0, 0.0);\n"
        "}\n";

    const char ShaderManagerGL::Basic_vertex[] =
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

    const char ShaderManagerGL::Basic_fragment[] =   
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_FragColor = vColor;\n"
        "}\n";

    const char ShaderManagerGL::BasicTextured_vertex[] =  
        "#version 100\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "attribute vec2 uv;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "varying vec4 vColor;\n"
        "varying vec3 vNormal;\n"
        "varying vec2 vUV;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * modelMatrix * pos;\n"
        "    vUV = uv;\n"
        "    vColor = color;\n"
        "}\n";

    const char ShaderManagerGL::BasicTextured_fragment[] =   
        "#version 100\n"
        "precision mediump float;\n"
        "uniform sampler2D textureA;\n"
        "varying vec4 vColor;\n"
        "varying vec2 vUV;\n"
        "void main() {\n"
        "    vec4 textureColor = texture2D(textureA, vUV);\n"
        "    gl_FragColor = textureColor;\n"
        "}\n";

    const char ShaderManagerGL::BasicTexturedLit_vertex[] =  
        "#version 100\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "attribute vec4 normal;\n"
        "attribute vec2 uv;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 modelInverseTransposeMatrix;\n"
        "varying vec4 vColor;\n"
        "varying vec3 vNormal;\n"
        "varying vec2 vUV;\n"
        "varying vec4 vPos;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * modelMatrix * pos;\n"
        "    vPos = modelMatrix * pos;\n"
        "    vUV = uv;\n"
        "    vColor = color;\n"
        "    vNormal = vec3(modelInverseTransposeMatrix * normal);\n"
        "}\n";

    const char ShaderManagerGL::BasicTexturedLit_fragment[] =   
        "#version 100\n"
        "precision mediump float;\n"
        "uniform sampler2D textureA;\n"
        "uniform vec4 lightPos;\n"
        "uniform float lightRange;\n"
        "uniform int lightType;\n"
        "uniform int lightEnabled;\n"
        "uniform vec4 lightColor;\n"
        "varying vec4 vColor;\n"
        "varying vec3 vNormal;\n"
        "varying vec2 vUV;\n"
        "varying vec4 vPos;\n"
        "void main() {\n"
        "    if (lightEnabled != 0) { \n"
        "         vec4 textureColor = texture2D(textureA, vUV);\n"
        "         vec4 fragPos = vPos;\n"
        "         vec4 realLightPos = lightPos;\n"
        "         vec3 toLight = normalize(vec3(realLightPos - fragPos));\n"
        "         float aAtten = max(dot(normalize(vNormal), toLight), 0.0);\n"
        "         gl_FragColor = vec4(textureColor.rgb * aAtten, textureColor.a);\n"
        "    } \n"
        "    else { \n"
        "         gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
        "    }\n"
        "}\n";

    const char ShaderManagerGL::BasicCube_vertex[] =  
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

    const char ShaderManagerGL::BasicCube_fragment[] =  
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