#pragma once

#include <unordered_map>

#include "../common/types.h"
#include "../common/Exception.h"
#include "../material/ShaderManager.h"

namespace Core {

    class ShaderManagerGL final: public ShaderManager {
    public:

        ~ShaderManagerGL();

        static const char Test_vertex[];
        static const char Test_fragment[];

        static const char Lighting_fragment[];

        static const char Depth_vertex[];
        static const char Depth_fragment[];

        static const char Distance_vertex[];
        static const char Distance_fragment[];

        static const char Basic_vertex[];
        static const char Basic_fragment[];

        static const char BasicLit_vertex[];
        static const char BasicLit_fragment[];

        static const char BasicTextured_vertex[];
        static const char BasicTextured_fragment[];

        static const char BasicTexturedLit_vertex[];
        static const char BasicTexturedLit_fragment[];

        static const char BasicCube_vertex[];
        static const char BasicCube_fragment[];

        void init();

    protected:

    };

}