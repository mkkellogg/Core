#include "ShaderManagerGL.h"

namespace Core {

    ShaderManagerGL::~ShaderManagerGL() {
        
    }

    void ShaderManagerGL::init() {

        this->setShader(ShaderType::Vertex, "Test", ShaderManagerGL::Test_vertex);
        this->setShader(ShaderType::Fragment, "Test", ShaderManagerGL::Test_fragment);

        this->setShader(ShaderType::Fragment, "Lighting", ShaderManagerGL::Lighting_fragment);

        this->setShader(ShaderType::Vertex, "Depth", ShaderManagerGL::Depth_vertex);
        this->setShader(ShaderType::Fragment, "Depth", ShaderManagerGL::Depth_fragment);

        this->setShader(ShaderType::Vertex, "Distance", ShaderManagerGL::Distance_vertex);
        this->setShader(ShaderType::Fragment, "Distance", ShaderManagerGL::Distance_fragment);

        this->setShader(ShaderType::Vertex, "Basic", ShaderManagerGL::Basic_vertex);
        this->setShader(ShaderType::Fragment, "Basic", ShaderManagerGL::Basic_fragment);

        this->setShader(ShaderType::Vertex, "BasicLit", ShaderManagerGL::BasicLit_vertex);
        this->setShader(ShaderType::Fragment, "BasicLit", ShaderManagerGL::BasicLit_fragment);

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

    const char ShaderManagerGL::Lighting_fragment[] =
        "uniform sampler2D lightShadowMap;\n"
        "uniform samplerCube lightShadowCubeMap;\n"
        "uniform float lightShadowBias;\n"
        "uniform mat4 lightMatrix;\n"
        "uniform vec4 lightPos;\n"
        "uniform float lightRange;\n"
        "uniform int lightType;\n"
        "uniform int lightEnabled;\n"
        "uniform vec4 lightColor;\n"

        "vec4 litColor(in vec4 baseColor, in vec4 fragPos, in vec3 fragNormal) {\n"
        "    if (lightEnabled != 0) {\n"
        "        if (lightType == 0) {\n"
        "            return vec4(baseColor.rgb * lightColor.rgb, baseColor.a);\n"
        "        }\n"
        "        else if (lightType == 2) {\n"
        "            vec3 lightLocalFragPos = vec3(lightMatrix * fragPos);\n"
        "            vec4 shadowDepthVec = texture(lightShadowCubeMap, lightLocalFragPos);\n"
        "            float shadowDepth = shadowDepthVec.r;\n"
        "            vec3 toLight = normalize(vec3(lightPos - fragPos));\n"
        "            float baseDot = max(cos(acos(dot(toLight, fragNormal)) * 1.025), 0.0); \n"
        "            float bias = (1.0 - baseDot) * lightShadowBias;\n"
        "            if (shadowDepth + bias > length(lightLocalFragPos) || shadowDepth < .001) {\n"
        "                float aAtten = baseDot;\n"
        "                return vec4(lightColor.rgb * baseColor.rgb * aAtten, baseColor.a);\n"
        "            }\n"
        "            return vec4(0.0, 0.0, 0.0, 1.0);\n"
        "        }\n"
        "    }\n"
        "    return baseColor;\n"
        "}\n";

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
        "#version 330\n"
        "attribute vec4 pos;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "out vec4 vPos;\n"
        "void main() {\n"
        "    vPos = viewMatrix * modelMatrix * pos;\n"
      //  "    vPos.y = 1.0 - vPos.y;\n"
        "    gl_Position = projection * vPos;\n"
        "}\n";

    const char ShaderManagerGL::Distance_fragment[] =   
        "#version 330\n"
        "precision highp float;\n"
        "in vec4 vPos;\n"
        "out vec4 out_color;\n"
        "void main() {\n"
        "   float len = length(vPos.xyz);\n"
       "    gl_FragColor = vec4(len, 0.0, 0.0, 0.0);\n"
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

     const char ShaderManagerGL::BasicLit_vertex[] =  
        "#version 150\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "attribute vec4 normal;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 modelInverseTransposeMatrix;\n"
        "out vec4 vColor;\n"
        "out vec3 vNormal;\n"
        "out vec4 vPos;\n"
        "void main() {\n"
        "    vPos = modelMatrix * pos;\n"
        "    gl_Position = projection * viewMatrix * vPos;\n"
        "    vColor = color;\n"
        "    vNormal = vec3(modelInverseTransposeMatrix * normal);\n"
        "}\n";

    const char ShaderManagerGL::BasicLit_fragment[] =   
        "#version 150\n"
        "#extension GL_NV_shadow_samplers_cube : enable\n"
        "precision highp float;\n"
        "#include \"Lighting\"\n"
        "in vec4 vColor;\n"
        "in vec3 vNormal;\n"
        "in vec4 vPos;\n"
        "out vec4 out_color;\n"

        "void main() {\n"
        "   out_color = litColor(vColor, vPos, normalize(vNormal));\n"
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
        "#version 150\n"
        "precision highp float;\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "attribute vec4 normal;\n"
        "attribute vec4 faceNormal;\n"
        "attribute vec2 uv;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 modelInverseTransposeMatrix;\n"
        "uniform vec4 lightPos;\n"
        "out vec4 vColor;\n"
        "out vec3 vNormal;\n"
        "out vec3 vFaceNormal;\n"
        "out vec2 vUV;\n"
        "out vec4 vPos;\n"
        "void main() {\n"
        "    vPos = modelMatrix * pos;\n"
        "    gl_Position = projection * viewMatrix * vPos;\n"
        "    vUV = uv;\n"
        "    vColor = color;\n"
        "    vec4 eNormal = normal;\n"
        "    vec3 toLight = normalize(lightPos.xyz - vPos.xyz);\n"
      //  "    if (dot(toLight, faceNormal.xyz) <= 0.0) eNormal = faceNormal;\n"
        "    vNormal = vec3(modelInverseTransposeMatrix * eNormal);\n"
        "    vFaceNormal = vec3(modelInverseTransposeMatrix * faceNormal);\n"
        "}\n";

    const char ShaderManagerGL::BasicTexturedLit_fragment[] =   
        "#version 150\n"
        "precision highp float;\n"
        "#include \"Lighting\"\n"
        "uniform sampler2D textureA;\n"
        "in vec4 vColor;\n"
        "in vec3 vNormal;\n"
        "in vec3 vFaceNormal;\n"
        "in vec2 vUV;\n"
        "in vec4 vPos;\n"
        "out vec4 out_color;\n"
        "void main() {\n"
        "   out_color = litColor(texture(textureA, vUV), vPos, normalize(vNormal));\n"
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