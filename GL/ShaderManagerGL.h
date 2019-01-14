#pragma once

#include <unordered_map>

#include "../common/types.h"
#include "../common/Exception.h"
#include "../material/ShaderManager.h"

namespace Core {

    class ShaderManagerGL final: public ShaderManager {
    public:

        ~ShaderManagerGL();
        ShaderManagerGL();

        std::string Test_vertex;
        std::string Test_fragment;

        std::string Lighting_vertex;
        std::string Lighting_fragment;

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

        std::string BasicTexturedLit_vertex;
        std::string BasicTexturedLit_fragment;

        std::string BasicCube_vertex;
        std::string BasicCube_fragment;

        void init();

    protected:

        static const std::string BaseString;
    };

}