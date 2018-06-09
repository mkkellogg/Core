#include "ImageLoader.h"
#include "RawImage.h"
#include <IL/il.h>

namespace Core {

    Bool ImageLoader::ilInitialized = false;

    Bool ImageLoader::initialize() {
        if (!ImageLoader::ilInitialized) {
            if (ilGetInteger(IL_VERSION_NUM) >= IL_VERSION ) {
              throw new ImageLoaderException("ImageLoader::initialize -> Invalid IL version.");
            }

            ilInit(); /// Initialization of DevIL
            ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
            ilEnable(IL_ORIGIN_SET);
            ImageLoader::ilInitialized = true;
        }

        return true;
    }

    RawImage * ImageLoader::loadImageU(const std::string& fullPath) {
        return loadImageU(fullPath, false);
    }

    RawImage * ImageLoader::loadImageU(const std::string& fullPath, Bool reverseOrigin) {
        if (!initialize()) {
              throw new ImageLoaderException("ImageLoader::LoadImageU -> Error occurred while initializing image loader.");
        }

        if (reverseOrigin) {
            ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
        }

        std::string extension = getFileExtension(fullPath);

        ILuint imageIds[1];
        ilGenImages(1, imageIds); //Generation of numTextures image names
        ilBindImage(imageIds[0]); // Binding of DevIL image name
        RawImage * rawImage = nullptr;

        ILboolean success = ilLoadImage(fullPath.c_str());

        if (success) { // If no error occurred:
            // Convert every color component into unsigned byte.If your image contains
            // alpha channel you can replace IL_RGB with IL_RGBA
            success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
            if (!success) {
                // Error occurred
                ilDeleteImages(1, imageIds);
                throw new ImageLoaderException("ImageLoader::LoadImageU -> Couldn't convert image.");
            }

            rawImage = getRawImageFromILData(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
        }
        else {
            ILenum i = ilGetError();
            std::string msg = "ImageLoader::LoadImage -> Couldn't load image: ";
            msg += fullPath.c_str();
            if (i == IL_INVALID_EXTENSION) {
                msg = std::string("ImageLoader::LoadImage -> Couldn't load image (invalid extension). ");
                msg += std::string("Is DevIL configured to load extension: ") + extension + std::string(" ?");
            }

            ilDeleteImages(1, imageIds);
            throw new ImageLoaderException(msg);

            return nullptr;
        }

        // Because we have already copied image data into texture data we can release memory used by image.
        ilDeleteImages(1, imageIds);

        if (reverseOrigin) {
            ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        }

        return rawImage;
    }

    RawImage * ImageLoader::getRawImageFromILData(const ILubyte * data, UInt32 width, UInt32 height) {
        if (data == nullptr) {
          throw new ImageLoaderException("ImportUtil::getRawImageFromILData -> [data] is null.");
        }

        RawImage * rawImage = new(std::nothrow) RawImage(width, height);
        if (rawImage == nullptr) {
          throw new ImageLoaderException("ImportUtil::GetRawImageFromILData -> Could not allocate RawImage.");
        }

        Bool initSuccess = rawImage->init();
        if (!initSuccess) {
            delete rawImage;
            throw new ImageLoaderException("ImportUtil::GetRawImageFromILData -> Could not init RawImage.");
        }

        for (UInt32 i = 0; i < width * height * 4; i++) {
            rawImage->setByte(i, (Byte)data[i]);
        }

        return rawImage;
    }

    void ImageLoader::destroyRawImage(RawImage * image) {
        if (image == nullptr) {
          throw new ImageLoaderException("ImageLoader::DestroyRawImage -> 'image' is null.");
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