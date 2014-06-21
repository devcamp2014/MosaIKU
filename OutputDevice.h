#pragma once 

#include <opencv/cv.h>
#include <opencv/highgui.h>

class OutputDevice {
public:
  OutputDevice() {}
  virtual ~OutputDevice() {}

  virtual void output(cv::VideoCapture& in_video){}
};
