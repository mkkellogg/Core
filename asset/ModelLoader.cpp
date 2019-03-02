#include <bitset>
#include <fstream>
#include <queue>

#include "assimp/DefaultLogger.hpp"
#include "assimp/Importer.hpp"
#include "assimp/LogStream.hpp"
#include "assimp/cimport.h"
#include "assimp/config.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "../Engine.h"
#include "../filesys/FileSystem.h"
#include "../scene/Object3D.h"
#include "../image/Texture.h"
#include "../image/TextureAttr.h"
#include "../image/Texture2D.h"
#include "../material/Material.h"
#include "../material/BasicTexturedMaterial.h"
#include "../material/BasicTexturedLitMaterial.h"
#include "../material/StandardPhysicalMaterial.h"
#include "../material/BasicMaterial.h"
#include "../material/MaterialLibrary.h"
#include "../material/ShaderMaterialCharacteristic.h"
#include "../render/MeshRenderer.h"
#include "../render/RenderableContainer.h"
#include "ModelLoader.h"

namespace Core {
    static std::shared_ptr<Assimp::Importer> importer = nullptr;

    ModelLoader::ModelLoader() {
    }

    ModelLoader::~ModelLoader() {
    }

    void ModelLoader::initImporter() {
        if (!importer) {
            importer = std::make_shared<Assimp::Importer>();
        }

        if (!importer) {
            throw ModelLoaderException("ModeLoader::initImporter -> Unable to create Assimp::Importer.");
        }
    }

    /**
     * Load an Assimp compatible model/scene located at [filePath]. [filePath] Must be a native file-system
     * compatible path, so the the engine's FileSystem singleton should be used to derive the correct platform-specific
     * path before calling this method.
     */
    const aiScene* ModelLoader::loadAIScene(const std::string& filePath, Bool preserveFBXPivots) {
        // the global Assimp scene object
        const aiScene* scene = nullptr;

        // Create an instance of the Assimp Importer class
        this->initImporter();

        // Check if model file exists
        std::ifstream fin(filePath.c_str());
        if (!fin.fail()) {
            fin.close();
        } else {
            std::string msg = std::string("ModeLoader -> Could not find file: ") + filePath;
            throw ModelLoaderException(msg);
        }

        // tell Assimp not to create extra nodes when importing FBX files
        importer->SetPropertyInteger(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, preserveFBXPivots ? 1 : 0);

        // read the model file in from disk
        scene = importer->ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality);

        // If the import failed, report it
        if (!scene) {
            std::string msg = std::string("ModeLoader::loadAIScene -> Could not import file: ") + std::string(importer->GetErrorString());
            throw ModelLoaderException(msg);
        }

