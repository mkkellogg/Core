#include "ImageLoader.h"
#include "PNGLoader.h"
#include "RawImage.h"

namespace Core {

    Bool ImageLoader::initialized = false;

    Bool ImageLoader::initialize() {
        if (!ImageLoader::initialized) {
            ImageLoader::initialized = true;
        }

        return true;
    }

    RawImage * ImageLoader::loadImageU(const std::string& fullPath) {
        return loadImageU(fullPath, false);
    }

    RawImage * ImageLoader::loadImageU(const std::string& fullPath, Bool reverseOrigin) {
        if (!initialize()) {
              throw ImageLoaderException("ImageLoader::LoadImageU -> Error occurred while initializing image loader.");
        }

        std::string extension = ImageLoader::getFileExtension(fullPath);

        if (extension == ".png") {
            return PNGLoader::loadPNG(fullPath);
        }

        std::string msg("ImageLoader::LoadImageU -> Unsupported image format: ");
        msg += extension;
        throw ImageLoaderException(msg);
    }
 
    void ImageLoader::destroyRawImage(RawImage * image) {
        if (image == nullptr) {
          throw ImageLoaderException("ImageLoader::DestroyRawImage -> 'image' is null.");
        }
        delete image;
    }

    std::string ImageLoader::getFileExtension(const std::string& filePath) {
        Int32 dotIndex = (Int32)filePath.find_last_of(".");
        if (dotIndex < 0)dotIndex = 0;
        std::string extension = filePath.substr(dotIndex, filePath.size() - dotIndex);
        return extension;
    }

}