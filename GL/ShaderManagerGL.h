#pragma once

#include <unordered_map>

#include "../common/gl.h"
#include "../common/types.h"
#include "../common/Exception.h"
#include "../material/ShaderManager.h"

namespace Core {

    class ShaderManagerGL final: public ShaderManager {
    public:

        virtual ~ShaderManagerGL();
        ShaderManagerGL();

        std::string Common_vertex;
        std::string Common_fragment;

        std::string Test_vertex;
        std::string Test_fragment;

        std::string Equirectangular_vertex;
        std::string Equirectangular_fragment;

        std::string Skybox_vertex;
        std::string Skybox_fragment;

        std::string Outline_vertex;
        std::string Outline_geometry;
        std::string Outline_fragment;

        std::string Lighting_Common_vertex;
        std::string Lighting_Common_fragment;

        std::string Lighting_Header_Single_vertex;
        std::string Lighting_Header_Single_fragment;

        std::string Lighting_Header_Multi_vertex;
        std::string Lighting_Header_Multi_fragment;

        std::string Lighting_Single_vertex;
        std::string Lighting_Single_fragment;

        std::string Lighting_Multi_vertex;
        std::string Lighting_Multi_fragment;

        std::string Lighting_vertex;
        std::string Lighting_fragment;

        std::string Lighting_Dir_Cascade_fragment;

        std::string PhysicalCommon_vertex;
        std::string PhysicalCommon_fragment;

        std::string Physical_Lighting_vertex;
        std::string Physical_Lighting_fragment;

        std::string Physical_Lighting_Single_vertex;
        std::string Physical_Lighting_Single_fragment;

        std::string Physical_Lighting_Multi_vertex;
        std::string Physical_Lighting_Multi_Once_fragment;
        std::string Physical_Lighting_Multi_Per_Light_fragment;

        std::string StandardPhysicalVars_fragment;
        std::string StandardPhysicalMain_fragment;
        std::string StandardPhysical_vertex;
        std::string StandardPhysical_fragment;
        std::string StandardPhysicalMulti_vertex;
        std::string StandardPhysicalMulti_fragment;

        std::string AmbientPhysical_vertex;
        std::string AmbientPhysical_fragment;

        std::string PhysicalSkybox_vertex;
        std::string PhysicalSkybox_fragment;

        std::string Tonemap_vertex;
        std::string Tonemap_fragment;

        std::string IrradianceRenderer_vertex;
        std::string IrradianceRenderer_fragment;

        std::string SpecularIBLPreFilteredRenderer_vertex;
        std::string SpecularIBLPreFilteredRenderer_fragment;

        std::string SpecularIBLBRDFRenderer_vertex;
        std::string SpecularIBLBRDFRenderer_fragment;

        std::string VertexSkinning_vertex;
        std::string VertexSkinning_fragment;

        std::string Depth_vertex;
        std::string Depth_fragment;

        std::string Distance_vertex;
        std::string Distance_fragment;

        std::string Basic_vertex;
        std::string Basic_fragment;

        std::string BasicExtrusion_vertex;
        std::string BasicExtrusion_fragment;

        std::string BasicColored_vertex;
        std::string BasicColored_fragment;

        std::string BasicLit_vertex;
        std::string BasicLit_fragment;

        std::string BasicTextured_vertex;
        std::string BasicTextured_fragment;

        std::string BasicTexturedFullScreenQuad_vertex;
        std::string BasicTexturedFullScreenQuad_fragment;

        std::string BasicTexturedLit_vertex;
        std::string BasicTexturedLit_fragment;

        std::string BasicCube_vertex;
        std::string BasicCube_fragment;

        std::string Normals_vertex;
        std::string Normals_fragment;

        std::string Positions_vertex;
        std::string Positions_fragment;

        std::string PositionsAndNormals_vertex;
        std::string PositionsAndNormals_fragment;

        std::string ScreenSpaceAmbientOcclusion_vertex;
        std::string ScreenSpaceAmbientOcclusion_fragment;

        void init();

    protected:

        static const std::string BaseString;
    };

}