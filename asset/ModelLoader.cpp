#include <fstream>

#include "assimp/cimport.h"
#include "assimp/config.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

#include "ModelLoader.h"
#include "../filesys/FileSystem.h"
#include "../image/Texture.h"
#include "../material/ShaderMaterialCharacteristic.h"
#include "../Engine.h"
#include "../image/TextureAttr.h"

namespace Core {

    ModelLoader::ModelLoader(Engine& engine): engine(engine), importer(nullptr) {

    }

    ModelLoader::~ModelLoader() {
        
    }

    void ModelLoader::initImporter() {
        if (!this->importer) {
            this->importer = std::make_shared<Assimp::Importer>();
        }

        if (!this->importer) {
            throw ModelLoaderException("ModeLoader::initImporter -> Unable to create Assimp::Importer.");
        }
    }

    /**
     * Load an Assimp compatible model/scene located at [filePath]. [filePath] Must be a native file-system
     * compatible path, so the the engine's FileSystem singleton should be used to derive the correct platform-specific
     * path before calling this method.
     */
     std::shared_ptr<const aiScene> ModelLoader::loadAIScene(const std::string& filePath, Bool preserveFBXPivots) {
        // the global Assimp scene object
        std::shared_ptr<const aiScene> scene;

        // Create an instance of the Assimp Importer class
        this->initImporter();

        // Check if model file exists
        std::ifstream fin(filePath.c_str());
        if (!fin.fail()) {
            fin.close();
        }
        else {
            std::string msg = std::string("ModeLoader -> Could not find file: ") + filePath;
            throw ModelLoaderException(msg);
        }

        // tell Assimp not to create extra nodes when importing FBX files
        importer->SetPropertyInteger(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, preserveFBXPivots ? 1 : 0);

        // read the model file in from disk
        scene = std::shared_ptr<const aiScene>(importer->ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality));

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
     WeakPointer<Object3D> ModelLoader::loadModel(const std::string& modelPath, Real importScale, Bool preserveFBXPivots) {
        std::shared_ptr<FileSystem> fileSystem = FileSystem::getInstance();
        std::string fixedModelPath = fileSystem->fixupPathForLocalFilesystem(modelPath);

        // the global Assimp scene object
        std::shared_ptr<const aiScene> scene = this->loadAIScene(fixedModelPath, preserveFBXPivots);

        if (scene) {
            // the model has been loaded from disk into Assimp data structures, now convert to engine-native structures
            WeakPointer<Object3D> result = processModelScene(fixedModelPath, *scene, importScale);
            return result;
        }
        else {
            throw ModelLoaderException("ModelLoder::loadModel() -> Error occured while loading Assimp scene.");
        }
    }

