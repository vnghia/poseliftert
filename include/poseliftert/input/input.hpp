#pragma once

namespace poseliftert {

class Input {
 public:
  Input() = default;

  Input(float* pose_2d_ptr, unsigned int width, unsigned int height,
        float* net_input_ptr, bool use_body25_output);

  void process();

 private:
  float* pose_2d_ptr_;

  unsigned int width_;
  unsigned int height_;

  float* net_input_ptr_;

  bool use_body25_output_;
};

}  // namespace poseliftert
