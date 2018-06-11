#pragma once 

namespace Core {

    enum class ShaderMaterialCharacteristic {
        DiffuseColored = 0,
        SpecularColored = 1,
        DiffuseTextured = 2,
        SpecularTextured = 3,
        Bumped = 4,
        EmissiveColored = 5,
        EmissiveTextured = 6,
        VertexColors = 7,
        VertexNormals = 8
    };

}
