#include "poseliftert/input/normalization.hpp"

#include "poseliftert/config.hpp"

namespace poseliftert {

void normalization(float* normalized_ptr, const float* const input_ptr,
                   unsigned int width, unsigned int height) {
  const auto scale = 0.5f * width;
  const auto offset_width = scale;
  const auto offset_height = 0.5f * height;
  for (int i = 0; i < kNum2DJoints; ++i) {
    normalized_ptr[2 * i] = (input_ptr[2 * i] - offset_width) / scale;
    normalized_ptr[2 * i + 1] = (input_ptr[2 * i + 1] - offset_height) / scale;
  }
}

}  // namespace poseliftert
