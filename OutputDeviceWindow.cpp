#include "OutputDeviceWindow.h"

#include <boost/filesystem.hpp>

#include "NearImageManager.h"
#include "ImageLoader.h"
#include "ImageCacheManager.h"

ImageCacheManager<cv::Mat, ImageLoader> icm;
NearImageManager nim;

#include "TiledImageCreator.h"

OutputDeviceWindow::OutputDeviceWindow() {
  std::cout << "begin load image" << std::endl;
  nim.load("img");
}

OutputDeviceWindow::~OutputDeviceWindow() {
}

void OutputDeviceWindow::output(cv::VideoCapture& in_video) {
  const std::string WIN_NAME = "mudou_win";
  const int KEYCODE_ESC = 0x1b;

  cv::Mat image;

  cv::namedWindow(WIN_NAME);

  in_video >> image;
  while(!image.empty()) {

    cv::imshow(WIN_NAME, create_tiled_img(image, nim, icm));
    
    int key = cv::waitKey(1);
    if( key == KEYCODE_ESC ) {
      break;
    }

    in_video >> image;
  }
}

