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
const std::string BONE_INDEX = _an(Core::StandardAttribute::BoneIndex);
const std::string BONE_WEIGHT = _an(Core::StandardAttribute::BoneWeight);

const std::string MODEL_MATRIX = _un(Core::StandardUniform::ModelMatrix);
const std::string MODEL_INVERSE_TRANSPOSE_MATRIX = _un(Core::StandardUniform::ModelInverseTransposeMatrix);
const std::string VIEW_INVERSE_TRANSPOSE_MATRIX = _un(Core::StandardUniform::ViewInverseTransposeMatrix);
const std::string VIEW_MATRIX = _un(Core::StandardUniform::ViewMatrix);
const std::string PROJECTION_MATRIX = _un(Core::StandardUniform::ProjectionMatrix);
const std::string CAMERA_POSITION = _un(Core::StandardUniform::CameraPosition);
const std::string TEXTURE0 = _un(Core::StandardUniform::Texture0);
const std::string DEPTH_TEXTURE = _un(Core::StandardUniform::DepthTexture);
const std::string BONES = _un(Core::StandardUniform::Bones);
const std::string SKINNING_ENABLED = _un(Core::StandardUniform::SkinningEnabled);
const std::string SSAO_MAP = _un(Core::StandardUniform::SSAOMap);
const std::string SSAO_ENABLED = _un(Core::StandardUniform::SSAOEnabled);
const std::string DEPTH_OUTPUT_OVERRIDE = _un(Core::StandardUniform::DepthOutputOverride);

const std::string MAX_BONES = std::to_string(Core::Constants::MaxBones);
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
const std::string LIGHT_SHADOWS_ENABLED = _un(Core::StandardUniform::LightShadowsEnabled);
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
const std::string LIGHT_IRRADIANCE_MAP = _un(Core::StandardUniform::LightIrradianceMap);
const std::string LIGHT_SPECULAR_IBL_PREFILTERED_MAP = _un(Core::StandardUniform::LightSpecularIBLPreFilteredMap);
const std::string LIGHT_SPECULAR_IBL_BRDF_MAP = _un(Core::StandardUniform::LightSpecularIBLBRDFMap);
const std::string AMBIENT_LIGHT_COUNT = _un(Core::StandardUniform::AmbientLightCount);
const std::string AMBIENT_IBL_LIGHT_COUNT = _un(Core::StandardUniform::AmbientIBLLightCount);
const std::string POINT_LIGHT_COUNT = _un(Core::StandardUniform::PointLightCount);
const std::string DIRECTIONAL_LIGHT_COUNT = _un(Core::StandardUniform::DirectionalLightCount);
const std::string MAX_IBL_LOD_LEVELS = std::to_string(Core::Constants::MaxIBLLODLevels);

const std::string POSITION_DEF = "in vec4 " + POSITION + ";\n";
const std::string NORMAL_DEF = "in vec4 " + NORMAL + ";\n";
const std::string AVERAGED_NORMAL_DEF = "in vec4 " + AVERAGED_NORMAL + ";\n";
const std::string FACE_NORMAL_DEF = "in vec4 " + FACE_NORMAL + ";\n";
const std::string TANGENT_DEF = "in vec4 " + TANGENT + ";\n";
const std::string COLOR_DEF = "in vec4 " + COLOR + ";\n";
const std::string ALBEDO_UV_DEF = "in vec2 " + ALBEDO_UV + ";\n";
const std::string NORMAL_UV_DEF = "in vec2 " + NORMAL_UV + ";\n";
const std::string BONE_INDEX_DEF = "in ivec4 " + BONE_INDEX + ";\n";
const std::string BONE_WEIGHT_DEF = "in vec4 " + BONE_WEIGHT + ";\n";

const std::string MODEL_MATRIX_DEF = "uniform mat4 " + MODEL_MATRIX + ";\n";
const std::string MODEL_INVERSE_TRANSPOSE_MATRIX_DEF = "uniform mat4 " + MODEL_INVERSE_TRANSPOSE_MATRIX + ";\n";
const std::string VIEW_INVERSE_TRANSPOSE_MATRIX_DEF = "uniform mat4 " + VIEW_INVERSE_TRANSPOSE_MATRIX + ";\n";
const std::string VIEW_MATRIX_DEF = "uniform mat4 " + VIEW_MATRIX + ";\n";
const std::string PROJECTION_MATRIX_DEF = "uniform mat4 " + PROJECTION_MATRIX + ";\n";
const std::string CAMERA_POSITION_DEF = "uniform vec4 " + CAMERA_POSITION + ";\n";
const std::string TEXTURE0_DEF = "uniform sampler2D " + TEXTURE0 + ";\n";
const std::string DEPTH_TEXTURE_DEF = "uniform sampler2D " + DEPTH_TEXTURE + ";\n";
const std::string BONES_DEF = "uniform mat4 " + BONES + "[" + MAX_BONES + "];\n";
const std::string SKINNING_ENABLED_DEF = "uniform int " + SKINNING_ENABLED + ";\n";
const std::string SSAO_MAP_DEF = "uniform sampler2D " + SSAO_MAP + ";\n";
const std::string SSAO_ENABLED_DEF = "uniform int " + SSAO_ENABLED + ";\n";
const std::string DEPTH_OUTPUT_OVERRIDE_DEF = "uniform int " + DEPTH_OUTPUT_OVERRIDE + ";\n";

// ------------------------------------
// Single-pass lighting definitions
// ------------------------------------

// Common single-pass light parameters
const std::string LIGHT_COUNT_SINGLE_DEF = "const int " + LIGHT_COUNT + " = 1;\n";
const std::string LIGHT_COLOR_SINGLE_DEF = "uniform vec4 " + LIGHT_COLOR + "[1];\n";
const std::string LIGHT_INTENSITY_SINGLE_DEF = "uniform float " + LIGHT_INTENSITY + "[1];\n";
const std::string LIGHT_TYPE_SINGLE_DEF = "uniform int " + LIGHT_TYPE + "[1];\n";
const std::string LIGHT_ENABLED_SINGLE_DEF = "uniform int " + LIGHT_ENABLED + "[1];\n";
const std::string LIGHT_SHADOWS_ENABLED_SINGLE_DEF = "uniform int " + LIGHT_SHADOWS_ENABLED + "[1];\n";
const std::string LIGHT_MATRIX_SINGLE_DEF = "uniform mat4 " + LIGHT_MATRIX + "[1];\n";
const std::string LIGHT_CONSTANT_SHADOW_BIAS_SINGLE_DEF = "uniform float " + LIGHT_CONSTANT_SHADOW_BIAS + "[1];\n";
const std::string LIGHT_ANGULAR_SHADOW_BIAS_SINGLE_DEF ="uniform float " + LIGHT_ANGULAR_SHADOW_BIAS + "[1];\n";
const std::string LIGHT_SHADOW_MAP_SIZE_SINGLE_DEF = "uniform float " + LIGHT_SHADOW_MAP_SIZE + "[1];\n";
const std::string LIGHT_SHADOW_SOFTNESS_SINGLE_DEF = "uniform int " + LIGHT_SHADOW_SOFTNESS + "[1];\n";
// Single-pass ambient IBL light parameters
const std::string LIGHT_IRRADIANCE_MAP_SINGLE_DEF = "uniform samplerCube " + LIGHT_IRRADIANCE_MAP + "[1];\n";
const std::string LIGHT_SPECULAR_IBL_PREFILTERED_MAP_SINGLE_DEF = "uniform samplerCube " + LIGHT_SPECULAR_IBL_PREFILTERED_MAP + "[1];\n";
const std::string LIGHT_SPECULAR_IBL_BRDF_MAP_SINGLE_DEF = "uniform sampler2D " + LIGHT_SPECULAR_IBL_BRDF_MAP + "[1];\n";
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
#ifdef MANUAL_2D_SHADOWS
const std::string LIGHT_SHADOW_MAP_SINGLE_DEF = "uniform sampler2D " + LIGHT_SHADOW_MAP + "[" + MAX_CASCADES + "];\n";
#else
const std::string LIGHT_SHADOW_MAP_SINGLE_DEF = "uniform sampler2DShadow " + LIGHT_SHADOW_MAP + "[" + MAX_CASCADES + "];\n";
#endif
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
const std::string DIRECTIONAL_LIGHT_DEF = "uniform int " + DIRECTIONAL_LIGHT_COUNT + ";\n";
const std::string AMBIENTL_LIGHT_DEF = "unfirm int " + AMBIENT_LIGHT_COUNT + ";\n";
const std::string AMBIENTL_IBL_LIGHT_DEF = "unfirm int " + AMBIENT_IBL_LIGHT_COUNT + ";\n";
const std::string LIGHT_COUNT_DEF = "uniform int " + LIGHT_COUNT + ";\n";
// Single-pass ambient IBL light parameters
const std::string LIGHT_IRRADIANCE_MAP_DEF = "uniform samplerCube " + LIGHT_IRRADIANCE_MAP + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_SPECULAR_IBL_PREFILTERED_MAP_DEF = "uniform samplerCube " + LIGHT_SPECULAR_IBL_PREFILTERED_MAP + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_SPECULAR_IBL_BRDF_MAP_DEF = "uniform sampler2D " + LIGHT_SPECULAR_IBL_BRDF_MAP + "[" + MAX_LIGHTS + "];\n";
// Common multi-pass light parameters
const std::string LIGHT_COLOR_DEF = "uniform vec4 " + LIGHT_COLOR + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_INTENSITY_DEF = "uniform float " + LIGHT_INTENSITY + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_TYPE_DEF = "uniform int " + LIGHT_TYPE + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_ENABLED_DEF = "uniform int " + LIGHT_ENABLED + "[" + MAX_LIGHTS + "];\n";
const std::string LIGHT_SHADOWS_ENABLED_DEF = "uniform int " + LIGHT_SHADOWS_ENABLED + "[" + MAX_LIGHTS + "];\n";
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

#ifdef MANUAL_2D_SHADOWS
const std::string LIGHT_SHADOW_MAP_DEF = "uniform sampler2D " + LIGHT_SHADOW_MAP + "[" + MAX_CASCADES_LIGHTS + "];\n";
#else
const std::string LIGHT_SHADOW_MAP_DEF = "uniform sampler2DShadow " + LIGHT_SHADOW_MAP + "[" + MAX_CASCADES_LIGHTS + "];\n";
#endif
const std::string LIGHT_CASCADE_END_DEF = "uniform float " + LIGHT_CASCADE_END + "[" + MAX_CASCADES_LIGHTS + "];\n";
const std::string LIGHT_SHADOW_MAP_ASPECT_DEF = "uniform float " + LIGHT_SHADOW_MAP_ASPECT + "[" + MAX_CASCADES_LIGHTS + "];\n";
const std::string MAX_CASCADES_DEF = "const int MAX_CASCADES =" + MAX_CASCADES + ";\n";

