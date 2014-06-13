#pragma once

#include <string>
#include <unordered_map>

template<typename ImageType, typename ImageLoader>
class ImageCacheManager {
public:

  ImageCacheManager() {
    
  }

  virtual ~ImageCacheManager() {
    
  }

  ImageType load(std::string name, int width, int height) {

    ImageKey imgkey(name, width, height);
    typename image_map_t::const_iterator it = image_map_.find(imgkey);
    if(it == image_map_.end()) {
      ImageType img = ImageLoader()(name, width, height);
      image_map_.insert(std::make_pair(imgkey, img));
      return img;
    } 
    
    return it->second;
  }
private:
  struct ImageKey {
    ImageKey(std::string n, int w, int h) : 
      name(n), width(w), height(h) {
    }

    std::string name;
    int width;
    int height;

    bool operator==(const ImageKey &rhs) const {
      return name == rhs.name && width == rhs.width && height == rhs.height;
    }
  };
  
  struct ImageKeyHasher {
    std::size_t operator()(const ImageKey &key) const {
      return std::hash<std::string>()(key.name) ^ 
        std::hash<int>()(key.width) ^
        std::hash<int>()(key.height);
    }
  };

  typedef typename std::unordered_map<ImageKey, ImageType, ImageKeyHasher> image_map_t;

  image_map_t image_map_;
};
