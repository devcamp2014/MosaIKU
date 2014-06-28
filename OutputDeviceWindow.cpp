#include "OutputDeviceWindow.h"

#include <boost/filesystem.hpp>

#include "NearImageManager.h"
#include "ImageLoader.h"
#include "ImageCacheManager.h"

#include "TiledImageCreator.h"

OutputDeviceWindow::OutputDeviceWindow() {
  division = 1;
  image_size = 0;
  small_image_size = 0;
  min_small_image_size = 5;
}

OutputDeviceWindow::~OutputDeviceWindow() {
}

void OutputDeviceWindow::output(cv::VideoCapture& in_video) {
  const std::string WIN_NAME = "mudou_win";
  const int KEY_CODE_ESC = 27;
  const int KEY_CODE_J   = 106;
  const int KEY_CODE_K   = 107;

  TiledImageCreator tic;

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

    tic.set_cell_size(small_image_size, small_image_size);
  }

  while(!image.empty()) {
    cv::imshow(WIN_NAME, tic.create_tiled_img(image));
    
    int key = cv::waitKey(1);
    // std::cout << "key = " << key << std::endl;
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

      tic.set_cell_size(small_image_size, small_image_size);
    }
    if( key == KEY_CODE_K ) {
      // 部品画像を大きくする
      division--;
      if(division < 0){
        division = 0;
      }

      small_image_size = image_size / (1 << division);
      tic.set_cell_size(small_image_size, small_image_size);
    }

    in_video >> image;
    if(image.empty()){
      in_video.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
      in_video >> image;
    }
  }
}

