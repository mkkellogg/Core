#include <string>

#include "ShaderManagerGL.h"
#include "../common/Constants.h"

static auto _un = Core::StandardUniforms::getUniformName;
static auto _an = Core::StandardAttributes::getAttributeName;

const std::string POSITION = _an(Core::StandardAttribute::Position);
const std::string NORMAL = _an(Core::StandardAttribute::Normal);
const std::string AVERAGED_NORMAL = _an(Core::StandardAttribute::AveragedNormal);
const std::string FACE_NORMAL = _an(Core::StandardAttribute::FaceNormal);
const std::string TANGENT = _an(Core::StandardAttribute::Tangent);
const std::string COLOR = _an(Core::StandardAttribute::Color);
const std::string ALBEDO_UV = _an(Core::StandardAttribute::AlbedoUV);
const std::string NORMAL_UV = _an(Core::StandardAttribute::NormalUV);
const std::string MODEL_MATRIX = _un(Core::StandardUniform::ModelMatrix);
const std::string MODEL_INVERSE_TRANSPOSE_MATRIX = _un(Core::StandardUniform::ModelInverseTransposeMatrix);
const std::string VIEW_MATRIX = _un(Core::StandardUniform::ViewMatrix);
const std::string PROJECTION_MATRIX = _un(Core::StandardUniform::ProjectionMatrix);
const std::string CAMERA_POSITION = _un(Core::StandardUniform::CameraPosition);

const std::string MAX_CASCADES = std::to_string(Core::Constants::MaxDirectionalCascades);
const std::string MAX_LIGHTS = std::to_string(Core::Constants::MaxShaderLights);
const std::string MAX_POINT_LIGHTS = std::to_string(Core::Constants::MaxShaderPointLights);
const std::string MAX_DIRECTIONAL_LIGHTS = std::to_string(Core::Constants::MaxShaderDirectionalLights);
const std::string MAX_CASCADES_LIGHTS = std::to_string(Core::Constants::MaxShaderLights * Core::Constants::MaxDirectionalCascades);
const std::string LIGHT_COUNT = _un(Core::StandardUniform::LightCount);
const std::string LIGHT_POSITION = _un(Core::StandardUniform::LightPosition);
const std::string LIGHT_DIRECTION = _un(Core::StandardUniform::LightDirection);
const std::string LIGHT_COLOR = _un(Core::StandardUniform::LightColor);
const std::string LIGHT_INTENSITY = _un(Core::StandardUniform::LightIntensity);
const std::string LIGHT_ATTENUATION = _un(Core::StandardUniform::LightAttenuation);
const std::string LIGHT_TYPE = _un(Core::StandardUniform::LightType);
const std::string LIGHT_RANGE = _un(Core::StandardUniform::LightRange);
const std::string LIGHT_ENABLED = _un(Core::StandardUniform::LightEnabled);
const std::string LIGHT_MATRIX = _un(Core::StandardUniform::LightMatrix);
const std::string LIGHT_VIEW_PROJECTION = _un(Core::StandardUniform::LightViewProjection);
const std::string LIGHT_CASCADE_COUNT = _un(Core::StandardUniform::LightCascadeCount);
const std::string LIGHT_SHADOW_MAP = _un(Core::StandardUniform::LightShadowMap);
const std::string LIGHT_CASCADE_END = _un(Core::StandardUniform::LightCascadeEnd);
const std::string LIGHT_SHADOW_MAP_ASPECT = _un(Core::StandardUniform::LightShadowMapAspect);
const std::string LIGHT_SHADOW_CUBE_MAP = _un(Core::StandardUniform::LightShadowCubeMap);
const std::string LIGHT_CONSTANT_SHADOW_BIAS = _un(Core::StandardUniform::LightConstantShadowBias);
const std::string LIGHT_ANGULAR_SHADOW_BIAS = _un(Core::StandardUniform::LightAngularShadowBias);
const std::string LIGHT_SHADOW_MAP_SIZE = _un(Core::StandardUniform::LightShadowMapSize);
const std::string LIGHT_SHADOW_SOFTNESS = _un(Core::StandardUniform::LightShadowSoftness);
const std::string LIGHT_NEAR_PLANE = _un(Core::StandardUniform::LightNearPlane);
const std::string POINT_LIGHT_COUNT = _un(Core::StandardUniform::PointLightCount);
const std::string DIRECTIONAL_LIGHT_COUNT = _un(Core::StandardUniform::DirectionalLightCount);

const std::string POSITION_DEF = "in vec4 " +  POSITION + ";\n";
const std::string NORMAL_DEF = "in vec4 " +  NORMAL + ";\n";
const std::string AVERAGED_NORMAL_DEF = "in vec4 " +  AVERAGED_NORMAL + ";\n";
const std::string FACE_NORMAL_DEF = "in vec4 " +  FACE_NORMAL + ";\n";
const std::string TANGENT_DEF = "in vec4 " +  TANGENT + ";\n";
const std::string COLOR_DEF = "in vec4 " +  COLOR + ";\n";
const std::string ALBEDO_UV_DEF = "in vec2 " +  ALBEDO_UV + ";\n";
const std::string NORMAL_UV_DEF = "in vec2 " +  NORMAL_UV + ";\n";
const std::string MODEL_MATRIX_DEF = "uniform mat4 " +  MODEL_MATRIX + ";\n";
const std::string MODEL_INVERSE_TRANSPOSE_MATRIX_DEF = "uniform mat4 " +  MODEL_INVERSE_TRANSPOSE_MATRIX + ";\n";
const std::string VIEW_MATRIX_DEF = "uniform mat4 " +  VIEW_MATRIX + ";\n";
const std::string PROJECTION_MATRIX_DEF = "uniform mat4 " +  PROJECTION_MATRIX + ";\n";
const std::string CAMERA_POSITION_DEF = "uniform vec4 " +  CAMERA_POSITION + ";\n";

// ------------------------------------
// Single-pass lighting definitions
// ------------------------------------

// Common single-pass light parameters
const std::string LIGHT_COUNT_SINGLE_DEF = "const int " + LIGHT_COUNT + " = 1;\n";
const std::string LIGHT_COLOR_SINGLE_DEF = "uniform vec4 " + LIGHT_COLOR + "[1];\n";
const std::string LIGHT_INTENSITY_SINGLE_DEF = "uniform float " + LIGHT_INTENSITY + "[1];\n";
const std::string LIGHT_TYPE_SINGLE_DEF = "uniform int " + LIGHT_TYPE + "[1];\n";
const std::string LIGHT_ENABLED_SINGLE_DEF = "uniform int " + LIGHT_ENABLED + "[1];\n";
const std::string LIGHT_MATRIX_SINGLE_DEF = "uniform mat4 " + LIGHT_MATRIX + "[1];\n";
const std::string LIGHT_CONSTANT_SHADOW_BIAS_SINGLE_DEF = "uniform float " + LIGHT_CONSTANT_SHADOW_BIAS + "[1];\n";
const std::string LIGHT_ANGULAR_SHADOW_BIAS_SINGLE_DEF ="uniform float " + LIGHT_ANGULAR_SHADOW_BIAS + "[1];\n";
const std::string LIGHT_SHADOW_MAP_SIZE_SINGLE_DEF = "uniform float " + LIGHT_SHADOW_MAP_SIZE + "[1];\n";
const std::string LIGHT_SHADOW_SOFTNESS_SINGLE_DEF = "uniform int " + LIGHT_SHADOW_SOFTNESS + "[1];\n";
// Single-pass point light parameters
const std::string LIGHT_POSITION_SINGLE_DEF = "uniform vec4 " + LIGHT_POSITION + "[1];\n";
const std::string LIGHT_ATTENUATION_SINGLE_DEF = "uniform float " + LIGHT_ATTENUATION + "[1];\n";
const std::string LIGHT_RANGE_SINGLE_DEF = "uniform float " + LIGHT_RANGE + "[1];\n";
const std::string LIGHT_NEAR_PLANE_SINGLE_DEF = "uniform float " + LIGHT_NEAR_PLANE + "[1];\n";
const std::string LIGHT_SHADOW_CUBE_MAP_SINGLE_DEF = "uniform samplerCube " + LIGHT_SHADOW_CUBE_MAP + "[1];\n";
// Single-pass directional light paramters
const std::string MAX_CASCADES_SINGLE_DEF = "const int MAX_CASCADES =" + MAX_CASCADES + ";\n";
const std::string LIGHT_DIRECTION_SINGLE_DEF = "uniform vec4 " + LIGHT_DIRECTION + "[1];\n";
const std::string LIGHT_CASCADE_COUNT_SINGLE_DEF = "uniform int " + LIGHT_CASCADE_COUNT + "[" + MAX_CASCADES + "];\n";
const std::string LIGHT_SHADOW_MAP_SINGLE_DEF = "uniform sampler2DShadow " + LIGHT_SHADOW_MAP + "[" + MAX_CASCADES + "];\n";
const std::string LIGHT_CASCADE_END_SINGLE_DEF = "uniform float " + LIGHT_CASCADE_END + "[" + MAX_CASCADES + "];\n";
const std::string LIGHT_SHADOW_MAP_ASPECT_SINGLE_DEF = "uniform float " + LIGHT_SHADOW_MAP_ASPECT + "[" + MAX_CASCADES + "];\n";
const std::string LIGHT_VIEW_PROJECTION_SINGLE_DEF = "uniform mat4 " + LIGHT_VIEW_PROJECTION + "[" + MAX_CASCADES + "];\n";

