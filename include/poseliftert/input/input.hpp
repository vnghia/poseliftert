#pragma once

namespace poseliftert {

class Input {
 public:
  Input() = default;

  Input(float* pose_2d_ptr, unsigned int width, unsigned int height,
        float* net_input_ptr);

  void process();

 private:
  float* pose_2d_ptr_;

  unsigned int width_;
  unsigned int height_;

  float* net_input_ptr_;
};

}  // namespace poseliftert
