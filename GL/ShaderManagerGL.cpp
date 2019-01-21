#include <string>

#include "ShaderManagerGL.h"
#include "../common/Constants.h"

auto _un = Core::StandardUniforms::getUniformName;
auto _an = Core::StandardAttributes::getAttributeName;

const std::string POSITION = _an(Core::StandardAttribute::Position);
const std::string NORMAL = _an(Core::StandardAttribute::Normal);
const std::string AVERAGED_NORMAL = _an(Core::StandardAttribute::AveragedNormal);
const std::string FACE_NORMAL = _an(Core::StandardAttribute::FaceNormal);
const std::string COLOR = _an(Core::StandardAttribute::Color);
const std::string UV0 = _an(Core::StandardAttribute::UV0);
const std::string TEXTURE2D0 = _un(Core::StandardUniform::Texture2D0);
const std::string CUBETEXTURE0 = _un(Core::StandardUniform::CubeTexture0);
const std::string MODEL_MATRIX = _un(Core::StandardUniform::ModelMatrix);
const std::string MODEL_INVERSE_TRANSPOSE_MATRIX = _un(Core::StandardUniform::ModelInverseTransposeMatrix);
const std::string VIEW_MATRIX = _un(Core::StandardUniform::ViewMatrix);
const std::string PROJECTION_MATRIX = _un(Core::StandardUniform::ProjectionMatrix);

const std::string MAX_CASCADES = std::to_string(Core::Constants::MaxDirectionalCascades);
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

const std::string POSITION_DEF = "in vec4 " +  POSITION + ";\n";
const std::string NORMAL_DEF = "in vec4 " +  NORMAL + ";\n";
const std::string AVERAGED_NORMAL_DEF = "in vec4 " +  AVERAGED_NORMAL + ";\n";
const std::string FACE_NORMAL_DEF = "in vec4 " +  FACE_NORMAL + ";\n";
const std::string COLOR_DEF = "in vec4 " +  COLOR + ";\n";
const std::string UV0_DEF = "in vec2 " +  UV0 + ";\n";
const std::string TEXTURE2D0_DEF = "uniform sampler2D " +  TEXTURE2D0 + ";\n";
const std::string CUBETEXTURE0_DEF = "uniform samplerCube " +  CUBETEXTURE0 + ";\n";
const std::string MODEL_MATRIX_DEF = "uniform mat4 " +  MODEL_MATRIX + ";\n";
const std::string MODEL_INVERSE_TRANSPOSE_MATRIX_DEF = "uniform mat4 " +  MODEL_INVERSE_TRANSPOSE_MATRIX + ";\n";
const std::string VIEW_MATRIX_DEF = "uniform mat4 " +  VIEW_MATRIX + ";\n";
const std::string PROJECTION_MATRIX_DEF = "uniform mat4 " +  PROJECTION_MATRIX + ";\n";

const std::string MAX_CASCADES_DEF = "const int MAX_CASCADES =" + MAX_CASCADES + ";\n";
const std::string LIGHT_POSITION_DEF = "uniform vec4 " + LIGHT_POSITION + ";\n";
const std::string LIGHT_DIRECTION_DEF = "uniform vec4 " + LIGHT_DIRECTION + ";\n";
const std::string LIGHT_COLOR_DEF = "uniform vec4 " + LIGHT_COLOR + ";\n";
const std::string LIGHT_INTENSITY_DEF = "uniform float " + LIGHT_INTENSITY + ";\n";
const std::string LIGHT_ATTENUATION_DEF = "uniform float " + LIGHT_ATTENUATION + ";\n";;
const std::string LIGHT_TYPE_DEF = "uniform int " + LIGHT_TYPE + ";\n";
const std::string LIGHT_RANGE_DEF = "uniform float " + LIGHT_RANGE + ";\n";
const std::string LIGHT_ENABLED_DEF = "uniform int " + LIGHT_ENABLED + ";\n";
const std::string LIGHT_MATRIX_DEF = "uniform mat4 " + LIGHT_MATRIX + ";\n";
const std::string LIGHT_VIEW_PROJECTION_DEF = "uniform mat4 " + LIGHT_VIEW_PROJECTION + "[" + MAX_CASCADES + "];\n";

