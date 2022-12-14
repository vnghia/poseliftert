#pragma once

#include "minrt/minrt.hpp"
#include "poseliftert/input/input.hpp"

namespace poseliftert {

using namespace minrt;

class Poseliftert {
 private:
  Engine engine_;

 public:
  Poseliftert(const fs::path& engine_path, unsigned int width,
              unsigned int height);

  Engine& engine() { return engine_; }

  auto get_input_data() { return input_data_.get(); }

  auto get_net_output_data() { return net_output_data_.get(); }

  void forward();

  const unsigned int width;
  const unsigned int height;

  const unsigned int pose_2d_dim = 2;

 private:
  void malloc_memory();

  std::shared_ptr<float[]> input_data_;
  std::shared_ptr<float[]> net_input_data_;

  Input input_;

  std::shared_ptr<float[]> net_output_data_;
};

}  // namespace poseliftert
