#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ImageLoader.h"
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
    ("input,i", value<std::string>(), "input device ('camera' or 'video' or 'image')")
    ("output,o", value<std::string>(), "output ('window' or 'file')")
    ("name", value<std::string>(), "filename");
  
  variables_map argmap;
   try {

    store(parse_command_line(argc, argv, opt), argmap);
    notify(argmap);

  } catch(...) {
    std::cout << opt << std::endl;
    return 1;
  }

    if (argmap.count("help") ||
        !argmap.count("input") ||
        !argmap.count("output")) {
      std::cout << opt << std::endl;
      return 0;
    } 

    if(argmap.count("verify")) {
      verify_image_files("img/","collapsed_img/");
      return 0;
    }

    
  std::string input = argmap["input"].as<std::string>();
  cv::VideoCapture in_video;
  if(input=="camera") {
    in_video.open(0);
  } else if(input=="video") {
    std::string name = argmap["name"].as<std::string>();
    if (!argmap.count("name")) {
      std::cout << opt << std::endl;
      return 0;
    }
    in_video.open(name);
  }

  std::string output = argmap["output"].as<std::string>();
  if(output=="window") {
    OutputDeviceWindow output_dev;  
    output_dev.output(in_video);
  } else if(output=="file") {
    OutputDeviceVideo output_dev;  
    output_dev.output(in_video);
  }

  return 0;
}
