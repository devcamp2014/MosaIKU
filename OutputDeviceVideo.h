#pragma once 

#include <string>

#include "OutputDevice.h"

class OutputDeviceVideo : public OutputDevice {
public:
  OutputDeviceVideo(std::string output_filename);
  virtual ~OutputDeviceVideo();

  void output(cv::VideoCapture& in_video);

private:
  std::string output_filename_;
};
