#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "assimp/scene.h"
#include "assimp/Importer.hpp"

#include "../common/types.h"
#include "../common/Exception.h"
#include "../scene/Object3D.h"
#include "../material/Material.h"
#include "../base/BitMask.h"
#include "../image/Texture.h"
#include "../image/ImageLoader.h"

namespace Core {

    // forward declarations
    class Engine;

    class ModelLoader {
        friend class Engine;

    public:

        class ModelLoaderException: public Exception {
        public:
            ModelLoaderException(const std::string& msg): Exception(msg) {}
            ModelLoaderException(const char* msg): Exception(msg) {}
        };

        ~ModelLoader();
        WeakPointer<Object3D> loadModel(const std::string& filePath, Real importScale, Bool preserveFBXPivots);

    private:
        enum class TextureType {
            Diffuse = 0,
            Specular = 1,
            BumpMap = 2,
            _None = 3
        };

        enum class SceneTraverseOrder {
            PreOrder = 0
        };

        class MeshSpecificMaterialDescriptor {
        public:

            Int32 vertexColorsIndex;
            LongMask shaderProperties;
            WeakPointer<Material> material;
            Bool invertVCoords;
            std::map<TextureType, int> uvMapping;

            MeshSpecificMaterialDescriptor() {
                vertexColorsIndex = -1;
                shaderProperties = 0L;
                invertVCoords = false;
            }

            Bool uvMappingHasKey(TextureType key) {
                if (uvMapping.find(key) != uvMapping.end())return true;
                return false;
            }
        };

        class MaterialImportDescriptor {
        public:

            std::map<int, MeshSpecificMaterialDescriptor> meshSpecificProperties;

            Bool usedByMesh(Int32 index) {
                if (meshSpecificProperties.find(index) != meshSpecificProperties.end())return true;
                return false;
            }
        };

        ModelLoader(Engine& engine);

        void initImporter();
        std::shared_ptr<const aiScene> loadAIScene(const std::string& filePath, Bool preserveFBXPivots);
        WeakPointer<Object3D> processModelScene(const std::string& modelPath, const aiScene& scene, Real importScale) const;
        Bool processMaterials(const std::string& modelPath, const aiScene& scene, std::vector<MaterialImportDescriptor>& materialImportDescriptors) const;
        WeakPointer<Texture> loadAITexture(aiMaterial& assimpMaterial, aiTextureType textureType, const std::string& modelPath) const;
        void getImportDetails(const aiMaterial* mtl, MaterialImportDescriptor& materialImportDesc, const aiScene& scene) const;

        Engine& engine;
        std::shared_ptr<Assimp::Importer> importer;
        ImageLoader imageLoader; 
    };

}