#include <IL/il.h>

#include "ImageLoader.h"
#include "RawImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "STBImage.h"

namespace Core {

    Bool ImageLoader::initialized = false;

    Bool ImageLoader::initialize() {
        if (!ImageLoader::initialized) {
            if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
                std::cerr << "ImageLoader::initialize -> wrong DevIL version" << std::endl;
                // throw ImageLoaderException("ImageLoader::initialize -> wrong DevIL version");
            }

            ilInit(); /// Initialization of DevIL
            ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
            ilEnable(IL_ORIGIN_SET);
            ImageLoader::initialized = true;
        }

        return true;
    }

    std::shared_ptr<StandardImage> ImageLoader::loadImageU(const std::string& fullPath) {
        return loadImageU(fullPath, false);
    }

    std::shared_ptr<StandardImage> ImageLoader::loadImageU(const std::string& fullPath, Bool reverseOrigin) {
        Bool initializeSuccess = initialize();

        if (!initializeSuccess) {
            throw ImageLoaderException("ImageLoader::LoadImageU -> Error occurred while initializing image loader.");
        }
     
        if (reverseOrigin) {
            ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
        }

        std::string extension = getFileExtension(fullPath);

        ILuint imageIds[1];
        ilGenImages(1, imageIds); // Generation of numTextures image names
        ilBindImage(imageIds[0]); // Binding of DevIL image name
        std::shared_ptr<StandardImage> rawImage;

        ILboolean success = ilLoadImage(fullPath.c_str());

        if (success) {
            // Convert every color component into unsigned byte.If your image contains
            // alpha channel you can replace IL_RGB with IL_RGBA
            success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
            if (!success) {
                ilDeleteImages(1, imageIds);
                throw ImageLoaderException("ImageLoader::LoadImage -> Couldn't convert image");
            }
            rawImage = getStandardImageFromILData(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
        } else {
            ILenum i = ilGetError();
            std::string msg = "ImageLoader::LoadImage -> Couldn't load image: ";
            msg += fullPath.c_str();
            if (i == IL_INVALID_EXTENSION) {
                msg = std::string("ImageLoader::LoadImage -> Couldn't load image (invalid extension). ");
                msg += std::string("Is DevIL configured to load extension: ") + extension + std::string(" ?");
            }
            ilDeleteImages(1, imageIds);
            throw ImageLoaderException(msg);
        }

        // Because we have already copied image data into texture data we can release memory used by image.
        ilDeleteImages(1, imageIds);

        if (reverseOrigin) {
            ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        }

        return rawImage;
    }

    std::shared_ptr<HDRImage> ImageLoader::loadImageHDR(const std::string& fullPath) {
        return loadImageHDR(fullPath, false);
    }

    std::shared_ptr<HDRImage> ImageLoader::loadImageHDR(const std::string& fullPath, bool invertY) {
        Bool initializeSuccess = initialize();

        if (!initializeSuccess) {
            throw ImageLoaderException("ImageLoader::loadImageHDR -> Error occurred while initializing image loader.");
        }

        stbi_set_flip_vertically_on_load(invertY);

        int width, height, nrComponents;
        float *hdr_data = stbi_loadf(fullPath.c_str(), &width, &height, &nrComponents, 0);

        if (hdr_data == NULL) {
            std::string msg("ImageLoader::loadImageHDR -> Could not load HDRImage: ");
            msg = msg + fullPath + std::string(", reason: ") + stbi_failure_reason();
            throw ImageLoaderException(msg);
        }

        HDRImage * hdrImagePtr = new(std::nothrow) HDRImage(width, height);
        if (hdrImagePtr == nullptr) throw ImageLoaderException("ImageLoader::loadImageHDR -> Could not allocate HDRImage.");

        std::shared_ptr<HDRImage> hdrImage(hdrImagePtr);
        Bool initSuccess = hdrImage->init();
        if (!initSuccess) {
            throw ImageLoaderException("ImageLoader::loadImageHDR -> Could not init HDRImage.");
        }
        for (UInt32 i = 0; i < width * height; i++) {
            UInt32 baseIndex = i * 4;
            UInt32 baseHDRIndex = i * 3;
            hdrImage->setElement(baseIndex, hdr_data[baseHDRIndex]);
            hdrImage->setElement(baseIndex + 1, hdr_data[baseHDRIndex + 1]);
            hdrImage->setElement(baseIndex + 2, hdr_data[baseHDRIndex + 2]);
            hdrImage->setElement(baseIndex + 3, 1.0f);
        }

        return hdrImage;
    }

    std::shared_ptr<StandardImage> ImageLoader::getStandardImageFromILData(const ILubyte * data, UInt32 width, UInt32 height) {
        if (data == nullptr) throw ImageLoaderException("ImageLoader::getStandardImageFromILData -> 'data' is null.");

        StandardImage * rawImagePtr = new(std::nothrow) StandardImage(width, height);
        if (rawImagePtr == nullptr) throw ImageLoaderException("ImageLoader::getStandardImageFromILData -> Could not allocate StandardImage.");

        std::shared_ptr<StandardImage> rawImage(rawImagePtr);

        Bool initSuccess = rawImage->init();
        if (!initSuccess) {
            throw ImageLoaderException("ImageLoader::getStandardImageFromILData -> Could not init StandardImage.");
        }

        for (UInt32 i = 0; i < width * height * 4; i++) {
            rawImage->setElement(i, (Byte)data[i]);
        }

        return rawImage;
    }
 
    std::string ImageLoader::getFileExtension(const std::string& filePath) {
        Int32 dotIndex = (Int32)filePath.find_last_of(".");
        if (dotIndex < 0)dotIndex = 0;
        std::string extension = filePath.substr(dotIndex, filePath.size() - dotIndex);
        return extension;
    }

}