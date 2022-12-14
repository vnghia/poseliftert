#include "poseliftert/input/input.hpp"

#include "poseliftert/input/from_body25.hpp"
#include "poseliftert/input/normalization.hpp"

namespace poseliftert {

Input::Input(float* pose_2d_ptr, unsigned int width, unsigned int height,
             float* net_input_ptr, bool use_body25_output)
    : pose_2d_ptr_(pose_2d_ptr),
      width_(width),
      height_(height),
      net_input_ptr_(net_input_ptr),
      use_body25_output_(use_body25_output) {}

void Input::process() {
  if (use_body25_output_) {
    from_body25(net_input_ptr_, pose_2d_ptr_, width_, height_);
  } else {
    normalization(net_input_ptr_, pose_2d_ptr_, width_, height_);
  }
}

}  // namespace poseliftert