namespace Core {

    const std::string ShaderManagerGL::BaseString("");

    ShaderManagerGL::~ShaderManagerGL() {
        
    }

    void ShaderManagerGL::init() {

        this->setShaderSource(ShaderType::Vertex, "Common", ShaderManagerGL::Common_vertex);
        this->setShaderSource(ShaderType::Fragment, "Common", ShaderManagerGL::Common_fragment);

        this->setShaderSource(ShaderType::Vertex, "Copy", ShaderManagerGL::Copy_vertex);
        this->setShaderSource(ShaderType::Fragment, "Copy", ShaderManagerGL::Copy_fragment);

        this->setShaderSource(ShaderType::Vertex, "Skybox", ShaderManagerGL::Skybox_vertex);
        this->setShaderSource(ShaderType::Fragment, "Skybox", ShaderManagerGL::Skybox_fragment);

        this->setShaderSource(ShaderType::Vertex, "Equirectangular", ShaderManagerGL::Equirectangular_vertex);
        this->setShaderSource(ShaderType::Fragment, "Equirectangular", ShaderManagerGL::Equirectangular_fragment);

        this->setShaderSource(ShaderType::Vertex, "Outline", ShaderManagerGL::Outline_vertex);
        this->setShaderSource(ShaderType::Geometry, "Outline", ShaderManagerGL::Outline_geometry);
        this->setShaderSource(ShaderType::Fragment, "Outline", ShaderManagerGL::Outline_fragment);

        this->setShaderSource(ShaderType::Vertex, "BufferOutline", ShaderManagerGL::BufferOutline_vertex);
        this->setShaderSource(ShaderType::Fragment, "BufferOutline", ShaderManagerGL::BufferOutline_fragment);

        this->setShaderSource(ShaderType::Vertex, "Blur", ShaderManagerGL::Blur_vertex);
        this->setShaderSource(ShaderType::Fragment, "Blur", ShaderManagerGL::Blur_fragment);

        this->setShaderSource(ShaderType::Vertex, "RedColorSet", ShaderManagerGL::RedColorSet_vertex);
        this->setShaderSource(ShaderType::Fragment, "RedColorSet", ShaderManagerGL::RedColorSet_fragment);

        this->setShaderSource(ShaderType::Vertex, "LightingCommon", ShaderManagerGL::Lighting_Common_vertex);
        this->setShaderSource(ShaderType::Fragment, "LightingCommon", ShaderManagerGL::Lighting_Common_fragment);

        this->setShaderSource(ShaderType::Vertex, "LightingHeaderSingle", ShaderManagerGL::Lighting_Header_Single_vertex);
        this->setShaderSource(ShaderType::Fragment, "LightingHeaderSingle", ShaderManagerGL::Lighting_Header_Single_fragment);

        this->setShaderSource(ShaderType::Vertex, "LightingHeaderMulti", ShaderManagerGL::Lighting_Header_Multi_vertex);
        this->setShaderSource(ShaderType::Fragment, "LightingHeaderMulti", ShaderManagerGL::Lighting_Header_Multi_fragment);

        this->setShaderSource(ShaderType::Vertex, "LightingSingle", ShaderManagerGL::Lighting_Single_vertex);
        this->setShaderSource(ShaderType::Fragment, "LightingSingle", ShaderManagerGL::Lighting_Single_fragment);

        this->setShaderSource(ShaderType::Vertex, "LightingMulti", ShaderManagerGL::Lighting_Multi_vertex);
        this->setShaderSource(ShaderType::Fragment, "LightingMulti", ShaderManagerGL::Lighting_Multi_fragment);

        this->setShaderSource(ShaderType::Vertex, "Lighting", ShaderManagerGL::Lighting_vertex);
        this->setShaderSource(ShaderType::Fragment, "Lighting", ShaderManagerGL::Lighting_fragment);

        this->setShaderSource(ShaderType::Fragment, "LightingDirCascade", ShaderManagerGL::Lighting_Dir_Cascade_fragment);
        
        this->setShaderSource(ShaderType::Vertex, "PhysicalCommon", ShaderManagerGL::PhysicalCommon_vertex);
        this->setShaderSource(ShaderType::Fragment, "PhysicalCommon", ShaderManagerGL::PhysicalCommon_fragment);

        this->setShaderSource(ShaderType::Vertex, "PhysicalLightingSingle", ShaderManagerGL::Physical_Lighting_Single_vertex);
        this->setShaderSource(ShaderType::Fragment, "PhysicalLightingSingle", ShaderManagerGL::Physical_Lighting_Single_fragment);

        this->setShaderSource(ShaderType::Vertex, "PhysicalLightingMulti", ShaderManagerGL::Physical_Lighting_Multi_vertex);
        this->setShaderSource(ShaderType::Fragment, "PhysicalLightingHeaderMulti", ShaderManagerGL::Physical_Lighting_Multi_Once_fragment);
        this->setShaderSource(ShaderType::Fragment, "PhysicalLightingMulti", ShaderManagerGL::Physical_Lighting_Multi_Per_Light_fragment);

        this->setShaderSource(ShaderType::Vertex, "PhysicalLighting", ShaderManagerGL::Physical_Lighting_vertex);
        this->setShaderSource(ShaderType::Fragment, "PhysicalLighting", ShaderManagerGL::Physical_Lighting_fragment);

        this->setShaderSource(ShaderType::Fragment, "StandardPhysicalVars", ShaderManagerGL::StandardPhysicalVars_fragment);
        this->setShaderSource(ShaderType::Fragment, "StandardPhysicalMain", ShaderManagerGL::StandardPhysicalMain_fragment);

        this->setShaderSource(ShaderType::Vertex, "StandardPhysical", ShaderManagerGL::StandardPhysical_vertex);
        this->setShaderSource(ShaderType::Fragment, "StandardPhysical", ShaderManagerGL::StandardPhysical_fragment);

        this->setShaderSource(ShaderType::Vertex, "StandardPhysicalMulti", ShaderManagerGL::StandardPhysicalMulti_vertex);
        this->setShaderSource(ShaderType::Fragment, "StandardPhysicalMulti", ShaderManagerGL::StandardPhysicalMulti_fragment);

        this->setShaderSource(ShaderType::Vertex, "AmbientPhysical", ShaderManagerGL::AmbientPhysical_vertex);
        this->setShaderSource(ShaderType::Fragment, "AmbientPhysical", ShaderManagerGL::AmbientPhysical_fragment);

        this->setShaderSource(ShaderType::Vertex, "PhysicalSkybox", ShaderManagerGL::PhysicalSkybox_vertex);
        this->setShaderSource(ShaderType::Fragment, "PhysicalSkybox", ShaderManagerGL::PhysicalSkybox_fragment);

        this->setShaderSource(ShaderType::Vertex, "Tonemap", ShaderManagerGL::Tonemap_vertex);
        this->setShaderSource(ShaderType::Fragment, "Tonemap", ShaderManagerGL::Tonemap_fragment);

        this->setShaderSource(ShaderType::Vertex, "IrradianceRenderer", ShaderManagerGL::IrradianceRenderer_vertex);
        this->setShaderSource(ShaderType::Fragment, "IrradianceRenderer", ShaderManagerGL::IrradianceRenderer_fragment);

        this->setShaderSource(ShaderType::Vertex, "SpecularIBLPreFilteredRenderer", ShaderManagerGL::SpecularIBLPreFilteredRenderer_vertex);
        this->setShaderSource(ShaderType::Fragment, "SpecularIBLPreFilteredRenderer", ShaderManagerGL::SpecularIBLPreFilteredRenderer_fragment);

        this->setShaderSource(ShaderType::Vertex, "SpecularIBLBRDFRenderer", ShaderManagerGL::SpecularIBLBRDFRenderer_vertex);
        this->setShaderSource(ShaderType::Fragment, "SpecularIBLBRDFRenderer", ShaderManagerGL::SpecularIBLBRDFRenderer_fragment);

        this->setShaderSource(ShaderType::Vertex, "VertexSkinning", ShaderManagerGL::VertexSkinning_vertex);
        this->setShaderSource(ShaderType::Fragment, "VertexSkinning", ShaderManagerGL::VertexSkinning_fragment);

        this->setShaderSource(ShaderType::Vertex, "Depth", ShaderManagerGL::Depth_vertex);
        this->setShaderSource(ShaderType::Fragment, "Depth", ShaderManagerGL::Depth_fragment);

        this->setShaderSource(ShaderType::Vertex, "Distance", ShaderManagerGL::Distance_vertex);
        this->setShaderSource(ShaderType::Fragment, "Distance", ShaderManagerGL::Distance_fragment);

        this->setShaderSource(ShaderType::Vertex, "Basic", ShaderManagerGL::Basic_vertex);
        this->setShaderSource(ShaderType::Fragment, "Basic", ShaderManagerGL::Basic_fragment);

        this->setShaderSource(ShaderType::Vertex, "BasicExtrusion", ShaderManagerGL::BasicExtrusion_vertex);
        this->setShaderSource(ShaderType::Fragment, "BasicExtrusion", ShaderManagerGL::BasicExtrusion_fragment);

        this->setShaderSource(ShaderType::Vertex, "BasicColored", ShaderManagerGL::BasicColored_vertex);
        this->setShaderSource(ShaderType::Fragment, "BasicColored", ShaderManagerGL::BasicColored_fragment);

        this->setShaderSource(ShaderType::Vertex, "BasicLit", ShaderManagerGL::BasicLit_vertex);
        this->setShaderSource(ShaderType::Fragment, "BasicLit", ShaderManagerGL::BasicLit_fragment);

        this->setShaderSource(ShaderType::Vertex, "BasicTextured", ShaderManagerGL::BasicTextured_vertex);
        this->setShaderSource(ShaderType::Fragment, "BasicTextured", ShaderManagerGL::BasicTextured_fragment);

        this->setShaderSource(ShaderType::Vertex, "BasicTexturedFullScreenQuad", ShaderManagerGL::BasicTexturedFullScreenQuad_vertex);
        this->setShaderSource(ShaderType::Fragment, "BasicTexturedFullScreenQuad", ShaderManagerGL::BasicTexturedFullScreenQuad_fragment);

        this->setShaderSource(ShaderType::Vertex, "BasicTexturedLit", ShaderManagerGL::BasicTexturedLit_vertex);
        this->setShaderSource(ShaderType::Fragment, "BasicTexturedLit", ShaderManagerGL::BasicTexturedLit_fragment);

        this->setShaderSource(ShaderType::Vertex, "BasicCube", ShaderManagerGL::BasicCube_vertex);
        this->setShaderSource(ShaderType::Fragment, "BasicCube", ShaderManagerGL::BasicCube_fragment);

        this->setShaderSource(ShaderType::Vertex, "Normals", ShaderManagerGL::Normals_vertex);
        this->setShaderSource(ShaderType::Fragment, "Normals", ShaderManagerGL::Normals_fragment);

        this->setShaderSource(ShaderType::Vertex, "Positions", ShaderManagerGL::Positions_vertex);
        this->setShaderSource(ShaderType::Fragment, "Positions", ShaderManagerGL::Positions_fragment);

        this->setShaderSource(ShaderType::Vertex, "PositionsAndNormals", ShaderManagerGL::PositionsAndNormals_vertex);
        this->setShaderSource(ShaderType::Fragment, "PositionsAndNormals", ShaderManagerGL::PositionsAndNormals_fragment);

        this->setShaderSource(ShaderType::Vertex, "ApplySSAO", ShaderManagerGL::ApplySSAO_vertex);
        this->setShaderSource(ShaderType::Fragment, "ApplySSAO", ShaderManagerGL::ApplySSAO_fragment);

        this->setShaderSource(ShaderType::Vertex, "SSAO", ShaderManagerGL::SSAO_vertex);
        this->setShaderSource(ShaderType::Fragment, "SSAO", ShaderManagerGL::SSAO_fragment);

        this->setShaderSource(ShaderType::Vertex, "SSAOBlur", ShaderManagerGL::SSAOBlur_vertex);
        this->setShaderSource(ShaderType::Fragment, "SSAOBlur", ShaderManagerGL::SSAOBlur_fragment);
    }

