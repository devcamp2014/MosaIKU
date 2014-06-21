#include "OutputDeviceVideo.h"

#include <boost/filesystem.hpp>

#include "TiledImageCreator.h"

OutputDeviceVideo::OutputDeviceVideo() {
  std::cout << "begin load image" << std::endl;
  nim.load("img");
}

OutputDeviceVideo::~OutputDeviceVideo() {
}

void OutputDeviceVideo::output(cv::VideoCapture& in_video) {

  cv::Mat image;
  in_video >> image;

  cv::VideoWriter out_video("out.avi", 
                            CV_FOURCC('X','V','I','D'), 
                            in_video.get(CV_CAP_PROP_FPS), 
                            image.size());

  while(!image.empty()) {

    out_video << create_tiled_img(image, nim, icm);
    
    in_video >> image;
  }
}