    WeakPointer<Object3D> ModelLoader::processModelScene(const std::string& modelPath, const aiScene& scene, Real importScale) const {
        // container for MaterialImportDescriptor instances that describe the engine-native
        // materials that get created during the call to ProcessMaterials()
        std::vector<MaterialImportDescriptor> materialImportDescriptors;

        // verify that we have a valid scene
        if (scene.mRootNode == nullptr) throw new ModelLoaderException("ModelLoader::processModelScene -> Assimp scene root is null.");
        WeakPointer<Object3D> root = engine.createObject3D();
        if (WeakPointer<Object3D>::isInvalid(root)) throw ModelLoaderException("ModelLoader::processModelScene -> Could not create root object.");
      
        std::shared_ptr<FileSystem> fileSystem = FileSystem::getInstance();
        std::string fixedModelPath = fileSystem->fixupPathForLocalFilesystem(modelPath);

        // process all the Assimp materials in [scene] and create equivalent engine native materials.
        // store those materials and their properties in MaterialImportDescriptor instances, which get
        // added to [materialImportDescriptors]
        Bool processMaterialsSuccess = this->processMaterials(fixedModelPath, scene, materialImportDescriptors);
        if (!processMaterialsSuccess) {
            throw ModelLoaderException("ModelLoader::processModelScene -> processMaterials() returned an error.");
        }

        return root;
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
    Bool ModelLoader::processMaterials(const std::string& modelPath, const aiScene& scene, std::vector<MaterialImportDescriptor>& materialImportDescriptors) const {
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

            aiMaterial * assimpMaterial = scene.mMaterials[m];
            if (assimpMaterial == nullptr) {
                throw ModelLoaderException("ModelImporter::processMaterials -> Scene contains a null material.");
            }
        
            aiString mtName;
            assimpMaterial->Get(AI_MATKEY_NAME, mtName);

            // build an import descriptor for this material
            MaterialImportDescriptor materialImportDescriptor;
            this->getImportDetails(assimpMaterial, materialImportDescriptor, scene);

            aiReturn texFound = AI_SUCCESS;

            /*WeakPointer<Texture> diffuseTexture;
            //	TextureRef bumpTexture;
            //	TextureRef specularTexture;

            // get diffuse texture (for now support only 1)
            texFound = assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath);
            if (texFound == AI_SUCCESS) {
                diffuseTexture = this->loadAITexture(*assimpMaterial, aiTextureType_DIFFUSE, fixedModelPath);
            }*/
   
            // loop through each mesh in the scene and check if it uses [material]. If so,
            // create a unique Material object for the mesh and attach it to [materialImportDescriptor]
            //
            // The [materialImportDescriptor] structure describes the unique per-mesh properties we need to be
            // concerned about when creating unique instances of a material for a mesh.
            /*for (UInt32 i = 0; i < scene.mNumMeshes; i++) {
                if (materialImportDescriptor.UsedByMesh(i)) {
                    // see if we can match a loaded shader to the properties of this material and the current mesh
                    ShaderSharedPtr loadedShader = engineObjectManager->GetLoadedShader(materialImportDescriptor.meshSpecificProperties[i].shaderProperties);

                    // if we can't find a loaded shader that matches the properties of this material and
                    // the current mesh...well we can't really load this material
                    if (!loadedShader.IsValid()) {
                        std::string msg = "Could not find loaded shader for: ";
                        msg += std::bitset<64>(materialImportDescriptor.meshSpecificProperties[i].shaderProperties).to_string();
                        Engine::Instance()->GetErrorManager()->SetAndReportError(ModelImporterErrorCodes::MaterialShaderMatchFailure, msg);
                        materialImportDescriptors.clear();
                        return false;
                    }

                    // create a new Material engine object
                    MaterialSharedPtr newMaterial = engineObjectManager->CreateMaterial(mtName.C_Str(), loadedShader);
                    if (!newMaterial.IsValid()) {
                        std::string msg = "ModelImporter::ProcessMaterials -> Could not create new Material object.";
                        Engine::Instance()->GetErrorManager()->SetAndReportError(ModelImporterErrorCodes::MaterialImportFailure, msg);
                        materialImportDescriptors.clear();
                        return false;
                    }

                    // map new material to its corresponding mesh
                    materialImportDescriptor.meshSpecificProperties[i].material = newMaterial;

                    // if there is a diffuse texture, set it up in the new material
                    if (diffuseTexture.IsValid()) {
                        // Add [diffuseTexture] to the new material (and for the appropriate shader variable), and store
                        // Assimp UV channel for it in [materialImportDescriptor] for later processing of the mesh
                        Bool setupSuccess = SetupMeshSpecificMaterialWithTexture(*assimpMaterial, TextureType::Diffuse, diffuseTexture, i, materialImportDescriptor);
                        if (!setupSuccess) {
                            std::string msg = "ModelImporter::ProcessMaterials -> Could not set up diffuse texture.";
                            Engine::Instance()->GetErrorManager()->SetAndReportError(ModelImporterErrorCodes::MaterialImportFailure, msg);
                            materialImportDescriptors.clear();
                            return false;
                        }
                    }
                }
            }*/

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
        LongMaskUtil::setBit(&flags, (Int16) ShaderMaterialCharacteristic::VertexNormals);

        // check for a diffuse texture
        if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &path)) {
            LongMaskUtil::setBit(&flags, (Int16) ShaderMaterialCharacteristic::DiffuseTextured);
        }

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
            const aiMesh * mesh = scene.mMeshes[i];

            // does the current mesh use [mtl] ?
            if (scene.mMaterials[mesh->mMaterialIndex] == mtl) {
                // for now only support one set of vertex colors, and look at index 0 for it
                if (mesh->HasVertexColors(0)) {
                    LongMaskUtil::setBit(&meshFlags, (Int16)ShaderMaterialCharacteristic::VertexColors);
                    materialImportDesc.meshSpecificProperties[i].vertexColorsIndex = 0;
                }
                // set mesh specific ShaderMaterialCharacteristic values
                materialImportDesc.meshSpecificProperties[i].shaderProperties = meshFlags;
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
       /* WeakPointer<Texture2D> texture;
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

        RawImage* textureImage = nullptr;
        // check if the file specified by the full path in the Assimp material exists
        if (fileSystem->fileExists(fullTextureFilePath)) {
            //texture = engineObjectManager->CreateTexture(fullTextureFilePath.c_str(), texAttributes);
            textureImage = ImageLoader::loadImageU(fullTextureFilePath);
            texture = this->engine.getGraphicsSystem()->createTexture2D(texAttributes);
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
                    texture = this->engine.getGraphicsSystem()->createTexture2D(texAttributes);
                }
            }
        }

        WeakPointer<Texture2D> texturePtr(texture);
        if (textureImage && texturePtr) {
            texturePtr->build(textureImage);
        }
        // did texture fail to load?
        if (!textureImage || !texturePtr || !texturePtr->isBuilt()) {
            std::string msg = std::string("ModelImporter::LoadAITexture -> Could not load texture file: ") + fullTextureFilePath;
            throw ModelLoaderException(msg);
        }

        return texture;*/
    }

}