#include <string>

#include "ShaderManagerGL.h"
#include "../common/Constants.h"

auto _un = Core::StandardUniforms::getUniformName;
auto _an = Core::StandardAttributes::getAttributeName;

const std::string POSITION = _an(Core::StandardAttribute::Position);
const std::string POSITION_DEF = "in vec4 " +  POSITION + ";\n";

const std::string NORMAL = _an(Core::StandardAttribute::Normal);
const std::string NORMAL_DEF = "attribute vec4 " +  NORMAL + ";\n";

const std::string FACE_NORMAL = _an(Core::StandardAttribute::FaceNormal);
const std::string FACE_NORMAL_DEF = "attribute vec4 " +  FACE_NORMAL + ";\n";

const std::string COLOR = _an(Core::StandardAttribute::Color);
const std::string COLOR_DEF = "attribute vec4 " +  COLOR + ";\n";

const std::string UV = _an(Core::StandardAttribute::UV0);
const std::string UV_DEF = "attribute vec2 " +  UV + ";\n";

const std::string MODEL_MATRIX = _un(Core::StandardUniform::ModelMatrix);
const std::string MODEL_MATRIX_DEF = "uniform mat4 " +  MODEL_MATRIX + ";\n";

const std::string VIEW_MATRIX = _un(Core::StandardUniform::ViewMatrix);
const std::string VIEW_MATRIX_DEF = "uniform mat4 " +  VIEW_MATRIX + ";\n";

const std::string PROJECTION_MATRIX = _un(Core::StandardUniform::ProjectionMatrix);
const std::string PROJECTION_MATRIX_DEF = "uniform mat4 " +  PROJECTION_MATRIX + ";\n";

namespace Core {

    const std::string ShaderManagerGL::BaseString("");

    ShaderManagerGL::~ShaderManagerGL() {
        
    }