    ShaderManagerGL::ShaderManagerGL() {

        this->Common_vertex =
            "const float PI = 3.14159265359;\n";

        this->Common_fragment =
            "const float PI = 3.14159265359;\n"
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

        this->Copy_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec2 vUV;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vUV = localPos.xy / 2.0 + 0.5; \n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "}\n";

        this->Copy_fragment =  
            "#version 330 core \n"
            "out vec4 out_color; \n"
            "in vec2 vUV; \n"
            + TEXTURE0_DEF + 
            "void main()  \n"
            "{ \n"
            "    out_color = texture(" + TEXTURE0 + ", vUV); \n"
            "}\n";

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
            + VIEW_MATRIX_DEF +
            "out vec4 TexCoord0;\n"
            "void main()\n"
            "{\n"
            "    TexCoord0 = " + POSITION + ";\n"
            "    vec4 vWorldPos = mat4(mat3(" + VIEW_MATRIX + ")) * " + POSITION + ";\n"
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
            "#include \"VertexSkinning\" \n"
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
            "    vec4 localPos = " + POSITION + "; \n"
            "    vec4 localNormal = " + NORMAL + "; \n"
            "    vec4 localFaceNormal = vec4(1.0, 0.0, 0.0, 0.0); \n"
            "    calculateSkinnedPositionAndNormals(localPos, localNormal, localFaceNormal); \n"
            "    VNormal = normalize(vec3(inverse(transpose(" + VIEW_MATRIX + " * " + MODEL_MATRIX + ")) * localNormal));\n"
            "    VPosition = vec3(" + VIEW_MATRIX + " * " + MODEL_MATRIX + " * localPos);\n"
            "    vec4 outPos = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
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
            "out float vAlpha; \n"
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
            "   vAlpha = 1.0; \n"
            "   EmitVertex();\n"
            "   gl_Position = vec4( e0.xy + n * outlineDir - ext, e0.z + epsilon, 1.0 ) * w0;\n"
            "   vAlpha = 0.0; \n"
            "   EmitVertex();\n"
            "   gl_Position = vec4( e1.xy + ext, e1.z + epsilon, 1.0 ) * w1;\n"
            "   vAlpha = 1.0; \n"
            "   EmitVertex();\n"
            "   gl_Position = vec4( e1.xy + n * outlineDir + ext, e1.z + epsilon, 1.0 ) * w1;\n"
            "   vAlpha = 0.0; \n"
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
            "in float vAlpha;\n"
            "uniform vec4 color;"
            "flat in int GIsEdge; // Whether or not we're drawing an edge \n"
            "layout( location = 0 ) out vec4 out_color;\n"
            "void main() {\n"
            "   float alpha = clamp(vAlpha * 2.0, 0.0, 1.0); \n"
            "   vec4 rColor = vec4(color.rgb, alpha); \n"
            "   out_color = rColor;\n"
            "}\n";

        this->BufferOutline_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec2 vUV;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vUV = localPos.xy / 2.0 + 0.5; \n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "}\n";

        this->BufferOutline_fragment =  
            "#version 330 core \n"
            "out vec4 out_color; \n"
            "in vec2 vUV; \n"
            + TEXTURE0_DEF + 
            "uniform int outlineSize; \n"
            "uniform vec4 outlineColor; \n"
            "void main()  \n"
            "{ \n"
            "    int _outlineSize = outlineSize; \n"
            "    vec2 texelSize = 1.0 / vec2(textureSize(" + TEXTURE0 + ", 0)); \n"
            "    float result = 0.0; \n"
            "    vec4 sampleColor; \n"
            "    for (int x = -_outlineSize; x <= _outlineSize; ++x)  \n"
            "    { \n"
            "        for (int y = -_outlineSize; y <= _outlineSize; ++y)  \n"
            "        { \n"
            "            vec2 offset = vec2(float(x), float(y)) * texelSize; \n"
            "            sampleColor = texture(" + TEXTURE0 + ", vUV + offset); \n"
            "            result = max(max(sampleColor.r, sampleColor.b), sampleColor.g); \n"
            "            if (result > 0.0) { \n"
            "                result = 1.0; \n"
            "                break; \n"
            "            }\n"
            "        } \n"
            "        if (result > 0.0) { \n"
            "           break; \n"
            "        }\n"
            "    } \n"
            "    out_color = result * vec4(sampleColor.rgb, 1.0); \n"
           //"       out_color = result * vec4(outlineColor.rgb, 1.0); \n"
            "}\n";

        this->Blur_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec2 vUV;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vUV = localPos.xy / 2.0 + 0.5; \n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "}\n";

        this->Blur_fragment =  
            "#version 330 core \n"
            "out vec4 out_color; \n"
            "in vec2 vUV; \n"
            + TEXTURE0_DEF + 
            "uniform int kernelSize; \n"
            "void main()  \n"
            "{ \n"
           /* "    int _kernelSize = kernelSize; \n"
            "    vec2 texelSize = 1.0 / vec2(textureSize(" + TEXTURE0 + ", 0)); \n"
            "    vec4 result = vec4(0.0, 0.0, 0.0, 0.0); \n"
            "    for (int x = -_kernelSize; x < _kernelSize; ++x)  \n"
            "    { \n"
            "        for (int y = -_kernelSize; y < _kernelSize; ++y)  \n"
            "        { \n"
            "            vec2 offset = vec2(float(x), float(y)) * texelSize; \n"
            "            result += texture(" + TEXTURE0 + ", vUV + offset); \n"
            "        } \n"
            "    } \n"
            "    out_color = result / (kernelSize * kernelSize); \n"*/

            "    float Pi = 6.28318530718; // Pi*2 \n"
            "    vec2 iResolution = vec2(textureSize(" + TEXTURE0 + ", 0)); \n"

            "    // GAUSSIAN BLUR SETTINGS {{{\n"
            "    float Directions = 16.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)\n"
            "    float Quality = 3.0; // BLUR QUALITY (Default 4.0 - More is better but slower)\n"
            "    float Size = float(kernelSize); // BLUR SIZE (Radius)\n"
            "    // GAUSSIAN BLUR SETTINGS }}}\n"
        
            "    vec2 Radius = Size/iResolution.xy;\n"
            
            "    // Normalized pixel coordinates (from 0 to 1)\n"
            "    vec2 uv = gl_FragCoord.xy/iResolution.xy;\n"
            "    // Pixel colour\n"
            "    vec4 Color = texture(" + TEXTURE0 + ", uv);\n"
            
            "    // Blur calculations\n"
            "    float totalAlpha = 0.0; \n"
            "    float totalWeight = 0.0; \n"
            "    for( float d=0.0; d<Pi; d+=Pi/Directions)\n"
            "    {\n"
            "        for(float i=1.0/Quality; i<=1.0; i+=1.0/Quality)\n"
            "        {\n"
            "            vec4 sample =  texture(" + TEXTURE0 + ", uv+vec2(cos(d),sin(d))*Radius*i); \n"
            "            Color += sample;		\n"
            "            totalAlpha += sample.a; \n"
             "           totalWeight += 1.0; \n"
            "        }\n"
            "    }\n"
            
            "    // Output to screen\n"
            "    out_color =  vec4(Color.rgb / totalAlpha, Color.a / totalWeight);\n"

            "}   \n";

        this->RedColorSet_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec2 vUV;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vUV = localPos.xy / 2.0 + 0.5; \n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "}\n";

        this->RedColorSet_fragment =  
            "#version 330 core \n"
            "out vec4 out_color; \n"
            "in vec2 vUV; \n"
            + TEXTURE0_DEF + 
            "uniform vec4 outputColor; \n"
            "void main()  \n"
            "{ \n"
            "    vec4 sample = texture(" + TEXTURE0 + ", vUV); \n"
            "    float stepVal = step(0.001, sample.r) * (1.0 - step(0.001, sample.g)) * (1.0 - step(0.001, sample.b)); \n"
            "    out_color =  outputColor * stepVal + sample * (1.0 - stepVal);\n"
            "}   \n";

        this->Lighting_Common_vertex = "";

        this->Lighting_Common_fragment =
            "const int AMBIENT_LIGHT = 0;\n"
            "const int AMBIENT_IBL_LIGHT = 1;\n"
            "const int DIRECTIONAL_LIGHT = 2;\n"
            "const int POINT_LIGHT = 3;\n"
            "const int SPOT_LIGHT = 4;\n"
            "const int PLANAR_LIGHT = 5;\n"
            "const int DEPTH_OUTPUT_DEPTH = 1;\n"
            "const int DEPTH_OUTPUT_DISTANCE = 2;\n";

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
            + LIGHT_SHADOWS_ENABLED_DEF
            + LIGHT_SHADOW_SOFTNESS_DEF
            + LIGHT_COLOR_DEF
            + LIGHT_INTENSITY_DEF
            + LIGHT_SHADOW_MAP_SIZE_DEF
            + LIGHT_NEAR_PLANE_DEF
            + LIGHT_IRRADIANCE_MAP_DEF
            + LIGHT_SPECULAR_IBL_PREFILTERED_MAP_DEF
            + LIGHT_SPECULAR_IBL_BRDF_MAP_DEF +
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
            + LIGHT_SHADOWS_ENABLED_SINGLE_DEF
            + LIGHT_SHADOW_SOFTNESS_SINGLE_DEF
            + LIGHT_COLOR_SINGLE_DEF
            + LIGHT_INTENSITY_SINGLE_DEF
            + LIGHT_SHADOW_MAP_SIZE_SINGLE_DEF
            + LIGHT_NEAR_PLANE_SINGLE_DEF
            + LIGHT_IRRADIANCE_MAP_SINGLE_DEF
            + LIGHT_SPECULAR_IBL_PREFILTERED_MAP_SINGLE_DEF
            + LIGHT_SPECULAR_IBL_BRDF_MAP_SINGLE_DEF +
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
            "_core_viewSpacePosZ[i] = abs(viewSpacePos.z);"
            "}";

        this->Lighting_Dir_Cascade_fragment = "vec3 calcDirShadowFactorCoordsSingleIndex_@lightIndex_@cascadeIndex(vec2 uv, float fragDepth, float angularBias) { \n"
            "   return vec3(uv.xy, fragDepth - angularBias - " + LIGHT_CONSTANT_SHADOW_BIAS + "[@lightIndex]); \n"
            "} \n"

            "float calcDirShadowFactor_@lightIndex_@cascadeIndex(float angularBias, vec4 fragPos)\n"
            "{ \n"
            "    int cascadeOffset = @lightIndex * " + MAX_CASCADES + " + @cascadeIndex;\n"
            "    vec4 lightSpacePos = _core_lightSpacePos[cascadeOffset]; \n"
            "    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w; \n"
            "    vec3 uvCoords = (projCoords * 0.5) + vec3(0.5, 0.5, 0.5); \n"
            "    float px = 1.0 / " + LIGHT_SHADOW_MAP_SIZE + "[@lightIndex]; \n"
            "    float py =  " + LIGHT_SHADOW_MAP_ASPECT + "[cascadeOffset] / " + LIGHT_SHADOW_MAP_SIZE + "[@lightIndex]; \n"

            "    float shadowFactor = 0.0; \n"
            "    vec2 uv = uvCoords.xy; \n"
            "    float z = uvCoords.z; \n"

            "    if (uvCoords.x < 0 || uvCoords.x > 1.0) return 0.0; \n"
            "    if (uvCoords.y < 0 || uvCoords.y > 1.0) return 0.0; \n"

           "    if (" + LIGHT_SHADOW_SOFTNESS + "[@lightIndex] == 2 || " + LIGHT_SHADOW_SOFTNESS + "[@lightIndex] == 1) { \n"
            "        for (int y = -" + LIGHT_SHADOW_SOFTNESS + "[@lightIndex]; y <= " + LIGHT_SHADOW_SOFTNESS + "[@lightIndex] ; y++) { \n"
            "            for (int x = -" + LIGHT_SHADOW_SOFTNESS + "[@lightIndex]; x <= " + LIGHT_SHADOW_SOFTNESS + "[@lightIndex] ; x++) { \n"
            "                vec2 coords2D = vec2(uv.x + x * px, uv.y + y * py); \n"
#ifdef MANUAL_2D_SHADOWS
            "                float shadowDepth = clamp(texture(" + LIGHT_SHADOW_MAP + "[cascadeOffset], coords2D).r, 0.0, 1.0); \n"
#else
            "                vec3 coords = calcDirShadowFactorCoordsSingleIndex_@lightIndex_@cascadeIndex(coords2D, z, angularBias); \n"
            "                float shadowDepth = clamp(texture(" + LIGHT_SHADOW_MAP + "[cascadeOffset], coords), 0.0, 1.0); \n"
#endif
            "                shadowFactor += (1.0-shadowDepth); \n"
            "            } \n"
            "        } \n "
            "        if (" + LIGHT_SHADOW_SOFTNESS + "[@lightIndex] == 2) shadowFactor /= 25.0; \n"
            "        else shadowFactor /= 9.0; \n"
            "    } \n"
            "    else { \n"
#ifdef MANUAL_2D_SHADOWS
            "        float shadowDepth = clamp(texture(" + LIGHT_SHADOW_MAP + "[cascadeOffset], uv).r, 0.0, 1.0); \n"
#else
            "        vec3 coords = calcDirShadowFactorCoordsSingleIndex_@lightIndex_@cascadeIndex(uv, z, angularBias); \n"
            "        float shadowDepth = clamp(texture(" + LIGHT_SHADOW_MAP + "[cascadeOffset], coords), 0.0, 1.0); \n"
#endif
            "        shadowFactor += (1.0-shadowDepth); \n"
            "    } \n"

            "    return shadowFactor; \n"
            "} \n";

        this->Lighting_fragment =
            "#include \"LightingDirCascade(lightIndex=@lightIndex,cascadeIndex=0)\"\n"
            "#include \"LightingDirCascade(lightIndex=@lightIndex,cascadeIndex=1)\"\n"
            "#include \"LightingDirCascade(lightIndex=@lightIndex,cascadeIndex=2)\"\n"

            "vec4 getDirLightColor@lightIndex(vec4 worldPos, float bias) { \n"
            "    float shadowFactor = 0.0;\n"
            "    vec3 lightColor = " + LIGHT_COLOR + "[@lightIndex].rgb; \n"
           /* "    for (int i = 0 ; i < " + LIGHT_CASCADE_COUNT + "[@lightIndex]; i++) { \n"
            "        int offset = @lightIndex * " + MAX_CASCADES + " + i;\n"
            "        if (_core_viewSpacePosZ[@lightIndex] <= " + LIGHT_CASCADE_END + "[offset]) { \n"
            "            shadowFactor = calcDirShadowFactor(i, _core_lightSpacePos[offset], bias, worldPos); \n"
            "            break; \n"
            "        } \n"
            "    } \n"*/

            "    if (" + LIGHT_SHADOWS_ENABLED + "[@lightIndex] != 0) {\n"
            "      int offset0 = @lightIndex * " + MAX_CASCADES + ";\n"
            "      int offset1 = @lightIndex * " + MAX_CASCADES + " + 1;\n"
            "      int offset2 = @lightIndex * " + MAX_CASCADES + " + 2;\n"
            "      if (_core_viewSpacePosZ[@lightIndex] <= " +
             LIGHT_CASCADE_END + "[offset0]) { \n"
            "          shadowFactor = calcDirShadowFactor_@lightIndex_0(bias, worldPos); \n"
            "      } else if (_core_viewSpacePosZ[@lightIndex] <= " + LIGHT_CASCADE_END + "[offset1]) { \n"
            "          shadowFactor = calcDirShadowFactor_@lightIndex_1(bias, worldPos); \n"
            "      } else if (_core_viewSpacePosZ[@lightIndex] <= " + LIGHT_CASCADE_END + "[offset2]) { \n"
            "          shadowFactor = calcDirShadowFactor_@lightIndex_2(bias, worldPos); \n"
            "      } \n"
            "    } \n"

            "    return vec4((1.0 - shadowFactor) * lightColor * " + LIGHT_INTENSITY + "[@lightIndex], 1.0);\n"    
            "} \n"

            "float getPointLightShadowFactor@lightIndex(vec3 lightLocalPos, float bias) { \n"
             "   vec4 shadowDepthVec = texture(" + LIGHT_SHADOW_CUBE_MAP + "[@lightIndex], lightLocalPos);\n"
            "    float shadowDepth = shadowDepthVec.r;\n"
            "    float shadowFactor = 0.0; \n"
            "    if (shadowDepth + bias > length(lightLocalPos) || shadowDepth < .001) {\n"
            "        shadowFactor = 1.0;\n"
            "    }\n"
            "    return shadowFactor;\n"
            "} \n"

            "vec4 getPointLightColor@lightIndex(vec3 lightLocalPos, float bias) { \n"
            "    float pxToWorld = 1.0 / " + LIGHT_SHADOW_MAP_SIZE + "[@lightIndex] * 0.2; \n"
            "    float near = " + LIGHT_NEAR_PLANE + "[@lightIndex]; \n"

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
            "    if (" + LIGHT_SHADOW_SOFTNESS + "[@lightIndex] == 2 || " + LIGHT_SHADOW_SOFTNESS + "[@lightIndex] == 1) { \n"
            "        shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos, bias); \n"
            "        for (int i = 1; i <= " + LIGHT_SHADOW_SOFTNESS + "[@lightIndex]; i++) { \n"
            "            shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos + right * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos - right * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos - up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos + right * i + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos + right * i - up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos - right * i + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos - right * i - up * i, bias); \n"
            "        } \n "
            "        if (" + LIGHT_SHADOW_SOFTNESS + "[@lightIndex] == 2) shadowFactor /= 17.0; \n"
            "        else shadowFactor /= 9.0; \n"
            "    } \n"
            "    else { \n"
            "        shadowFactor += getPointLightShadowFactor@lightIndex(lightLocalPos, bias); \n"
            "    } \n"       
           
            "   return vec4(" + LIGHT_COLOR + "[@lightIndex].rgb * shadowFactor * " + LIGHT_INTENSITY + "[@lightIndex], 1.0);\n"
            "}\n"

            "void getDirLightParameters@lightIndex(in vec3 worldNormal, in vec3 toViewer, out vec3 toLight, out vec3 halfwayVec, out float NdotL, out float bias) { \n"
            "    toLight = vec3(-" + LIGHT_DIRECTION + "[@lightIndex]);\n"
            "    NdotL = max(cos(acos(dot(toLight, worldNormal)) * 1.1), 0.0); \n"  
            "    halfwayVec = normalize(toViewer + toLight); \n"
            "    bias = (1.0 - NdotL) * " + LIGHT_ANGULAR_SHADOW_BIAS + "[@lightIndex];"
            "} \n"

            "void getPointLightParameters@lightIndex(in vec4 worldPos, in vec3 worldNormal, in vec3 toViewer, out vec3 lightLocalPos, out vec3 toLight, out vec3 halfwayVec, out float NdotL, out float bias, out float attenuation) { \n"
            "    lightLocalPos = vec3(" + LIGHT_MATRIX + "[@lightIndex] * worldPos);\n"
            "    toLight = vec3(" + LIGHT_POSITION + "[@lightIndex] - worldPos);\n"
            "    float distance = length(toLight); \n"
            "    vec3 toLightNormalized = normalize(toLight);\n"
            "    NdotL = max(cos(acos(dot(toLightNormalized, worldNormal)) * 1.025), 0.0); \n"
            "    halfwayVec = normalize(toViewer + toLight); \n"
            "    attenuation = clamp(" + LIGHT_RANGE + "[@lightIndex] / (distance * distance), 0.0, 1.0); \n"
            "    bias = (1.0 - NdotL) * " + LIGHT_ANGULAR_SHADOW_BIAS + "[@lightIndex] + " + LIGHT_CONSTANT_SHADOW_BIAS + "[@lightIndex];\n"
            "} \n"

            "vec4 litColorBlinnPhong@lightIndex(in vec4 albedo, in vec4 worldPos, in vec3 worldNormal, in vec4 cameraPos) {\n"
            "    if (" + LIGHT_ENABLED + "[@lightIndex] != 0) {\n"
            "        if (" + LIGHT_TYPE + "[@lightIndex] == AMBIENT_LIGHT) {\n"
            "            return vec4(albedo.rgb * " + LIGHT_COLOR + "[@lightIndex].rgb * " + LIGHT_INTENSITY + "[@lightIndex], albedo.a);\n"
            "        }\n"
            "        else { \n"
            "            vec3 radiance; \n"
            "            vec3 lightLocalPos, toLight, halfwayVec; \n"
            "            float NdotL, bias, attenuation; \n"
            "            vec3 toViewer = vec3(cameraPos - worldPos); \n"
            "            if (" + LIGHT_TYPE + "[@lightIndex] == DIRECTIONAL_LIGHT) {\n"
            "                getDirLightParameters@lightIndex(worldNormal, toViewer, toLight, halfwayVec, NdotL, bias); \n"
            "                vec4 lightColor = getDirLightColor@lightIndex(worldPos, bias); \n"
            "                radiance = lightColor.rgb; \n"
            "            }\n"
            "            else if (" + LIGHT_TYPE + "[@lightIndex] == POINT_LIGHT) {\n"
            "                getPointLightParameters@lightIndex(worldPos, worldNormal, toViewer, lightLocalPos, toLight, halfwayVec, NdotL, bias, attenuation); \n"
            "                vec4 lightColor = getPointLightColor@lightIndex(lightLocalPos, bias);\n"
            "                radiance = lightColor.rgb * attenuation; \n"
            "            }\n"
            "            return vec4(radiance * albedo.rgb * NdotL, albedo.a);\n"
            "        }\n"
            "    }\n"
            "    return albedo;\n"
            "}\n";

        this->PhysicalCommon_vertex =
            "\n";

        this->PhysicalCommon_fragment =
            "const float MAX_REFLECTION_LOD = " + MAX_IBL_LOD_LEVELS + "; \n"
            "const int TONE_MAP_REINHARD = 0; \n"
            "const int TONE_MAP_EXPOSURE = 1; \n"
            "vec3 toneMapReinhard(vec3 color) { \n"
            "    return color / (color + vec3(1.0));\n"
            "}\n"
            "vec3 toneMapExposure(vec3 color, float exposure) { \n"
            "    return vec3(1.0) - exp(-color * exposure);\n"
            "}\n"
            "vec3 gammaCorrectBasic(vec3 color) { \n"
            "    return pow(color, vec3(1.0/2.2));\n"
            "}\n"
            "vec3 gammaCorrectCustom(vec3 color, float gamma) { \n"
            "    return pow(color, vec3(1.0 / gamma));\n"
            "}\n"
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

            "vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) \n "
            "{ \n "
            "    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0); \n "
            "}  \n "

            "float radicalInverse_VdC(uint bits) \n"
            "{ \n"
            "    bits = (bits << 16u) | (bits >> 16u); \n"
            "    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u); \n"
            "    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u); \n"
            "    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u); \n"
            "    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u); \n"
            "    return float(bits) * 2.3283064365386963e-10; \n"
            "} \n"
            
            "vec2 hammersley(uint i, uint N) \n"
            "{ \n"
            "    return vec2(float(i)/float(N), radicalInverse_VdC(i)); \n"
            "} \n"

            "vec3 importanceSampleGGX(vec2 Xi, vec3 N, float roughness) \n"
            "{ \n"
            "    float a = roughness*roughness; \n"
                
            "    float phi = 2.0 * PI * Xi.x; \n"
            "    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y)); \n"
            "    float sinTheta = sqrt(1.0 - cosTheta*cosTheta); \n"
                
            // from spherical coordinates to cartesian coordinates
            "    vec3 H; \n"
            "    H.x = cos(phi) * sinTheta; \n"
            "    H.y = sin(phi) * sinTheta; \n"
            "    H.z = cosTheta; \n"
                
            // from tangent-space vector to world-space sample vector
            "    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0); \n"
            "    vec3 tangent   = normalize(cross(up, N)); \n"
            "    vec3 bitangent = cross(N, tangent); \n"
                
            "    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z; \n"
            "    return normalize(sampleVec); \n"
            "} \n";

        this->Physical_Lighting_Single_vertex =
            "#include \"LightingHeaderSingle\" \n"
            "#include \"Lighting\" \n";

        this->Physical_Lighting_Single_fragment =
            "#include \"LightingHeaderSingle\" \n"
            "#include \"Lighting(lightIndex=0)\" \n"
            "#include \"PhysicalCommon\" \n"
            "#include \"PhysicalLighting(lightIndex=0)\" \n";

        this->Physical_Lighting_Multi_vertex =
            "#include \"LightingHeaderMulti\" \n"
            "#include \"Lighting\" \n";

        this->Physical_Lighting_Multi_Once_fragment =
            "#include \"LightingHeaderMulti\" \n"
            "#include \"PhysicalCommon\" \n";
   
        this->Physical_Lighting_Multi_Per_Light_fragment =
            "#include \"Lighting(lightIndex=@lightIndex)\" \n"
            "#include \"PhysicalLighting(lightIndex=@lightIndex)\" \n";

        this->Physical_Lighting_vertex =
            "\n";

        this->Physical_Lighting_fragment =
            "vec4 litColorPhysical@lightIndex(in vec4 albedo, in vec4 worldPos, in vec3 worldNormal, in vec4 cameraPos, in float metallic, in float roughness, in float ao) {\n"
            "    if (" + LIGHT_ENABLED + "[@lightIndex] != 0) {\n"
            "        vec3 V = normalize(vec3(cameraPos - worldPos)); \n "
            "        vec3 F0 = vec3(0.04); \n "
            "        F0 = mix(F0, albedo.rgb, metallic); \n " 
            "        if (" + LIGHT_TYPE + "[@lightIndex] == AMBIENT_LIGHT) {\n"
             "            return vec4(albedo.rgb * " + LIGHT_COLOR + "[@lightIndex].rgb * " + LIGHT_INTENSITY + "[@lightIndex], albedo.a);\n"
            "        }\n"
            "        else if (" + LIGHT_TYPE + "[@lightIndex] == AMBIENT_IBL_LIGHT) {\n"
            "             vec3 irradiance = texture(" + LIGHT_IRRADIANCE_MAP + "[@lightIndex], worldNormal).rgb; \n"
            "             vec3 F = fresnelSchlickRoughness(max(dot(worldNormal, V), 0.0), F0, roughness);  \n"
            "             vec3 kS = F; \n"
            "             vec3 kD = 1.0 - kS;  \n"
            "             vec3 diffuseIBL  = irradiance * albedo.rgb;  \n"

            "             vec3 R = reflect(-V, worldNormal); \n"
            "             kD *= 1.0 - metallic; \n"

            "             vec3 prefilteredColor = textureLod(" + LIGHT_SPECULAR_IBL_PREFILTERED_MAP + "[@lightIndex], R, roughness * MAX_REFLECTION_LOD).rgb; \n"
            "             vec2 envBRDF  = texture(" + LIGHT_SPECULAR_IBL_BRDF_MAP + "[@lightIndex], vec2(max(dot(worldNormal, V), 0.0), roughness)).rg; \n"
            "             vec3 specularIBL = prefilteredColor * F * (envBRDF.x + envBRDF.y); \n"
            "             vec3 ambient = (kD * diffuseIBL + specularIBL) * ao; \n"
            "             return vec4(max(ambient, 0.0), 1.0); \n"
            "        }\n"
            "        else { \n"
            "            vec3 lightLocalPos, toLight, halfwayVec, radiance; \n"
            "            float NdotL, bias, attenuation; \n"  
            "            if (" + LIGHT_TYPE + "[@lightIndex] == DIRECTIONAL_LIGHT) {\n"
            "                getDirLightParameters@lightIndex(worldNormal, V, toLight, halfwayVec, NdotL, bias); \n"
            "                vec4 lightColor = getDirLightColor@lightIndex(worldPos, bias); \n"
            "                radiance = lightColor.rgb; \n"
            "            }\n"
            "            else if (" + LIGHT_TYPE + "[@lightIndex] == POINT_LIGHT) {\n"
            "                getPointLightParameters@lightIndex(worldPos, worldNormal, V, lightLocalPos, toLight, halfwayVec, NdotL, bias, attenuation); \n"
            "                vec4 lightColor = getPointLightColor@lightIndex(lightLocalPos, bias);\n"
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

            "            return vec4(color, albedo.a); \n"
            "        }\n"
            "    }\n"
            "    return albedo;\n"
            "}\n";

        this->StandardPhysical_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\" \n"
            "#include \"PhysicalLightingSingle\" \n"
            "#include \"VertexSkinning\" \n"
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
            "out vec4 vViewPos; \n"
            "out vec4 vClipPos; \n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vec4 localNormal = " + NORMAL + "; \n"
            "    vec4 localFaceNormal = " + FACE_NORMAL + "; \n"
            "    calculateSkinnedPositionAndNormals(localPos, localNormal, localFaceNormal); \n"
            "    vWorldPos = " +  MODEL_MATRIX + " * localPos;\n"
            "    vViewPos = " + VIEW_MATRIX + " * vWorldPos;\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * vWorldPos;\n"
            "    vClipPos = " + PROJECTION_MATRIX + " * vViewPos; \n"
            "    vAlbedoUV = " + ALBEDO_UV + ";\n"
            "    vNormalUV = " + NORMAL_UV + ";\n"
            "    vColor = " + COLOR + ";\n"
            "    vec4 eNormal = localNormal;\n"
            "    vNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    vec4 eTangent = " + TANGENT + ";\n"
            "    vTangent = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eTangent);\n"
            "    vFaceNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * localFaceNormal);\n"
            "    TRANSFER_LIGHTING(localPos, gl_Position, vViewPos) \n"
            "}\n";

        this->StandardPhysicalVars_fragment =
            CAMERA_POSITION_DEF
            + SSAO_MAP_DEF
            + SSAO_ENABLED_DEF
            + DEPTH_OUTPUT_OVERRIDE_DEF +
            "uniform int enabledMap; \n"
            "uniform int enabledOpacityChannel; \n"
            "uniform int discardMask; \n"
            "uniform vec4 albedo; \n"
            "uniform sampler2D albedoMap; \n"
            "uniform sampler2D normalMap; \n"
            "uniform sampler2D roughnessMap; \n"
            "uniform sampler2D metallicMap; \n"
            "uniform sampler2D opacityMap; \n"
            "uniform float metallic; \n"
            "uniform float roughness; \n"
            "uniform float opacity; \n"
            "uniform float ambientOcclusion; \n"
            "in vec4 vColor;\n"
            "in vec3 vNormal;\n"
            "in vec3 vTangent;\n"
            "in vec3 vFaceNormal;\n"
            "in vec2 vAlbedoUV;\n"
            "in vec2 vNormalUV;\n"
            "in vec4 vWorldPos;\n"
            "in vec4 vViewPos; \n"
            "in vec4 vClipPos;\n"
            "out vec4 out_color;\n";

        this->StandardPhysicalMain_fragment =
            "   int opacityChannelRedEnabled = enabledOpacityChannel & 1; \n"
            "   int opacityChannelAlphaEnabled = enabledOpacityChannel & 8; \n"

            "   int albedoMapEnabled = enabledMap & 1; \n"
            "   int normalMapEnabled = enabledMap & 2; \n"
            "   int roughnessMapEnabled = enabledMap & 4; \n"
            "   int metallicMapEnabled = enabledMap & 8; \n"
            "   int opacityMapEnabled = enabledMap & 16; \n"
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
            "   float _roughness; \n"
            "   if (roughnessMapEnabled != 0) { \n"
            "       vec3 fullRoughness = texture(roughnessMap, vAlbedoUV).rgb; \n"
            "      _roughness = fullRoughness.r; \n"
            "   } else { \n"
            "       _roughness = roughness; \n"
            "   } \n"
            "   float _metallic; \n"
            "   if (metallicMapEnabled != 0) { \n"
            "      vec4 fullMetallic = texture(metallicMap, vAlbedoUV); \n"
            "      _metallic = fullMetallic.r; \n"
            "   } else { \n"
            "       _metallic = metallic; \n"
            "   } \n"
            "   float _opacity; \n"
            "   if (opacityMapEnabled != 0) { \n"
            "       vec4 opacitySample = texture(opacityMap, vAlbedoUV); \n"
            "      _opacity = clamp(opacitySample.r * opacityChannelRedEnabled + opacitySample.a * opacityChannelAlphaEnabled, 0.0, 1.0); \n"
            "   } else { \n"
            "      _opacity = opacity; \n"
            "   } \n"
            "   if ((int(_opacity * 255.0) & discardMask) == 0) discard; \n";

        this->StandardPhysical_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\" \n"
            "#include \"LightingCommon\" \n"
            "#include \"PhysicalLightingSingle\"\n"
            "#include \"StandardPhysicalVars\" \n"
            "#include \"ApplySSAO(lightIndex=0)\" \n"
            "void main() {\n"
            "   #include \"StandardPhysicalMain\" \n" 
            "   if (" + DEPTH_OUTPUT_OVERRIDE + " == DEPTH_OUTPUT_DEPTH) {\n"
            "       out_color = vec4(gl_FragCoord.z, 0.0, 0.0, 0.0);\n"
            "   } else if (" + DEPTH_OUTPUT_OVERRIDE + " == DEPTH_OUTPUT_DISTANCE) {\n"
            "       float len = length(vViewPos.xyz);\n"
            "       out_color = vec4(len, 0.0, 0.0, 0.0);\n"
            "   } else { \n"
            "       vec4 finalColor = litColorPhysical0(_albedo, vWorldPos, _normal, " + CAMERA_POSITION + ", _metallic, _roughness, ambientOcclusion);\n"
            "       checkAndApplySSAO0(vClipPos, finalColor); \n"
            "       out_color = vec4(finalColor.rgb, _opacity); \n"
            "   } \n"
            "} \n";

        this->StandardPhysicalMulti_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\" \n"
            "#include \"PhysicalLightingMulti\" \n"
            "#include \"VertexSkinning\" \n"
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
            "out vec4 vClipPos;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vec4 localNormal = " + NORMAL + "; \n"
            "    vec4 localFaceNormal = " + FACE_NORMAL + "; \n"
            "    calculateSkinnedPositionAndNormals(localPos, localNormal, localFaceNormal); \n"
            "    vWorldPos = " +  MODEL_MATRIX + " * localPos;\n"
            "    vec4 viewSpacePos = " + VIEW_MATRIX + " * vWorldPos;\n"
            "    vClipPos = " + PROJECTION_MATRIX + " * viewSpacePos; \n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * vWorldPos;\n"
            "    vAlbedoUV = " + ALBEDO_UV + ";\n"
            "    vNormalUV = " + NORMAL_UV + ";\n"
            "    vColor = " + COLOR + ";\n"
            "    vec4 eNormal = localNormal;\n"
            "    vNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    vec4 eTangent = " + TANGENT + ";\n"
            "    vTangent = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eTangent);\n"
            "    vFaceNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * localFaceNormal);\n"
            "    TRANSFER_LIGHTING(localPos, gl_Position, viewSpacePos) \n"
            "}\n";

        this->StandardPhysicalMulti_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\" \n"
            "#include \"LightingCommon\" \n"
            "#include \"PhysicalLightingHeaderMulti\"\n"
            "#include \"PhysicalLightingMulti(lightIndex=0)\"\n"
            "#include \"PhysicalLightingMulti(lightIndex=1)\"\n"
            "#include \"PhysicalLightingMulti(lightIndex=2)\"\n"
            "#include \"PhysicalLightingMulti(lightIndex=3)\"\n"
            "#include \"StandardPhysicalVars\" \n"
            "#include \"ApplySSAO(lightIndex=0)\" \n"
            "void main() {\n"
            "   #include \"StandardPhysicalMain\" \n"
            "   if (" + DEPTH_OUTPUT_OVERRIDE + " == DEPTH_OUTPUT_DEPTH) {\n"
            "       out_color = vec4(gl_FragCoord.z, 0.0, 0.0, 0.0);\n"
            "   } else if (" + DEPTH_OUTPUT_OVERRIDE + " == DEPTH_OUTPUT_DISTANCE) {\n"
            "       float len = length(vViewPos.xyz);\n"
            "       out_color = vec4(len, 0.0, 0.0, 0.0);\n"
            "   } else { \n"
            "       vec4 curColor = vec4(0.0, 0.0, 0.0, 0.0); \n"
            "       if (" + LIGHT_COUNT + " >= 1 && " + MAX_LIGHTS + " >= 1) curColor += litColorPhysical0(_albedo, vWorldPos, _normal, " + CAMERA_POSITION + ", _metallic, _roughness, ambientOcclusion);\n"
            "       checkAndApplySSAO0(vClipPos, curColor); \n"
            "       if (" + LIGHT_COUNT + " >= 2 && " + MAX_LIGHTS + " >= 2) curColor += litColorPhysical1(_albedo, vWorldPos, _normal, " + CAMERA_POSITION + ", _metallic, _roughness, ambientOcclusion);\n"
            "       if (" + LIGHT_COUNT + " >= 3 && " + MAX_LIGHTS + " >= 3) curColor += litColorPhysical2(_albedo, vWorldPos, _normal, " + CAMERA_POSITION + ", _metallic, _roughness, ambientOcclusion);\n"
            "       if (" + LIGHT_COUNT + " >= 4 && " + MAX_LIGHTS + " >= 4) curColor += litColorPhysical3(_albedo, vWorldPos, _normal, " + CAMERA_POSITION + ", _metallic, _roughness, ambientOcclusion);\n"
            "       out_color = vec4(curColor.rgb, _opacity); \n"
            "   } \n"
            "}\n";

        this->AmbientPhysical_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\" \n"
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

        this->AmbientPhysical_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\" \n"
            "#include \"LightingCommon\" \n"
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
            "    if (" + LIGHT_TYPE + "[0] == AMBIENT_IBL_LIGHT) { \n"
            "       out_color = litColorPhysical0(texture(albedoMap, vAlbedoUV), vWorldPos, _normal, " + CAMERA_POSITION + ", metallic, roughness, ambientOcclusion);\n"
            "   } else { \n"
            "       out_color = litColorBlinnPhong0(texture(albedoMap, vAlbedoUV), vWorldPos, _normal, " + CAMERA_POSITION + ");\n"
            "   } \n"
            "}\n";
        
        this->PhysicalSkybox_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF +
            "out vec4 TexCoord0;\n"
            "void main()\n"
            "{\n"
            "    TexCoord0 = " + POSITION + ";\n"
            "    vec4 vWorldPos = mat4(mat3(" + VIEW_MATRIX + ")) * " + POSITION + ";\n"
            "    gl_Position = (" + PROJECTION_MATRIX + " * vWorldPos).xyww;\n"
            "}\n";

        this->PhysicalSkybox_fragment =   
            "#version 330\n"
            "#include \"Common\" \n"
            "#include \"PhysicalCommon\" \n"
            "precision highp float;\n"
            "uniform samplerCube cubeTexture; \n"
            "uniform float exposure; \n"
            "in vec4 TexCoord0;\n"
            "out vec4 out_color;\n"
            "void main()\n"
            "{\n"
            "    vec3 color =  texture(cubeTexture, TexCoord0.xyz).rgb; \n "
            "    out_color = vec4(color, 1.0);\n"
            "}\n";

        this->Tonemap_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF + 
            "out vec4 localPos;\n"
            "out vec2 vUV; \n"
            "void main()\n"
            "{\n"
            "    localPos = " + POSITION + ";\n"
            "    vUV = localPos.xy / 2.0 + 0.5; \n"
            "    vec4 out_pos = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    gl_Position = out_pos; \n"
            "}\n";

        this->Tonemap_fragment =   
            "#version 330\n"
            "#include \"Common\" \n"
            "#include \"PhysicalCommon\" \n"
            "precision highp float;\n"
            + TEXTURE0_DEF +
            "uniform float exposure; \n"
            "uniform float gamma; \n"
            "uniform int toneMapType; \n"
            "in vec4 localPos;\n"
            "in vec2 vUV; \n"
            "out vec4 out_color;\n"
            "void main()\n"
            "{\n"
            "    vec3 color = texture(" + TEXTURE0 + ", vUV).rgb; \n "
            "    if (toneMapType == TONE_MAP_EXPOSURE) { \n"
            "        color = toneMapExposure(color, exposure); \n"
            "    } \n"
            "    else { \n"
            "        color = toneMapReinhard(color); \n"
            "    } \n"
            "    color = gammaCorrectCustom(color, gamma); \n"
            "    out_color = vec4(color, 1.0);\n"
            "}\n";

        this->IrradianceRenderer_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF + 
            "out vec4 localPos;\n"
            "void main()\n"
            "{\n"
            "    localPos = " + POSITION + ";\n"
            "    vec4 vWorldPos = mat4(mat3(" + VIEW_MATRIX + ")) * " + POSITION + ";\n"
            "    gl_Position = (" + PROJECTION_MATRIX + " * vWorldPos).xyww;\n"
            "}\n";

        this->IrradianceRenderer_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\" \n"
            "out vec4 FragColor;\n"
            "in vec4 localPos;\n"
            "uniform samplerCube cubeTexture;\n"
            "void main()\n"
            "{\n"
            // the sample direction equals the hemisphere's orientation 
            "    vec3 normal = normalize(localPos.xyz);\n"
            "    vec3 irradiance = vec3(0.0);\n"
            "    vec3 up    = vec3(0.0, 1.0, 0.0);\n"
            "    vec3 right    = vec3(1.0, 0.0, 0.0);\n"
            "    float upDot = dot(up, normal); \n"
            "    right = cross(normal, up);\n"
            "    up = cross(right, normal);\n"
  
            "    float sampleDelta = 0.025;\n"
            "    float samplesTaken = 0.0;\n" 
            "    float rangePhi = 2.0 * PI; \n"
            "    float rangeTheta = 0.5 * PI; \n"
            "    float nrSamples = (rangePhi / sampleDelta) * (rangeTheta / sampleDelta);"
            "    float avgScale = 1.0 / nrSamples;"
            "    for(float phi = 0.0; phi < rangePhi; phi += sampleDelta) { \n"
            "        for(float theta = 0.0; theta < rangeTheta; theta += sampleDelta) { \n"
                         // spherical to cartesian (in tangent space)
            "            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));\n"
                         // tangent space to world
            "            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;\n" 
            "            vec3 texColor = textureLod(cubeTexture, sampleVec, 0.0).rgb; \n"
            "            texColor = clamp(texColor, 0.0, 128.0); \n"
            "            irradiance += texColor * cos(theta) * sin(theta);\n"
            "            samplesTaken++;\n"
            "        }\n"
            "    }\n"
            "    irradiance = PI * irradiance * (1.0 / samplesTaken);\n"
            "    FragColor = vec4(irradiance, 1.0);\n"
            "}\n";

        this->SpecularIBLPreFilteredRenderer_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF + 
            "out vec4 localPos;\n"
            "void main()\n"
            "{\n"
            "    localPos = " + POSITION + ";\n"
            "    vec4 vWorldPos = mat4(mat3(" + VIEW_MATRIX + ")) * " + POSITION + ";\n"
            "    gl_Position = (" + PROJECTION_MATRIX + " * vWorldPos).xyww;\n"
            "}\n";

        this->SpecularIBLPreFilteredRenderer_fragment =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\" \n"
            "#include \"LightingCommon\" \n"
            "#include \"PhysicalLightingSingle\" \n"
            "out vec4 out_color; \n"
            "in vec4 localPos; \n"

            "uniform samplerCube cubeTexture; \n"
            "uniform int cubeTextureResolution; \n"
            "uniform float roughness; \n"

            "void main() \n"
            "{  \n"		
            "    vec3 N = normalize(localPos.xyz); \n"    
            "    vec3 R = N; \n"
            "    vec3 V = R; \n"

            "    const uint SAMPLE_COUNT = 1024u; \n"
            "    float totalWeight = 0.0; \n"
            "    vec3 prefilteredColor = vec3(0.0); \n"   

            "    for(uint i = 0u; i < SAMPLE_COUNT; ++i) \n"
            "    {  \n"
            "        vec2 Xi = hammersley(i, SAMPLE_COUNT);  \n"
            "        vec3 H  = importanceSampleGGX(Xi, N, roughness); \n"
            "        vec3 L  = normalize(2.0 * dot(V, H) * H - V); \n"

            "        float NdotH = dot(N, H); \n"
            "        float HdotV = dot(H, V); \n"
            "        float D   = distributionGGX(N, H, roughness); \n"
            "        float pdf = (D * NdotH / (4.0 * HdotV)) + 0.0001; \n"
            "        float saTexel  = 4.0 * PI / (6.0 * cubeTextureResolution * cubeTextureResolution); \n"
            "        float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001); \n"
            "        float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); \n"

            "        float NdotL = max(dot(N, L), 0.0); \n"
            "        if(NdotL > 0.0) \n"
            "        { \n"
            "            vec3 texColor = textureLod(cubeTexture, L, mipLevel).rgb; \n"
            "            texColor = clamp(texColor, 0.0, 16.0) * NdotL ; \n"
            "            prefilteredColor += texColor; \n"
            "            totalWeight      += NdotL; \n"
            "        } \n"
            "    } \n"
            "    prefilteredColor = prefilteredColor / totalWeight; \n"
            "    out_color = vec4(prefilteredColor, 1.0); \n"
            "} \n";

          this->SpecularIBLBRDFRenderer_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "layout (location = 0 ) " + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF + 
            "out vec4 localPos;\n"
            "out vec2 vUV; \n"
            "void main()\n"
            "{\n"
            "    localPos = " + POSITION + ";\n"
            "    vUV = localPos.xy / 2.0 + 0.5; \n"
            "    vec4 vWorldPos = mat4(mat3(" + VIEW_MATRIX + ")) * " + POSITION + ";\n"
            "    gl_Position = (" + PROJECTION_MATRIX + " * vWorldPos).xyww;\n"
            "}\n";

        this->SpecularIBLBRDFRenderer_fragment =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\"\n"
            "#include \"LightingCommon\" \n"
            "#include \"PhysicalLightingSingle\" \n"
            "out vec2 out_color; \n"
            "in vec2 vUV; \n"

            "float geometrySchlickGGXSpecularIBL(float NdotV, float roughness) \n"
            "{ \n"
            "    float a = roughness; \n"
            "    float k = (a * a) / 2.0; \n"
            "    float nom   = NdotV; \n"
            "    float denom = NdotV * (1.0 - k) + k; \n"
            "    return nom / denom; \n"
            "} \n"

            "float geometrySmithSpecularIBL(vec3 N, vec3 V, vec3 L, float roughness) \n"
            "{ \n"
            "    float NdotV = max(dot(N, V), 0.0); \n"
            "    float NdotL = max(dot(N, L), 0.0); \n"
            "    float ggx2 = geometrySchlickGGXSpecularIBL(NdotV, roughness); \n"
            "    float ggx1 = geometrySchlickGGXSpecularIBL(NdotL, roughness); \n"
            "    return ggx1 * ggx2; \n"
            "} \n"

            "vec2 integrateBRDF(float NdotV, float roughness) \n"
            "{ \n"
            "    vec3 V; \n"
            "    V.x = sqrt(1.0 - NdotV*NdotV); \n"
            "    V.y = 0.0; \n"
            "    V.z = NdotV; \n"

            "    float A = 0.0; \n"
            "    float B = 0.0; \n"

            "    vec3 N = vec3(0.0, 0.0, 1.0); \n"

            "    const uint SAMPLE_COUNT = 1024u; \n"
            "    for(uint i = 0u; i < SAMPLE_COUNT; ++i) \n"
            "    { \n"
            "        vec2 Xi = hammersley(i, SAMPLE_COUNT); \n"
            "        vec3 H  = importanceSampleGGX(Xi, N, roughness); \n"
            "        vec3 L  = normalize(2.0 * dot(V, H) * H - V); \n"

            "        float NdotL = max(L.z, 0.0); \n"
            "        float NdotH = max(H.z, 0.0); \n"
            "        float VdotH = max(dot(V, H), 0.0); \n"

            "        if(NdotL > 0.0) \n"
            "        { \n"
            "            float G = geometrySmithSpecularIBL(N, V, L, roughness); \n"
            "            float G_Vis = (G * VdotH) / (NdotH * NdotV); \n"
            "            float Fc = pow(1.0 - VdotH, 5.0); \n"

            "            A += (1.0 - Fc) * G_Vis; \n"
            "            B += Fc * G_Vis; \n"
            "        } \n"
            "    } \n"
            "    A /= float(SAMPLE_COUNT); \n"
            "    B /= float(SAMPLE_COUNT); \n"
            "    return vec2(A, B); \n"
            "} \n"

            "void main()  \n"
            "{ \n"
            "    vec2 integratedBRDF = integrateBRDF(vUV.x, vUV.y); \n"
            "    out_color = integratedBRDF; \n"
            "} \n";

        std::string BONE_TRANSFORM_DEF = 
            "    mat4 boneTransform = " + BONES + "[" + BONE_INDEX + ".x] * " + BONE_WEIGHT + ".x;\n"
            "    boneTransform += " + BONES + "[" + BONE_INDEX + ".y] * " + BONE_WEIGHT + ".y;\n"
            "    boneTransform += " + BONES + "[" + BONE_INDEX + ".z] * " + BONE_WEIGHT + ".z; \n"
            "    boneTransform += " + BONES + "[" + BONE_INDEX + ".w] * " + BONE_WEIGHT + ".w; \n";

        this->VertexSkinning_vertex =  
            SKINNING_ENABLED_DEF
            + BONES_DEF 
            + BONE_WEIGHT_DEF
            + BONE_INDEX_DEF +

            "void calculateSkinnedPositionAndNormals(inout vec4 skinnedPosition, inout vec4 skinnedNormal, inout vec4 skinnedFaceNormal) {\n"
            "    if (" + SKINNING_ENABLED + " == 1) { \n"
            +        BONE_TRANSFORM_DEF +
            "        skinnedPosition = boneTransform * skinnedPosition; \n"
            "        skinnedNormal = boneTransform * skinnedNormal; \n"
            "        skinnedFaceNormal = boneTransform * skinnedFaceNormal; \n"
            "    } \n"
            "}\n"

            "void calculateSkinnedPosition(inout vec4 skinnedPosition) {\n"
            "    if (" + SKINNING_ENABLED + " == 1) { \n"
            +        BONE_TRANSFORM_DEF +
            "        skinnedPosition = boneTransform * skinnedPosition; \n"
            "    } \n"
            "}\n"

            "void calculateSkinnedNormals(inout vec4 skinnedNormal, inout vec4 skinnedFaceNormal) {\n"
            "    if (" + SKINNING_ENABLED + " == 1) { \n"
            +        BONE_TRANSFORM_DEF + 
            "        skinnedNormal = boneTransform * skinnedNormal; \n"
            "        skinnedFaceNormal = boneTransform * skinnedFaceNormal; \n"
            "    }\n"
            "}\n";

        this->VertexSkinning_fragment = "";

        this->Depth_vertex =
            "#version 330\n"
            "precision highp float;\n"
            "#include \"VertexSkinning\" \n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    calculateSkinnedPosition(localPos); \n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
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
            "#include \"VertexSkinning\" \n"
            + POSITION_DEF 
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 vPos;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    calculateSkinnedPosition(localPos); \n"
            "    vPos = " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
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
            "#include \"VertexSkinning\" \n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            " uniform vec4 objectColor;"
            " uniform float zOffset;"
            "out vec4 vColor;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    calculateSkinnedPosition(localPos); \n"
            "    vec4 outPos = " + PROJECTION_MATRIX + "  * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "    outPos.z += zOffset; \n"
            "    gl_Position = outPos; \n"
            "    vColor = objectColor;\n"
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
            "#include \"Common\"\n"
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
            "#include \"Common\"\n"
            "#include \"LightingCommon\" \n"
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
            "   out_color = litColorBlinnPhong0(vec4(vColor.r, vColor.g, vColor.b, 1.0), vPos, normalize(vNormal), " + CAMERA_POSITION + ");\n"
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
            "    vec4 textureColor = texture(twoDtexture, vUV);\n"
            "    out_color = textureColor;\n"
            "}\n";

        this->BasicTexturedFullScreenQuad_vertex =  
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
            "    vUV = " + ALBEDO_UV + "; \n"
            "    vColor = " + COLOR + ";\n"
            "}\n";

        this->BasicTexturedFullScreenQuad_fragment =   
            "#version 330\n"
            "precision mediump float;\n"
            "uniform sampler2D twoDtexture; \n"
            "in vec4 vColor;\n"
            "in vec2 vUV;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    vec4 textureColor = texture(twoDtexture, vUV);\n"
            "    out_color = textureColor;\n"
            "}\n";

        this->BasicTexturedLit_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Common\"\n"
            "#include \"PhysicalLightingSingle\" \n"
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
            "#include \"Common\"\n"
            "#include \"LightingCommon\" \n"
            "#include \"PhysicalLightingSingle\"\n"
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
            "   out_color = litColorBlinnPhong0(texture(albedoMap, vAlbedoUV), vPos, _normal, " + CAMERA_POSITION + ");\n"
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
            "#include \"Common\" \n"
            "#include \"PhysicalCommon\" \n"
            "uniform samplerCube cubeTexture; \n"
            "uniform sampler2D rectTexture; \n"
            "in vec4 vColor;\n"
            "in vec3 vUV;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    vec3 textureColor = texture(cubeTexture, vUV).rgb;\n"
            "    out_color = vec4(textureColor, 1.0);\n"
            "}\n";

        this->Normals_vertex =  
            "#version 330\n"
            "#include \"VertexSkinning\" \n"
            + POSITION_DEF
            + NORMAL_DEF
            + FACE_NORMAL_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF
            + VIEW_INVERSE_TRANSPOSE_MATRIX_DEF +
            "uniform int viewSpace; \n"
            "out vec3 vNormal;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vec4 localNormal = " + NORMAL + "; \n"
            "    vec4 localFaceNormal = " + FACE_NORMAL + "; \n"
            "    calculateSkinnedPositionAndNormals(localPos, localNormal, localFaceNormal); \n"
            "    vec4 eNormal =  localNormal;\n"
            "    if (viewSpace == 1) vNormal = vec3(" + VIEW_INVERSE_TRANSPOSE_MATRIX + " * " + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    else vNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "}\n";

        this->Normals_fragment =  
            "#version 330\n"
            "precision mediump float;\n"
            "#include \"Common\" \n"
            "in vec3 vNormal;\n"
            "layout (location = 0) out vec4 out_normal; \n"
            "void main() {\n"
            "    out_normal = vec4(normalize(vNormal), 1.0);\n"
            "}\n";

        this->Positions_vertex =  
            "#version 330\n"
            "#include \"VertexSkinning\" \n"
            + POSITION_DEF
            + NORMAL_DEF
            + FACE_NORMAL_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "uniform int viewSpace; \n"
            "out vec3 vPosition;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vec4 localNormal = " + NORMAL + "; \n"
            "    vec4 localFaceNormal = " + FACE_NORMAL + "; \n"
            "    calculateSkinnedPositionAndNormals(localPos, localNormal, localFaceNormal); \n"
            "    if (viewSpace == 1) vPosition = vec3(" + VIEW_MATRIX + " * " + MODEL_MATRIX + " * localPos);\n"
            "    else vPosition = vec3(" + MODEL_MATRIX + " * localPos);\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "}\n";

