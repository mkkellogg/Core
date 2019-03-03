#pragma once 

namespace Core {

    enum class ShaderMaterialCharacteristic {
        AlbedoSolid = 0,
        SpecularColored = 1,
        AlbedoMapped = 2,
        SpecularTextured = 3,
        CubeTextured = 4,
        Bumped = 5,
        EmissiveColored = 6,
        EmissiveTextured = 7,
        VertexColors = 8,
        VertexNormals = 9,
        Lit = 10,
        NormalMapped = 11
    };

}