    void ShaderManagerGL::init() {

        this->setShader(ShaderType::Vertex, "Lighting", ShaderManagerGL::Lighting_vertex);
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

    ShaderManagerGL::ShaderManagerGL() {

        this->Lighting_vertex =
            ShaderManagerGL::BaseString + 
            "const int MAX_CASCADES =" + std::to_string(Constants::MaxDirectionalCascades) + ";\n" +
            "uniform int lightCascadeCount;\n"
            "uniform mat4 lightViewProjection[MAX_CASCADES];\n"
            "out vec4 lightSpacePos[MAX_CASCADES];\n"
            "out float viewSpacePosZ;\n"
            "#define TRANSFER_LIGHTING(localPos, clipSpacePos, viewSpacePos) "
            "for (int i = 0 ; i < lightCascadeCount; i++) { "
            "    lightSpacePos[i] = lightViewProjection[i] * " + MODEL_MATRIX + " * (localPos); "
            "} "
            "viewSpacePosZ = (viewSpacePos).z;\n";

        this->Lighting_fragment =
            ShaderManagerGL::BaseString + 
            "const int MAX_CASCADES =" + std::to_string(Constants::MaxDirectionalCascades) + ";\n" +
            "uniform int lightCascadeCount;\n"
            "uniform sampler2DShadow lightShadowMap[MAX_CASCADES];\n"
            "uniform float lightCascadeEnd[MAX_CASCADES];\n"
            "uniform float lightShadowMapAspect[MAX_CASCADES];\n"
            "in vec4 lightSpacePos[MAX_CASCADES];\n"
            "in float viewSpacePosZ;\n"
            "uniform samplerCube lightShadowCubeMap;\n"
            "uniform float lightAngularShadowBias;\n"
            "uniform float lightConstantShadowBias;\n"
            "uniform mat4 lightMatrix;\n"
            "uniform vec4 lightPos;\n"
            "uniform vec4 lightDir;\n"
            "uniform float lightRange;\n"
            "uniform int lightType;\n"
            "uniform int lightEnabled;\n"
            "uniform int lightShadowSoftness;\n"
            "uniform vec4 lightColor;\n"
            "uniform float lightShadowMapSize;\n"

            "float calDirShadowFactorSingleIndex(int index, vec2 uv, float fragDepth, float angularBias) { \n"
            "    vec3 coords = vec3(uv.xy, fragDepth - angularBias - lightConstantShadowBias); \n"
            "    float shadowDepth = clamp(texture(lightShadowMap[index], coords), 0.0, 1.0); \n"
            "    float realFragDepth = clamp(fragDepth - angularBias - lightConstantShadowBias, 0.0, 1.0); \n"
            "    return (1.0-shadowDepth); \n"
            "} \n"

            "float calcDirShadowFactor(int cascadeIndex, vec4 lSpacePos, float angularBias, float baseDot, vec4 fragPos)\n"
            "{ \n"
            "    vec3 projCoords = lSpacePos.xyz / lSpacePos.w; \n"
            "    vec3 uvCoords = (projCoords * 0.5) + vec3(0.5, 0.5, 0.5); \n"
            "    float pxMag = clamp(1.0 / (baseDot + .0001), 1.0, 1.0); \n"
            "    float px = 1.0 / lightShadowMapSize * pxMag; \n"
            "    float py =  lightShadowMapAspect[cascadeIndex] / lightShadowMapSize * pxMag; \n"

            "    float shadowFactor = 0.0; \n"
            "    vec2 uv = uvCoords.xy; \n"
            "    float z = uvCoords.z; \n"

            "    if (uvCoords.x < 0 || uvCoords.x > 1.0) return 0.0; \n"
            "    if (uvCoords.y < 0 || uvCoords.y > 1.0) return 0.0; \n"

            "    if (lightShadowSoftness == 2 || lightShadowSoftness == 1) { \n"
            "        for (int y = -lightShadowSoftness ; y <= lightShadowSoftness ; y++) { \n"
            "            for (int x = -lightShadowSoftness ; x <= lightShadowSoftness ; x++) { \n"
            "                shadowFactor += calDirShadowFactorSingleIndex(cascadeIndex, vec2(uv.x + x * px, uv.y + y * py), z, angularBias); \n"
            "            } \n"
            "        } \n "
            "        if (lightShadowSoftness == 2) shadowFactor /= 25.0; \n"
            "        else shadowFactor /= 9.0; \n"
            "    } \n"
            "    else { \n"
            "        shadowFactor += calDirShadowFactorSingleIndex(cascadeIndex, uv, z, angularBias); \n"
            "    } \n"

            "    return shadowFactor; \n"
            "} \n"

            "vec4 getDirLightColor(vec4 baseColor, float bias, float baseDot, vec4 fragPos) { \n"
            "    float shadowFactor = 0.0;\n"
            "    for (int i = 0 ; i < lightCascadeCount ; i++) { \n"
            "        if (viewSpacePosZ <= lightCascadeEnd[i]) { \n"
            "            shadowFactor = calcDirShadowFactor(i, lightSpacePos[i], bias, baseDot, fragPos); \n"
            "            break; \n"
            "        } \n"
            "    } \n"
            "    return vec4((1.0 - shadowFactor) * lightColor.rgb * baseColor.rgb * baseDot, baseColor.a);\n"      
            "} \n"

            "float getPointLightShadowFactor(vec3 lightLocalFragPos, float bias) { \n"
            "   vec4 shadowDepthVec = texture(lightShadowCubeMap, lightLocalFragPos);\n"
            "   float shadowDepth = shadowDepthVec.r;\n"
            "   float shadowFactor = 0.0; \n"
            "   if (shadowDepth + bias > length(lightLocalFragPos) || shadowDepth < .001) {\n"
            "       shadowFactor = 1.0;\n"
            "   }\n"
            "   return shadowFactor;\n"
            "} \n"

            "vec4 getPointLightColor(vec4 baseColor, vec3 lightLocalFragPos, float bias, float atten) { \n"
            "   vec3 sVec = normalize(lightLocalFragPos); \n"
            "   float pxToWorld = 1.0 / lightShadowMapSize * 0.2; \n"
            "   float shadowFactor = 0.0; \n"
            "   vec3 forward = vec3(0.0, 0.0, 0.0); \n "
            "   vec3 up = vec3(0.0, 1.0, 0.0); \n "
            "   vec3 right = vec3(0.0, 0.0, 0.0); \n "
            "   float scale = 1.0; \n"
            "   float near = 0.1; \n"
            "   float absX = abs(lightLocalFragPos.x); \n"
            "   float absY = abs(lightLocalFragPos.y); \n"
            "   float absZ = abs(lightLocalFragPos.z); \n"
            "   if (absZ > absX && absZ > absY) {"
            "      forward = vec3(0.0, 0.0, sign(lightLocalFragPos.z)); \n"
            "      scale = absZ / near; \n"
            "   }"
            "   else if (absX > absZ && absX > absY) {"
            "      forward = vec3(sign(lightLocalFragPos.x), 0.0, 0.0); \n"
            "      scale = absX / near; \n"
            "   }"
            "   else {"
            "      forward = vec3(0.0, sign(lightLocalFragPos.y), 0.0); \n"
            "      up = vec3(0.0, 0.0, forward.y); \n"
            "      scale = absY / near; \n"
            "   }"
            "   right = cross(forward, up); \n"

            "   right = right * pxToWorld * scale; \n"
            "   up = up * pxToWorld * scale; \n"

            "    if (lightShadowSoftness == 2 || lightShadowSoftness == 1) { \n"
            "        shadowFactor += getPointLightShadowFactor(lightLocalFragPos, bias); \n"
            "        for (int i = 1; i <= lightShadowSoftness; i++) { \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos + right * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos - right * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos - up * i, bias); \n"

            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos + right * i + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos + right * i - up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos - right * i + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos - right * i - up * i, bias); \n"

            "        } \n "
            "        if (lightShadowSoftness == 2) shadowFactor /= 17.0; \n"
            "        else shadowFactor /= 9.0; \n"
            "    } \n"
            "    else { \n"
            "        shadowFactor += getPointLightShadowFactor(lightLocalFragPos, bias); \n"
            "    } \n"       
           
            "   return vec4(lightColor.rgb * baseColor.rgb * atten * shadowFactor, baseColor.a);\n"
            "} \n"

            "vec4 litColor(in vec4 baseColor, in vec4 fragPos, in vec3 fragNormal) {\n"
            "    if (lightEnabled != 0) {\n"
            "        if (lightType == 0) {\n"
            "            return vec4(baseColor.rgb * lightColor.rgb, baseColor.a);\n"
            "        }\n"
            "        else if (lightType == 1) {\n"
            "            int shadowCount = 0; \n"
            "            vec3 toLight = vec3(-lightDir);\n"
            "            float baseDot = max(cos(acos(dot(toLight, fragNormal)) * 1.1), 0.0); \n"  
            "            float bias = (1.0 - baseDot) * lightAngularShadowBias;"
            "            return getDirLightColor(baseColor, bias, baseDot, fragPos); \n"
            "        }\n"
            "        else if (lightType == 2) {\n"
            "            vec3 lightLocalFragPos = vec3(lightMatrix * fragPos);\n"
            "            vec3 toLight = normalize(vec3(lightPos - fragPos));\n"
            "            float baseDot = max(cos(acos(dot(toLight, fragNormal)) * 1.025), 0.0); \n"
            "            float bias = (1.0 - baseDot) * lightAngularShadowBias + lightConstantShadowBias;\n"
            "            vec4 color = vec4(0.0, 0.0, 0.0, 0.0); \n"
            "            vec3 lfp = lightLocalFragPos; \n"
            "            color += getPointLightColor(baseColor, lfp, bias, baseDot); \n"
            "            return color; \n"
            "        }\n"
            "    }\n"
            "    return baseColor;\n"
            "}\n";

        this->Depth_vertex =
            "#version 330\n"
            "precision highp float;\n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "}\n";

