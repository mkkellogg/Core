#pragma once 

namespace Core {

    enum class ShaderMaterialCharacteristic {
        DiffuseColored = 0,
        SpecularColored = 1,
        DiffuseTextured = 2,
        SpecularTextured = 3,
        CubeTextured = 4,
        Bumped = 5,
        EmissiveColored = 6,
        EmissiveTextured = 7,
        VertexColors = 8,
        VertexNormals = 9
    };

}
