#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char *argv[])
{
  cv::VideoCapture in_video;
  cv::Mat image;
  in_video.open(argv[1]);

  int cnt = 0;
  in_video >> image;
  while(!image.empty()) {
    char name[100];
    sprintf(name, "ret_%03d.png", cnt);

    // image info
    // std::cout << image.rows << "," <<image.cols<<","<<image.step<<std::endl;

    int es   = image.elemSize();
    int step = image.step;
    uint64_t r = 0;
    uint64_t g = 0;
    uint64_t b = 0;

    // sum image pix
    for(int y = 0 ; y < image.rows; y++){
      for(int x = 0 ; x < image.cols; x++){
	b += image.data[y * step + x * es + 0]; //b
	g += image.data[y * step + x * es + 1]; //g
	r += image.data[y * step + x * es + 2]; //r
      }
    }

    // image avg
    float pixsum = image.rows*image.cols;
    std::cout << (r/pixsum) << "," << (g/pixsum) << "." << (b/pixsum) << std::endl;

    // output img
    cv::imwrite(name, image);

    in_video >> image;
    cnt++;
  }

  return 0;
}
