#pragma once

#include <string>

typedef cv::Point3f color_t;

class NearImageManager {
public:
  NearImageManager() {
  }

  virtual ~NearImageManager() {
  }

  void load(const std::string& image_dir_path) {
    namespace fs = boost::filesystem;

    fs::path dir(image_dir_path);
    fs::directory_iterator end;
    for(fs::directory_iterator it(dir); it!=end; ++it) {
      color_t color;
      
      ImageInfo ii;
      ii.imagename = it->path().string();
      ii.color     = mat2avgcolor(cv::imread(it->path().string(), 1));

      image_list_.push_back(ii);
    }
  }

  std::string get_near_imagename(color_t cp) {
    // 一番色が近い画像を求める
    int min_len=std::numeric_limits<int>::max();
    std::string min_imagename;

    for(auto i : image_list_) {
      const color_t& p = i.color;
      int len = (p.x-cp.x)*(p.x-cp.x) + (p.y-cp.y)*(p.y-cp.y) + (p.z-cp.z)*(p.z-cp.z);
      if(len < min_len) {
        min_imagename = i.imagename; 
        min_len       = len;
      }
    }

    return min_imagename;
  }


private:
  struct ImageInfo {
    color_t color;
    std::string imagename;
  };

  typedef std::vector<ImageInfo> image_list_t;
  image_list_t image_list_;

  color_t mat2avgcolor(const cv::Mat &image) {
    int es   = image.elemSize();
    int step = image.step;
    uint64_t r = 0;
    uint64_t g = 0;
    uint64_t b = 0;

    // sum image pix
    for(int y = 0 ; y < image.rows; y++){
      for(int x = 0 ; x < image.cols; x++){
        b += image.data[y * step + x * es + 0]; //b
        g += image.data[y * step + x * es + 1]; //g
        r += image.data[y * step + x * es + 2]; //r
      }
    }

    // image avg
    float pixsum = image.rows*image.cols;

    color_t color;
    color.x = b/pixsum;
    color.y = g/pixsum;
    color.z = r/pixsum;

    return color;
  }
};
