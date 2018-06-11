#include <fstream>

#include "assimp/cimport.h"
#include "assimp/config.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

#include "ModelLoader.h"

namespace Core {

    ModelLoader::ModelLoader(): importer(nullptr) {

    }

    ModelLoader::~ModelLoader() {
        if(this->importer != nullptr) {
            delete this->importer;
            this->importer = nullptr;
        }
    }

    void ModelLoader::initImporter() {
        if (this->importer == nullptr) {
            this->importer = new(std::nothrow) Assimp::Importer();
        }

        if (this->importer == nullptr) {
            throw ModelLoaderException("ModeLoader::initImporter -> importer is null.");
        }
    }

    /**
     * Load an Assimp compatible model/scene located at [filePath]. [filePath] Must be a native file-system
     * compatible path, so the the engine's FileSystem singleton should be used to derive the correct platform-specific
     * path before calling this method.
     */
    const aiScene * ModelLoader::loadAIScene(const std::string& filePath, Bool preserveFBXPivots) {
        // the global Assimp scene object
        const aiScene* scene = nullptr;

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
        scene = importer->ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality);

        // If the import failed, report it
        if (!scene) {
            std::string msg = std::string("ModeLoader::loadAIScene -> Could not import file: ") + std::string(importer->GetErrorString());
            throw ModelLoaderException(msg);
            return nullptr;
        }

        return scene;
    }

}