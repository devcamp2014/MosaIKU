#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ImageLoader.h"
#include "TiledImageCreator.h"
#include "OutputDeviceWindow.h"
#include "OutputDeviceVideo.h"

using namespace boost::program_options;

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

  options_description opt("option");
  opt.add_options()
    ("help,h", "show this message")
    ("verify,v", "verify image set")
    ("input,i", value<std::string>(), "input type ('camera' or 'video' or 'image')")
    ("output,o", value<std::string>(), "output type (output filename or 'window')")
    ("name", value<std::string>(), "input filename")
    ("cellsize", value<int>()->default_value(10), "image cell size")
    ("scale", value<float>()->default_value(1.0), "output image scale")
    ;
  
  variables_map argmap;
   try {

    store(parse_command_line(argc, argv, opt), argmap);
    notify(argmap);

  } catch(...) {
    std::cout << opt << std::endl;
    return 1;
  }

    if(argmap.count("verify")) {
      verify_image_files("img/","collapsed_img/");
      return 0;
    }

    if (argmap.count("help") ||
        !argmap.count("input") ||
        !argmap.count("output")) {
      std::cout << opt << std::endl;
      return 0;
    } 
    
  std::string input = argmap["input"].as<std::string>();
  cv::VideoCapture in_video;
  cv::Mat in_image;
  if(input=="camera") {
    in_video.open(0);
  } else if(input=="video") {
    std::string name = argmap["name"].as<std::string>();
    if (!argmap.count("name")) {
      std::cout << opt << std::endl;
      return 0;
    }
    in_video.open(name);
  } else if(input=="image") {
    std::string name = argmap["name"].as<std::string>();
    if (!argmap.count("name")) {
      std::cout << opt << std::endl;
      return 0;
    }
    float scale = argmap["scale"].as<float>();

    cv::Mat orig_img = cv::imread(name, 1);
    in_image.create(orig_img.size().height * scale, 
		    orig_img.size().width * scale, 
		    orig_img.type());
    cv::resize(orig_img, in_image, in_image.size(), cv::INTER_CUBIC);
  }

  std::string output_filename = argmap["output"].as<std::string>();
  if(output_filename=="window") {
    OutputDeviceWindow output_dev;  
    output_dev.output(in_video);
  } else {
    if(input=="video") {
      OutputDeviceVideo output_dev(output_filename);  
      output_dev.output(in_video);
    } else {
      TiledImageCreator tic;

      int cs = argmap["cellsize"].as<int>();
      tic.set_cell_size(cs, cs);
      cv::imwrite(output_filename, tic.create_tiled_img(in_image));      
    }
  }

  return 0;
}