const std::string LIGHT_CASCADE_COUNT_DEF = "uniform int " + LIGHT_CASCADE_COUNT + ";\n";
const std::string LIGHT_SHADOW_MAP_DEF = "uniform sampler2DShadow " + LIGHT_SHADOW_MAP + "[" + MAX_CASCADES + "];\n";
const std::string LIGHT_CASCADE_END_DEF = "uniform float " + LIGHT_CASCADE_END + "[" + MAX_CASCADES + "];\n";
const std::string LIGHT_SHADOW_MAP_ASPECT_DEF = "uniform float " + LIGHT_SHADOW_MAP_ASPECT + "[" + MAX_CASCADES + "];\n";
const std::string LIGHT_SHADOW_CUBE_MAP_DEF = "uniform samplerCube " + LIGHT_SHADOW_CUBE_MAP + ";\n";
const std::string LIGHT_CONSTANT_SHADOW_BIAS_DEF = "uniform float " + LIGHT_CONSTANT_SHADOW_BIAS + ";\n";
const std::string LIGHT_ANGULAR_SHADOW_BIAS_DEF ="uniform float " + LIGHT_ANGULAR_SHADOW_BIAS + ";\n";
const std::string LIGHT_SHADOW_MAP_SIZE_DEF = "uniform float " + LIGHT_SHADOW_MAP_SIZE + ";\n";
const std::string LIGHT_SHADOW_SOFTNESS_DEF = "uniform int " + LIGHT_SHADOW_SOFTNESS + ";\n";
const std::string LIGHT_NEAR_PLANE_DEF = "uniform float " + LIGHT_NEAR_PLANE + ";\n";

namespace Core {

    const std::string ShaderManagerGL::BaseString("");

    ShaderManagerGL::~ShaderManagerGL() {
        
    }