        this->Positions_fragment =  
            "#version 330\n"
            "precision mediump float;\n"
            "#include \"Common\" \n"
            "in vec3 vPosition;\n"
            "layout (location = 0) out vec4 out_position; \n"
            "void main() {\n"
            "    out_position = vec4(vPosition, 1.0);\n"
            "}\n";

        this->PositionsAndNormals_vertex =  
            "#version 330\n"
            "#include \"VertexSkinning\" \n"
            + POSITION_DEF
            + NORMAL_DEF
            + FACE_NORMAL_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF
            + VIEW_INVERSE_TRANSPOSE_MATRIX_DEF +
            "uniform int viewSpace; \n"
            "out vec3 vPosition;\n"
            "out vec3 vNormal;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    vec4 localNormal = " + NORMAL + "; \n"
            "    vec4 localFaceNormal = " + FACE_NORMAL + "; \n"
            "    calculateSkinnedPositionAndNormals(localPos, localNormal, localFaceNormal); \n"
            "    if (viewSpace == 1) vPosition = vec3(" + VIEW_MATRIX + " * " + MODEL_MATRIX + " * localPos);\n"
            "    else vPosition = vec3(" + MODEL_MATRIX + " * localPos);\n"
            "    vec4 eNormal = localNormal;\n"
            "    if (viewSpace == 1) vNormal = vec3(" + VIEW_INVERSE_TRANSPOSE_MATRIX + " * " + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    else vNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "}\n";