        return scene;
    }

    /**
     * Load an Assimp compatible model/scene located at [modelPath]. [modelPath] Must be a native file-system
     * compatible path, so the the engine's FileSystem singleton should be used to derive the correct platform-specific
     * path before calling this method.
     *
     * [importScale] - Allows for the adjustment of the model's scale
     */
    WeakPointer<Object3D> ModelLoader::loadModel(const std::string& modelPath, Real importScale, UInt32 smoothingThreshold, 
                                                 Bool castShadows, Bool receiveShadows, Bool preserveFBXPivots) {
        std::shared_ptr<FileSystem> fileSystem = FileSystem::getInstance();
        std::string fixedModelPath = fileSystem->fixupPathForLocalFilesystem(modelPath);

        // the global Assimp scene object
        const aiScene* scene = this->loadAIScene(fixedModelPath, preserveFBXPivots);

        if (scene) {
            // the model has been loaded from disk into Assimp data structures, now convert to engine-native structures
            WeakPointer<Object3D> result = processModelScene(fixedModelPath, *scene, importScale, smoothingThreshold, castShadows, receiveShadows);
            result->setActive(true);
            return result;
        } else {
            throw ModelLoaderException("ModelLoder::loadModel() -> Error occured while loading Assimp scene.");
        }
    }

    WeakPointer<Object3D> ModelLoader::processModelScene(const std::string& modelPath, const aiScene& scene, Real importScale,
                                                         UInt32 smoothingThreshold, Bool castShadows, Bool receiveShadows) const {
        // container for MaterialImportDescriptor instances that describe the engine-native
        // materials that get created during the call to ProcessMaterials()
        std::vector<MaterialImportDescriptor> materialImportDescriptors;

        // verify that we have a valid scene
        if (scene.mRootNode == nullptr) throw new ModelLoaderException("ModelLoader::processModelScene -> Assimp scene root is null.");
        
        std::shared_ptr<FileSystem> fileSystem = FileSystem::getInstance();
        std::string fixedModelPath = fileSystem->fixupPathForLocalFilesystem(modelPath);

        // process all the Assimp materials in [scene] and create equivalent engine native materials.
        // store those materials and their properties in MaterialImportDescriptor instances, which get
        // added to [materialImportDescriptors]
        Bool processMaterialsSuccess = this->processMaterials(fixedModelPath, scene, materialImportDescriptors);
        if (!processMaterialsSuccess) {
            throw ModelLoaderException("ModelLoader::processModelScene -> processMaterials() returned an error.");
        }

        // container for all the SceneObject instances that get created during this process
        std::vector<WeakPointer<Object3D>> createdSceneObjects;

        // recursively move down the Assimp scene hierarchy and process each node one by one.
        // all instances of SceneObject that are generated get stored in [createdSceneObjects].
        // any time meshes or mesh renderers are created, the information in [materialImportDescriptors]
        // will be used to link their materials and textures as appropriate.

        WeakPointer <Object3D> root = recursiveProcessModelScene(scene, *(scene.mRootNode), materialImportDescriptors, createdSceneObjects,
                                                                 smoothingThreshold, castShadows, receiveShadows);
        root->getTransform().getLocalMatrix().scale(importScale, importScale, importScale);

        // deactivate the root scene object so that it is not immediately
        // active or visible in the scene after it has been loaded
        root->setActive(false);

        // loop through each instance of SceneObject that was created in the call to RecursiveProcessModelScene()
        // and for each instance that contains a SkinnedMesh3DRenderer instance, clone the Skeleton instance
        // created earlier in the call to LoadSkeleton() and assign the cloned skeleton to that renderer.
        for (UInt32 s = 0; s < createdSceneObjects.size(); s++) {
            // does the SceneObject instance have a SkinnedMesh3DRenderer ?

            /*  SkinnedMesh3DRendererSharedPtr skinnedRenderer = GTE::DynamicCastEngineObject<GTE::Renderer,
              GTE::SkinnedMesh3DRenderer>(createdSceneObjects[s]->GetRenderer()); if (skinnedRenderer.IsValid()) {
                  // clone [skeleton]
                  SkeletonSharedPtr skeletonClone = objectManager->CloneSkeleton(skeleton);
                  if (!skeletonClone.IsValid()) {
                      Debug::PrintWarning("ModelImporter::ProcessModelScene -> Could not clone scene skeleton.");
                      continue;
                  }

                  // assign the clones skeleton to [renderer]
                  skinnedRenderer->SetSkeleton(skeletonClone);

                  Matrix4x4 mat;
                  createdSceneObjects[s]->GetTransform().CopyMatrix(mat);

                  // if the transformation matrix for this scene object has an inverted scale, we need to process the
                  // vertex bone map in reverse order. we pass the [reverseVertexOrder] flag to SetupVertexBoneMapForRenderer()
                  Bool reverseVertexOrder = HasOddReflections(mat);
                  SetupVertexBoneMapForRenderer(scene, skeletonClone, skinnedRenderer, reverseVertexOrder);
              }*/
        }

        return root;
    }

    WeakPointer<Object3D> ModelLoader::recursiveProcessModelScene(const aiScene& scene, const aiNode& node,
                                                                  std::vector<MaterialImportDescriptor>& materialImportDescriptors,
                                                                  std::vector<WeakPointer<Object3D>>& createdSceneObjects, 
                                                                  UInt32 smoothingThreshold, Bool castShadows, Bool receiveShadows) const {
        WeakPointer<Object3D> nodeObject;
        nodeObject = Engine::instance()->createObject3D();
        if (WeakPointer<Object3D>::isInvalid(nodeObject)) throw ModelLoaderException("ModelLoader::recursiveProcessModelScene -> Could not create scene object.");
        nodeObject->setName(node.mName.C_Str());

        Matrix4x4 mat;
        aiMatrix4x4 matBaseTransformation = node.mTransformation;
        ModelLoader::convertAssimpMatrix(matBaseTransformation, mat);
        nodeObject->getTransform().getLocalMatrix().copy(mat);

        std::queue<WeakPointer<Mesh>> tempMeshes;
        std::queue<std::string> tempMeshNames;
        WeakPointer<Material> lastMaterial;
        // are there any meshes in the model/scene?
        if (node.mNumMeshes > 0) {
            // loop through each Assimp mesh attached to the current Assimp node and
            // create a Mesh instance for it
            for (UInt32 n = 0; n <= node.mNumMeshes; n++) {
                WeakPointer<Material> material;
                if (n < node.mNumMeshes) {
                    // get the index of the sub-mesh in the master list of meshes
                    UInt32 sceneMeshIndex = node.mMeshes[n];

                    // get a pointer to the Assimp mesh
                    const aiMesh* mesh = scene.mMeshes[sceneMeshIndex];
                    if (mesh == nullptr) {
                        throw ModelLoaderException("ModelLoader::recursiveProcessModelScene -> Assimp node mesh is null.");
                    }

                    Int32 materialIndex = mesh->mMaterialIndex;
                    MaterialImportDescriptor& materialImportDescriptor = materialImportDescriptors[materialIndex];
                    material = materialImportDescriptor.meshSpecificProperties[sceneMeshIndex].material;
                    if (!material.isValid()) {
                        throw ModelLoaderException("ModelLoader::recursiveProcessModelScene -> nullptr Material object encountered.");
                    }

                    // if the transformation matrix for this node has an inverted scale, we need to process the mesh
                    // differently or else it won't display correctly. we pass the [invert] flag to convertAssimpMesh()
                    Bool invert = ModelLoader::hasOddReflections(mat);

                    // convert Assimp mesh to a Mesh object
                    WeakPointer<Mesh> subMesh = this->convertAssimpMesh(sceneMeshIndex, scene, materialImportDescriptor, invert, smoothingThreshold);
                    tempMeshes.push(subMesh);
                    std::string meshName(mesh->mName.C_Str());
                    if (meshName.size() == 0) {
                        meshName = std::string("Mesh") + std::to_string(n);
                    }
                    tempMeshNames.push(meshName);
                }

                UInt32 newChildrenCount = 0;
                if (n == node.mNumMeshes || (n > 0 && material.get() != lastMaterial.get())) {
                    // create new scene object to hold the meshes object and its renderer
                    WeakPointer<RenderableContainer<Mesh>> meshContainer = Engine::instance()->createObject3D<RenderableContainer<Mesh>>();
                    if (!meshContainer.isValid()) {
                        throw ModelLoaderException("ModelLoader::recursiveProcessModelScene -> Could not create mesh container.");
                    };
                
                    std::string objName;
                    unsigned int targetRemainingCount = n == node.mNumMeshes ? 0 : 1;
                    while (tempMeshes.size() > targetRemainingCount) {
                        WeakPointer<Mesh> subMesh = tempMeshes.front();
                        tempMeshes.pop();
                        objName = tempMeshNames.front();
                        tempMeshNames.pop();
                        meshContainer->addRenderable(subMesh);
                    }
                    meshContainer->setName(objName);

                    WeakPointer<MeshRenderer> meshRenderer = Engine::instance()->createRenderer<MeshRenderer>(lastMaterial, meshContainer);
                    nodeObject->addChild(meshContainer);

                    createdSceneObjects.push_back(meshContainer);

                    newChildrenCount++;
                }

                lastMaterial = material;
            }

            /*Mesh3DFilterSharedPtr filter = engineObjectManager->CreateMesh3DFilter();
            NONFATAL_ASSERT(filter.IsValid(), "ModelImporter::RecursiveProcessModelScene -> Unable to create mesh#D filter object.", false);

            // set shadow properties
            filter->SetCastShadows(castShadows);
            filter->SetReceiveShadows(receiveShadows);
            filter->SetMesh3D(mesh3D);
            sceneObject->SetMesh3DFilter(filter);

            sceneObject->SetRenderer(GTE::DynamicCastEngineObject<GTE::Mesh3DRenderer, GTE::Renderer>(meshRenderer));*/
        }

        for (UInt32 i = 0; i < node.mNumChildren; i++) {
            const aiNode* childNode = node.mChildren[i];
            if (childNode != nullptr) {
                WeakPointer<Object3D> childObject = this->recursiveProcessModelScene(scene, *childNode, materialImportDescriptors, createdSceneObjects, 
                                                                                     smoothingThreshold, castShadows, receiveShadows);
                nodeObject->addChild(childObject);
            }
        }

        return nodeObject;
    }

    /**
     * Convert an Assimp mesh to an engine-native SubMesh3D instance.
     *
     * [meshIndex] - The index of the target Assimp mesh in the scene's list of meshes
     * [scene] - The Assimp scene/model.
     * [materialImportDescriptor] - Descriptor for the mesh's material.
     * [invert] - If true it means the mesh has an inverted scale transformation to deal with
     */
    WeakPointer<Mesh> ModelLoader::convertAssimpMesh(UInt32 meshIndex, const aiScene& scene, MaterialImportDescriptor& materialImportDescriptor,
                                                     Bool invert, UInt32 smoothingThreshold) const {
        if (meshIndex >= scene.mNumMeshes) {
            throw ModelLoaderException("ModelLoader::convertAssimpMesh -> mesh index is out of range.");
        }

        aiMesh& mesh = *scene.mMeshes[meshIndex];

        UInt32 vertexCount = mesh.mNumFaces * 3;

        // create a set of standard attributes that will dictate the standard attributes
        // to be used by the function object created by this function.
        StandardAttributeSet meshAttributes = StandardAttributes::createAttributeSet();

        // all meshes must have vertex positions
        StandardAttributes::addAttribute(&meshAttributes, StandardAttribute::Position);

        Int32 diffuseTextureUVIndex = -1;
        // update the StandardAttributeSet to contain appropriate attributes (UV coords) for a diffuse texture
        if (materialImportDescriptor.meshSpecificProperties[meshIndex].uvMappingHasKey(TextureType::Diffuse)) {
            StandardAttributes::addAttribute(&meshAttributes, ModelLoader::mapTextureTypeToAttribute(TextureType::Diffuse));
            diffuseTextureUVIndex = materialImportDescriptor.meshSpecificProperties[meshIndex].uvMapping[TextureType::Diffuse];
        }

        Int32 normalsTextureUVIndex = -1;
        // update the StandardAttributeSet to contain appropriate attributes (UV coords) for a normals texture
        if (materialImportDescriptor.meshSpecificProperties[meshIndex].uvMappingHasKey(TextureType::Normals)) {
            StandardAttributes::addAttribute(&meshAttributes, ModelLoader::mapTextureTypeToAttribute(TextureType::Normals));
            normalsTextureUVIndex = materialImportDescriptor.meshSpecificProperties[meshIndex].uvMapping[TextureType::Normals];
        }

        // add normals & tangents regardless of whether the mesh has them or not. if the mesh does not
        // have them, they can be calculated
        StandardAttributes::addAttribute(&meshAttributes, StandardAttribute::Normal);
        StandardAttributes::addAttribute(&meshAttributes, StandardAttribute::Tangent);
        StandardAttributes::addAttribute(&meshAttributes, StandardAttribute::FaceNormal);

        // if the Assimp mesh's material specifies vertex colors, add vertex colors
        // to the StandardAttributeSet
        if (materialImportDescriptor.meshSpecificProperties[meshIndex].vertexColorsIndex >= 0) {
            StandardAttributes::addAttribute(&meshAttributes, StandardAttribute::Color);
        }

        // create Mesh3D object with the constructed StandardAttributeSet
        WeakPointer<Mesh> coreMesh = Engine::instance()->createMesh(vertexCount, 0);
        if (!coreMesh.isValid()) {
            throw ModelLoaderException("ModeLoader::convertAssimpMesh -> Could not create Mesh3D object.");
        }

        Bool hasNormals = false;
        Bool hasColors = false;
        Bool hasAlbedoUVs = false;
        Bool hasNormalUVs = false;

        std::vector<Real> positions;
        if (!coreMesh->initVertexPositions()) {
            throw ModelLoaderException("ModeLoader::convertAssimpMesh -> Unable to initialize vertex positions.");
        }
        positions.reserve(mesh.mNumFaces * 12);
        coreMesh->enableAttribute(StandardAttribute::Position);

        std::vector<Real> normals;
        std::vector<Real> faceNormals;
        normals.reserve(mesh.mNumFaces * 12);
        faceNormals.reserve(mesh.mNumFaces * 12);
        if (!coreMesh->initVertexNormals()) {
            throw ModelLoaderException("ModeLoader::convertAssimpMesh -> Unable to initialize vertex normals.");
        }
        if (!coreMesh->initVertexFaceNormals()) {
            throw ModelLoaderException("ModeLoader::convertAssimpMesh -> Unable to initialize face normals.");
        }
        if (!coreMesh->initVertexTangents()) {
            throw ModelLoaderException("ModeLoader::convertAssimpMesh -> Unable to initialize vertex tangents.");
        }
        coreMesh->enableAttribute(StandardAttribute::Normal);
        coreMesh->enableAttribute(StandardAttribute::FaceNormal);
        coreMesh->enableAttribute(StandardAttribute::Tangent);
        hasNormals = true;

        std::vector<Real> colors;
        Int32 colorsIndex = materialImportDescriptor.meshSpecificProperties[meshIndex].vertexColorsIndex;
        if (colorsIndex >= 0) {
            colors.reserve(mesh.mNumFaces * 12);
            if (!coreMesh->initVertexColors()) {
                throw ModelLoaderException("ModeLoader::convertAssimpMesh -> Unable to initialize vertex colors.");
            }
            coreMesh->enableAttribute(StandardAttribute::Color);
            hasColors = true;
        }

        std::vector<Real> albedoUVs;
        if (diffuseTextureUVIndex >= 0) {
            albedoUVs.reserve(mesh.mNumFaces * 6);
            if (!coreMesh->initVertexAlbedoUVs()) {
                throw ModelLoaderException("ModeLoader::convertAssimpMesh -> Unable to initialize albedo UVs.");
            }
            coreMesh->enableAttribute(StandardAttribute::AlbedoUV);
            hasAlbedoUVs = true;
        }

        std::vector<Real> normalUVs;
        if (normalsTextureUVIndex >= 0) {
            normalUVs.reserve(mesh.mNumFaces * 6);
            if (!coreMesh->initVertexNormalUVs()) {
                throw ModelLoaderException("ModeLoader::convertAssimpMesh -> Unable to initialize normal UVs.");
            }
            coreMesh->enableAttribute(StandardAttribute::NormalUV);
            hasNormalUVs = true;
        }

        // loop through each face in the mesh and copy relevant vertex attributes
        // into the newly created Mesh3D object
        for (UInt32 faceIndex = 0; faceIndex < mesh.mNumFaces; faceIndex++) {
            const aiFace* face = mesh.mFaces + faceIndex;

            Int32 start, end, inc;
            if (!invert) {
                start = face->mNumIndices - 1;
                end = -1;
                inc = -1;
            } else {
                start = 0;
                end = face->mNumIndices;
                inc = 1;
            }

            // ** IMPORTANT ** Normally we iterate through face vertices in reverse order. This is
            // necessary because vertices are stored in counter-clockwise order for each face.
            // if [invert] == true, then we instead iterate in forward order
            for (Int32 i = start; i != end; i += inc) {
                Int32 vIndex = face->mIndices[i];
                aiVector3D srcPosition = mesh.mVertices[vIndex];

                // copy vertex position
                positions.push_back(srcPosition.x);
                positions.push_back(srcPosition.y);
                positions.push_back(srcPosition.z);
                positions.push_back(1.0f);

                // copy mesh normals
                if (hasNormals) {
                    if (mesh.mNormals != nullptr) {
                        aiVector3D& srcNormal = mesh.mNormals[vIndex];
                        normals.push_back(srcNormal.x);
                        normals.push_back(srcNormal.y);
                        normals.push_back(srcNormal.z);
                        faceNormals.push_back(srcNormal.x);
                        faceNormals.push_back(srcNormal.y);
                        faceNormals.push_back(srcNormal.z);
                    }
                    else {
                        normals.push_back(0.0f);
                        normals.push_back(0.0f);
                        normals.push_back(0.0f);
                        faceNormals.push_back(0.0f);
                        faceNormals.push_back(0.0f);
                        faceNormals.push_back(0.0f);
                    }
                    normals.push_back(0.0f);
                }

                // copy vertex colors (if present)
                if (hasColors) {
                    auto color = mesh.mColors[colorsIndex];
                    colors.push_back(color->r);
                    colors.push_back(color->g);
                    colors.push_back(color->b);
                    colors.push_back(color->a);
                }

                // copy relevant data for albedo texture (UV coords)
                if (hasAlbedoUVs) {
                    albedoUVs.push_back(mesh.mTextureCoords[diffuseTextureUVIndex][vIndex].x);
                    if (materialImportDescriptor.meshSpecificProperties[meshIndex].invertVCoords) {
                        albedoUVs.push_back(1 - mesh.mTextureCoords[diffuseTextureUVIndex][vIndex].y);
                    } else {
                        albedoUVs.push_back(mesh.mTextureCoords[diffuseTextureUVIndex][vIndex].y);
                    }
                }

                if (hasNormalUVs) {
                    normalUVs.push_back(mesh.mTextureCoords[normalsTextureUVIndex][vIndex].x);
                    if (materialImportDescriptor.meshSpecificProperties[meshIndex].invertVCoords) {
                        normalUVs.push_back(1 - mesh.mTextureCoords[normalsTextureUVIndex][vIndex].y);
                    } else {
                        normalUVs.push_back(mesh.mTextureCoords[normalsTextureUVIndex][vIndex].y);
                    }
                }
            }
        }

        coreMesh->getVertexPositions()->store(positions.data());
        if (hasNormals) {
            coreMesh->getVertexNormals()->store(normals.data());
            coreMesh->getVertexFaceNormals()->store(faceNormals.data());
        }
        if (hasColors) coreMesh->getVertexColors()->store(colors.data());
        if (hasAlbedoUVs) coreMesh->getVertexAlbedoUVs()->store(albedoUVs.data());
        if (hasNormalUVs) coreMesh->getVertexNormalUVs()->store(normalUVs.data());

        // if (invert) mesh3D->SetInvertNormals(true);
        coreMesh->setNormalsSmoothingThreshold((Real)smoothingThreshold * Math::DegreesToRads);
        coreMesh->setCalculateNormals(true);
        coreMesh->setCalculateTangents(true);
        coreMesh->setCalculateBoundingBox(true);
        coreMesh->update();

        return coreMesh;
    }

    /**
     * Process the Assimp materials (instances of aiMaterial) in the Assimp scene [scene]. This method loops through each
     * Assimp material and then examines which Assimp meshes use it. For each Assimp mesh that uses an Assimp material,
     * a unique (and equivalent) engine-native Material instance is created.
     *
     * For each mesh that uses a given Assimp material, we MUST create a unique engine-native Material object. Engine-native
     * Material objects are linked to shaders, and since different meshes may have different attributes, they may potentially
     * require DIFFERENT SHADERS. For example: Two meshes may share an Assimp material, but only one mesh might have vertex
     * colors. In this case two different shaders are needed: one that supports vertex colors and one that doesn't require them.
     *
     * [modelPath] - Native file-system compatible path that points to the model file in the file system.
     * [scene] - The Assimp model/scene.
     * [materialImportDescriptors] - A vector of MaterialImportDescriptor structures that will be populated by ProcessMaterials().
     */
    Bool ModelLoader::processMaterials(const std::string& modelPath, const aiScene& scene,
                                       std::vector<MaterialImportDescriptor>& materialImportDescriptors) const {
        // TODO: Implement support for embedded textures
        if (scene.HasTextures()) {
            throw ModelLoaderException("ModelLoader::processMaterials -> Support for meshes with embedded textures is not implemented");
        }

        std::shared_ptr<FileSystem> fileSystem = FileSystem::getInstance();
        std::string fixedModelPath = fileSystem->fixupPathForLocalFilesystem(modelPath);

        // loop through each scene material and extract relevant textures and
        // other properties and create a MaterialDescriptor object that will hold those
        // properties and all corresponding Material objects
        for (UInt32 m = 0; m < scene.mNumMaterials; m++) {
            aiString aiTexturePath;

            aiMaterial* assimpMaterial = scene.mMaterials[m];
            if (assimpMaterial == nullptr) {
                throw ModelLoaderException("ModelLoader::processMaterials -> Scene contains a null material.");
            }

            aiString mtName;
            assimpMaterial->Get(AI_MATKEY_NAME, mtName);

            // build an import descriptor for this material
            MaterialImportDescriptor materialImportDescriptor;
            this->getImportDetails(assimpMaterial, materialImportDescriptor, scene);

            aiReturn texFound = AI_SUCCESS;

            WeakPointer<Texture> diffuseTexture;
            WeakPointer<Texture> normalTexture;
            //	TextureRef specularTexture;

            // get diffuse texture (for now support only 1)
            texFound = assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath);
            if (texFound == AI_SUCCESS) {
                diffuseTexture = this->loadAITexture(*assimpMaterial, aiTextureType_DIFFUSE, fixedModelPath);
            }

            texFound = assimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexturePath);
            if (texFound == AI_SUCCESS) {
                normalTexture = this->loadAITexture(*assimpMaterial, aiTextureType_NORMALS, fixedModelPath);
            }

            MaterialLibrary& materialLibrary = Engine::instance()->getMaterialLibrary();
            // loop through each mesh in the scene and check if it uses [material]. If so,
            // create a unique Material object for the mesh and attach it to [materialImportDescriptor]
            //
            // The [materialImportDescriptor] structure describes the unique per-mesh properties we need to be
            // concerned about when creating unique instances of a material for a mesh.
            for (UInt32 i = 0; i < scene.mNumMeshes; i++) {
                if (materialImportDescriptor.usedByMesh(i)) {

                    // see if we can match a loaded shader to the properties of this material and the current mesh
                    LongMask shaderMaterialChacteristics = materialImportDescriptor.meshSpecificProperties[i].shaderMaterialChacteristics;
                    Bool hasMaterial = materialLibrary.hasMaterial(shaderMaterialChacteristics);

                    // if we can't find a loaded shader that matches the properties of this material and
                    // the current mesh...well we can't really load this material
                    if (!hasMaterial) {
                        std::string msg = "Could not find loaded material for: ";
                        msg += std::bitset<64>(shaderMaterialChacteristics).to_string();
                        throw ModelLoaderException(msg);
                    }

                    WeakPointer<Material> matchingMaterial = materialLibrary.getMaterial(shaderMaterialChacteristics);
                    matchingMaterial = matchingMaterial->clone();

                    // map new material to its corresponding mesh
                    materialImportDescriptor.meshSpecificProperties[i].material = matchingMaterial;

                    // if there is a diffuse texture, set it up in the new material
                    if (diffuseTexture.isValid()) {
                        // Add [diffuseTexture] to the new material (and for the appropriate shader variable), and store
                        // Assimp UV channel for it in [materialImportDescriptor] for later processing of the mesh
                        Bool setupSuccess =
                            this->setupMeshSpecificMaterialWithTextures(*assimpMaterial, diffuseTexture, normalTexture, i, materialImportDescriptor);

                        if (!setupSuccess) {
                            throw ModelLoaderException("ModelLoader::ProcessMaterials -> Could not set up diffuse texture.");
                        }
                    }
                }
            }

            // add the new MaterialImportDescriptor instance to [materialImportDescriptors]
            materialImportDescriptors.push_back(materialImportDescriptor);
        }

        return true;
    }

    /**
     * Get the global import properties for an Assimp material [mtl], and store in the global section  of the
     * supplied MaterialImportDescriptor instance [materialImportDesc].
     *
     * Additionally, get the mesh-specific properties for the Assimp material and store in the mesh-specific
     * properties section of the supplied MaterialImportDescriptor instance.
     */
    void ModelLoader::getImportDetails(const aiMaterial* mtl, MaterialImportDescriptor& materialImportDesc, const aiScene& scene) const {
        LongMask flags = LongMaskUtil::createMask();
        aiString path;
        aiColor4t<Real> color;

        // automatically give normals to all materials & meshes (if a mesh doesn't have them by
        // default, they will be calculated)
        LongMaskUtil::setBit(&flags, (Int16)ShaderMaterialCharacteristic::VertexNormals);

        // check for a diffuse texture
        if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &path)) {
            LongMaskUtil::setBit(&flags, (Int16)ShaderMaterialCharacteristic::DiffuseTextured);
        }

        // check for a normals texture
        if (AI_SUCCESS == mtl->GetTexture(aiTextureType_NORMALS, 0, &path)) {
            LongMaskUtil::setBit(&flags, (Int16)ShaderMaterialCharacteristic::NormalMapped);
        }

        LongMaskUtil::setBit(&flags, (Int16)ShaderMaterialCharacteristic::Lit);

        /*if(AI_SUCCESS == mtl->GetTexture(aiTextureType_SPECULAR, 0, &path))
        {
            LongMaskUtil::SetBit(&flags, (Int16)ShaderMaterialProperty::SpecularTextured);
        }
        if(AI_SUCCESS == mtl->GetTexture(aiTextureType_NORMALS, 0, &path))
        {
            LongMaskUtil::SetBit(&flags, (Int16)ShaderMaterialProperty::Bumped);
        }
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color))
        {
            LongMaskUtil::SetBit(&flags, (Int16)ShaderMaterialProperty::DiffuseColored);
        }
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color))
        {
            LongMaskUtil::SetBit(&flags, (Int16)ShaderMaterialProperty::SpecularColored);
        }
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &color))
        {
            LongMaskUtil::SetBit(&flags, (Int16)ShaderMaterialProperty::EmissiveColored);
        }*/

        // Even though multiple meshes may share an Assimp material, that doesn't necessarily
        // mean they can share a Material object. A Material object is linked to a shader
        // so if, for example, multiple Assimp meshes share a single Assimp material, but
        // only one of those meshes supplies vertex colors, then we actually need two Material objects
        // since the shaders will be different.
        //
        // This loop runs through each Assimp mesh that uses [mtl] and determines the unique
        // material properties of that mesh to form a final LongMask value that holds the
        // active ShaderMaterialCharacteristic values for that mesh.
        for (UInt32 i = 0; i < scene.mNumMeshes; i++) {
            // copy the existing set of ShaderMaterialCharacteristic values
            LongMask meshFlags = flags;
            // get mesh
            const aiMesh* mesh = scene.mMeshes[i];

            // does the current mesh use [mtl] ?
            if (scene.mMaterials[mesh->mMaterialIndex] == mtl) {
                // for now only support one set of vertex colors, and look at index 0 for it
                if (mesh->HasVertexColors(0)) {
                    LongMaskUtil::setBit(&meshFlags, (Int16)ShaderMaterialCharacteristic::VertexColors);
                    materialImportDesc.meshSpecificProperties[i].vertexColorsIndex = 0;
                }
                // set mesh specific ShaderMaterialCharacteristic values
                materialImportDesc.meshSpecificProperties[i].shaderMaterialChacteristics = meshFlags;
            }
        }
    }

    /**
     * Take an Assimp material [assimpMaterial] and use its properties to create and load an engine-native Texture instance
     * for it that matches the type specified by [textureType].
     *
     * This method looks in two places in the file system for the image files for the texture:
     *
     *    1. Using the full path that is specified in the [assimpMaterial] structure.
     *    2. In [modelPath], which is the location in the file system of model/scene to which [assimpMaterial] belongs.
     *
     * [modelPath] - Native file-system compatible path that points to the model file in the file system.
     * [assimpMaterial] - The Assimp material.
     * [textureType] - The type of texture to look for (diffuse, specular, normal map, etc...)
     */
    WeakPointer<Texture> ModelLoader::loadAITexture(aiMaterial& assimpMaterial, aiTextureType textureType, const std::string& modelPath) const {
        // temp variables
        WeakPointer<Texture2D> texture;
        aiString aiTexturePath;
        aiReturn texFound = AI_SUCCESS;

        // get the path to the directory that contains the scene/model
        std::shared_ptr<FileSystem> fileSystem = FileSystem::getInstance();
        std::string fixedModelPath = fileSystem->fixupPathForLocalFilesystem(modelPath);
        std::string modelDirectory = fileSystem->getBasePath(fixedModelPath);

        // retrieve the first texture descriptor (at index 0) matching [textureType] from the Assimp material
        texFound = assimpMaterial.GetTexture(textureType, 0, &aiTexturePath);

        if (texFound != AI_SUCCESS) {
            throw ModelLoaderException("ModelLoader::loadAITexture -> Assimp material does not have desired texture type.");
        }

        // build the full path to the texture image as specified by the Assimp material
        std::string texPath = fileSystem->fixupPathForLocalFilesystem(std::string(aiTexturePath.data));
        std::string fullTextureFilePath = fileSystem->concatenatePaths(modelDirectory, texPath);

        TextureAttributes texAttributes;
        texAttributes.FilterMode = TextureFilter::TriLinear;
        texAttributes.MipMapLevel = 4;
        texAttributes.Format = TextureFormat::RGBA8;

        std::shared_ptr<RawImage> textureImage;
        // check if the file specified by the full path in the Assimp material exists
        if (fileSystem->fileExists(fullTextureFilePath)) {
            textureImage = ImageLoader::loadImageU(fullTextureFilePath);
            texture = Engine::instance()->getGraphicsSystem()->createTexture2D(texAttributes);
        }
        // if it does not exist, try looking for the texture image file in the model's directory
        else {
            // get just the filename portion of the path
            std::string filename = fileSystem->getFileName(fullTextureFilePath);
            if (!(filename.length() <= 0)) {
                // concatenate the file name with the model's directory location
                filename = fileSystem->concatenatePaths(modelDirectory, filename);
                // check if the image file is in the same directory as the model and if so, load it
                if (fileSystem->fileExists(filename)) {
                    textureImage = ImageLoader::loadImageU(filename.c_str());
                    texture = Engine::instance()->getGraphicsSystem()->createTexture2D(texAttributes);
                }
            }
        }

        WeakPointer<Texture2D> texturePtr(texture);
        if (textureImage && texturePtr) {
            texturePtr->build(textureImage);
        }
        
        // did texture fail to load?
        if (!textureImage || !texturePtr || !texturePtr->isBuilt()) {
            std::string msg = std::string("ModelLoader::loadAITexture -> Could not load texture file: ") + fullTextureFilePath;
            throw ModelLoaderException(msg);
        }

        return texture;
    }

    /**
     * Set the material for the mesh specified by [meshIndex] in a material import descriptor [materialImportDesc] with an instance of
     * Texture that has already been loaded [texture]. This method determines the correct shader variable name for the texture based on
     * the type of texture [textureType], and sets that variable in the material for the mesh specified by [meshIndex] with [texture].
     * The method then locates the Assimp UV data for that texture and stores that in the mesh-specific properties of
     * [materialImportDesc].
     */
    Bool ModelLoader::setupMeshSpecificMaterialWithTextures(const aiMaterial& assimpMaterial, WeakPointer<Texture> diffuseTexture,
                                                           WeakPointer<Texture> normalsTexture,  UInt32 meshIndex,
                                                           MaterialImportDescriptor& materialImportDesc) const {
       
        // set the diffuse texture in the material for the mesh specified by [meshIndex]
        WeakPointer<Material> material = materialImportDesc.meshSpecificProperties[meshIndex].material;

        // TODO: Need to not have this material hard coded in here....
        WeakPointer<StandardPhysicalMaterial> texturedMaterial = WeakPointer<Material>::dynamicPointerCast<StandardPhysicalMaterial>(material);

        Int32 mappedIndex;

        if (diffuseTexture) {
             // get the Assimp material key for textures of type [textureType]
            UInt32 aiDiffuseTextureKey = this->convertTextureTypeToAITextureKey(TextureType::Diffuse);
            texturedMaterial->setAlbedoMap(diffuseTexture);
            texturedMaterial->setAlbedoMapEnabled(true);
            
            if (AI_SUCCESS == aiGetMaterialInteger(&assimpMaterial, AI_MATKEY_UVWSRC(aiDiffuseTextureKey, 0), &mappedIndex))
                materialImportDesc.meshSpecificProperties[meshIndex].uvMapping[TextureType::Diffuse] = mappedIndex;
            else
                materialImportDesc.meshSpecificProperties[meshIndex].uvMapping[TextureType::Diffuse] = 0;
        }

        if (normalsTexture) {
            UInt32 aiNormalsTextureKey = this->convertTextureTypeToAITextureKey(TextureType::Normals);
            texturedMaterial->setNormalMap(normalsTexture);
            texturedMaterial->setNormalMapEnabled(true);

            if (AI_SUCCESS == aiGetMaterialInteger(&assimpMaterial, AI_MATKEY_UVWSRC(aiNormalsTextureKey, 0), &mappedIndex))
                materialImportDesc.meshSpecificProperties[meshIndex].uvMapping[TextureType::Normals] = mappedIndex;
            else
                materialImportDesc.meshSpecificProperties[meshIndex].uvMapping[TextureType::Normals] = 0;
        }

        return true;
    }

    ModelLoader::TextureType ModelLoader::convertAITextureKeyToTextureType(Int32 aiTextureKey) {
        TextureType textureType = TextureType::_None;
        if (aiTextureKey == aiTextureType_SPECULAR)
            textureType = TextureType::Specular;
        else if (aiTextureKey == aiTextureType_NORMALS)
            textureType = TextureType::Normals;
        else if (aiTextureKey == aiTextureType_DIFFUSE)
            textureType = TextureType::Diffuse;
        return textureType;
    }

    StandardAttribute ModelLoader::mapTextureTypeToAttribute(TextureType textureType) {
        switch (textureType) {
            case TextureType::Diffuse:
                return StandardAttribute::AlbedoUV;
                break;
             case TextureType::Normals:
                return StandardAttribute::NormalUV;
                break;
            default:
                return StandardAttribute::_None;
                break;
        }

        return StandardAttribute::_None;
    }

    int ModelLoader::convertTextureTypeToAITextureKey(TextureType textureType) {
        Int32 aiTextureKey = -1;
        if (textureType == TextureType::Specular)
            aiTextureKey = aiTextureType_SPECULAR;
        else if (textureType == TextureType::Normals)
            aiTextureKey = aiTextureType_NORMALS;
        else if (textureType == TextureType::Diffuse)
            aiTextureKey = aiTextureType_DIFFUSE;
        return aiTextureKey;
    }

    /*
     * Determine if [mat] has an odd number of reflections.
     */
    Bool ModelLoader::hasOddReflections(Matrix4x4& mat) {
        Real determinant = mat.calculateDeterminant();
        if (determinant < 0.0) return true;
        return false;
    }

    void ModelLoader::convertAssimpMatrix(const aiMatrix4x4& source, Matrix4x4& dest) {
        Real data[16];

        data[0] = source.a1;
        data[1] = source.b1;
        data[2] = source.c1;
        data[3] = source.d1;

        data[4] = source.a2;
        data[5] = source.b2;
        data[6] = source.c2;
        data[7] = source.d2;

        data[8] = source.a3;
        data[9] = source.b3;
        data[10] = source.c3;
        data[11] = source.d3;

        data[12] = source.a4;
        data[13] = source.b4;
        data[14] = source.c4;
        data[15] = source.d4;

        dest.copy(data);
    }
}