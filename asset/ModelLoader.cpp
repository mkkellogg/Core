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

}