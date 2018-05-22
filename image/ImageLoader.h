#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "RawImage.h"

namespace Core {
  class ImageLoader {
    std::unordered_map<std::string, std::shared_ptr<RawImage>> cache;
  protected:
    void addImageToCache(std::shared_ptr<RawImage> image, const std::string& name);
    std::shared_ptr<RawImage> getCachedImage(const std::string& name);
    Bool isImageInCache(const std::string& name);
  public:
    virtual std::shared_ptr<RawImage> loadJPEG(const std::string& name, Bool fromAssets = true) = 0;
  };
}
