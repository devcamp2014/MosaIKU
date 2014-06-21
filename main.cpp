#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ImageLoader.h"
#include "NearImageManager.h"
#include "ImageCacheManager.h"

ImageCacheManager<cv::Mat, ImageLoader> icm;
NearImageManager nim;

void resize_and_copy(cv::Mat dst, cv::Mat src, int px, int py, float w, float h) {

  cv::Mat src_resize(w, h, src.type());
  cv::resize(src, src_resize, src_resize.size(), cv::INTER_CUBIC);
  
  src_resize.copyTo(dst(cv::Rect(px, py, w, h)));
}

cv::Mat create_tiled_img(cv::Mat src_img) {

  cv::Mat dst_img = cv::Mat::zeros(src_img.rows, src_img.cols, src_img.type());

  int es   = src_img.elemSize();
  int step = src_img.step;
  int cell_width  = 5;
  int cell_height = 5;

  // 画像をセル単位に分割し、一番近い色の画像を貼りつける
  for(int by = 0 ; by < src_img.rows; by+=cell_height){
    for(int bx = 0 ; bx < src_img.cols; bx+=cell_width){
      int ey = std::min(by+cell_height, src_img.rows);
      int ex = std::min(bx+cell_width,  src_img.cols);
      
      // セル内の色の平均値算出
      uint64_t r = 0;
      uint64_t g = 0;
      uint64_t b = 0;
      int pixsum = 0;
      for(int y=by; y<ey; y++) {
	for(int x=bx; x<ex; x++) {
	  b += src_img.data[y * step + x * es + 0]; //b
	  g += src_img.data[y * step + x * es + 1]; //g
	  r += src_img.data[y * step + x * es + 2]; //r

	  pixsum++;
	}
      }
      cv::Point3f cp((float)b/pixsum, (float)g/pixsum, (float)r/pixsum);

      // 一番色が近い画像を求める
      std::string near_imagename = nim.get_near_imagename(cp);

      // 一番色が違い画像の貼りつけ
      int w = ex-bx;
      int h = ey-by;
      //      std::cout << bx << "," << by << "," << w << "," << h << std::endl;

      cv::Mat tile_img = icm.load(near_imagename, cell_width, cell_height);
      tile_img(cv::Rect(0,0,w,h)).copyTo(dst_img(cv::Rect(bx, by, w, h)));
    }
  }

  return dst_img;
}

void video_real2escape(const std::string& videoname) {
  cv::VideoCapture in_video;
  cv::Mat image;

  in_video.open(videoname);

  in_video >> image;

  cv::VideoWriter out_video("out.avi", 
                            CV_FOURCC('X','V','I','D'), 
                            in_video.get(CV_CAP_PROP_FPS), 
                            image.size());

  while(!image.empty()) {

    out_video << create_tiled_img(image);
    
    in_video >> image;
  }
}

const std::string WIN_NAME = "mudou_win";
const int KEYCODE_ESC = 0x1b;

void window_view(const std::string& videoname) {
  cv::VideoCapture in_video;
  cv::Mat image;


  cv::namedWindow(WIN_NAME);

  in_video.open(videoname);

  in_video >> image;
  while(!image.empty()) {

    cv::imshow(WIN_NAME, create_tiled_img(image));

    int key = cv::waitKey(1);
    if( key == KEYCODE_ESC ) {
      break;
    }

    in_video >> image;
  }
}

int main(int argc, char *argv[]) {
  
  std::cout << "begin load image" << std::endl;
  nim.load("img");

  std::cout << "begin real2escape" << std::endl;
  //video_real2escape(argv[1]);
  window_view(argv[1]);

  return 0;
}