        this->PositionsAndNormals_fragment =  
            "#version 330\n"
            "precision mediump float;\n"
            "#include \"Common\" \n"
            "in vec3 vPosition;\n"
            "in vec3 vNormal;\n"
            "layout (location = 0) out vec4 out_position; \n"
            "layout (location = 1) out vec4 out_normal; \n"
            "void main() {\n"
            "    out_position = vec4(vPosition, 1.0);\n"
            "    out_normal = vec4(vNormal, 1.0);\n"
            "}\n";

        this->ApplySSAO_vertex = "";

        this->ApplySSAO_fragment = 
            "void checkAndApplySSAO@lightIndex(in vec4 clipPos, inout vec4 outColor) { \n"
            "   if (" + SSAO_ENABLED + " == 1 && " + LIGHT_TYPE + "[@lightIndex] == AMBIENT_IBL_LIGHT || " + LIGHT_TYPE + "[@lightIndex] == AMBIENT_LIGHT) { \n"
            "      vec2 ndcPos = clipPos.xy / clipPos.w; \n"
            "      vec2 ssaoSampleCoords = vec2(ndcPos.x / 2.0 + 0.5, ndcPos.y / 2.0 + 0.5); \n"
            "      outColor *= texture(" + SSAO_MAP + ", ssaoSampleCoords).r; \n"
            "   } \n"
            "} \n";

