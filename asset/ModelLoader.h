#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#ifdef CORE_USE_PRIVATE_INCLUDES
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#endif

#include "../util/WeakPointer.h"
#include "../image/ImageLoader.h"
#include "../image/TextureAttr.h"
#include "../base/BitMask.h"
#include "../common/Exception.h"
#include "../common/types.h"
#include "../material/StandardUniforms.h"
#include "../material/StandardAttributes.h"
#include "../math/Matrix4x4.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Texture;
    class Material;
    class Engine;
    class Mesh;
    class Skeleton;
    class VertexBoneMap;
    class Animation;

    class ModelLoader {
    public:
        class ModelLoaderException : public Exception {
        public:
            ModelLoaderException(const std::string& msg) : Exception(msg) {
            }
            ModelLoaderException(const char* msg) : Exception(msg) {
            }
        };

        ModelLoader();
        virtual ~ModelLoader();
        WeakPointer<Object3D> loadModel(const std::string& filePath, Real importScale, UInt32 smoothingThreshold, 
                                        Bool castShadows, Bool receiveShadows, Bool preserveFBXPivots, Bool preferPhysicalMaterial);
        WeakPointer<Animation> loadAnimation(const std::string& filePath, Bool addLoopPadding, Bool preserveFBXPivots);

    private:

#ifdef CORE_USE_PRIVATE_INCLUDES
        void setTexturesOnMaterial(WeakPointer<Material> material, WeakPointer<Texture> albedoMap, WeakPointer<Texture> normalMap,
                                  WeakPointer<Texture> roughnessGlossMap) const;
        enum class TextureType { Albedo = 0, SpecularMetallic = 1, RoughnessGloss = 2, Normals = 3, _None = 4 };

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

        void initImporter();
        const aiScene* loadAIScene(const std::string& filePath, Bool preserveFBXPivots);

        WeakPointer<Object3D> processModelScene(const std::string& modelPath, const aiScene& scene, Real importScale,  UInt32 smoothingThreshold,
                                                Bool castShadows, Bool receiveShadows, Bool preferPhysicalMaterial) const;
        Bool processMaterials(const std::string& modelPath, const aiScene& scene,
                             std::vector<MaterialImportDescriptor>& materialImportDescriptors, Bool preferPhysicalMaterial) const;
        WeakPointer<Texture> loadAITexture(aiMaterial& assimpMaterial, aiTextureType textureType, const std::string& modelPath, TextureFilter filter, UInt32 mipLevel) const;
        void getImportDetails(const aiMaterial* mtl, MaterialImportDescriptor& materialImportDesc, const aiScene& scene, Bool preferPhysicalMaterial) const;
        Bool setupMeshSpecificMaterialWithTextures(const aiMaterial& assimpMaterial, WeakPointer<Texture> diffuseTexture,
                                                  WeakPointer<Texture> normalsTexture, WeakPointer<Texture> roughnessGlossTexture,
                                                  UInt32 meshIndex, MaterialImportDescriptor& materialImportDesc) const;
        WeakPointer<Object3D> recursiveProcessModelScene(const aiScene& scene, const aiNode& node,
                                                         std::vector<MaterialImportDescriptor>& materialImportDescriptors,
                                                         WeakPointer<Skeleton> skeleton,
                                                         std::vector<WeakPointer<Object3D>>& createdSceneObjects, 
                                                         UInt32 smoothingThreshold, Bool castShadows, Bool receiveShadows) const;
        void mapSkeletonNodeToObject3D(WeakPointer<Skeleton> skeleton, const std::string& nodeName, WeakPointer<Object3D> object3D, const Matrix4x4& mat) const;
        WeakPointer<Mesh> convertAssimpMesh(UInt32 meshIndex, const aiScene& scene, MaterialImportDescriptor& materialImportDescriptor, Bool invert, UInt32 smoothingThreshold) const;
        
        static ModelLoader::TextureType convertAITextureKeyToTextureType(Int32 aiTextureKey);
        static int convertTextureTypeToAITextureKey(TextureType textureType);                      
        static StandardAttribute mapTextureTypeToAttribute(TextureType textureType);

        //void setupVertexBoneMapForRenderer(const aiScene& scene, WeakPointer<Skeleton> skeleton, SkinnedMesh3DRendererSharedPtr target, Bool reverseVertexOrder) const;
        WeakPointer<VertexBoneMap> expandIndexBoneMapping(WeakPointer<VertexBoneMap> indexBoneMap, const aiMesh& mesh, Bool reverseVertexOrder) const;
        WeakPointer<Skeleton> loadSkeleton(const aiScene& scene) const;
        void addMeshBoneMappingsToSkeleton(WeakPointer<Skeleton> skeleton, const aiMesh& mesh, UInt32& currentBoneIndex) const;
        Bool setupVertexBoneMapMappingsFromAIMesh(WeakPointer<const Skeleton> skeleton, const aiMesh& mesh, WeakPointer<VertexBoneMap> vertexIndexBoneMap) const;
        UInt32 countBones(const aiScene& scene) const;
        Bool createAndMapNodeHierarchy(WeakPointer<Skeleton> skeleton, const aiScene& scene) const;
        WeakPointer<Animation> loadAnimation(aiAnimation& animation, Bool addLoopPadding) const;
        void traverseScene(const aiScene& scene, SceneTraverseOrder traverseOrder, std::function<Bool(const aiNode&)> callback) const;
        void preOrderTraverseScene(const aiScene& scene, const aiNode& node, std::function<Bool(const aiNode&)> callback) const;

        static Bool hasOddReflections(Matrix4x4& mat);      
        static void convertAssimpMatrix(const aiMatrix4x4& source, Matrix4x4& dest);                  
#endif

        ImageLoader imageLoader;

    };
}