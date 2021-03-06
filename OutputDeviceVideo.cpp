#include "OutputDeviceVideo.h"

#include "TiledImageCreator.h"

OutputDeviceVideo::OutputDeviceVideo(std::string output_filename) :
  output_filename_(output_filename) {
}

OutputDeviceVideo::~OutputDeviceVideo() {
}

void OutputDeviceVideo::output(cv::VideoCapture& in_video) {

  TiledImageCreator tic;

  cv::Mat image;
  in_video >> image;

  cv::VideoWriter out_video(output_filename_, 
                            CV_FOURCC('X','V','I','D'), 
                            in_video.get(CV_CAP_PROP_FPS), 
                            image.size());

  while(!image.empty()) {

    out_video << tic.create_tiled_img(image);
    
    in_video >> image;
  }
}