        this->Depth_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "void main() {\n"
            "    gl_FragColor = vec4(gl_FragCoord.z, 0.0, 0.0, 0.0);\n"
            "}\n";

        this->Distance_vertex =
            "#version 330\n"
            + POSITION_DEF 
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 vPos;\n"
            "void main() {\n"
            "    vPos = " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * vPos;\n"
            "}\n";

        this->Distance_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "in vec4 vPos;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    float len = length(vPos.xyz);\n"
            "    gl_FragColor = vec4(len, 0.0, 0.0, 0.0);\n"
            "}\n";

        this->Basic_vertex =
            "#version 100\n"
            + POSITION_DEF +
            "attribute vec4 color;\n"
            "attribute vec2 uv;\n"
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "varying vec4 vColor;\n"
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + "  * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vColor = color;\n"
            "}\n";

        this->Basic_fragment =   
            "#version 100\n"
            "precision mediump float;\n"
            "varying vec4 vColor;\n"
            "void main() {\n"
            "    gl_FragColor = vColor;\n"
            "}\n";

        this->BasicLit_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Lighting\" \n"
            + POSITION_DEF +
            "in vec4 color;\n"
            "in vec4 normal;\n"
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "uniform mat4 modelInverseTransposeMatrix;\n"
            "out vec4 vColor;\n"
            "out vec3 vNormal;\n"
            "out vec4 vPos;\n"
            "void main() {\n"
            "    vPos = " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vec4 viewSpacePos = " + VIEW_MATRIX + " * vPos;\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * vPos;\n"
            "    vColor = color;\n"
            "    vNormal = vec3(modelInverseTransposeMatrix * normal);\n"
            "    TRANSFER_LIGHTING(" + POSITION + ", gl_Position, viewSpacePos) \n"
            "}\n";

