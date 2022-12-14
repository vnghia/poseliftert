#include <cstring>
#include <filesystem>
#include <iostream>
#include <vector>

#include "cxxopts.hpp"
#include "poseliftert/poseliftert.hpp"
#include "spdlog/fmt/bundled/ranges.h"
#include "spdlog/spdlog.h"

using namespace minrt;
using namespace poseliftert;

int main(int argc, char* argv[]) {
  cxxopts::Options options("poseliftert", "Poselifter TensorRT Engine");
  auto options_adder = options.add_options();
  options_adder("help", "Print help");
  options_adder("e,engine", "Engine path", cxxopts::value<fs::path>());
  options_adder("w,width", "Image width", cxxopts::value<unsigned int>());
  options_adder("h,height", "Image height", cxxopts::value<unsigned int>());
  auto args = options.parse(argc, argv);

  if (args.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  auto poseliftert = Poseliftert(args["engine"].as<fs::path>(),
                                 args["width"].as<unsigned int>(),
                                 args["height"].as<unsigned int>());

  return 0;
}
