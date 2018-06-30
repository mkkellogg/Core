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

        static const char Basic_vertex[];
        static const char Basic_fragment[];

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