#pragma once

#include <algorithm>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "NearImageManager.h"
#include "ImageLoader.h"
#include "ImageCacheManager.h"

class TiledImageCreator {
public:
  TiledImageCreator();
  virtual ~TiledImageCreator();

  cv::Mat create_tiled_img(cv::Mat src_img);

  void set_cell_size(int w, int h) {
    cell_width = w;
    cell_height = h;

    if(80 > std::max(w, h)) {
      nim.set_select_mode(NearImageManager::SELECT_MODE_RECYCLE);
    } else {
      nim.set_select_mode(NearImageManager::SELECT_MODE_USE_ONCE);
    }

    icm.clear();
  }

private:
  int cell_width;
  int cell_height;

  NearImageManager nim;
  ImageCacheManager<cv::Mat, ImageLoader> icm;
};