        this->BasicLit_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Lighting\"\n"
            "in vec4 vColor;\n"
            "in vec3 vNormal;\n"
            "in vec4 vPos;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "   out_color = litColor(vec4(vColor.r, vColor.g, vColor.b, 1.0), vPos, normalize(vNormal));\n"
            //"   out_color = litColor(vColor, vPos, normalize(vNormal));\n"
           // "   out_color = litColor(vec4(1.0, 0.0, 0.0, 1.0), vPos, normalize(vNormal));\n"
            "}\n";

        this->BasicTextured_vertex =  
            "#version 100\n"
            + POSITION_DEF +
            "attribute vec4 color;\n"
            "attribute vec2 uv;\n"
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "varying vec4 vColor;\n"
            "varying vec3 vNormal;\n"
            "varying vec2 vUV;\n"
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vUV = uv;\n"
            "    vColor = color;\n"
            "}\n";

        this->BasicTextured_fragment =   
            "#version 100\n"
            "precision mediump float;\n"
            "uniform sampler2D textureA;\n"
            "varying vec4 vColor;\n"
            "varying vec2 vUV;\n"
            "void main() {\n"
            "    vec4 textureColor = texture2D(textureA, vUV);\n"
            "    gl_FragColor = textureColor;\n"
            "}\n";

        this->BasicTexturedLit_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Lighting\" \n"
            + POSITION_DEF +
            "in vec4 color;\n"
            "in vec4 normal;\n"
            "in vec4 faceNormal;\n"
            "in vec2 uv;\n"
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "uniform mat4 modelInverseTransposeMatrix;\n"
            "uniform vec4 lightPos;\n"
            "out vec4 vColor;\n"
            "out vec3 vNormal;\n"
            "out vec3 vFaceNormal;\n"
            "out vec2 vUV;\n"
            "out vec4 vPos;\n"
            "void main() {\n"
            "    vPos = " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vec4 viewSpacePos = " + VIEW_MATRIX + " * vPos;\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * vPos;\n"
            "    vUV = uv;\n"
            "    vColor = color;\n"
            "    vec4 eNormal = normal;\n"
            "    vNormal = vec3(modelInverseTransposeMatrix * eNormal);\n"
            "    vFaceNormal = vec3(modelInverseTransposeMatrix * faceNormal);\n"
            "    TRANSFER_LIGHTING(" + POSITION + ", gl_Position, viewSpacePos) \n"
            "}\n";

        this->BasicTexturedLit_fragment =   
            "#version 330\n"
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

        this->BasicCube_vertex =  
            "#version 330\n"
            + POSITION_DEF +
            "attribute vec4 color;\n"
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF +
            "mat4 scale = mat4(1.0, 0.0, 0.0, 0.0,\n"
            "                  0.0, 1.0, 0.0, 0.0,\n"
            "                  0.0, 0.0, 1.0, -20.0,\n"
            "                  0.0, 0.0, 0.0, 1.0);\n"
            "varying vec4 vColor;\n"
            "varying vec3 vUV;\n"
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " + POSITION + ";\n"
            "    vUV = normalize(" + POSITION + ".xyz);\n"
            "    vColor = color;\n"
            "}\n";

        this->BasicCube_fragment =  
            "#version 330\n"
            "precision mediump float;\n"
            "uniform samplerCube skybox;\n"
            "varying vec4 vColor;\n"
            "varying vec3 vUV;\n"
            "void main() {\n"
            "    vec4 textureColor = texture(skybox, vUV);\n"
            "    gl_FragColor = textureColor;\n"
            "}\n";
    }

}