// ------------------------------------
// Multi-pass lighting definitions
// ------------------------------------
const std::string MAX_LIGHTS_DEF = "const int MAX_LIGHTS = " + MAX_LIGHTS + ";\n";
const std::string MAX_POINT_LIGHTS_DEF = "const int MAX_POINT_LIGHTS = " + MAX_POINT_LIGHTS + ";\n";
const std::string MAX_DIRECTIONAL_LIGHTS_DEF = "const int MAX_DIRECTIONAL_LIGHTS = " + MAX_DIRECTIONAL_LIGHTS + ";\n";
const std::string POINT_LIGHT_COUNT_DEF = "uniform int " + POINT_LIGHT_COUNT + ";\n";
const std::string DIRECTIONAL_LIGHT_DEF = "unfirm int " + DIRECTIONAL_LIGHT_COUNT + ";\n";
const std::string LIGHT_COUNT_DEF = "uniform int " + LIGHT_COUNT + ";\n";
// Common multi-pass light parameters
const std::string LIGHT_COLOR_DEF = "uniform vec4 " + LIGHT_COLOR + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_INTENSITY_DEF = "uniform float " + LIGHT_INTENSITY + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_TYPE_DEF = "uniform int " + LIGHT_TYPE + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_ENABLED_DEF = "uniform int " + LIGHT_ENABLED + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_MATRIX_DEF = "uniform mat4 " + LIGHT_MATRIX + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_SHADOW_MAP_SIZE_DEF = "uniform float " + LIGHT_SHADOW_MAP_SIZE + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_SHADOW_SOFTNESS_DEF = "uniform int " + LIGHT_SHADOW_SOFTNESS + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_CONSTANT_SHADOW_BIAS_DEF = "uniform float " + LIGHT_CONSTANT_SHADOW_BIAS + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_ANGULAR_SHADOW_BIAS_DEF ="uniform float " + LIGHT_ANGULAR_SHADOW_BIAS + "[" + MAX_LIGHTS + "];\n";
// Multi-pass point light parameters
const std::string LIGHT_POSITION_DEF = "uniform vec4 " + LIGHT_POSITION + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_ATTENUATION_DEF = "uniform float " + LIGHT_ATTENUATION + "[" + MAX_LIGHTS + "];\n";;
const std::string LIGHT_RANGE_DEF = "uniform float " + LIGHT_RANGE + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_SHADOW_CUBE_MAP_DEF = "uniform samplerCube " + LIGHT_SHADOW_CUBE_MAP + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_NEAR_PLANE_DEF = "uniform float " + LIGHT_NEAR_PLANE + "[" + MAX_LIGHTS + "];\n";
// Multi-pass directional light parameters
const std::string LIGHT_DIRECTION_DEF = "uniform vec4 " + LIGHT_DIRECTION + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_VIEW_PROJECTION_DEF = "uniform mat4 " + LIGHT_VIEW_PROJECTION + "[" + MAX_CASCADES_LIGHTS + "];\n";
const std::string LIGHT_CASCADE_COUNT_DEF = "uniform int " + LIGHT_CASCADE_COUNT + "[" + MAX_CASCADES_LIGHTS + "];\n";
const std::string LIGHT_SHADOW_MAP_DEF = "uniform sampler2DShadow " + LIGHT_SHADOW_MAP + "[" + MAX_CASCADES_LIGHTS + "];\n";
const std::string LIGHT_CASCADE_END_DEF = "uniform float " + LIGHT_CASCADE_END + "[" + MAX_CASCADES_LIGHTS + "];\n";
const std::string LIGHT_SHADOW_MAP_ASPECT_DEF = "uniform float " + LIGHT_SHADOW_MAP_ASPECT + "[" + MAX_CASCADES_LIGHTS + "];\n";
const std::string MAX_CASCADES_DEF = "const int MAX_CASCADES =" + MAX_CASCADES + ";\n";

namespace Core {

    const std::string ShaderManagerGL::BaseString("");

    ShaderManagerGL::~ShaderManagerGL() {
        
    }