        this->SSAO_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + ALBEDO_UV_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec2 vUV;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "    vUV = " + ALBEDO_UV + "; \n"
            "}\n";

        this->SSAO_fragment =  
            "#version 330\n"
            "out float out_color;\n"
            "in vec2 vUV;\n"

            "uniform sampler2D viewPositions;\n"
            "uniform sampler2D viewNormals;\n"
            "uniform sampler2D noise;\n"
            "uniform vec3 samples[" + std::to_string(Constants::SSAOSamples) + "];\n"
            "uniform mat4 projection;\n"
            "uniform float radius; \n"
            "uniform float bias; \n"
            "uniform float screenWidth; \n"
            "uniform float screenHeight; \n"

            "float eRadius = radius; \n"

            // parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
            "const int kernelSize = " + std::to_string(Constants::SSAOSamples) + ";\n"
            //"const float bias = 0.05;\n"

            "void main() {\n"
                // get input for SSAO algorithm
                "vec3 fragPos = texture(viewPositions, vUV).xyz;\n"
                "vec3 normal = normalize(texture(viewNormals, vUV).rgb);\n"
                "vec3 randomVec = normalize(texture(noise, vUV * vec2(screenWidth/4.0, screenHeight/4.0)).xyz);\n"
               // "vec3 randomVec = normalize(noise3(2.0));\n"
                // create TBN change-of-basis matrix: from tangent-space to view-space
                "vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));\n"
                "vec3 bitangent = cross(normal, tangent);\n"
                "mat3 TBN = mat3(tangent, bitangent, normal);\n"
                // iterate over the sample kernel and calculate occlusion factor
                "float occlusion = 0.0;\n"
                "for(int i = 0; i < kernelSize; ++i) \n"
                "{\n"
                    // get sample position
                    "vec3 sample = TBN * samples[i]; \n" // from tangent to view-space
                    "sample = fragPos + sample * eRadius; \n"
                    
