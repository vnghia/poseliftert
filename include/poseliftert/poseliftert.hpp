#pragma once

#include <memory>

#include "minrt/minrt.hpp"
#include "poseliftert/input/input.hpp"
#include "poseliftert/output/output.hpp"

namespace poseliftert {

using namespace minrt;

class Poseliftert {
 private:
  Engine engine_;

 public:
  Poseliftert(const fs::path& engine_path, unsigned int width,
              unsigned int height, float* body25_output_ptr = nullptr);

  Engine& engine() { return engine_; }

  auto get_input_data() { return input_ptr_; }

  auto get_output_data() { return output_data_.get(); }

  void forward();

  const unsigned int width;
  const unsigned int height;

 private:
  void malloc_memory(float* body25_output_ptr);

  float* input_ptr_;
  std::shared_ptr<float[]> input_data_;
  std::shared_ptr<float[]> net_input_data_;

  Input input_;

  std::shared_ptr<float[]> net_output_data_;
  std::shared_ptr<float[]> output_data_;

  Output output_;
};

}  // namespace poseliftert
