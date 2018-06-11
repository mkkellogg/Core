#pragma once

#include <string>
#include <memory>

#include "assimp/scene.h"
#include "assimp/Importer.hpp"

#include "../common/types.h"
#include "../common/Exception.h"

namespace Core {

    // forward declarations
    class Engine;

    class ModelLoader {
    public:

        class ModelLoaderException: Exception {
        public:
            ModelLoaderException(const std::string& msg): Exception(msg) {}
            ModelLoaderException(const char* msg): Exception(msg) {}
        };

        ~ModelLoader();
        void loadModel(const std::string& filePath);

    private:
        std::shared_ptr<Assimp::Importer> importer;

        ModelLoader();

        void initImporter();
        std::shared_ptr<const aiScene> loadAIScene(const std::string& filePath, Bool preserveFBXPivots);
    };

}