                    // project sample position (to sample texture) (to get position on screen/texture)
                    "vec4 offset = vec4(sample, 1.0);\n"
                    "offset = projection * offset;\n" // from view to clip-space
                    "offset.xyz /= offset.w;\n" // perspective divide
                    "offset.xyz = offset.xyz * 0.5 + 0.5;\n" // transform to range 0.0 - 1.0
                    
                    // get sample depth
                    "float sampleDepth = texture(viewPositions, offset.xy).z;\n" // get depth value of kernel sample
                    
                    // range check & accumulate
                    "float rangeCheck = smoothstep(0.0, 1.0, eRadius / abs(fragPos.z - sampleDepth));\n"
                    "occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;\n"
                "}\n"
                "occlusion = 1.0 - (occlusion / kernelSize);\n"
                "out_color = occlusion;\n"
               // "out_color = max(texture(viewNormals, vUV).x, 0.0); //length(texture(viewNormals, vUV).xyz);\n"
            "}\n";

        this->SSAOBlur_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec2 vUV;\n"
            "void main() {\n"
            "    vec4 localPos = " + POSITION + "; \n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * localPos;\n"
            "    vUV = localPos.xy / 2.0 + 0.5; \n"
            "}\n";

        this->SSAOBlur_fragment =  
            "#version 330 core \n"
            "out float out_color; \n"
            "in vec2 vUV; \n"
            "uniform sampler2D ssaoInput; \n"
            "void main()  \n"
            "{ \n"
            "    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0)); \n"
            "    float result = 0.0; \n"
            "    for (int x = -2; x < 2; ++x)  \n"
            "    { \n"
            "        for (int y = -2; y < 2; ++y)  \n"
            "        { \n"
            "            vec2 offset = vec2(float(x), float(y)) * texelSize; \n"
            "            result += texture(ssaoInput, vUV + offset).r; \n"
            "        } \n"
            "    } \n"
            "    out_color = result / (4.0 * 4.0); \n"
            "}   \n";
    }

}