#pragma once 

#include "OutputDevice.h"
#include "NearImageManager.h"
#include "ImageLoader.h"
#include "ImageCacheManager.h"

class OutputDeviceVideo : public OutputDevice {
public:
  OutputDeviceVideo();
  virtual ~OutputDeviceVideo();

  void output(cv::VideoCapture& in_video);

private:
  ImageCacheManager<cv::Mat, ImageLoader> icm;
  NearImageManager nim;
};
