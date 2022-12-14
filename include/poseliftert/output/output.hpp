#pragma once

namespace poseliftert {

class Output {
 public:
  Output() = default;

  Output(float* net_output_ptr, float* output_ptr);

  void process();

 private:
  float* net_output_ptr_;
  float* output_ptr_;
};

}  // namespace poseliftert
