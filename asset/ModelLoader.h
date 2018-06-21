#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#ifdef CORE_USE_PRIVATE_INCLUDES
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#endif

#include "../base/BitMask.h"
#include "../common/Exception.h"
#include "../common/types.h"
#include "../image/ImageLoader.h"
#include "../image/Texture.h"
#include "../material/Material.h"
#include "../scene/Object3D.h"
#include "../util/WeakPointer.h"
#include "../material/StandardUniforms.h"

namespace Core {

    // forward declarations
    class Engine;

    class ModelLoader {
        friend class Engine;

    public:
        class ModelLoaderException : public Exception {
        public:
            ModelLoaderException(const std::string& msg) : Exception(msg) {
            }
            ModelLoaderException(const char* msg) : Exception(msg) {
            }
        };

        ~ModelLoader();
        WeakPointer<Object3D> loadModel(const std::string& filePath, Real importScale, Bool castShadows, Bool receiveShadows, Bool preserveFBXPivots);

    private:

#ifdef CORE_USE_PRIVATE_INCLUDES
        enum class TextureType { Diffuse = 0, Specular = 1, BumpMap = 2, _None = 3 };

        enum class SceneTraverseOrder { PreOrder = 0 };

        class MeshSpecificMaterialDescriptor {
        public:
            Int32 vertexColorsIndex;
            LongMask shaderMaterialChacteristics;
            WeakPointer<Material> material;
            Bool invertVCoords;
            std::map<TextureType, int> uvMapping;

            MeshSpecificMaterialDescriptor() {
                vertexColorsIndex = -1;
                shaderMaterialChacteristics = 0L;
                invertVCoords = false;
            }

            Bool uvMappingHasKey(TextureType key) {
                if (uvMapping.find(key) != uvMapping.end()) return true;
                return false;
            }
        };

        class MaterialImportDescriptor {
        public:
            std::map<int, MeshSpecificMaterialDescriptor> meshSpecificProperties;

            Bool usedByMesh(Int32 index) {
                if (meshSpecificProperties.find(index) != meshSpecificProperties.end()) return true;
                return false;
            }
        };

        ModelLoader(Engine& engine);

        void initImporter();
        std::shared_ptr<const aiScene> loadAIScene(const std::string& filePath, Bool preserveFBXPivots);

        WeakPointer<Object3D> processModelScene(const std::string& modelPath, const aiScene& scene, Real importScale, Bool castShadows, Bool receiveShadows) const;
        Bool processMaterials(const std::string& modelPath, const aiScene& scene, std::vector<MaterialImportDescriptor>& materialImportDescriptors) const;
        WeakPointer<Texture> loadAITexture(aiMaterial& assimpMaterial, aiTextureType textureType, const std::string& modelPath) const;
        void getImportDetails(const aiMaterial* mtl, MaterialImportDescriptor& materialImportDesc, const aiScene& scene) const;
        Bool setupMeshSpecificMaterialWithTexture(const aiMaterial& assimpMaterial, TextureType textureType, const WeakPointer<Texture> texture,
                                                  UInt32 meshIndex, MaterialImportDescriptor& materialImportDesc) const;
        void recursiveProcessModelScene(const aiScene& scene, const aiNode& node, Real scale, WeakPointer<Object3D> parent, std::vector<MaterialImportDescriptor>& materialImportDescriptors,
                                        std::vector<WeakPointer<Object3D>>& createdSceneObjects, Bool castShadows, Bool receiveShadows) const;
        WeakPointer<Mesh> convertAssimpMesh(UInt32 meshIndex, const aiScene& scene, MaterialImportDescriptor& materialImportDescriptor, Bool invert) const;
        
        static ModelLoader::TextureType convertAITextureKeyToTextureType(Int32 aiTextureKey);
        static int convertTextureTypeToAITextureKey(TextureType textureType);
        static Bool hasOddReflections(Matrix4x4& mat);                           
        static const std::string* getBuiltinVariableNameForTextureType(TextureType textureType);
        static StandardUniform mapTextureTypeToUniform(TextureType textureType);  
        static StandardAttribute mapTextureTypeToAttribute(TextureType textureType);
        static void convertAssimpMatrix(const aiMatrix4x4& source, Matrix4x4& dest);                  

        Engine& engine;
        std::shared_ptr<Assimp::Importer> importer;
        ImageLoader imageLoader;
#endif

    };
}