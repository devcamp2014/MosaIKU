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

void resize_and_copy(cv::Mat dst, cv::Mat src, int px, int py, float w, float h) {

  cv::Mat src_resize(w, h, src.type());
  cv::resize(src, src_resize, src_resize.size(), cv::INTER_CUBIC);
  
  src_resize.copyTo(dst(cv::Rect(px, py, w, h)));
}

cv::Mat create_tiled_img(int cnt, const color_list_t& color_list) {

  cv::Mat src_img = cv::imread(get_split_img_name(cnt), 1);
  //  if(src_img.empty()) return; 

  cv::Mat dst_img = cv::Mat::zeros(src_img.rows, src_img.cols, src_img.type());

  int es   = src_img.elemSize();
  int step = src_img.step;
  int cell_width  = 10;
  int cell_height = 10;

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
      int min_len=std::numeric_limits<int>::max();
      int min_index=0;
      for(unsigned int i=0; i<color_list.size(); i++) {
	const cv::Point3f& p = color_list[i];
	int len = (p.x-cp.x)*(p.x-cp.x) + (p.y-cp.y)*(p.y-cp.y) + (p.z-cp.z)*(p.z-cp.z);
	if(len < min_len) {
	  min_index = i;
	  min_len = len;
	}
      }

      // 一番色が違い画像の貼りつけ
      cv::Mat img = cv::imread(get_split_img_name(min_index), 1);
      resize_and_copy(dst_img, img, bx, by, ex-bx, ey-by);
    }
  }

  return dst_img;
}

int main(int argc, char *argv[]) {

  color_list_t color_list;
  split_video(argv[1], color_list);

  cv::imwrite("ret_xx.png", create_tiled_img(0, color_list));

  return 0;
}
