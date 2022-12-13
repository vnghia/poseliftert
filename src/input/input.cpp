#include "poseliftert/input/input.hpp"

#include "poseliftert/input/normalization.hpp"

namespace poseliftert {

Input::Input(float* pose_2d_ptr, unsigned int width, unsigned int height,
             float* net_input_ptr)
    : pose_2d_ptr_(pose_2d_ptr),
      width_(width),
      height_(height),
      net_input_ptr_(net_input_ptr) {}

void Input::process() {
  normalization(net_input_ptr_, pose_2d_ptr_, width_, height_);
}

}  // namespace poseliftert
