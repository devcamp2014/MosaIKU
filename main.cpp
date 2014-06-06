#include <iostream>
#include <vector>
#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>

typedef std::vector<cv::Point3f> color_list_t;

std::string get_split_img_name(int cnt) {
  char name[255];
  sprintf(name, "ret_%04d.png", cnt);
  return name;
}

void split_video(const std::string& videoname, color_list_t& color_list) {

  cv::VideoCapture in_video;
  cv::Mat image;

  in_video.open(videoname);

  int cnt = 0;
  in_video >> image;
  while(!image.empty()) {

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
    cv::Point3f cp(b/pixsum, g/pixsum, r/pixsum);
    color_list.push_back(cp);

    // output img
    cv::imwrite(get_split_img_name(cnt), image);

    in_video >> image;
    cnt++;
  }
}

int main(int argc, char *argv[]) {

  color_list_t color_list;
  split_video(argv[1], color_list);

  return 0;
}