    void ShaderManagerGL::init() {

        this->setShader(ShaderType::Vertex, "Outline", ShaderManagerGL::Outline_vertex);
        this->setShader(ShaderType::Geometry, "Outline", ShaderManagerGL::Outline_geometry);
        this->setShader(ShaderType::Fragment, "Outline", ShaderManagerGL::Outline_fragment);

        this->setShader(ShaderType::Vertex, "Lighting", ShaderManagerGL::Lighting_vertex);
        this->setShader(ShaderType::Fragment, "Lighting", ShaderManagerGL::Lighting_fragment);

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

        this->Outline_vertex =
            "#version 330\n"
            "precision highp float;\n"
            " layout (location = 0 ) " + POSITION_DEF + 
            " layout (location = 1 ) " + NORMAL_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF +
            " uniform vec4 color;"
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
            
            /*#version 400
            layout (location = 0 ) in vec3 VertexPosition;
            layout (location = 1 ) in vec3 VertexNormal;
            out vec3 VNormal;
            out vec3 VPosition;
            uniform mat4 ModelViewMatrix;
            uniform mat3 NormalMatrix;
            uniform mat4 ProjectionMatrix;
            uniform mat4 MVP;
            void main()
            {
            VNormal = normalize( NormalMatrix * VertexNormal);
            VPosition = vec3(ModelViewMatrix *
            vec4(VertexPosition,1.0));
            gl_Position = MVP * vec4(VertexPosition,1.0);
            }
            */

        this->Outline_geometry =
            "#version 330\n"
            "precision highp float;\n"
            "layout( triangles ) in;\n"
            "layout( triangle_strip, max_vertices = 18) out;\n"
            "out vec3 GNormal;\n"
            "out vec3 GPosition;\n"
            "// Which output primitives are silhouette edges\n"
            "flat out int GIsEdge;\n"
            "in vec3 VNormal[]; // Normal in camera coords.\n"
            "in vec3 VPosition[]; // Position in camera coords.\n"
            //"uniform float EdgeWidth; // Width of sil. edge in clip cds.\n"
            //"uniform float PctExtend; // Percentage to extend quad\n"
            "const float EdgeWidth = .005; // Width of sil. edge in clip cds.\n"
            "const float PctExtend = 0.0; // Percentage to extend quad\n"
            "const float AbsExtend = 0.0025; // Percentage to extend quad\n"
            "bool isFrontFacing( vec3 a, vec3 b, vec3 c )\n"
            "{\n"
            "   return ((a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y)) > 0;\n"
            "}\n"
            "void emitEdgeQuad( vec3 e0, vec3 e1, bool isFront, float w0, float w1)\n"
            "{\n"
            "vec2 v = normalize(e1.xy - e0.xy);\n"
            "vec2 extPct = PctExtend * (e1.xy - e0.xy);\n"
            "vec2 extAbs = AbsExtend * v;\n"
            "vec2 ext = extPct + extAbs;\n"
            "vec2 n = -vec2(-v.y, v.x) * EdgeWidth;\n"
            "if (!isFront) { \n"
            "// Emit the quad\n"
            "GIsEdge = 1; // This is part of the sil. edge\n"
            "float epsilon = 0.0001;\n"
            "gl_Position = vec4( e0.xy - ext, e0.z + epsilon, 1.0 ) * w0;\n"
            "EmitVertex();\n"
            "gl_Position = vec4( e0.xy - n - ext, e0.z + epsilon, 1.0 ) * w0;\n"
            "EmitVertex();\n"
            "gl_Position = vec4( e1.xy + ext, e1.z + epsilon, 1.0 ) * w1;\n"
            "EmitVertex();\n"
            "gl_Position = vec4( e1.xy - n + ext, e1.z + epsilon, 1.0 ) * w1;\n"
            "EmitVertex();\n"
            "EndPrimitive();\n"
            "}\n"
            "else { \n"
            "// Emit the quad\n"
            "GIsEdge = 1; // This is part of the sil. edge\n"
            "float epsilon = 0.0001;\n"
            "gl_Position = vec4( e0.xy - ext, e0.z + epsilon, 1.0 ) * w0;\n"
            "EmitVertex();\n"
            "gl_Position = vec4( e0.xy + n - ext, e0.z + epsilon, 1.0 ) * w0;\n"
            "EmitVertex();\n"
            "gl_Position = vec4( e1.xy + ext, e1.z + epsilon, 1.0 ) * w1;\n"
            "EmitVertex();\n"
            "gl_Position = vec4( e1.xy + n + ext, e1.z + epsilon, 1.0 ) * w1;\n"
            "EmitVertex();\n"
            "EndPrimitive();\n"
            "}\n"

            /*"gl_Position = vec4(-1.0, -1.0, 0.0, 1.0 );\n"
            "EmitVertex();\n"
            "gl_Position = vec4(1.0, -1.0, 0.0, 1.0 );\n"
            "EmitVertex();\n"
            "gl_Position = vec4(1.0, 1.0, 0.0, 1.0 );\n"
            "EmitVertex();\n"
            "gl_Position = vec4(-1.0, 1.0, 0.0, 1.0 );\n"
            "EmitVertex();\n"
            "EndPrimitive();\n"*/

            /*"gl_Position = vec4(-1.0, -1.0, 0.0, 1.0 );\n"
            "EmitVertex();\n"
            "gl_Position = vec4(-1.0, 1.0, 0.0, 1.0 );\n"
            "EmitVertex();\n"
            "gl_Position = vec4(1.0, -1.0, 0.0, 1.0 );\n"
            "EmitVertex();\n"
            "gl_Position = vec4(1.0, 1.0, 0.0, 1.0 );\n"
            "EmitVertex();\n"
            "EndPrimitive();\n"*/

            "}\n"
            "void main()\n"
            "{\n"
            "float w0 = gl_in[0].gl_Position.w;\n"
            "float w1 = gl_in[1].gl_Position.w;\n"
            "float w2 = gl_in[2].gl_Position.w;\n"
            "vec3 p0 = gl_in[0].gl_Position.xyz / w0;\n"
            "vec3 p1 = gl_in[1].gl_Position.xyz / w1;\n"
            "vec3 p2 = gl_in[2].gl_Position.xyz / w2;\n"
            //"vec3 p3 = gl_in[3].gl_Position.xyz / gl_in[3].gl_Position.w;\n"
            //"vec3 p4 = gl_in[4].gl_Position.xyz / gl_in[4].gl_Position.w;\n"
            //"vec3 p5 = gl_in[5].gl_Position.xyz / gl_in[5].gl_Position.w;\n"
            /*"if( isFrontFacing(p0, p2, p4) ) {\n"
            "if( ! isFrontFacing(p0,p1,p2) ) emitEdgeQuad(p0,p2);\n"
            "if( ! isFrontFacing(p2,p3,p4) ) emitEdgeQuad(p2,p4);\n"
            "if( ! isFrontFacing(p4,p5,p0) ) emitEdgeQuad(p4,p0);\n"
            "}\n"*/
            //"emitEdgeQuad(p1,p0);\n"
            //"emitEdgeQuad(p2,p1);\n"
            //"emitEdgeQuad(p0,p2);\n"
            "bool isFront = isFrontFacing(p0, p1, p2);\n"
            "emitEdgeQuad(p0, p1, isFront, w0, w1);\n"
            "emitEdgeQuad(p1, p2, isFront, w1, w2);\n"
            "emitEdgeQuad(p2, p0, isFront, w2, w0);\n"
            "// Output the original triangle\n"
            "GIsEdge = 0; // This triangle is not part of an edge.\n"
            "GNormal = VNormal[0];\n"
            "GPosition = VPosition[0];\n"
            "gl_Position = gl_in[0].gl_Position;\n"
            "EmitVertex();\n"
            "GNormal = VNormal[1];\n"
            "GPosition = VPosition[1];\n"
            "gl_Position = gl_in[1].gl_Position;\n"
            "EmitVertex();\n"
            "GNormal = VNormal[2];\n"
            "GPosition = VPosition[2];\n"
            "gl_Position = gl_in[2].gl_Position;\n"
            "EmitVertex();\n"
            "EndPrimitive();\n"
            "}\n";


            /*
            #version 400
            layout( triangles_adjacency ) in;
            layout( triangle_strip, max_vertices = 15 ) out;
            out vec3 GNormal;
            out vec3 GPosition;
            // Which output primitives are silhouette edges
            flat out bool GIsEdge;
            in vec3 VNormal[]; // Normal in camera coords.
            in vec3 VPosition[]; // Position in camera coords.
            uniform float EdgeWidth; // Width of sil. edge in clip cds.
            uniform float PctExtend; // Percentage to extend quad
            bool isFrontFacing( vec3 a, vec3 b, vec3 c )
            {
            return ((a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y)
            + (c.x * a.y - a.x * c.y)) > 0;
            }

            void emitEdgeQuad( vec3 e0, vec3 e1 )
            {
            vec2 ext = PctExtend * (e1.xy - e0.xy);
            vec2 v = normalize(e1.xy - e0.xy);
            vec2 n = vec2(-v.y, v.x) * EdgeWidth;
            // Emit the quad
            GIsEdge = true; // This is part of the sil. edge
            gl_Position = vec4( e0.xy - ext, e0.z, 1.0 );
            EmitVertex();
            gl_Position = vec4( e0.xy - n - ext, e0.z, 1.0 );
            EmitVertex();
            gl_Position = vec4( e1.xy + ext, e1.z, 1.0 );
            EmitVertex();
            gl_Position = vec4( e1.xy - n + ext, e1.z, 1.0 );
            EmitVertex();
            EndPrimitive();
            }
            void main()
            {
            vec3 p0 = gl_in[0].gl_Position.xyz /
            gl_in[0].gl_Position.w;
            vec3 p1 = gl_in[1].gl_Position.xyz /
            gl_in[1].gl_Position.w;
            vec3 p2 = gl_in[2].gl_Position.xyz /
            gl_in[2].gl_Position.w;
            vec3 p3 = gl_in[3].gl_Position.xyz /
            gl_in[3].gl_Position.w;
            vec3 p4 = gl_in[4].gl_Position.xyz /
            gl_in[4].gl_Position.w;
            vec3 p5 = gl_in[5].gl_Position.xyz /
            gl_in[5].gl_Position.w;
            if( isFrontFacing(p0, p2, p4) ) {
            if( ! isFrontFacing(p0,p1,p2) ) emitEdgeQuad(p0,p2);
            if( ! isFrontFacing(p2,p3,p4) ) emitEdgeQuad(p2,p4);
            if( ! isFrontFacing(p4,p5,p0) ) emitEdgeQuad(p4,p0);
            }
            // Output the original triangle
            GIsEdge = false; // This triangle is not part of an edge.
            GNormal = VNormal[0];
            GPosition = VPosition[0];
            gl_Position = gl_in[0].gl_Position;
            EmitVertex();
            GNormal = VNormal[2];
            GPosition = VPosition[2];
            gl_Position = gl_in[2].gl_Position;
            EmitVertex();
            GNormal = VNormal[4];
            GPosition = VPosition[4];
            gl_Position = gl_in[4].gl_Position;
            EmitVertex();
            EndPrimitive();
            }
            */

        this->Outline_fragment = 
            "#version 330\n"
            "precision highp float;\n"
            "in vec4 vColor;\n"
            "uniform vec4 color;"
            "flat in int GIsEdge; // Whether or not we're drawing an edge \n"
            "layout( location = 0 ) out vec4 out_color;\n"
            "void main() {\n"
           // "    out_color = vec4(1.0, 0.0, 0.0, 1.0);\n"
           // otherwise, shade the poly.
            "if( GIsEdge == 1) {\n"
            "out_color = color;\n"
            "} else {\n"
            "out_color = color;\n"
            "}\n"
            "}\n";

        this->Lighting_vertex =
            MAX_CASCADES_DEF
            + LIGHT_CASCADE_COUNT_DEF
            + LIGHT_VIEW_PROJECTION_DEF +
            "out vec4 _core_lightSpacePos[MAX_CASCADES];\n"
            "out float _core_viewSpacePosZ;\n"
            "#define TRANSFER_LIGHTING(localPos, clipSpacePos, viewSpacePos) "
            "for (int i = 0 ; i < " + LIGHT_CASCADE_COUNT + "; i++) { "
            "    _core_lightSpacePos[i] = " + LIGHT_VIEW_PROJECTION + "[i] * " + MODEL_MATRIX + " * (localPos); "
            "} "
            "_core_viewSpacePosZ = (viewSpacePos).z;\n";

        this->Lighting_fragment =
            MAX_CASCADES_DEF
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
            "in float _core_viewSpacePosZ;\n"
            "in vec4 _core_lightSpacePos[MAX_CASCADES];\n"

            "float calDirShadowFactorSingleIndex(int index, vec2 uv, float fragDepth, float angularBias) { \n"
            "    vec3 coords = vec3(uv.xy, fragDepth - angularBias - " + LIGHT_CONSTANT_SHADOW_BIAS + "); \n"
            "    float shadowDepth = clamp(texture(" + LIGHT_SHADOW_MAP + "[index], coords), 0.0, 1.0); \n"
            "    return (1.0-shadowDepth); \n"
            "} \n"

            "float calcDirShadowFactor(int cascadeIndex, vec4 lSpacePos, float angularBias, float baseDot, vec4 fragPos)\n"
            "{ \n"
            "    vec3 projCoords = lSpacePos.xyz / lSpacePos.w; \n"
            "    vec3 uvCoords = (projCoords * 0.5) + vec3(0.5, 0.5, 0.5); \n"
            "    float pxMag = clamp(1.0 / (baseDot + .0001), 1.0, 1.0); \n"
            "    float px = 1.0 / " + LIGHT_SHADOW_MAP_SIZE + " * pxMag; \n"
            "    float py =  " + LIGHT_SHADOW_MAP_ASPECT + "[cascadeIndex] / " + LIGHT_SHADOW_MAP_SIZE + " * pxMag; \n"

            "    float shadowFactor = 0.0; \n"
            "    vec2 uv = uvCoords.xy; \n"
            "    float z = uvCoords.z; \n"

            "    if (uvCoords.x < 0 || uvCoords.x > 1.0) return 0.0; \n"
            "    if (uvCoords.y < 0 || uvCoords.y > 1.0) return 0.0; \n"

            "    if (" + LIGHT_SHADOW_SOFTNESS + " == 2 || " + LIGHT_SHADOW_SOFTNESS + " == 1) { \n"
            "        for (int y = -" + LIGHT_SHADOW_SOFTNESS + " ; y <= " + LIGHT_SHADOW_SOFTNESS + " ; y++) { \n"
            "            for (int x = -" + LIGHT_SHADOW_SOFTNESS + " ; x <= " + LIGHT_SHADOW_SOFTNESS + " ; x++) { \n"
            "                shadowFactor += calDirShadowFactorSingleIndex(cascadeIndex, vec2(uv.x + x * px, uv.y + y * py), z, angularBias); \n"
            "            } \n"
            "        } \n "
            "        if (" + LIGHT_SHADOW_SOFTNESS + " == 2) shadowFactor /= 25.0; \n"
            "        else shadowFactor /= 9.0; \n"
            "    } \n"
            "    else { \n"
            "        shadowFactor += calDirShadowFactorSingleIndex(cascadeIndex, uv, z, angularBias); \n"
            "    } \n"

            "    return shadowFactor; \n"
            "} \n"

            "vec4 getDirLightColor(vec4 baseColor, float bias, float baseDot, vec4 fragPos) { \n"
            "    float shadowFactor = 0.0;\n"
            "    for (int i = 0 ; i < " + LIGHT_CASCADE_COUNT + " ; i++) { \n"
            "        if (_core_viewSpacePosZ <= " + LIGHT_CASCADE_END + "[i]) { \n"
            "            shadowFactor = calcDirShadowFactor(i, _core_lightSpacePos[i], bias, baseDot, fragPos); \n"
            "            break; \n"
            "        } \n"
            "    } \n"
            "    return vec4((1.0 - shadowFactor) * " + LIGHT_COLOR + ".rgb * baseColor.rgb * baseDot, baseColor.a);\n"      
            "} \n"

            "float getPointLightShadowFactor(vec3 lightLocalFragPos, float bias) { \n"
            "   vec4 shadowDepthVec = texture(" + LIGHT_SHADOW_CUBE_MAP + ", lightLocalFragPos);\n"
            "   float shadowDepth = shadowDepthVec.r;\n"
            "   float shadowFactor = 0.0; \n"
            "   if (shadowDepth + bias > length(lightLocalFragPos) || shadowDepth < .001) {\n"
            "       shadowFactor = 1.0;\n"
            "   }\n"
            "   return shadowFactor;\n"
            "} \n"

            "vec4 getPointLightColor(vec4 baseColor, vec3 lightLocalFragPos, float bias, float atten) { \n"
            "    float pxToWorld = 1.0 / " + LIGHT_SHADOW_MAP_SIZE + " * 0.2; \n"
            "    float near = " + LIGHT_NEAR_PLANE + "; \n"

            "    vec3 absVec = abs(lightLocalFragPos); \n"
            "    float maxComponent = max(max(absVec.x, absVec.y), absVec.z); \n"
            "    vec3 normAbs = absVec / maxComponent; \n"

            "    float aFactor = step(absVec.x, absVec.z) * step(absVec.y, absVec.z); \n"
            "    float bFactor = step(absVec.z, absVec.x) * step(absVec.y, absVec.x); \n"
            "    bFactor = clamp(bFactor - aFactor, 0.0, 1.0); \n"
            "    float cFactor = clamp(1.0 - (aFactor + bFactor), 0.0, 1.0); \n"
            "    vec3 mask = vec3(0.0, 0.0, 1.0) * aFactor +  vec3(1.0, 0.0, 0.0) * bFactor + vec3(0.0, 1.0, 0.0) *  cFactor; \n"

            "    vec3 inverseMask = vec3(1.0, 1.0, 1.0) - mask; \n"
            "    vec3 signVec = sign(lightLocalFragPos); \n"
            "    vec3 forward = signVec * mask; \n"
            "    float scale = maxComponent / near; \n"
            "    vec3 up = vec3(0.0, 1.0, 0.0) * inverseMask + vec3(0.0, 0.0, forward.y) * inverseMask; \n"
            "    vec3 right = cross(forward, up); \n"
            "    right = right * pxToWorld * scale; \n"
            "    up = up * pxToWorld * scale; \n"
            
            "    float shadowFactor = 0.0; \n"
            "    if (" + LIGHT_SHADOW_SOFTNESS + " == 2 || " + LIGHT_SHADOW_SOFTNESS + " == 1) { \n"
            "        shadowFactor += getPointLightShadowFactor(lightLocalFragPos, bias); \n"
            "        for (int i = 1; i <= " + LIGHT_SHADOW_SOFTNESS + "; i++) { \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos + right * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos - right * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos - up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos + right * i + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos + right * i - up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos - right * i + up * i, bias); \n"
            "            shadowFactor += getPointLightShadowFactor(lightLocalFragPos - right * i - up * i, bias); \n"
            "        } \n "
            "        if (" + LIGHT_SHADOW_SOFTNESS + " == 2) shadowFactor /= 17.0; \n"
            "        else shadowFactor /= 9.0; \n"
            "    } \n"
            "    else { \n"
            "        shadowFactor += getPointLightShadowFactor(lightLocalFragPos, bias); \n"
            "    } \n"       
           
            "   return vec4(" + LIGHT_COLOR + ".rgb * baseColor.rgb * atten * shadowFactor, baseColor.a);\n"
            "}\n"

            "vec4 litColor(in vec4 baseColor, in vec4 fragPos, in vec3 fragNormal) {\n"
            "    if (" + LIGHT_ENABLED + " != 0) {\n"
            "        if (" + LIGHT_TYPE + " == 0) {\n"
            "            return vec4(baseColor.rgb * " + LIGHT_COLOR + ".rgb, baseColor.a);\n"
            "        }\n"
            "        else if (" + LIGHT_TYPE + " == 1) {\n"
            "            vec3 toLight = vec3(-" + LIGHT_DIRECTION + ");\n"
            "            float baseDot = max(cos(acos(dot(toLight, fragNormal)) * 1.1), 0.0); \n"  
            "            float bias = (1.0 - baseDot) * " + LIGHT_ANGULAR_SHADOW_BIAS + ";"
            "            return getDirLightColor(baseColor, bias, baseDot, fragPos); \n"
            "        }\n"
            "        else if (" + LIGHT_TYPE + " == 2) {\n"
            "            vec3 lightLocalFragPos = vec3(" + LIGHT_MATRIX + " * fragPos);\n"
            "            vec3 toLight = normalize(vec3(" + LIGHT_POSITION + " - fragPos));\n"
            "            float baseDot = max(cos(acos(dot(toLight, fragNormal)) * 1.025), 0.0); \n"
            "            float bias = (1.0 - baseDot) * " + LIGHT_ANGULAR_SHADOW_BIAS + " + " + LIGHT_CONSTANT_SHADOW_BIAS + ";\n"
            "            return getPointLightColor(baseColor, lightLocalFragPos, bias, baseDot); \n"
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
            "#include \"Lighting\" \n"
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
            "#include \"Lighting\"\n"
            "in vec4 vColor;\n"
            "in vec3 vNormal;\n"
            "in vec4 vPos;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            // instead of passing [vColor] directly to the litColor function, we copy it into a new
            // vector. passing it directly causes a weird bug on some platforms/GPUs/versions of Linux
            // where shadows are not rendered
            "   out_color = litColor(vec4(vColor.r, vColor.g, vColor.b, 1.0), vPos, normalize(vNormal));\n"
            "}\n";

        this->BasicTextured_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + COLOR_DEF 
            + UV0_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF +
            "out vec4 vColor;\n"
            "out vec3 vNormal;\n"
            "out vec2 vUV;\n"
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " +  MODEL_MATRIX + " * " + POSITION + ";\n"
            "    vUV = " + UV0 + ";\n"
            "    vColor = " + COLOR + ";\n"
            "}\n";

        this->BasicTextured_fragment =   
            "#version 330\n"
            "precision mediump float;\n"
            + TEXTURE2D0_DEF + 
            "in vec4 vColor;\n"
            "in vec2 vUV;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    vec4 textureColor = texture2D(" + TEXTURE2D0 + ", vUV);\n"
            "    out_color = textureColor;\n"
            "}\n";

        this->BasicTexturedLit_vertex =  
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Lighting\" \n"
            + POSITION_DEF
            + COLOR_DEF
            + NORMAL_DEF
            + FACE_NORMAL_DEF
            + UV0_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF
            + MODEL_MATRIX_DEF
            + MODEL_INVERSE_TRANSPOSE_MATRIX_DEF + 
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
            "    vUV = " + UV0 + ";\n"
            "    vColor = " + COLOR + ";\n"
            "    vec4 eNormal = " + NORMAL + ";\n"
            "    vNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * eNormal);\n"
            "    vFaceNormal = vec3(" + MODEL_INVERSE_TRANSPOSE_MATRIX + " * " + FACE_NORMAL + ");\n"
            "    TRANSFER_LIGHTING(" + POSITION + ", gl_Position, viewSpacePos) \n"
            "}\n";

        this->BasicTexturedLit_fragment =   
            "#version 330\n"
            "precision highp float;\n"
            "#include \"Lighting\"\n"
            + TEXTURE2D0_DEF +
            "in vec4 vColor;\n"
            "in vec3 vNormal;\n"
            "in vec3 vFaceNormal;\n"
            "in vec2 vUV;\n"
            "in vec4 vPos;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "   out_color = litColor(texture(" + TEXTURE2D0 + ", vUV), vPos, normalize(vNormal));\n"
            "}\n";

        this->BasicCube_vertex =  
            "#version 330\n"
            + POSITION_DEF
            + COLOR_DEF
            + PROJECTION_MATRIX_DEF
            + VIEW_MATRIX_DEF +
            "mat4 scale = mat4(1.0, 0.0, 0.0, 0.0,\n"
            "                  0.0, 1.0, 0.0, 0.0,\n"
            "                  0.0, 0.0, 1.0, -20.0,\n"
            "                  0.0, 0.0, 0.0, 1.0);\n"
            "out vec4 vColor;\n"
            "out vec3 vUV;\n"
            "void main() {\n"
            "    gl_Position = " + PROJECTION_MATRIX + " * " + VIEW_MATRIX + " * " + POSITION + ";\n"
            "    vUV = normalize(" + POSITION + ".xyz);\n"
            "    vColor = " + COLOR + ";\n"
            "}\n";

        this->BasicCube_fragment =  
            "#version 330\n"
            "precision mediump float;\n"
            + CUBETEXTURE0_DEF +
            "in vec4 vColor;\n"
            "in vec3 vUV;\n"
            "out vec4 out_color;\n"
            "void main() {\n"
            "    vec4 textureColor = texture(" + CUBETEXTURE0 + ", vUV);\n"
            "    out_color = textureColor;\n"
            "}\n";
    }

}