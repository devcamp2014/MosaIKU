#include "OutputDeviceWindow.h"

#include <boost/filesystem.hpp>

#include "NearImageManager.h"
#include "ImageLoader.h"
#include "ImageCacheManager.h"

#include "TiledImageCreator.h"

OutputDeviceWindow::OutputDeviceWindow() {
  std::cout << "begin load image" << std::endl;
  nim.load("img");
}

OutputDeviceWindow::~OutputDeviceWindow() {
}

int division = 1;
int image_size = 0;
int small_image_size = 0;
int min_small_image_size = 5;
int use_once_threshold = 4;

void OutputDeviceWindow::output(cv::VideoCapture& in_video) {
  const std::string WIN_NAME = "mudou_win";
  const int KEY_CODE_ESC = 1048603;
  const int KEY_CODE_J   = 1048682;
  const int KEY_CODE_K   = 1048683;

  cv::Mat image;

  cv::namedWindow(WIN_NAME);

  in_video >> image;
  if(!image.empty()) {
    if(image.rows < image.cols){
      image_size = image.cols;
    }else{
      image_size = image.rows;
    }
    small_image_size = image_size / (1 << division);
    if(small_image_size < 1){
      small_image_size = 1;
    }
    cell_width  = small_image_size;
    cell_height = small_image_size;
  }

  while(!image.empty()) {
    cv::imshow(WIN_NAME, create_tiled_img(image, nim, icm));
    
    int key = cv::waitKey(1);
    std::cout << "key = " << key << std::endl;
    if( key == KEY_CODE_ESC ) {
      //break;
      return;
    }
    if( key == KEY_CODE_J ) {
      // 部品画像を小さくする
      division++;
      small_image_size = image_size / (1 << division);

      if(small_image_size < min_small_image_size){
        small_image_size = min_small_image_size;
        division--;
      }

      cell_width  = small_image_size;
      cell_height = small_image_size;
      if(division >= use_once_threshold){
        nim.set_recycle_mode(1);
      }
      icm.clear();
    }
    if( key == KEY_CODE_K ) {
      // 部品画像を大きくする
      division--;
      if(division < 0){
        division = 0;
      }

      small_image_size = image_size / (1 << division);
      cell_width  = small_image_size;
      cell_height = small_image_size;
      if(division < use_once_threshold){
        nim.set_recycle_mode(0);
      }

      icm.clear();
    }

    in_video >> image;
    if(image.empty()){
      in_video.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
      in_video >> image;
    }
  }
}

