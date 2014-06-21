#pragma once

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "NearImageManager.h"
#include "ImageLoader.h"
#include "ImageCacheManager.h"

cv::Mat create_tiled_img(cv::Mat src_img, NearImageManager& nim, ImageCacheManager<cv::Mat, ImageLoader>& icm);

extern int cell_width;
extern int cell_height;
