#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <opencv/cv.h>

typedef cv::Point3f color_t;

class NearImageManager {
public:
  enum SELECT_MODE {
    SELECT_MODE_RECYCLE,
    SELECT_MODE_USE_ONCE
  };

  NearImageManager();
  virtual ~NearImageManager();

  void load(const std::string& image_dir_path);

  std::string get_near_imagename(color_t cp) {
    ImageInfo& image = get_near_image(cp);
    return image.imagename;
  }

  void reset_used_info() {
    for(ImageInfo& i : image_list_) {
      i.used = 0;
    }
  }

  void set_select_mode(SELECT_MODE mode) {
    select_mode = mode;
  }

  void set_recycle_mode(bool mode);

private:
  struct ImageInfo {
    color_t color;
    std::string imagename;
    unsigned int used;
  
    bool operator==(const ImageInfo& rhs) const {
      return color == rhs.color && imagename==imagename; 
    }
  };

  typedef std::vector<ImageInfo> image_list_t;
  image_list_t image_list_;
  ImageInfo dummy_image;

  int select_mode;
  int max_use_count;

#define COLOR_ARRAY_SIZE 20
  const int color_array_size = COLOR_ARRAY_SIZE;
  const int color_array_size_2 = (COLOR_ARRAY_SIZE * COLOR_ARRAY_SIZE);
  const float color_step = COLOR_ARRAY_SIZE / 256.0;
  image_list_t color_image_list_[(COLOR_ARRAY_SIZE * COLOR_ARRAY_SIZE * COLOR_ARRAY_SIZE)];

  ImageInfo &get_near_image(color_t cp);
  ImageInfo& get_near_unused_image(color_t cp);
  color_t mat2avgcolor(const cv::Mat &image);
  ImageInfo& get_near_image_precise(color_t cp);
  void set_max_use_count(int count) {
    max_use_count = count;
  }
};
