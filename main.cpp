#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ImageLoader.h"
#include "OutputDeviceWindow.h"
#include "OutputDeviceVideo.h"

void verify_image_files(const std::string& image_dir_path, const std::string& collapsed_image_dir_path) {
  namespace fs = boost::filesystem;
  int file_count = 0;

  fs::path dir(image_dir_path);
  fs::directory_iterator end;

  std::cout << "verify start" << std::endl;
  for(fs::directory_iterator it(dir); it!=end; ++it) {
    try {
      ImageLoader()(it->path().string(), 1, 1);
    } catch(cv::Exception &e) {
      // image may be collapsed...
      std::string moveTo = collapsed_image_dir_path + "/" + it->path().filename().string();
      std::cout << "collapsed image:" << it->path() << " moved to " << moveTo << std::endl;
      rename(it->path(), moveTo);
    }

    file_count++;
    if(file_count % 100 == 0){
      std::cout << file_count << " files checked ..." << std::endl;
    }
  }
  std::cout << "verify end" << std::endl;
}

int main(int argc, char *argv[]) {
  int verify = 0;  
  if(verify) {
    verify_image_files("img/","collapsed_img/");
    return 0;
  }

  std::cout << "begin real2escape" << std::endl;

  cv::VideoCapture in_video;
  in_video.open(argv[1]);

  // OutputDeviceWindow output_dev;  
  // output_dev.output(in_video);

  OutputDeviceVideo output_dev;  
  output_dev.output(in_video);


  // cv::VideoCapture in_video;
  // in_video.open(0);
  // window_view(in_video);


  return 0;
}
