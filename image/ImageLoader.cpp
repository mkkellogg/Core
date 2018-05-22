//
// Created by Mark Kellogg on 12/19/17.
//

#include "ImageLoader.h"

namespace Core {

  Bool ImageLoader::isImageInCache(const std::string& name) {
    auto got = this->cache.find(name);
    if (got == this->cache.end()) return false;
    return true;
  }

  void ImageLoader::addImageToCache(std::shared_ptr<RawImage> image, const std::string& name) {
    this->cache[name] = image;
  }

  std::shared_ptr<RawImage> ImageLoader::getCachedImage(const std::string& name) {
    return this->cache[name];
  }

}
