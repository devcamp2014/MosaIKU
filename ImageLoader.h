#pragma once

#include <opencv/cv.h>

class ImageLoader {
public:
  cv::Mat operator()(std::string name, int width, int height) {
    std::cout << name << std::endl;

    cv::Mat orig_img = cv::imread(name, 1);

    cv::Mat resize_img(width, height, orig_img.type());
    cv::resize(orig_img, resize_img, resize_img.size(), cv::INTER_CUBIC);
    return resize_img;    
  }
};
