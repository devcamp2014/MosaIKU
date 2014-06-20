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

std::string get_split_img_name(int cnt) {
  char name[255];
  sprintf(name, "ret_%04d.png", cnt);
  return name;
}

void split_video(const std::string& videoname) {

  cv::VideoCapture in_video;
  cv::Mat image;

  in_video.open(videoname);

  int cnt = 0;
  in_video >> image;
  while(!image.empty()) {

    // output img
    cv::imwrite(get_split_img_name(cnt), image);

    in_video >> image;
    cnt++;
  }
}

void resize_and_copy(cv::Mat dst, cv::Mat src, int px, int py, float w, float h) {

  cv::Mat src_resize(w, h, src.type());
  cv::resize(src, src_resize, src_resize.size(), cv::INTER_CUBIC);
  
  src_resize.copyTo(dst(cv::Rect(px, py, w, h)));
}

cv::Mat create_tiled_img(int cnt) {


  cv::Mat src_img = cv::imread(get_split_img_name(cnt), 1);
  //  if(src_img.empty()) return; 

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
      icm.
        load(near_imagename, w, h).
        copyTo(dst_img(cv::Rect(bx, by, w, h)));
    }
  }

  return dst_img;
}

int main(int argc, char *argv[]) {
  
  std::cout << "begin load image" << std::endl;
  nim.load("img");

  std::cout << "begin split video" << std::endl;
  split_video(argv[1]);

  std::cout << "begin tiled img" << std::endl;
  cv::imwrite("ret_xx.png", create_tiled_img(0));

  return 0;
}
