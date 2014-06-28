#include "TiledImageCreator.h"

TiledImageCreator::TiledImageCreator() :
  cell_width(100),
  cell_height(100) {

  std::cout << "begin load image" << std::endl;
  nim.load("img");
}

TiledImageCreator::~TiledImageCreator() {
}

cv::Mat TiledImageCreator::create_tiled_img(cv::Mat src_img) {

  cv::Mat dst_img = cv::Mat::zeros(src_img.rows, 
                                   src_img.cols, 
                                   src_img.type());

  int es   = src_img.elemSize();
  int step = src_img.step;
  // int cell_width  = 100;
  // int cell_height = 100;

  // 画像使用回数をリセット
  nim.reset_used_info();

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
