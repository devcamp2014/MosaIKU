#pragma once 

#include "OutputDevice.h"

class OutputDeviceWindow : public OutputDevice {
public:
  OutputDeviceWindow();
  virtual ~OutputDeviceWindow();

  void output(cv::VideoCapture& in_video);
};
