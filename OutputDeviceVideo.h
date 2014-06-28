#pragma once 

#include "OutputDevice.h"

class OutputDeviceVideo : public OutputDevice {
public:
  OutputDeviceVideo();
  virtual ~OutputDeviceVideo();

  void output(cv::VideoCapture& in_video);

private:
};