    void ShaderManagerGL::init() {

        this->setShader(ShaderType::Vertex, "Skybox", ShaderManagerGL::Skybox_vertex);
        this->setShader(ShaderType::Fragment, "Skybox", ShaderManagerGL::Skybox_fragment);

        this->setShader(ShaderType::Vertex, "Equirectangular", ShaderManagerGL::Equirectangular_vertex);
        this->setShader(ShaderType::Fragment, "Equirectangular", ShaderManagerGL::Equirectangular_fragment);

        this->setShader(ShaderType::Vertex, "Outline", ShaderManagerGL::Outline_vertex);
        this->setShader(ShaderType::Geometry, "Outline", ShaderManagerGL::Outline_geometry);
        this->setShader(ShaderType::Fragment, "Outline", ShaderManagerGL::Outline_fragment);

        this->setShader(ShaderType::Vertex, "LightingHeaderSingle", ShaderManagerGL::Lighting_Header_Single_vertex);
        this->setShader(ShaderType::Fragment, "LightingHeaderSingle", ShaderManagerGL::Lighting_Header_Single_fragment);

        this->setShader(ShaderType::Vertex, "LightingHeaderMulti", ShaderManagerGL::Lighting_Header_Multi_vertex);
        this->setShader(ShaderType::Fragment, "LightingHeaderMulti", ShaderManagerGL::Lighting_Header_Multi_fragment);

        this->setShader(ShaderType::Vertex, "LightingSingle", ShaderManagerGL::Lighting_Single_vertex);
        this->setShader(ShaderType::Fragment, "LightingSingle", ShaderManagerGL::Lighting_Single_fragment);

        this->setShader(ShaderType::Vertex, "LightingMulti", ShaderManagerGL::Lighting_Multi_vertex);
        this->setShader(ShaderType::Fragment, "LightingMulti", ShaderManagerGL::Lighting_Multi_fragment);

        this->setShader(ShaderType::Vertex, "Lighting", ShaderManagerGL::Lighting_vertex);
        this->setShader(ShaderType::Fragment, "Lighting", ShaderManagerGL::Lighting_fragment);
        
        this->setShader(ShaderType::Vertex, "PhysicalCommon", ShaderManagerGL::PhysicalCommon_vertex);
        this->setShader(ShaderType::Fragment, "PhysicalCommon", ShaderManagerGL::PhysicalCommon_fragment);

        this->setShader(ShaderType::Vertex, "PhysicalLightingSingle", ShaderManagerGL::Physical_Lighting_Single_vertex);
        this->setShader(ShaderType::Fragment, "PhysicalLightingSingle", ShaderManagerGL::Physical_Lighting_Single_fragment);

        this->setShader(ShaderType::Vertex, "PhysicalLightingMulti", ShaderManagerGL::Physical_Lighting_Multi_vertex);
        this->setShader(ShaderType::Fragment, "PhysicalLightingMulti", ShaderManagerGL::Physical_Lighting_Multi_fragment);

        this->setShader(ShaderType::Vertex, "PhysicalLighting", ShaderManagerGL::Physical_Lighting_vertex);
        this->setShader(ShaderType::Fragment, "PhysicalLighting", ShaderManagerGL::Physical_Lighting_fragment);

        this->setShader(ShaderType::Vertex, "StandardPhysical", ShaderManagerGL::StandardPhysical_vertex);
        this->setShader(ShaderType::Fragment, "StandardPhysical", ShaderManagerGL::StandardPhysical_fragment);

        this->setShader(ShaderType::Vertex, "PhysicalSkybox", ShaderManagerGL::PhysicalSkybox_vertex);
        this->setShader(ShaderType::Fragment, "PhysicalSkybox", ShaderManagerGL::PhysicalSkybox_fragment);

        this->setShader(ShaderType::Vertex, "IrridianceRenderer", ShaderManagerGL::IrridianceRenderer_vertex);
        this->setShader(ShaderType::Fragment, "IrridianceRenderer", ShaderManagerGL::IrridianceRenderer_fragment);

        this->setShader(ShaderType::Vertex, "Depth", ShaderManagerGL::Depth_vertex);
        this->setShader(ShaderType::Fragment, "Depth", ShaderManagerGL::Depth_fragment);

        this->setShader(ShaderType::Vertex, "Distance", ShaderManagerGL::Distance_vertex);
        this->setShader(ShaderType::Fragment, "Distance", ShaderManagerGL::Distance_fragment);

        this->setShader(ShaderType::Vertex, "Basic", ShaderManagerGL::Basic_vertex);
        this->setShader(ShaderType::Fragment, "Basic", ShaderManagerGL::Basic_fragment);

        this->setShader(ShaderType::Vertex, "BasicExtrusion", ShaderManagerGL::BasicExtrusion_vertex);
        this->setShader(ShaderType::Fragment, "BasicExtrusion", ShaderManagerGL::BasicExtrusion_fragment);

        this->setShader(ShaderType::Vertex, "BasicColored", ShaderManagerGL::BasicColored_vertex);
        this->setShader(ShaderType::Fragment, "BasicColored", ShaderManagerGL::BasicColored_fragment);

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

        this->Equirectangular_vertex =
            "#version 330 core\n "
            "layout (location = 0) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec3 localPos;\n "
            "void main()\n "
            "{\n "
            "    localPos = " + POSITION + ".xyz;  \n "
            "    gl_Position =  " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " + MODEL_MATRIX + " * " + POSITION + ";\n"
            "}\n ";

        this->Equirectangular_fragment =   
            "#version 330 core \n"
            "out vec4 FragColor; \n"
            "in vec3 localPos; \n"
            "uniform sampler2D equirectangularTexture; \n"
            "const vec2 invAtan = vec2(0.1591, 0.3183); \n"
            "vec2 SampleSphericalMap(vec3 v) \n"
            "{\n"
            "    vec2 uv = vec2(atan(v.z, v.x), asin(v.y)); \n"
            "    uv *= invAtan; \n"
            "    uv += 0.5; \n"
            "    return uv; \n"
            "}\n"
            "void main()\n"
            "{	\n"
            "    vec2 uv = SampleSphericalMap(normalize(localPos)); \n"
            "    vec3 color = texture(equirectangularTexture, uv).rgb; \n"
            "    FragColor = vec4(color, 1.0);\n"
            "}\n";

        this->Skybox_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 TexCoord0;\n"
            "void main()\n"
            "{\n"
            "    TexCoord0 = " + POSITION + ";\n"
            "    vec4 vWorldPos = " + VIEW_MATRIX + " * " + MODEL_MATRIX + " * " + POSITION + ";\n"
            "    gl_Position = (" + PROJECTION_MATRIX + " * vWorldPos).xyww;\n"
            "}\n";

        this->Skybox_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "uniform samplerCube cubeTexture; \n"
            "in vec4 TexCoord0;\n"
            "out vec4 out_color;\n"
            "void main()\n"
            "{\n"
            "    vec3 color =  texture(cubeTexture, TexCoord0.xyz).rgb; \n "
            "    out_color = vec4(color, 1.0);\n"
            "}\n";

        this->Outline_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF + 
            "layout (location = 1 ) " + NORMAL_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF +
            "uniform vec4 color;"
            "out vec4 vColor;\n"
            "out vec3 VNormal;\n"
            "out vec3 VPosition;\n"
            "void main()\n"
            "{\n"
            "    VNormal = normalize(vec3(inverse(transpose(" + VIEW_MATRIX + " * " + MODEL_MATRIX + ")) * " + NORMAL + "));\n"
            "    VPosition = vec3(" + VIEW_MATRIX + " * " + MODEL_MATRIX + " * " + POSITION + ");\n"
            "    vec4 outPos = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    gl_Position = outPos;\n"
            "    vColor = color;\n"
            "}\n";

        this->Outline_geometry =
            "#version 330\n"
            "precision highp float;\n"
            "layout( triangles ) in;\n"
            + PROJECTION_MATRIX_DEF +
            "layout( triangle_strip, max_vertices = 18) out;\n"
            "uniform float edgeWidth = .005; // Width of sil. edge in clip cds.\n"
            "uniform float pctExtend = 0.0; // Percentage to extend quad\n"
            "uniform float absExtend = 0.0025; // Percentage to extend quad\n"
            "in vec3 VNormal[]; // Normal in camera coords.\n"
            "in vec3 VPosition[]; // Position in camera coords.\n"
            "out vec3 GNormal;\n"
            "out vec3 GPosition;\n"
            "flat out int GIsEdge;\n"

            "bool isFrontFacing( vec3 a, vec3 b, vec3 c )\n"
            "{\n"
            "   return ((a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y)) > 0;\n"
            "}\n"

            "void emitEdgeQuad( vec3 e0, vec3 e1, float outlineDir, float w0, float w1)\n"
            "{\n"
            "   vec2 v = normalize(e1.xy - e0.xy);\n"
            "   vec2 extPct = pctExtend * (e1.xy - e0.xy);\n"
            "   vec2 extAbs = absExtend * v;\n"
            "   vec2 ext = extPct + extAbs;\n"
            "   vec2 n = -vec2(-v.y, v.x) * edgeWidth;\n"

            "   // Emit the quad\n"
            "   GIsEdge = 1; // This is part of the sil. edge\n"
            "   float epsilon = 0.0001;\n"
            "   gl_Position = vec4( e0.xy - ext, e0.z + epsilon, 1.0 ) * w0;\n"
            "   EmitVertex();\n"
            "   gl_Position = vec4( e0.xy + n * outlineDir - ext, e0.z + epsilon, 1.0 ) * w0;\n"
            "   EmitVertex();\n"
            "   gl_Position = vec4( e1.xy + ext, e1.z + epsilon, 1.0 ) * w1;\n"
            "   EmitVertex();\n"
            "   gl_Position = vec4( e1.xy + n * outlineDir + ext, e1.z + epsilon, 1.0 ) * w1;\n"
            "   EmitVertex();\n"
            "   EndPrimitive();\n"
            "}\n"

            "void main()\n"
            "{\n"
            "   float w0 = gl_in[0].gl_Position.w;\n"
            "   float w1 = gl_in[1].gl_Position.w;\n"
            "   float w2 = gl_in[2].gl_Position.w;\n"
            "   vec3 p0 = gl_in[0].gl_Position.xyz / w0;\n"
            "   vec3 p1 = gl_in[1].gl_Position.xyz / w1;\n"
            "   vec3 p2 = gl_in[2].gl_Position.xyz / w2;\n"
            "   bool isBack = !isFrontFacing(p0, p1, p2);\n"
            "   mat4 proj = " + PROJECTION_MATRIX + ";\n"
            "   float upperBound = -proj[2][2];\n"
            "   bool isBehind = p0.z > upperBound || p1.z > upperBound || p2.z > upperBound;\n"
            "   bool flip = isBack ^^ isBehind;\n"
            "   float outlineDir = flip ? -1.0 : 1.0;"
            
            "   emitEdgeQuad(p0, p1, outlineDir, w0, w1);\n"
            "   emitEdgeQuad(p1, p2, outlineDir, w1, w2);\n"
            "   emitEdgeQuad(p2, p0, outlineDir, w2, w0);\n"
            
            "   // Output the original triangle\n"
            "   GIsEdge = 0; // This triangle is not part of an edge.\n"
            "   GNormal = VNormal[0];\n"
            "   GPosition = VPosition[0];\n"
            "   gl_Position = gl_in[0].gl_Position;\n"
            "   EmitVertex();\n"
            "   GNormal = VNormal[1];\n"
            "   GPosition = VPosition[1];\n"
            "   gl_Position = gl_in[1].gl_Position;\n"
            "   EmitVertex();\n"
            "   GNormal = VNormal[2];\n"
            "   GPosition = VPosition[2];\n"
            "   gl_Position = gl_in[2].gl_Position;\n"
            "   EmitVertex();\n"
            "   EndPrimitive();\n"
            "}\n";

        this->Outline_fragment = 
            "#version 330\n"
            "precision highp float;\n"
            "in vec4 vColor;\n"
            "uniform vec4 color;"
            "flat in int GIsEdge; // Whether or not we're drawing an edge \n"
            "layout( location = 0 ) out vec4 out_color;\n"
            "void main() {\n"
            "   out_color = color;\n"
            "}\n";

        this->Lighting_Header_Multi_vertex =
            MAX_CASCADES_DEF
            + MAX_LIGHTS_DEF
            + LIGHT_COUNT_DEF
            + LIGHT_CASCADE_COUNT_DEF
            + LIGHT_VIEW_PROJECTION_DEF +
            "out vec4 _core_lightSpacePos[" + MAX_CASCADES_LIGHTS + "];\n"
            "out float _core_viewSpacePosZ[" + MAX_LIGHTS + "];\n";

        this->Lighting_Header_Multi_fragment =
            MAX_CASCADES_DEF
            + MAX_LIGHTS_DEF
            + LIGHT_COUNT_DEF
            + LIGHT_CASCADE_COUNT_DEF
            + LIGHT_SHADOW_MAP_DEF
            + LIGHT_CASCADE_END_DEF
            + LIGHT_SHADOW_MAP_ASPECT_DEF
            + LIGHT_SHADOW_CUBE_MAP_DEF
            + LIGHT_CONSTANT_SHADOW_BIAS_DEF
            + LIGHT_ANGULAR_SHADOW_BIAS_DEF
            + LIGHT_MATRIX_DEF
            + LIGHT_POSITION_DEF
            + LIGHT_DIRECTION_DEF
            + LIGHT_RANGE_DEF
            + LIGHT_TYPE_DEF
            + LIGHT_ENABLED_DEF
            + LIGHT_SHADOW_SOFTNESS_DEF
            + LIGHT_COLOR_DEF
            + LIGHT_SHADOW_MAP_SIZE_DEF
            + LIGHT_NEAR_PLANE_DEF +
            "in float _core_viewSpacePosZ[" + MAX_LIGHTS + "];\n"
            "in vec4 _core_lightSpacePos[" + MAX_CASCADES_LIGHTS + "];\n";

        this->Lighting_Header_Single_vertex =
            LIGHT_COUNT_SINGLE_DEF
            + MAX_CASCADES_SINGLE_DEF
            + LIGHT_CASCADE_COUNT_SINGLE_DEF
            + LIGHT_VIEW_PROJECTION_SINGLE_DEF
            + LIGHT_CASCADE_END_SINGLE_DEF +
            "out vec4 _core_lightSpacePos[" + MAX_CASCADES + "];\n"
            "out float _core_viewSpacePosZ[1];\n";

        this->Lighting_Header_Single_fragment =
            MAX_CASCADES_SINGLE_DEF
            + LIGHT_CASCADE_COUNT_SINGLE_DEF
            + LIGHT_SHADOW_MAP_SINGLE_DEF
            + LIGHT_CASCADE_END_SINGLE_DEF
            + LIGHT_SHADOW_MAP_ASPECT_SINGLE_DEF
            + LIGHT_SHADOW_CUBE_MAP_SINGLE_DEF
            + LIGHT_CONSTANT_SHADOW_BIAS_SINGLE_DEF
            + LIGHT_ANGULAR_SHADOW_BIAS_SINGLE_DEF
            + LIGHT_MATRIX_SINGLE_DEF
            + LIGHT_POSITION_SINGLE_DEF
            + LIGHT_DIRECTION_SINGLE_DEF
            + LIGHT_RANGE_SINGLE_DEF
            + LIGHT_TYPE_SINGLE_DEF
            + LIGHT_ENABLED_SINGLE_DEF
            + LIGHT_SHADOW_SOFTNESS_SINGLE_DEF
            + LIGHT_COLOR_SINGLE_DEF
            + LIGHT_SHADOW_MAP_SIZE_SINGLE_DEF
            + LIGHT_NEAR_PLANE_SINGLE_DEF +
            "in float _core_viewSpacePosZ[1];\n"
            "in vec4 _core_lightSpacePos[" + MAX_CASCADES + "];\n";

        this->Lighting_Single_vertex =
            "#include \"LightingHeaderSingle\" \n"
            "#include \"Lighting\" \n";

        this->Lighting_Single_fragment =
            "#include \"LightingHeaderSingle\" \n"
            "#include \"Lighting\" \n";

        this->Lighting_Multi_vertex =
            "#include \"LightingHeaderMulti\" \n"
            "#include \"Lighting\" \n";

        this->Lighting_Multi_fragment =
            "#include \"LightingHeaderMulti\" \n"
            "#include \"Lighting\" \n";

        this->Lighting_vertex = 
            "#define TRANSFER_LIGHTING(localPos, clipSpacePos, viewSpacePos) "
            "for (int l = 0 ; l < " + MAX_CASCADES + " * " + LIGHT_COUNT + "; l++) { "
            "    _core_lightSpacePos[l] = " + LIGHT_VIEW_PROJECTION + "[l] * " + MODEL_MATRIX + " * (localPos); "
            "}"
            "for (int i = 0 ; i < " + LIGHT_COUNT + "; i++) { "
            "_core_viewSpacePosZ[i] = (viewSpacePos).z;"
            "}";

        this->Lighting_fragment =
            "const float PI = 3.14159265359;\n"
            "const int AMBIENT_LIGHT = 0;\n"
            "const int AMBIENT_IBL_LIGHT = 1;\n"
            "const int DIRECTIONAL_LIGHT = 2;\n"
            "const int POINT_LIGHT = 3;\n"
            "const int SPOT_LIGHT = 4;\n"
            "const int PLANAR_LIGHT = 5;\n"
            "float calDirShadowFactorSingleIndex(int index, vec2 uv, float fragDepth, float angularBias, int lightIndex) { \n"
            "    vec3 coords = vec3(uv.xy, fragDepth - angularBias - " + LIGHT_CONSTANT_SHADOW_BIAS + "[lightIndex]); \n"
            "    float shadowDepth = clamp(texture(" + LIGHT_SHADOW_MAP + "[index], coords), 0.0, 1.0); \n"
            "    return (1.0-shadowDepth); \n"
            "} \n"

            "float calcDirShadowFactor(int cascadeIndex, vec4 lightSpacePos, float angularBias, vec4 fragPos, int lightIndex)\n"
            "{ \n"
            "    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w; \n"
            "    vec3 uvCoords = (projCoords * 0.5) + vec3(0.5, 0.5, 0.5); \n"
            "    float px = 1.0 / " + LIGHT_SHADOW_MAP_SIZE + "[lightIndex]; \n"
            "    float py =  " + LIGHT_SHADOW_MAP_ASPECT + "[cascadeIndex] / " + LIGHT_SHADOW_MAP_SIZE + "[lightIndex]; \n"

            "    float shadowFactor = 0.0; \n"
            "    vec2 uv = uvCoords.xy; \n"
            "    float z = uvCoords.z; \n"

            "    if (uvCoords.x < 0 || uvCoords.x > 1.0) return 0.0; \n"
            "    if (uvCoords.y < 0 || uvCoords.y > 1.0) return 0.0; \n"

            "    if (" + LIGHT_SHADOW_SOFTNESS + "[lightIndex] == 2 || " + LIGHT_SHADOW_SOFTNESS + "[lightIndex] == 1) { \n"
            "        for (int y = -" + LIGHT_SHADOW_SOFTNESS + "[lightIndex]; y <= " + LIGHT_SHADOW_SOFTNESS + "[lightIndex] ; y++) { \n"
            "            for (int x = -" + LIGHT_SHADOW_SOFTNESS + "[lightIndex]; x <= " + LIGHT_SHADOW_SOFTNESS + "[lightIndex] ; x++) { \n"
            "                shadowFactor += calDirShadowFactorSingleIndex(cascadeIndex, vec2(uv.x + x * px, uv.y + y * py), z, angularBias, lightIndex); \n"
            "            } \n"
            "        } \n "
            "        if (" + LIGHT_SHADOW_SOFTNESS + "[lightIndex] == 2) shadowFactor /= 25.0; \n"
            "        else shadowFactor /= 9.0; \n"
            "    } \n"
            "    else { \n"
            "        shadowFactor += calDirShadowFactorSingleIndex(cascadeIndex, uv, z, angularBias, lightIndex); \n"
            "    } \n"

            "    return shadowFactor; \n"
            "} \n"

            "vec4 getDirLightColor(vec4 worldPos, float bias, int lightIndex) { \n"
            "    float shadowFactor = 0.0;\n"
            "    for (int i = 0 ; i < " + LIGHT_CASCADE_COUNT + "[lightIndex]; i++) { \n"
            "        int offset = lightIndex * " + MAX_CASCADES + " + i;\n"
            "        if (_core_viewSpacePosZ[lightIndex] <= " + LIGHT_CASCADE_END + "[offset]) { \n"
            "            shadowFactor = calcDirShadowFactor(i, _core_lightSpacePos[offset], bias, worldPos, lightIndex); \n"
            "            break; \n"
            "        } \n"
            "    } \n"
            "    return vec4((1.0 - shadowFactor) * " + LIGHT_COLOR + "[lightIndex].rgb, 1.0);\n"      
            "} \n"

            "float getPointLightShadowFactor(vec3 lightLocalPos, float bias, int lightIndex) { \n"
            "   vec4 shadowDepthVec = texture(" + LIGHT_SHADOW_CUBE_MAP + "[lightIndex], lightLocalPos);\n"
            "   float shadowDepth = shadowDepthVec.r;\n"
            "   float shadowFactor = 0.0; \n"
            "   if (shadowDepth + bias > length(lightLocalPos) || shadowDepth < .001) {\n"
            "       shadowFactor = 1.0;\n"
            "   }\n"
            "   return shadowFactor;\n"
            "} \n"

            "vec4 getPointLightColor(vec3 lightLocalPos, float bias, int lightIndex) { \n"
            "    float pxToWorld = 1.0 / " + LIGHT_SHADOW_MAP_SIZE + "[lightIndex] * 0.2; \n"
            "    float near = " + LIGHT_NEAR_PLANE + "[lightIndex]; \n"

            "    vec3 absVec = abs(lightLocalPos); \n"
            "    float maxComponent = max(max(absVec.x, absVec.y), absVec.z); \n"
            "    vec3 normAbs = absVec / maxComponent; \n"

            "    float aFactor = step(absVec.x, absVec.z) * step(absVec.y, absVec.z); \n"
            "    float bFactor = step(absVec.z, absVec.x) * step(absVec.y, absVec.x); \n"
            "    bFactor = clamp(bFactor - aFactor, 0.0, 1.0); \n"
            "    float cFactor = clamp(1.0 - (aFactor + bFactor), 0.0, 1.0); \n"
            "    vec3 mask = vec3(0.0, 0.0, 1.0) * aFactor +  vec3(1.0, 0.0, 0.0) * bFactor + vec3(0.0, 1.0, 0.0) *  cFactor; \n"

            "    vec3 inverseMask = vec3(1.0, 1.0, 1.0) - mask; \n"
            "    vec3 signVec = sign(lightLocalPos); \n"
            "    vec3 forward = signVec * mask; \n"
            "    float scale = maxComponent / near; \n"
            "    vec3 up = vec3(0.0, 1.0, 0.0) * inverseMask + vec3(0.0, 0.0, forward.y) * inverseMask; \n"
            "    vec3 right = cross(forward, up); \n"
            "    right = right * pxToWorld * scale; \n"
            "    up = up * pxToWorld * scale; \n"
            
            "    float shadowFactor = 0.0; \n"
            "    if (" + LIGHT_SHADOW_SOFTNESS + "[lightIndex] == 2 || " + LIGHT_SHADOW_SOFTNESS + "[lightIndex] == 1) { \n"
            "        shadowFactor += getPointLightShadowFactor(lightLocalPos, bias, lightIndex); \n"
            "        for (int i = 1; i <= " + LIGHT_SHADOW_SOFTNESS + "[lightIndex]; i++) { \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalPos + right * i, bias, lightIndex); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalPos + up * i, bias, lightIndex); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalPos - right * i, bias, lightIndex); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalPos - up * i, bias, lightIndex); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalPos + right * i + up * i, bias, lightIndex); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalPos + right * i - up * i, bias, lightIndex); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalPos - right * i + up * i, bias, lightIndex); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalPos - right * i - up * i, bias, lightIndex); \n"
            "        } \n "
            "        if (" + LIGHT_SHADOW_SOFTNESS + "[lightIndex] == 2) shadowFactor /= 17.0; \n"
            "        else shadowFactor /= 9.0; \n"
            "    } \n"
            "    else { \n"
            "        shadowFactor += getPointLightShadowFactor(lightLocalPos, bias, lightIndex); \n"
            "    } \n"       
           
            "   return vec4(" + LIGHT_COLOR + "[lightIndex].rgb * shadowFactor, 1.0);\n"
            "}\n"

            "void getDirLightParameters(in int lightIndex, in vec3 worldNormal, in vec3 toViewer, out vec3 toLight, out vec3 halfwayVec, out float NdotL, out float bias) { \n"
            "    toLight = vec3(-" + LIGHT_DIRECTION + "[lightIndex]);\n"
            "    NdotL = max(cos(acos(dot(toLight, worldNormal)) * 1.1), 0.0); \n"  
            "    halfwayVec = normalize(toViewer + toLight); \n"
            "    bias = (1.0 - NdotL) * " + LIGHT_ANGULAR_SHADOW_BIAS + "[lightIndex];"
            "} \n"

            "void getPointLightParameters(in int lightIndex, in vec4 worldPos, in vec3 worldNormal, in vec3 toViewer, out vec3 lightLocalPos, out vec3 toLight, out vec3 halfwayVec, out float NdotL, out float bias, out float attenuation) { \n"
            "    lightLocalPos = vec3(" + LIGHT_MATRIX + "[lightIndex] * worldPos);\n"
            "    toLight = vec3(" + LIGHT_POSITION + "[lightIndex] - worldPos);\n"
            "    float distance = length(toLight); \n"
            "    vec3 toLightNormalized = normalize(toLight);\n"
            "    NdotL = max(cos(acos(dot(toLightNormalized, worldNormal)) * 1.025), 0.0); \n"
            "    halfwayVec = normalize(toViewer + toLight); \n"
            "    attenuation = clamp(" + LIGHT_RANGE + "[lightIndex] / (distance * distance), 0.0, 1.0); \n"
            "    bias = (1.0 - NdotL) * " + LIGHT_ANGULAR_SHADOW_BIAS + "[lightIndex] + " + LIGHT_CONSTANT_SHADOW_BIAS + "[lightIndex];\n"
            "} \n"

            "vec4 litColorBlinnPhong(in int lightIndex, in vec4 albedo, in vec4 worldPos, in vec3 worldNormal, in vec4 cameraPos) {\n"
            "    if (" + LIGHT_ENABLED + "[lightIndex] != 0) {\n"
            "        if (" + LIGHT_TYPE + "[lightIndex] == AMBIENT_LIGHT) {\n"
            "            return vec4(albedo.rgb * " + LIGHT_COLOR + "[lightIndex].rgb, albedo.a);\n"
            "        }\n"
            "        else { \n"
            "            vec3 radiance; \n"
            "            vec3 lightLocalPos, toLight, halfwayVec; \n"
            "            float NdotL, bias, attenuation; \n"
            "            vec3 toViewer = vec3(cameraPos - worldPos); \n"
            "            if (" + LIGHT_TYPE + "[lightIndex] == DIRECTIONAL_LIGHT) {\n"
            "                getDirLightParameters(lightIndex, worldNormal, toViewer, toLight, halfwayVec, NdotL, bias); \n"
            "                vec4 lightColor = getDirLightColor(worldPos, bias, lightIndex); \n"
            "                radiance = lightColor.rgb; \n"
            "            }\n"
            "            else if (" + LIGHT_TYPE + "[lightIndex] == POINT_LIGHT) {\n"
            "                getPointLightParameters(lightIndex, worldPos, worldNormal, toViewer, lightLocalPos, toLight, halfwayVec, NdotL, bias, attenuation); \n"
            "                vec4 lightColor = getPointLightColor(lightLocalPos, bias, lightIndex);\n"
            "                radiance = lightColor.rgb * attenuation; \n"
            "            }\n"
            "            return vec4(radiance * albedo.rgb * NdotL, albedo.a);\n"
            "        }\n"
            "    }\n"
            "    return albedo;\n"
            "}\n"

            "vec3 calcMappedNormal(vec3 mappedNormal, vec3 normal, vec3 tangent) \n"
            "{ \n"
            "    normal = normalize(normal); \n "
            "    tangent = normalize(tangent); \n "
            "    tangent = normalize(tangent - dot(tangent, normal) * normal); \n "
            "    vec3 biTangent = cross(tangent, normal); \n "
            "    vec3 bumpMapNormal = mappedNormal; \n "
            "    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0); \n "
            "    vec3 newNormal; \n "
            "    mat3 tbn = mat3(tangent, biTangent, normal); \n "

            "    newNormal = tbn * bumpMapNormal; \n "
            "    newNormal = normalize(newNormal); \n "
            "    return newNormal; \n "
            "} \n ";

        this->PhysicalCommon_vertex =
            "\n";

        this->PhysicalCommon_fragment =
            "vec3 toneMapReinhard(vec3 color) { \n"
            "    return color / (color + vec3(1.0));\n"
            "}\n"
            "vec3 toneMapExposure(vec3 color, float exposure) { \n"
            "    return vec3(1.0) - exp(-color * exposure);\n"
            "}\n"
            "vec3 gammaCorrectBasic(vec3 color) { \n"
            "    return pow(color, vec3(1.0/2.2));\n"
            "}\n";

        this->Physical_Lighting_Single_vertex =
            "#include \"LightingHeaderSingle\" \n"
            "#include \"Lighting\" \n";

        this->Physical_Lighting_Single_fragment =
            "#include \"LightingHeaderSingle\" \n"
            "#include \"Lighting\" \n"
            "#include \"PhysicalLighting\" \n";

        this->Physical_Lighting_Multi_vertex =
            "#include \"LightingHeaderMulti\" \n"
            "#include \"Lighting\" \n";

        this->Physical_Lighting_Multi_fragment =
            "#include \"LightingHeaderMulti\" \n"
            "#include \"Lighting\" \n"
            "#include \"PhysicalLighting\" \n";

        this->Physical_Lighting_vertex =
            "\n";

        this->Physical_Lighting_fragment =
            "#include \"PhysicalCommon\" \n"
            "float distributionGGX(vec3 N, vec3 H, float roughness) { \n"
            "    float a      = roughness*roughness; \n"
            "    float a2     = a*a; \n"
            "    float NdotH  = max(dot(N, H), 0.0); \n"
            "    float NdotH2 = NdotH*NdotH; \n"

            "    float num   = a2; \n"
            "    float denom = (NdotH2 * (a2 - 1.0) + 1.0); \n"
            "    denom = PI * denom * denom; \n"

            "    return num / denom; \n"
            "} \n"

            "float geometrySchlickGGX(float NdotV, float roughness) { \n"
            "    float r = (roughness + 1.0); \n"
            "    float k = (r*r) / 8.0; \n"

            "    float num   = NdotV; \n"
            "    float denom = NdotV * (1.0 - k) + k; \n"

            "    return num / denom; \n"
            "} \n"

            "float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) { \n"
            "    float NdotV = max(dot(N, V), 0.0); \n"
            "    float NdotL = max(dot(N, L), 0.0); \n"
            "    float ggx2  = geometrySchlickGGX(NdotV, roughness); \n"
            "    float ggx1  = geometrySchlickGGX(NdotL, roughness); \n"

            "    return ggx1 * ggx2; \n"
            "} \n"

            "vec3 fresnelSchlick(float cosTheta, vec3 F0) { \n"
            "    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0); \n"
            "} \n"

            "vec4 litColorPhysical(in int lightIndex, in vec4 albedo, in vec4 worldPos, in vec3 worldNormal, in vec4 cameraPos, in float metallic, in float roughness, in float ao) {\n"
            "    if (" + LIGHT_ENABLED + "[lightIndex] != 0) {\n"
            "        if (" + LIGHT_TYPE + "[lightIndex] == AMBIENT_LIGHT) {\n"
            "            return vec4(albedo.rgb * " + LIGHT_COLOR + "[lightIndex].rgb, albedo.a);\n"
            "        }\n"
            "        else { \n"
            "            vec3 lightLocalPos, toLight, halfwayVec, radiance; \n"
            "            float NdotL, bias, attenuation; \n"
            "            vec3 V = normalize(vec3(cameraPos - worldPos)); \n "
            "            vec3 F0 = vec3(0.04); \n "
            "            F0 = mix(F0, albedo.rgb, metallic); \n "   

            "            if (" + LIGHT_TYPE + "[lightIndex] == DIRECTIONAL_LIGHT) {\n"
            "                getDirLightParameters(lightIndex, worldNormal, V, toLight, halfwayVec, NdotL, bias); \n"
            "                vec4 lightColor = getDirLightColor(worldPos, bias, lightIndex); \n"
            "                radiance = lightColor.rgb; \n"
            "            }\n"
            "            else if (" + LIGHT_TYPE + "[lightIndex] == POINT_LIGHT) {\n"
            "                getPointLightParameters(lightIndex, worldPos, worldNormal, V, lightLocalPos, toLight, halfwayVec, NdotL, bias, attenuation); \n"
            "                vec4 lightColor = getPointLightColor(lightLocalPos, bias, lightIndex);\n"
            "                radiance = lightColor.rgb * attenuation; \n"
            "            }\n"

            "            float NDF = distributionGGX(worldNormal, halfwayVec, roughness); \n "
            "            float G   = geometrySmith(worldNormal, V, toLight, roughness); \n "
            "            vec3 F    = fresnelSchlick(max(dot(halfwayVec, V), 0.0), F0);     \n "  
                        
            "            vec3 kS = F; \n "
            "            vec3 kD = vec3(1.0) - kS; \n "
            "            kD *= 1.0 - metallic;	   \n "
                        
            "            vec3 numerator    = NDF * G * F; \n "
            "            float denominator = 4.0 * max(dot(worldNormal, V), 0.0) * max(dot(worldNormal, toLight), 0.0); \n "
            "            vec3 specular     = numerator / max(denominator, 0.001);  \n "
                            
            "            vec3 Lo = (kD * albedo.rgb / PI + specular) * radiance * NdotL; \n "

            "            vec3 ambient = vec3(0.03) * albedo.rgb * ao; \n"
            "            vec3 color = ambient + Lo;  \n"
                        
            "            color = toneMapReinhard(color);  \n"
            "            color = gammaCorrectBasic(color);  \n"

            "            return vec4(color, albedo.a); \n" 
            "        }\n"
            "    }\n"
            "    return albedo;\n"
            "}\n";

        this->StandardPhysical_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"PhysicalLightingSingle\" \n"
            + POSITION_DEF
            + TANGENT_DEF
            + COLOR_DEF
            + NORMAL_DEF
            + FACE_NORMAL_DEF
            + ALBEDO_UV_DEF
            + NORMAL_UV_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF +
            "out vec4 vColor;\n"
            "out vec3 vNormal;\n"
            "out vec3 vTangent;\n"
            "out vec3 vFaceNormal;\n"
            "out vec2 vAlbedoUV;\n"
            "out vec2 vNormalUV;\n"
            "out vec4 vWorldPos;\n"
            "void main() {\n"
            "    vWorldPos = " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vec4 viewSpacePos = " + VIEW_MATRIX + " * vWorldPos;\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * vWorldPos;\n"
            "    vAlbedoUV = " + ALBEDO_UV + ";\n"
            "    vNormalUV = " + NORMAL_UV + ";\n"
            "    vColor = " + COLOR + ";\n"
            "    vec4 eNormal = " + NORMAL + ";\n"
            "    vNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    vec4 eTangent = " + TANGENT + ";\n"
            "    vTangent = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eTangent);\n"
            "    vFaceNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * " + FACE_NORMAL + ");\n"
            "    TRANSFER_LIGHTING(" + POSITION + ", gl_Position, viewSpacePos) \n"
            "}\n";

        this->StandardPhysical_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"PhysicalLightingSingle\"\n"
            + CAMERA_POSITION_DEF +
            "uniform int enabledMap; \n"
            "uniform vec4 albedo; \n"
            "uniform sampler2D albedoMap; \n"
            "uniform sampler2D normalMap; \n"
            "uniform float metallic; \n"
            "uniform float roughness; \n"
            "uniform float ambientOcclusion; \n"
            "in vec4 vColor;\n"
            "in vec3 vNormal;\n"
            "in vec3 vTangent;\n"
            "in vec3 vFaceNormal;\n"
            "in vec2 vAlbedoUV;\n"
            "in vec2 vNormalUV;\n"
            "in vec4 vWorldPos;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "   int albedoMapEnabled = enabledMap & 1; \n"
            "   int normalMapEnabled = enabledMap & 2; \n"
            "   int roughnessMapEnabled = enabledMap & 4; \n"
            "   vec4 _albedo; \n"
            "   if (albedoMapEnabled != 0) { \n"
            "       _albedo = texture(albedoMap, vAlbedoUV); \n"
            "   } else { \n"
            "      _albedo = albedo; \n"
            "   } \n"
            "   vec3 _normal; \n"
            "   if (normalMapEnabled != 0) { \n"
            "      _normal = calcMappedNormal(texture(normalMap, vNormalUV).xyz, vNormal, vTangent); \n"
            "   } else { \n"
            "       _normal = normalize(vNormal); \n"
            "   } \n"
            "   out_color = litColorPhysical(0, _albedo, vWorldPos, _normal, " + CAMERA_POSITION + ", metallic, roughness, ambientOcclusion);\n"
            "}\n";
        
        this->PhysicalSkybox_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 TexCoord0;\n"
            "void main()\n"
            "{\n"
            "    TexCoord0 = " + POSITION + ";\n"
            "    vec4 vWorldPos = " + VIEW_MATRIX + " * " + MODEL_MATRIX + " * " + POSITION + ";\n"
            "    gl_Position = (" + PROJECTION_MATRIX + " * vWorldPos).xyww;\n"
            "}\n";

        this->PhysicalSkybox_fragment =   
            "#version 330\n"
            "#include \"PhysicalCommon\" \n"
            "precision highp float;\n"
            "uniform samplerCube cubeTexture; \n"
            "uniform float exposure; \n"
            "in vec4 TexCoord0;\n"
            "out vec4 out_color;\n"
            "void main()\n"
            "{\n"
            "    vec3 color =  texture(cubeTexture, TexCoord0.xyz).rgb; \n "
            "    color = toneMapExposure(color, exposure); \n"
            "    color = gammaCorrectBasic(color); \n"
            "    out_color = vec4(color, 1.0);\n"
            "}\n";

        this->IrridianceRenderer_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 localPos;\n"
            "void main()\n"
            "{\n"
            "    localPos = " + POSITION + ";\n"
            "    vec4 vWorldPos = " + VIEW_MATRIX + " * " + MODEL_MATRIX + " * " + POSITION + ";\n"
            "    gl_Position = (" + PROJECTION_MATRIX + " * vWorldPos).xyww;\n"
            "}\n";

        this->IrridianceRenderer_fragment =   
            "#version 330\n"
            "out vec4 FragColor;\n"
            "in vec4 localPos;\n"
            "uniform samplerCube cubeTexture;\n"
            "const float PI = 3.14159265359;\n"
            "void main()\n"
            "{\n"
            // the sample direction equals the hemisphere's orientation 
            "    vec3 normal = normalize(localPos.xyz);\n"
            "    vec3 irradiance = vec3(0.0);\n"
            "    vec3 up    = vec3(0.0, 1.0, 0.0);\n"
            "    vec3 right = cross(up, normal);\n"
            "    up         = cross(normal, right);\n"

            "    float sampleDelta = 0.025;\n"
            "    float nrSamples = 0.0;\n" 
            "    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) { \n"
            "        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) { \n"
                         // spherical to cartesian (in tangent space)
            "            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));\n"
                         // tangent space to world
            "            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;\n" 
            "            irradiance += texture(cubeTexture, sampleVec).rgb * cos(theta) * sin(theta);\n"
            "            nrSamples++;\n"
            "        }\n"
            "    }\n"
            "    irradiance = PI * irradiance * (1.0 / float(nrSamples));\n"
            "    FragColor = vec4(irradiance, 1.0);\n"
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
            "out vec4 out_color;\n"
            "void main() {\n"
            "    out_color = vec4(gl_FragCoord.z, 0.0, 0.0, 0.0);\n"
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
            "    out_color = vec4(len, 0.0, 0.0, 0.0);\n"
            "}\n";

        this->Basic_vertex =
            "#version 330\n"
            + POSITION_DEF
            + COLOR_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 vColor;\n"
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + "  * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vColor = " + COLOR + ";\n"
            "}\n";

        this->Basic_fragment =   
            "#version 330\n"
            "precision mediump float;\n"
            "in vec4 vColor;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    out_color = vColor;\n"
            "}\n";

        this->BasicExtrusion_vertex =
            "#version 330\n"
            + POSITION_DEF
            + AVERAGED_NORMAL_DEF
            + NORMAL_DEF
            + PROJECTION_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            " uniform float extrusionFactor;"
            " uniform vec4 color;"
            " uniform float zOffset;"
            "out vec4 vColor;\n"
            "void main() {\n"
            "    vec3 offsetNormal = normalize(vec3(" +  MODEL_INVERSE_TRANSPOSE_MATRIX + " * " + AVERAGED_NORMAL + ")) * extrusionFactor;\n"
            "    vec4 worldPos =  vec4(offsetNormal, 0.0) + " + MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vec4 outPos = " + PROJECTION_MATRIX + "  * " + VIEW_MATRIX + " * worldPos;\n"
            "    outPos.z += zOffset; \n"
            "    gl_Position = outPos; \n"
            "    vColor = color;\n"
            "}\n";

        this->BasicExtrusion_fragment =   
            "#version 330\n"
            "precision mediump float;\n"
            "in vec4 vColor;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    out_color = vColor;\n"
            "}\n";


        this->BasicColored_vertex =
            "#version 330\n"
            "precision highp float;\n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            " uniform vec4 color;"
            " uniform float zOffset;"
            "out vec4 vColor;\n"
            "void main() {\n"
            "    vec4 outPos = " + PROJECTION_MATRIX + "  * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    outPos.z += zOffset; \n"
            "    gl_Position = outPos; \n"
            "    vColor = color;\n"
            "}\n";

        this->BasicColored_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "in vec4 vColor;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    out_color = vColor;\n"
            "}\n";


        this->BasicLit_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"LightingSingle\" \n"
            + POSITION_DEF
            + COLOR_DEF
            + NORMAL_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF +
            "out vec4 vColor;\n"
            "out vec3 vNormal;\n"
            "out vec4 vPos;\n"
            "void main() {\n"
            "    vPos = " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vec4 viewSpacePos = " + VIEW_MATRIX + " * vPos;\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * vPos;\n"
            "    vColor = " + COLOR + ";\n"
            "    vNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * " + NORMAL + ");\n"
            "    TRANSFER_LIGHTING(" + POSITION + ", gl_Position, viewSpacePos) \n"
            "}\n";

        this->BasicLit_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"LightingSingle\"\n"
            + CAMERA_POSITION_DEF +
            "in vec4 vColor;\n"
            "in vec3 vNormal;\n"
            "in vec4 vPos;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            // instead of passing [vColor] directly to the litColorBlinnPhong function, we copy it into a new
            // vector. passing it directly causes a weird bug on some platforms/GPUs/versions of Linux
            // where shadows are not rendered
            "   out_color = litColorBlinnPhong(0, vec4(vColor.r, vColor.g, vColor.b, 1.0), vPos, normalize(vNormal), " + CAMERA_POSITION + ");\n"
            "}\n";

        this->BasicTextured_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + COLOR_DEF 
            + ALBEDO_UV_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 vColor;\n"
            "out vec3 vNormal;\n"
            "out vec2 vUV;\n"
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vUV = " + ALBEDO_UV + ";\n"
            "    vColor = " + COLOR + ";\n"
            "}\n";

        this->BasicTextured_fragment =   
            "#version 330\n"
            "precision mediump float;\n"
            "uniform sampler2D twoDtexture; \n"
            "in vec4 vColor;\n"
            "in vec2 vUV;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    vec4 textureColor = texture2D(twoDtexture, vUV);\n"
            "    out_color = textureColor;\n"
            "}\n";

        this->BasicTexturedLit_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"LightingSingle\" \n"
            + POSITION_DEF
            + COLOR_DEF
            + NORMAL_DEF
            + FACE_NORMAL_DEF
            + TANGENT_DEF
            + ALBEDO_UV_DEF
            + NORMAL_UV_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF + 
            "uniform vec4 lightPos;\n"
            "out vec4 vColor;\n"
            "out vec3 vNormal;\n"
            "out vec3 vTangent;\n"
            "out vec3 vFaceNormal;\n"
            "out vec2 vAlbedoUV;\n"
            "out vec2 vNormalUV;\n"
            "out vec4 vPos;\n"
            "void main() {\n"
            "    vPos = " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vec4 viewSpacePos = " + VIEW_MATRIX + " * vPos;\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * vPos;\n"
            "    vAlbedoUV = " + ALBEDO_UV + ";\n"
            "    vNormalUV = " + NORMAL_UV + ";\n"
            "    vColor = " + COLOR + ";\n"
            "    vec4 eNormal = " + NORMAL + ";\n"
            "    vNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    vec4 eTangent = " + TANGENT + ";\n"
            "    vTangent = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eTangent);\n"
            "    vFaceNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * " + FACE_NORMAL + ");\n"
            "    TRANSFER_LIGHTING(" + POSITION + ", gl_Position, viewSpacePos) \n"
            "}\n";

        this->BasicTexturedLit_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"LightingSingle\"\n"
            + CAMERA_POSITION_DEF + 
            "uniform int albedoMapEnabled; \n"
            "uniform int normalMapEnabled; \n"
            "uniform vec4 albedo; \n"
            "uniform sampler2D albedoMap; \n"
            "uniform sampler2D normalMap; \n"
            "in vec4 vColor;\n"
            "in vec3 vNormal;\n"
            "in vec3 vTangent;\n"
            "in vec3 vFaceNormal;\n"
            "in vec2 vAlbedoUV;\n"
            "in vec2 vNormalUV;\n"
            "in vec4 vPos;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "   vec4 _albedo; \n"
            "   if (albedoMapEnabled != 0) { \n"
            "       _albedo = texture(albedoMap, vAlbedoUV); \n"
            "   } else { \n"
            "      _albedo = albedo; \n"
            "   } \n"
            "   vec3 _normal; \n"
            "   if (normalMapEnabled != 0) { \n"
            "      _normal = calcMappedNormal(texture(normalMap, vNormalUV).xyz, vNormal, vTangent); \n"
            "   } else { \n"
            "       _normal = normalize(vNormal); \n"
            "   } \n"
            "   out_color = litColorBlinnPhong(0, texture(albedoMap, vAlbedoUV), vPos, _normal, " + CAMERA_POSITION + ");\n"
            "}\n";

        this->BasicCube_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + COLOR_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 vColor;\n"
            "out vec3 vUV;\n"
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vUV = normalize(" + POSITION + ".xyz);\n"
            "    vColor = " + COLOR + ";\n"
            "}\n";

        this->BasicCube_fragment =  
            "#version 330\n"
            "precision mediump float;\n"
            "uniform samplerCube cubeTexture; \n"
            "in vec4 vColor;\n"
            "in vec3 vUV;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    vec4 textureColor = texture(cubeTexture, vUV);\n"
            "    out_color = textureColor;\n"
            "}\n";
    }

}