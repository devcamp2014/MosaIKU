#pragma once 

#include "OutputDevice.h"

class OutputDeviceWindow : public OutputDevice {
public:
  OutputDeviceWindow();
  virtual ~OutputDeviceWindow();

  void output(cv::VideoCapture& in_video);

private:
  int division;
  int image_size;
  int small_image_size;
  int min_small_image_size;
};
